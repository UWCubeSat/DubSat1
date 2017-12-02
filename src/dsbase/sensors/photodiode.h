/*
 * ltc2481.h
 *
 *  Created on: Oct 8, 2017
 *      Author: jerrolde
 */

#include <stdint.h>
#include "../core/i2c.h"

#define defaultWrite    0x00
#define temperature     0x08

void ltc2481Init(uint8_t addr);
double ltc2481Read(uint8_t write);
double ltc2481Voltage();
double ltc2481Temperature();
