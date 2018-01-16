/*
 * visibility_device_utilities.h
 *
 *  Created on: Jan 6, 2017
 *      Author: emc2
 */

#ifndef VISIBILITY_DEVICE_UTILITIES_H_
#define VISIBILITY_DEVICE_UTILITIES_H_

//bool rain_sensor;
#include "hmi_utilities.h"
#include "Application_Protocol.h"



namespace Visibilty_Device_Utilities
{

	class WasherFluidSensor_Control
	{

		public:
			int GetFluidlevel(void);


	};

	class RainSensor_Control
	{
		public:
			int Get_Rainlevel(void);
			int Set_Rainlevel(int16_t rainsensor_setter);

	};

 	class WindScreen_WiperMotor_Control
	{
		public:
			int WipingMotorRequest(int speed);
			void Wiper_Stop_Position(int return_stop_value);

	};


	class WindScree_WasherMotor_Control
	{
		public:
			int WasherMotorRequest(int Washer_Speed);
	};


}




#endif /* VISIBILITY_DEVICE_UTILITIES_H_ */
