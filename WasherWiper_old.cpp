/*
 * Copyright (C) Your copyright.
 *
 * Author: sreekanth
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the PG_ORGANIZATION nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY	THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS-IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include "Washerwiper_old.h"
#include <sstream>
#include "Application_Protocol.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include <pthread.h>
#include "hmi_utilities.h"
#include "visibility_device_utilites.h"
#include "visibility_utilities.h"
#include "WasherwiperApplication.h"
#include "errno.h"
#include "LIN_signals.h"

using namespace std;
using namespace Visibility_Utilities;
using namespace Ethernet_Packet;
using namespace HMI_Utilities;
using namespace application_protocol;

int buffer[4];
bool new_msg;
bool new_wiping_req = false;
bool new_washing_req= false;

lindata_SM1_HMIIOM hmi_input;
lindata_to_RComp rains_to_comp;
lindata_to_CRains comp_to_rains;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // Mutex Initializer
pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;  // Conditional Initializer

void Running_Applications::Lin_frame_extractor(int lin_frame_choose)
{

	if(lin_frame_choose == 1)
	{
								/* Need to redo the work on bits */
		hmi_input.DiagInfoSM1 = (buffer[1] & 0x0000007F) ;
		cout<<"lindata.DiagInfoSM1 : " << (buffer[1] & 0x0000007F) <<endl;

		hmi_input.ResponseErrorSM1 = ((buffer[1] & 0x00000080) >> 7);
		cout<<"lindata.ResponseErrorSM1 : " << ((buffer[1] & 0x00000080) >> 7) <<endl;

		hmi_input.LIN_BrakeProgramButtonStatus = ((buffer[1] & 0x00000100) >> 8);
		cout<<"lindata.LIN_BrakeProgramButtonStatus : " << ((buffer[1] & 0x00000100) >> 8) <<endl;

		hmi_input.LIN_DirInd_StalkStatus_1 = ((buffer[1] & 0x00000E00) >> 9);
		cout<<"lindata.LIN_DirInd_StalkStatus_1 : " << ((buffer[1] & 0x00000E00) >> 9) <<endl;

		hmi_input.LIN_MainBeam_StalkStatus_1 = ((buffer[1] & 0x00003000) >> 12);
		cout<<"lindata.LIN_MainBeam_StalkStatus_1 : " << ((buffer[1] & 0x00003000) >> 12) <<endl;

		hmi_input.LIN_Rainsensor_ButtonStatus = ((buffer[1] & 0x00004000) >> 14);
		cout<<"lindata.LIN_Rainsensor_ButtonStatus : " << ((buffer[1] & 0x00004000) >> 14) <<endl;

		hmi_input.LIN_Washing_Status_1 = ((buffer[1] & 0x00008000) >> 15);
		cout<<"lindata.LIN_Washing_Status_1 : " << ((buffer[1] & 0x00008000) >> 15) <<endl;

		hmi_input.LIN_RetarderStalkPosition_1 = ((buffer[1] & 0x00070000) >> 16);
		cout<<"lindata.LIN_RetarderStalkPosition_1 : " << ((buffer[1] & 0x00070000) >> 16) <<endl;

		hmi_input.LIN_TrailerBrakeInputStatus = ((buffer[1] & 0x00380000) >> 19);
		cout<<"lindata.LIN_TrailerBrakeInputStatus : " << ((buffer[1] & 0x00380000) >> 19) <<endl;

		hmi_input.LIN_WiperAdjustStatus_1 = ((buffer[1] & 0x00400000) >> 22);
		cout<<"lindata.LIN_WiperAdjustStatus_1 : " << ((buffer[1] & 0x00400000) >> 22) <<endl;

		hmi_input.LIN_WiperStalkStat_1 = ((buffer[1] & 0x07000000) >> 24);
		cout<<"lindata.LIN_WiperStalkStat_1 : " << ((buffer[1] & 0x07000000) >> 24) <<endl;

	}

	else if(lin_frame_choose == 2)
	{

		comp_to_rains.LIN_WheelBasedVehicleSpdLowRes = (buffer[1] & 0x000000FF);
		cout <<"lindata_to_CRains.LIN_WheelBasedVehicleSpdLowRes"<< comp_to_rains.LIN_WheelBasedVehicleSpdLowRes << endl;
		comp_to_rains.LIN_RS_Activation_rqst = ((buffer[1] & 0x00000300) >> 8);
		comp_to_rains.LIN_RS_Sensitivity_rqst = ((buffer[1] & 0x3C00) >> 12);
		comp_to_rains.LIN_RS_WasherStatus = ((buffer[1] & 0xC000) >> 16);
		comp_to_rains.LIN_RS_WiperParkStatus = ((buffer[1] & 0x30000) >> 18);
		comp_to_rains.LIN_RS_Mode_rqst = ((buffer[1] & 0xC0000) >> 20);
		comp_to_rains.LIN_RS_VehicleType = ((buffer[1] & 0xF00000) >> 24);
		comp_to_rains.LIN_RS_WindScreenType = ((buffer[1] & 0xF000000) >> 28);

	}

	else if(lin_frame_choose == 3)
	{
		rains_to_comp.DiagInfoRAINS = (buffer[1] & 0x0000007F);
		rains_to_comp.ResponseErrorRAINS = ((buffer[1] & 0x00000080) >> 7);
		rains_to_comp.LIN_RS_Wiping_rqst = ((buffer[1] & 0x00000700) >> 8);
		rains_to_comp.LIN_AutoHeadlightSwitch_rqst = ((buffer[1] & 0x00001800) >> 11);
		rains_to_comp.LIN_RS_Mode = ((buffer[1] & 0x00006000) >> 13);
		rains_to_comp.LIN_AmbientLight_stat = ((buffer[1] & 0x7FFF8000) >> 15);
	}
	else
	{
		//Do Nothing
	}
}

