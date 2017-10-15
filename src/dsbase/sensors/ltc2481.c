/*
 * ltc2481.c
 *
 *  Created on: Oct 8, 2017
 *      Author: jerrolde
 */

#include <stdint.h>
#include "../core/utils.h"
#include "ltc2481.h"

FILE_STATIC uint8_t read[3];
FILE_STATIC uint8_t i2cInitialized = 0;

void ltc2481Init(uint8_t addr)
{
//Connect to the LTC2481 at addr and initialize default settings
	if (i2cInitialized != 0)
		return;
	
	i2cInitialized = 1;
	i2cEnable();
	i2cInit(addr);
	i2cRawWrite(defaultWrite, 1);
}

double ltc2481Voltage()
{
//Return voltage reading from the LTC2481 as a double
	return ltc2481Read(defaultWrite);
}

double ltc2481Temperature()
{
//Return temperature reading from the LTC2481 as a double
    volatile double temp = ltc2481Read(temperature);
	temp = (temp / 0.0014) - 273.15;
	return temp;
}

double ltc2481Read(uint8_t write)
{
	volatile double reading;
	volatile uint32_t x;
	i2cCombinedAddressWriteThenRead(write, read, 3);
    read[2] &= 0b10000000;
    read[0] &= 0b01111111;
    x = 0;
    x |= read[0];
    x = x << 8;
    x |= read[1];
    x = x << 1;
    x |= (read[2] >> 7);
    if (read[0] >> 6) {
		x = ~x + 1;
        x &= 0x0000FFFF;
    }
	//x contains the positive reading in the format of [16 zeroes,16-bit number]
    reading = (double) x;
    reading = reading * 3.3 / 65536.0;
	
	return reading;
}
