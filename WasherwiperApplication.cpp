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


using namespace std;
using namespace Visibility_Utilities;
using namespace Ethernet_Packet;
using namespace HMI_Utilities;

bool flag = false;

void WasherWiper::washerwiper_init()
{
	Visibility_Utilities::WasherWiper_Controller WasherWier_init;

	//WasherWier_init.Set_RainSensor_Value(OFF);
	//WasherWier_init.WashingRequest(OFF);
	//WasherWier_init.WipingRequest(OFF);
	WasherWier_init.Set_RainSensor_Value(ON);
	//sleep(1);
	cout << "Init Function call" <<endl;

}


void WasherWiper::washerwiper_start(int8_t value)
{
	int wiper_input=0, ack = 0;
	bool washer_input=0;


	Visibility_Utilities::WasherWiper_Controller WasherWiper1;

	if(flag == false)
	{

		//WasherWiper1.Set_RainSensor_Value(ON);
		//cout << "Init Function call" <<endl;

		flag = true;
	}

	//cout <<"***************************** Washer Wiper Application ***************************"<< endl;

	switch(value)
		{

			case 1:
				wiper_input = 2;
				ack = WasherWiper1.WipingRequest(wiper_input);
				if(ack == 0)
				{
					wiper_input = 0;
					ack = WasherWiper1.WipingRequest(wiper_input);
				}
				else
				{

				}
				cout << "Single Sweep !"<< endl;
			break;

			case 2:
				wiper_input = 2;
				ack = WasherWiper1.WipingRequest(wiper_input);
				cout << "Continuous Slow Sweep !"<<endl;
			break;

			case 3:
				wiper_input = 1;
				ack = WasherWiper1.WipingRequest(wiper_input);
				cout <<"Continuous Fast Sweep!" << endl;
			break;

			case 4:
				wiper_input = 0;
				ack = WasherWiper1.WipingRequest(wiper_input);
				cout<<"Wiper Sweep OFF !"<<endl;
			break;

			case 5:
				washer_input = 1;
				ack = WasherWiper1.WashingRequest(washer_input);
				cout <<"Washer ON !"<< endl;

			break;

			case 6:
				washer_input = 0;
				ack = WasherWiper1.WashingRequest(washer_input);
				cout <<"Washer OFF !"<< endl;
			break;

			default:
				cout <<"Default case !" <<endl;
				break;

		}
}



