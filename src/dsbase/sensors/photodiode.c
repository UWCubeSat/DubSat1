/*
 * photodiode.c
 *
 *  Created on: Oct 8, 2017
 *      Author: jerrolde
 */

#include <stdint.h>
#include "../core/utils.h"
#include "photodiode.h"

FILE_STATIC uint8_t read[3];
FILE_STATIC PhotodiodeData pdData[8];
FILE_STATIC uint8_t currIndex = 0;

uint8_t photodiodeInit(uint8_t addr)
{
//Connect to the LTC2481 at addr and initialize default settings
	//TODO: add i2cInitialized check
	/*if (i2cInitialized != 0) 
	{
		return;
	}
	i2cInitialized = 1;*/
	i2cEnable(I2CBus2);
	hDev hSensor = i2cInit(I2CBus2, addr);
	pdData[currIndex].addr = addr;
	pdData[currIndex].handle = hSensor;
	pdData[currIndex].i2cInitialized = 1;
	i2cMasterWrite(hSensor, defaultWrite, 1);
	return currIndex++;
}

double photodiodeVoltage(uint8_t handle)
{
//Return voltage reading from the LTC2481 as a double
	volatile double temp;
	temp = photodiodeRead(defaultWrite, handle);
	pdData[handle].lastVoltage = temp;
	return temp;
}

double photodiodeTemperature(uint8_t handle)
{
//Return temperature reading from the LTC2481 as a double
    volatile double temp = photodiodeRead(temperature, handle);
	temp = (temp / 0.0014) - 273.15;
	pdData[handle].lastTemperature = temp;
	return temp;
}

double photodiodeRead(uint8_t write, uint8_t handle)
{
	volatile double reading;
	volatile uint32_t x;
	//i2cMasterRegisterRead(handle, write, read, 3);
	i2cMasterCombinedWriteRead(pdData[handle].handle, &write, 1, &read[0], 3);
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
