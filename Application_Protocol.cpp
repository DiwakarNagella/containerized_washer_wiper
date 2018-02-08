#include "Application_Protocol.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <pthread.h>

using namespace std;
using namespace udp_client_server;
using namespace Ethernet_Packet;
using namespace application_protocol;


/* ///IP ADDRESSES AND PORT NUMBER LIST */
#define COMPUTATIONAL_NODE_IP "0.0.0.0"

#define COMPUTATIONAL_NODE_PORT_NUMBER_SEND 16807
#define COMPUTATIONAL_NODE_PORT_NUMBER_RECEIVE 16808

#define WASPER_WIPER_IP "10.221.37.10" // WASHER WIPER NODE
#define WASHER_WIPER_PORT_NUMBER 1234
#define WASHER_WIPER_STOCK_IP "10.221.37.15" // STOCK NODE
#define WASHER_WIPER_STOCK_PORT_NUMBER 1234

pthread_mutex_t rcv_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t rcv_condition = PTHREAD_COND_INITIALIZER;

/* OPERATION CODE LIST */
#define Set_channel_state 10
int acknowledgment,send_channel;

namespace udp_client_server
{


// ========================= CLIENT =========================

/** \brief Initialize a UDP client object.
 *
 * This function initializes the UDP client object using the address and the
 * port as specified.
 *
 * The port is expected to be a host side port number
 *
 * The \p addr parameter is a textual address. It may be an IPv4 or IPv6
 * address and it can represent a host name or an address defined with
 * just numbers.
 */
udp_client::udp_client(const std::string& addr, int port)
    : f_port(port)
    , f_addr(addr)
{
    char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));
    if(r != 0 || f_addrinfo == NULL)
    {
        throw udp_client_server_runtime_error(("invalid address or port: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(f_socket == -1)
    {
        freeaddrinfo(f_addrinfo);
        throw udp_client_server_runtime_error(("could not create socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
}

/** \brief Clean up the UDP client object.
 *
 * This function frees the address information structure and close the socket
 * before returning.
 */
udp_client::~udp_client()
{
    freeaddrinfo(f_addrinfo);
    close(f_socket);
}

/** \brief Retrieve a copy of the socket identifier.
 *
 * This function return the socket identifier as returned by the socket()
 * function.
 */
int udp_client::get_socket() const
{
    return f_socket;
}

/** \brief Retrieve the port used by this UDP client.
 *
 * This function returns the port used by this UDP client.
 */
int udp_client::get_port() const
{
    return f_port;
}

/** \brief Retrieve a copy of the address.
 *
 * This function returns a copy of the address as it was specified in the
 * constructor.
 */
std::string udp_client::get_addr() const
{
    return f_addr;
}

/** \brief Send a message through this UDP client.
 *
 * This function sends \p msg through the UDP client socket.
 */
int udp_client::send(int *msg, size_t size)
{
    return sendto(f_socket, msg, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
}

// ========================= SEVER =========================

/** \brief Initialize a UDP server object.
 *
 * This function initializes a UDP server object making it ready to
 * receive messages.
 *
 * The server address and port are specified in the constructor so
 * if you need to receive messages from several different addresses
 * and/or port, you'll have to create a server for each.
 *
 * The address is a string and it can represent an IPv4 or IPv6
 * address.
 *
 */
udp_server::udp_server(const std::string& addr, int port)
    : f_port(port)
    , f_addr(addr)
{
    char decimal_port[16];
    int fd;
string address;
    struct ifreq ifr;
    fd=socket(AF_INET,SOCK_DGRAM,0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0",IFNAMSIZ-1);
    ioctl(fd,SIOCGIFADDR,&ifr);
    close(fd); 
 address=inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);	
   snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r(getaddrinfo(address.c_str(), decimal_port, &hints, &f_addrinfo));
    if(r != 0 || f_addrinfo == NULL)
    {
        throw udp_client_server_runtime_error(("invalid address or port for UDP socket: \"" + address + ":" + decimal_port + "\"").c_str());
    }
    f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(f_socket == -1)
    {
        freeaddrinfo(f_addrinfo);
        throw udp_client_server_runtime_error(("could not create UDP socket for: \"" + address + ":" + decimal_port + "\"").c_str());
    }
    r = bind(f_socket, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
    if(r != 0)
    {
        freeaddrinfo(f_addrinfo);
        close(f_socket);
        throw udp_client_server_runtime_error(("could not bind UDP socket with: \"" + address + ":" + decimal_port + "\"").c_str());
    }
}

/** \brief Clean up the UDP server.
 *
 * This function frees the address info structures and close the socket.
 */
udp_server::~udp_server()
{
    freeaddrinfo(f_addrinfo);
    close(f_socket);
}

/** \brief The socket used by this UDP server.
 *
 * This function returns the socket identifier. It can be useful if you are
 * doing a select() on many sockets.
 */
int udp_server::get_socket() const
{
    return f_socket;
}

/** \brief The port used by this UDP server.
 *
 * This function returns the port attached to the UDP server. It is a copy
 * of the port specified in the constructor.
 *
 */
int udp_server::get_port() const
{
    return f_port;
}

/** \brief Return the address of this UDP server.
 *
 * This function returns a verbatim copy of the address as passed to the
 * constructor of the UDP server (i.e. it does not return the canonalized
 * version of the address.)
 */
std::string udp_server::get_addr() const
{
    return f_addr;
}

/** \brief Wait on a message.
 *
 * This function waits until a message is received on this UDP server.

 */
int udp_server::recv( int *msg, size_t max_size)
{
    return ::recv(f_socket, msg, max_size, 0);
}

/** \brief Wait for data to come in.
 *
 * This function waits for a given amount of time for data to come in.
 */
int udp_server::timed_recv(char *msg, size_t max_size, int max_wait_ms)
{
    fd_set s;
    FD_ZERO(&s);
    FD_SET(f_socket, &s);
    struct timeval timeout;
    timeout.tv_sec = max_wait_ms / 1000;
    timeout.tv_usec = (max_wait_ms % 1000) * 1000;
    int retval = select(f_socket + 1, &s, &s, &s, &timeout);
    if(retval == -1)
    {
        // select() set errno accordingly
        return -1;
    }
    if(retval > 0)
    {
        // our socket has data
        return ::recv(f_socket, msg, max_size, 0);
    }

    // our socket has no data
    return -1;
}


} // namespace udp_client_server


namespace Ethernet_Packet
{

		int UDP_protocol::senddata(string ip, int port, int value)
		{
			 int data_send_single[2]={0};
			 int ack_single;

			 //Sending data to I/O node
			 data_send_single[0] = value;
			 //cout << "Send Value " << std::hex << value <<endl;

			 send_channel = ((value & 0x00FF0000) >> 16);

			 udp_client client1(ip,port); // receiver ip address and sender port number
			 client1.send(data_send_single,sizeof(data_send_single[0]));


			 ack_single = acknowledgment;

			 return ack_single;

		}

		int UDP_protocol::senddata(string send_ip, int send_port, int *linvalue)
		{
			 int data_send_LIN[2]={0,0};
			 int ack_LIN,i;

			 //Sending data to I/O node
			 for(i=0;i<2;i++)
			 {
				 data_send_LIN[i] = *(linvalue+i);
				 cout << "Send Value input " << std::hex << *(linvalue+i) <<endl;
				 cout << "Send Value output" << std::hex << data_send_LIN[i] <<endl;
			 }

			 udp_client client2(send_ip,send_port); // receiver ip address and sender port number
			 client2.send(data_send_LIN,sizeof(data_send_LIN));

			 cout << "Size of Data" << sizeof(data_send_LIN) << endl;
		         cout << "Send Value input " << std::hex << send_ip <<endl;
		         cout << "Send Value output" << std::hex << send_port <<endl;		
			 ack_LIN = acknowledgment;

			 return ack_LIN;

		}

		int *UDP_protocol::receivedata(void)
		{

			static int receive_message[4];

			//cout << "Receive Call" << endl;

			udp_server server1(COMPUTATIONAL_NODE_IP,COMPUTATIONAL_NODE_PORT_NUMBER_RECEIVE); // sender ip address and port number
			server1.recv(receive_message,sizeof(receive_message));

			cout << "Only Recevice Value " << std::hex << receive_message[0] << endl;

			if(((receive_message[0] & 0x00ff0000) >> 16) == send_channel)
			{
				//cout <<"Recevie Channel " << ((receive_message[0] & 0x00ff0000) >> 16)<<endl;
				acknowledgment = (receive_message[0] & 0x000000ff);
				//cout << "Acknowledgment " << (receive_message[0] & 0x000000ff)<<endl;
			}
			else if((receive_message[0] & 0x0000FFFF) == 0x38)
			{
				acknowledgment = 1;
			}
			else
			{
				// Do Nothing
			}

			return receive_message;

		}

}


namespace application_protocol
{
	int protocol::set_channel_state(int channel, int state)
	{
		string IP_Address;
		int Port_Number = 0, data = 0, ack = 0;

		UDP_protocol setchannel;

		switch(channel)
		{
			case 1:
				IP_Address = WASPER_WIPER_IP;
				Port_Number = WASHER_WIPER_PORT_NUMBER;

				/*UDP_PAYLOAD = (UDP_PAYLOAD | ((OPERATION << 24)|(CHANNEL_NUMBER << 16)|(CHANNEL_STATE << 8))); */
				data =(data | ((Set_channel_state << 24)|(channel << 16)|(state << 8)));
				break;

			case 2:
				IP_Address = WASPER_WIPER_IP;
				Port_Number = WASHER_WIPER_PORT_NUMBER;

				/*UDP_PAYLOAD = (UDP_PAYLOAD | ((OPERATION << 24)|(CHANNEL_NUMBER << 16)|(CHANNEL_STATE << 8))); */
				data =(data | ((Set_channel_state << 24)|(channel << 16)|(state << 8)));
				break;
			case 4:
				IP_Address = WASPER_WIPER_IP;
				Port_Number = WASHER_WIPER_PORT_NUMBER;
				data =(data | ((Set_channel_state << 24)|(channel << 16)|(state << 8)));
				break;

		}
		ack = setchannel.senddata(IP_Address,Port_Number,data);
		if(ack > 0)
		{
			return 0;
		}
		else
		{
			return -1;
		}


	}

	int protocol::lindata_send(int8_t application_ref,int *data)
	{
		UDP_protocol send_Lin;
		static int reply = 0;

		if(application_ref == 0x01)
		{
			reply = send_Lin.senddata(WASHER_WIPER_STOCK_IP,WASHER_WIPER_STOCK_PORT_NUMBER, data);

		}
		else
		{
			//Do Nothing
		}

		return reply;

	}

	int *protocol::input_event(void)
	{
		static int *received_value;
		UDP_protocol Event_receive;

		received_value = Event_receive.receivedata();

		return received_value;
	}
}


