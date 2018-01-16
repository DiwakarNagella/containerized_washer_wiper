/*
 * visibility_utilities.cpp
 *
 *  Created on: Jan 6, 2017
 *      Author: emc2
 */


#include "visibility_utilities.h"
#include "visibility_device_utilites.h"
#include "Application_Protocol.h"
#include <iostream>
#include <sstream>
#include "sys/types.h"
#include "stdlib.h"

using namespace std;
using namespace Ethernet_Packet;
using namespace application_protocol;

/* CHANNEL LIST */
#define CHANNEL_WIPER 1
#define CHANNEL_WASHER 2
#define CHANNEL_STOCK 3

namespace Visibility_Utilities
{
	int WasherWiper_Controller::GetWasherFluidLevel(void)
	{
		int level = 0;

		Visibilty_Device_Utilities::WasherFluidSensor_Control fluid_value;

		level = fluid_value.GetFluidlevel();

		// Translate the Raw values to control values

		return level;
	}

/*	int WasherWiper_Controller::Get_RainSensor_Value()
	{
		int Rain_level = 0;

		Visibilty_Device_Utilities::RainSensor_Control rain_sense;

		//Rain_level = rain_sense.Get_Rainlevel();

		// Translate the Raw values to control values

		return Rain_level;
	}*/

	int WasherWiper_Controller::Set_RainSensor_Value(int16_t rainsensor_value)
	{
		int ack = 0;

		Visibilty_Device_Utilities::RainSensor_Control rainsensor_setting;

		ack = rainsensor_setting.Set_Rainlevel(rainsensor_value);

		return ack;
	}


	int WasherWiper_Controller::WipingRequest(int WiperState)
	{

		int speed_value = 0,Ack = 0;

		Visibilty_Device_Utilities::WindScreen_WiperMotor_Control wipercontrol1;

		if(WiperState == 1)
		{
			speed_value = LOW_SPEED;
		}
		else if (WiperState == 2)
		{
			speed_value = HIGH_SPEED;
		}
		else if(WiperState == 0)
		{
			speed_value = OFF;
		}

		Ack = wipercontrol1.WipingMotorRequest(speed_value);

		return Ack;

	}
	int WasherWiper_Controller::WashingRequest(bool WasherState)
	{
		int value = 0,Ack = 0;

		Visibilty_Device_Utilities::WindScree_WasherMotor_Control washercontrol;

		if(WasherState == 1)
		{
			value = ON;
		}
		else if (WasherState == 0)
		{
			value = OFF;
		}
		else
		{
			// Do Nothing
		}

		Ack = washercontrol.WasherMotorRequest(value);

		return Ack;
	}

}
