/*
 * visibility_device_utilites.cpp
 *
 *  Created on: Jan 6, 2017
 *      Author: emc2
 */

#include "visibility_device_utilites.h"
#include "Application_Protocol.h"
#include <iostream>
#include <sstream>
#include "stdlib.h"
#include "LIN_signals.h"

using namespace std;
using namespace Ethernet_Packet;
using namespace application_protocol;

/* CHANNEL LIST */
#define CHANNEL_WIPER 1
#define CHANNEL_WASHER 2
#define RAIN_SENSOR 4

bool stop_value_flag;
//lindata_to_CRains comp_to_rains;


namespace Visibilty_Device_Utilities
{

	int WasherFluidSensor_Control::GetFluidlevel()
	{
		int level_value = 0;

		return level_value;
	}
/*
	int RainSensor_Control::Get_Rainlevel()
	{
		uint16_t value_rain = 0;

		value_rain =

		return value_rain;
	}*/

	int RainSensor_Control::Set_Rainlevel(int16_t rainsensor_setter)
	{
		int rainsensor_data[2];
		int rainsensor_return;
		int8_t app = 0x01;

		application_protocol::protocol set_rain_sensor;

		comp_to_rains.LIN_WheelBasedVehicleSpdLowRes = 0x00;
		comp_to_rains.LIN_RS_Activation_rqst = rainsensor_setter;
		comp_to_rains.LIN_RS_Sensitivity_rqst = 0x00;
		comp_to_rains.LIN_RS_WasherStatus = 0x00;
		comp_to_rains.LIN_RS_WiperParkStatus = 0x00;
		comp_to_rains.LIN_RS_Mode_rqst = 0x00;
		comp_to_rains.LIN_RS_VehicleType = 0x00;
		comp_to_rains.LIN_RS_WindScreenType = 0x00;


		rainsensor_data[0] = 0x38;

		if(rainsensor_setter == 0)
		{
			rainsensor_data[1] = 0x000; // combining the entire linframe into one integer value
		}
		else if(rainsensor_setter == 1)
		{
			rainsensor_data[1] = 0x100; // combining the entire linframe into one integer value
		}
		else
		{
			//Do Nothing

		}
		//rainsensor_data[2] = 0x00;
		//rainsensor_data[4] = 0x00;

		rainsensor_return = set_rain_sensor.lindata_send(app, rainsensor_data);

		return rainsensor_return;
	}

	void WindScreen_WiperMotor_Control::Wiper_Stop_Position(int return_stop_value )
	{
		static int stop_value, op_code, channel_no, dummy ;
		int return_svalue = 0;

		Ethernet_Packet::UDP_protocol receive_stoplevel;

		return_svalue = return_stop_value;

		op_code = ((return_stop_value & 0xFF000000)>>24);
		channel_no = ((return_stop_value & 0x00FF0000)>>16);

		if((op_code == 10) && (channel_no == CHANNEL_WIPER))
		{
			stop_value = (return_stop_value & 0x000000FF);
			stop_value_flag = 1;
		}
		else
		{
			//Do Nothing
			stop_value_flag = 0;
		}

	}

	int WindScreen_WiperMotor_Control::WipingMotorRequest(int speed)
	{
		int wiper_channel = 0;
		int wiper_state = 0;
		int wiper_ack=0;
		int stop_condition = 0;

		protocol wiper_call;

		if(speed == 1)
		{
			wiper_channel = CHANNEL_WIPER;
			wiper_state = 1;
			wiper_ack=wiper_call.set_channel_state(wiper_channel,wiper_state);

		}
		else if(speed == 2)
		{
			wiper_channel = CHANNEL_WIPER;
			wiper_state = 2;
			wiper_ack = wiper_call.set_channel_state(wiper_channel,wiper_state);

		}
		else if(speed == 0)
		{
			wiper_channel = CHANNEL_WIPER;

//			while(1)
//			{
//				if(0 == stop_value_flag)
//				{
//					//Do Nothing
//				}
//				else if(stop_value_flag == 1)
//				{
					wiper_state = 0;
					wiper_ack = wiper_call.set_channel_state(wiper_channel,wiper_state);
					//break;
//				}
//				else
//				{
//					//Do Nothing
//				}

			//}

		}
		else
		{
			// Do Nothing
		}

		return wiper_ack;
	}


	int WindScree_WasherMotor_Control::WasherMotorRequest(int WasherSpeed)
	{

		int washer_channel = 0;
		int washer_state = 0;
		int washer_ack = 0;

		protocol washer_call;

		washer_channel = CHANNEL_WASHER;
		washer_state = WasherSpeed;

		washer_ack = washer_call.set_channel_state(washer_channel,washer_state);

		return washer_ack;
	}

}


