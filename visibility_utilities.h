/*
 * visibility_utilities.h
 *
 *  Created on: Jan 6, 2017
 *      Author: emc2
 */

#ifndef VISIBILITY_UTILITIES_H_
#define VISIBILITY_UTILITIES_H_

#include "sys/types.h"

#define LOW_SPEED 1
#define HIGH_SPEED 2
#define ON 1
#define OFF 0


namespace Visibility_Utilities
{

	class WasherWiper_Controller
	{
		public:
			int GetWasherFluidLevel(void);
			int Get_RainSensor_Value(void);
			int Set_RainSensor_Value(int16_t rainsensor_value);

			int WipingRequest(int WiperState);
			int WashingRequest(bool WasherState);


			/* Newly Added */
			static bool washer_flag;
			static int wiper_flag;

	};


}




#endif /* VISIBILITY_UTILITIES_H_ */
