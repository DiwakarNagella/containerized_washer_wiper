/*
 * hmi_utilities.cpp
 *
 *  Created on: Jan 6, 2017
 *      Author: emc2
 */

#include "hmi_utilities.h"
#include "Application_Protocol.h"
#include <iostream>
#include <sstream>
#include "stdlib.h"
#include "WasherwiperApplication.h"
#include "visibility_utilities.h"
#include "LIN_signals.h"

using namespace std;
using namespace Ethernet_Packet;
using namespace application_protocol;
using namespace Visibility_Utilities;
using namespace HMI_Utilities;

/* CHANNEL LIST */
#define CHANNEL_STALK 3
#define INVALID_VALUE  NULL

//lindata_SM1_HMIIOM hmi_input;
//lindata_to_RComp rains_to_comp;
//lindata_to_CRains comp_to_rains;

namespace HMI_Utilities
{
	void HMI_WasherWiper::Stock_input(lindata_SM1_HMIIOM *hmi_input, bool* washer, bool* wiper)
	{
		WasherWiper Application_start;

		static uint8_t wiper_flag;
		uint8_t  stalk_value;

		if(*wiper == true)
		{
			if((hmi_input->LIN_WiperStalkStat_1 <= 4) && (hmi_input->LIN_WiperStalkStat_1 >= 0 ))
			{
				if(hmi_input->LIN_WiperStalkStat_1 == 1) // Single Sweep
				{
					stalk_value = 1;
					wiper_flag = 1;
				}
				else if((hmi_input->LIN_WiperStalkStat_1 == 3)||(rains_to_comp.LIN_RS_Wiping_rqst == 1)) // Continuous Sweep Slow
				{
					stalk_value = 2;
					wiper_flag = 2;
				}
				else if((hmi_input->LIN_WiperStalkStat_1 == 4)||(rains_to_comp.LIN_RS_Wiping_rqst == 2)) // Continuous Sweep Fast
				{
					stalk_value = 3;
					wiper_flag = 3;
				}
				else if((hmi_input->LIN_WiperStalkStat_1 == 0) ||(rains_to_comp.LIN_RS_Wiping_rqst == 0)) // Sweep Stop
				{
					stalk_value = 4;
					wiper_flag = 4;
				}
				else //any other value put the wiper off
				{
					stalk_value = 4;
				}

				Application_start.washerwiper_start(stalk_value);
				*wiper = false;
			}
		}

		if(*washer == true)
		{
			if ((hmi_input->LIN_Washing_Status_1 >= 0) && (hmi_input->LIN_Washing_Status_1 <= 1))
			{

				if(hmi_input->LIN_Washing_Status_1 == 0x01)
				{
					stalk_value = 5;
				}
				else if(hmi_input->LIN_Washing_Status_1 == 0x00)
				{
					stalk_value = 6;
				}
				else //any other value, put the washer off
				{
					stalk_value = 6;
				}

				Application_start.washerwiper_start(stalk_value);
				*washer = false;
			}
		}

	}

	HMI_WasherWiper::HMI_WasherWiper()
	{
		stalk_input = 0;
	}
}
