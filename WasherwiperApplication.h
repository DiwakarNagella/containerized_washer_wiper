/*
 * WasherWiperApplication.h
 *
 *  Created on: Oct 6, 2016
 *      Author: root
 */

#ifndef WASHERWIPERAPPLICATION_H_
#define WASHERWIPERAPPLICATION_H_


#define FLUID_MAXIMUM 100
#define FLUID_MINIMUM 10
#define RAIN_MAXIMUM 100
#define RAIN_MINIMUM 10


#define WIPER_SINGLE_SWEEP 1
#define WIPER_MULTISWEEP_SLOW 2
#define WIPER_MULTISWEEP_FAST 3
#define WIPER_STOP 4
#define WASHER_START 5
#define WASHER_STOP 6

class WasherWiper
{
	public:
		void washerwiper_init(void);
		void washerwiper_start(int8_t value);
};




#endif /* WASHERWIPERAPPLICATION_H_ */

