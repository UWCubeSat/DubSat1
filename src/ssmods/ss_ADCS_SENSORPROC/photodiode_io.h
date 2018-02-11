/*
 * photodiode_io.h
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#ifndef PHOTODIODE_IO_H_
#define PHOTODIODE_IO_H_

#include <stdint.h>

#include "core/i2c.h"

// photodiode addresses
//both addr lines wired to vcc
#define PD_ADDR_LH 0x14
#define PD_ADDR_LF 0x15
#define PD_ADDR_FH 0x17
#define PD_ADDR_FF 0x24
#define PD_ADDR_HH 0x26
#define PD_ADDR_HF 0x27

// photodiode i2c bus
#define I2C_BUS_PHOTODIODES I2CBus1

void photodiodeioInit();
void photodiodeioUpdate();
void photodiodeioSendData();

#endif /* PHOTODIODE_IO_H_ */