void *receive_thread(void *receive_parameter)
{
	int i;
	int *rcv_val;
	protocol Event_input;

	while(1)
	{

		rcv_val = Event_input.input_event();
		pthread_mutex_lock(&mutex1);
		new_msg = false;

		for(i=0;i<2;i++)
		{
			if(*(rcv_val+i) != buffer[i])
			{
				buffer[i] = *(rcv_val+i);
				new_msg = true;
			}
		}
		if(new_msg) {
			pthread_cond_signal(&got_request);
		}
		pthread_mutex_unlock(&mutex1);
	}
	return NULL;
}




void *running_thread(void *running_parameter)
{
	int dummy_value,channel[4],channel_value[4];
	int j,send_value = 0;

	HMI_WasherWiper stalk_call;
	Running_Applications App1;
	WasherWiper initialization;
	Visibilty_Device_Utilities::WindScreen_WiperMotor_Control stop_read;


	initialization.washerwiper_init();

	while(1)
	{
		pthread_mutex_lock(&mutex1);

		while (!new_msg) {
			pthread_cond_wait(&got_request,&mutex1);
		}

		for(j=0;j<1;j++)
		{
			if((buffer[0] & 0x0000FFFF) == 0x1002)
			{
				cout << "Washer Wiper Lin Frame Incoming...!" <<endl;
				App1.Lin_frame_extractor(1);

			}
			else if((buffer[0] & 0x0000FFFF) == 0x0038)
			{
				cout << "Washer Wiper Lin Frame Incoming from Comp to Rain sensor...!" <<endl;
				App1.Lin_frame_extractor(2);

			}
			else if((buffer[0] & 0x0000FFFF) == 0x0014)
			{
				cout << "Washer Wiper Lin Frame Incoming from Rain sensor to Comp Node...!" <<endl;
				App1.Lin_frame_extractor(3);

			}

			else if((buffer[0]) == 4)
			{
				cout << " Wiper Lin Frame Incoming from adapt/virtual stock...!" <<endl;
				App1.Lin_frame_extractor(1);
				new_wiping_req = true;

			}
			else if((buffer[0]) == 5)
			{
				cout << " Washer Lin Frame Incoming from adapt/virtual stock...!" <<endl;
				App1.Lin_frame_extractor(1);
				new_washing_req = true;

			}
			else
			{
				channel[j] = ((buffer[j] & 0x00FF0000) >> 16);
				channel_value [j]= ((buffer[j] & 0x0000FF00) >> 8);

				send_value = buffer[j];

				stop_read.Wiper_Stop_Position(send_value);

				//cout << "Channel " << channel[j]<<endl;
				//cout <<" Channel Value " << channel_value[j] << endl;
			}

		}
		new_msg = false;
		pthread_mutex_unlock(&mutex1);

		stalk_call.Stock_input(&hmi_input, &new_washing_req, &new_wiping_req); //HMI Input
	}
	return NULL;
}


int main(void)
{
	pthread_t T_receive,T_running,T_initialisor;
	int status1 = 0, status2 = 0, status0 = 0;
	int *data_pointer;


// Creating the receive thread and the main running functionality thread

	/*status0 = pthread_create(&T_initialisor,NULL,initialisor_thread,NULL);

	if(status0 != 0)
	{
		perror("Error creating thread \n");
		exit(-1);
	}
*/

	status1 = pthread_create(&T_receive,NULL,receive_thread,NULL);

	if(status1 != 0)
	{
		perror("Error creating thread \n");
		exit(-1);
	}

	status2 = pthread_create(&T_running,NULL,running_thread,NULL);

	if(status2 != 0)
	{
		perror("Error creating thread \n");
		exit(-1);
	}


	// Waiting for the threads to finish the work

	/*if(pthread_join(T_initialisor,NULL))
	{
		perror("Error Joining Initializor Thread");
		return 2;
	}*/

	if(pthread_join(T_receive,NULL))
	{
		perror("Error Joining Receive Thread");
		return 2;
	}

	if(pthread_join(T_running,NULL))
	{
		perror("Error Joining Running Thread");
		return 2;
	}

	return 0;
}


