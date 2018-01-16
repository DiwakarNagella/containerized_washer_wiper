/*
 * Washerwiper_old.h
 *
 *  Created on: Jan 11, 2017
 *      Author: emc2
 */

#ifndef WASHERWIPER_OLD_H_
#define WASHERWIPER_OLD_H_

#include "sys/types.h"

#define WIPER_SINGLE_SWEEP 1
#define WIPER_MULTISWEEP_SLOW 2
#define WIPER_MULTISWEEP_FAST 3
#define WIPER_STOP 4
#define WASHER_START 5
#define WASHER_STOP 6

class Running_Applications
{

	public:

		void Lin_frame_extractor(int lin_frame_choose);



};



#endif /* WASHERWIPER_OLD_H_ */
