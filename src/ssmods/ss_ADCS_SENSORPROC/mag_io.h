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
#include "interfaces/canWrap.h"

// telemetry segment for mag readings in nanoTeslas
TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    int16_t x;
    int16_t y;
    int16_t z;
} mag_segment;

void magioInit();
void magioUpdate();
void magioSendBackchannel();
void magioSendBackchannelVector(); // send processed vector
void magioSendCAN();

// rollcall
void magioRcPopulate6(rc_adcs_sp_6 *rc);
void magioRcPopulate7(rc_adcs_sp_7 *rc);
void magioRcPopulate8(rc_adcs_sp_8 *rc);
void magioRcPopulate9(rc_adcs_sp_9 *rc);
void magioRcPopulate10(rc_adcs_sp_10 *rc);
void magioRcPopulate11(rc_adcs_sp_11 *rc);
void magioRcPopulate12(rc_adcs_sp_12 *rc);
void magioRcPopulate13(rc_adcs_sp_13 *rc);
void magioRcPopulate14(rc_adcs_sp_14 *rc);

#endif /* MAG_IO_H_ */
