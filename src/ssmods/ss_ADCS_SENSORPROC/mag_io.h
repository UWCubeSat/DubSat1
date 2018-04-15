/*
 * mag_io.h
 *
 *  Created on: Mar 29, 2018
 *      Author: djdup
 */

#ifndef MAG_IO_H_
#define MAG_IO_H_

#define DELAY_MAG_UPDATE_MS 200

#define MAG1_I2CBUS I2CBus1
#define MAG2_I2CBUS I2CBus2

#include "core/debugtools.h"

// telemetry segment for mag readings in nanoTeslas
TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    int16_t x;
    int16_t y;
    int16_t z;
} mag_segment;

void magioInit1();
void magioInit2();
void magioUpdate1();
void magioUpdate2();
void magioSendBackchannel1();
void magioSendBackchannel2();
void magioSendCAN();

#endif /* MAG_IO_H_ */
