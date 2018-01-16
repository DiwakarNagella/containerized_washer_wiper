/*
 * hmi_utilities.h
 *
 *  Created on: Jan 6, 2017
 *      Author: emc2
 */

#ifndef HMI_UTILITIES_H_
#define HMI_UTILITIES_H_

#include <iostream>
#include "Washerwiper_old.h"
#include <sstream>
#include "Application_Protocol.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "LIN_signals.h"


namespace HMI_Utilities
{

	class HMI_WasherWiper
	{

		public:
			int8_t stalk_input;
			void Stock_input(lindata_SM1_HMIIOM *hmi_input, bool* washer, bool* wiper);
			HMI_WasherWiper();

	};
}




#endif /* HMI_UTILITIES_H_ */
