/*----------------------------------------------------------------------------
 Header file for MAXIM DS1631 2-wire temperature sensor
 *----------------------------------------------------------------------------*/
#ifndef DS1631_H
#define DS1613_H

#include "mbed.h"

class DS1631{
	public:
		DS1631(PinName sda, PinName scl, int addr);
	  float read();
	
	private:
		I2C temp_sensor;
		int _addr;
};

#endif

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
