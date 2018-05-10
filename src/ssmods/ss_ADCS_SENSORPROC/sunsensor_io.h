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
#include "interfaces/canWrap.h"

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

// rollcall
void sunsensorioRcPopulate4(rc_adcs_sp_4 *rc);
void sunsensorioRcPopulate5(rc_adcs_sp_5 *rc);
void sunsensorioRcPopulate6(rc_adcs_sp_6 *rc);
void sunsensorioRcPopulate13(rc_adcs_sp_13 *rc);
void sunsensorioRcPopulate14(rc_adcs_sp_14 *rc);

#endif /* SUNSENSOR_IO_H_ */
