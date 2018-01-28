/*
 * photodiode.h
 *
 *  Created on: Oct 8, 2017
 *      Author: jerrolde
 */

#include <stdint.h>
#include "../core/i2c.h"

#define defaultWrite    0x00 // all default settings
#define temperatureWrite     0x08 //all default settings w/ temperature read
#define GAIN1 0b00000000 // G = 1
#define GAIN2 0b00100000 // G = 4
#define GAIN3 0b01000000 // G = 8
#define GAIN4 0b01100000 // G = 16
#define GAIN5 0b10000000 // G = 32
#define GAIN6 0b10100000 // G = 64
#define GAIN7 0b11000000 // G = 128
#define GAIN8 0b11100000 // G = 256


uint8_t photodiodeInit(uint8_t addr);
uint32_t photodiodeVoltage(uint8_t handle, uint8_t gain);
double photodiodeVoltageDouble(uint8_t handle, uint8_t gain);
double photodiodeTemperature(uint8_t handle);
uint32_t photodiodeRead(uint8_t write, uint8_t handle);
double photodiodeReadDouble(uint8_t write, uint8_t handle);

typedef struct
{
	uint8_t addr;
	hDev handle;
	uint32_t lastVoltage;
	double lastTemperature;
} PhotodiodeData;
