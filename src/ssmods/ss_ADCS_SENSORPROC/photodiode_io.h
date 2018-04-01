/*
 * photodiode_io.h
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#ifndef PHOTODIODE_IO_H_
#define PHOTODIODE_IO_H_

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
#define PD_MAX_VAL 32768 // 2^15

#include <stdint.h>

#include "core/i2c.h"
#include "core/utils.h"

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint32_t center;
    uint32_t right;
    uint32_t left;
    uint8_t cCenter;
    uint8_t cRight;
    uint8_t cLeft;
} photodiode_segment;

void photodiodeioInit();
void photodiodeioUpdate();
void photodiodeioSendData();

#endif /* PHOTODIODE_IO_H_ */
