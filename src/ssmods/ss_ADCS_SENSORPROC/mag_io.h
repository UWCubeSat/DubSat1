/*
 * mag_io.h
 *
 *  Created on: Mar 29, 2018
 *      Author: djdup
 */

#ifndef MAG_IO_H_
#define MAG_IO_H_

#define DELAY_MAG_UPDATE_MS 200

// TODO confirm this bus after descope board redesign
#define MAG_I2CBUS I2CBus1

#include "core/debugtools.h"

// telemetry segment for mag readings in nanoTeslas
TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    int16_t x;
    int16_t y;
    int16_t z;
} mag_segment;

void magioInit();
void magioUpdate();

#endif /* MAG_IO_H_ */
