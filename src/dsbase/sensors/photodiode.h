/*
 * photodiode.h
 *
 *  Created on: Oct 8, 2017
 *      Author: jerrolde
 */

#include <stdint.h>
#include "../core/i2c.h"

#define defaultWrite    0x00
#define temperature     0x08

uint8_t photodiodeInit(uint8_t addr);
double photodiodeRead(uint8_t write, uint8_t handle);
double photodiodeVoltage(uint8_t handle);
double photodiodeTemperature(uint8_t handle);

typedef struct
{
	uint8_t addr;
    uint8_t i2cInitialized;
	hDev handle;
	double lastVoltage;
	double lastTemperature;
} PhotodiodeData;