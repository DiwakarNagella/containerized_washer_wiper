/*
 * application_protocol.h
 *
 *  Created on: Oct 19, 2016
 *      Author: root
 */

#ifndef APPLICATION_PROTOCOL_H_
#define APPLICATION_PROTOCOL_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include "visibility_device_utilites.h"

using namespace std;


namespace udp_client_server
{

class udp_client_server_runtime_error : public std::runtime_error
{
public:
    udp_client_server_runtime_error(const char *w) : std::runtime_error(w) {}
};


class udp_client
{
public:
                        udp_client(const std::string& addr, int port);
                        ~udp_client();

    int                 get_socket() const;
    int                 get_port() const;
    std::string         get_addr() const;

    int                 send(int *msg, size_t size);

private:
    int                 f_socket;
    int                 f_port;
    std::string         f_addr;
    struct addrinfo *   f_addrinfo;
};


class udp_server
{
public:
                        udp_server(const std::string& addr, int port);
                        ~udp_server();

    int                 get_socket() const;
    int                 get_port() const;
    std::string         get_addr() const;

    int                 recv(int *msg, size_t max_size);
    int                 timed_recv(char *msg, size_t max_size, int max_wait_ms);

private:
    int                 f_socket;
    int                 f_port;
    std::string         f_addr;
    struct addrinfo *   f_addrinfo;
};


} // namespace udp_client_server


namespace Ethernet_Packet
{
	class UDP_protocol
	{
		public:
			int senddata(string ip, int port, int value);
			int *receivedata(void);
			int senddata(string send_ip, int send_port, int *linvalue);

	};

}


namespace application_protocol
{
	class protocol
	{
		public:
			int set_channel_state(int channel, int state);
			int output_channel(int channel, int value);
			int *input_event(void);
			int lindata_send(int8_t application_ref, int *data);


	};
}



#endif /* APPLICATION_PROTOCOL_H_ */

