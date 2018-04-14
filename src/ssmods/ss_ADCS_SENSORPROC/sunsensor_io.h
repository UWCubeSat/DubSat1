/*
 * sunsensor_io.h
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#ifndef SUNSENSOR_IO_H_
#define SUNSENSOR_IO_H_

// sun sensor i2c bus
#define I2C_BUS_SUNSENSOR I2CBus1

#include <stdint.h>

#include "core/utils.h"

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    float alpha;
    float beta;
    uint8_t error;
} sunsensor_segment;

void sunsensorioInit();
void sunsensorioUpdate();
void sunsensorioSendBackchannel();
void sunsensorioSendCAN();

#endif /* SUNSENSOR_IO_H_ */
