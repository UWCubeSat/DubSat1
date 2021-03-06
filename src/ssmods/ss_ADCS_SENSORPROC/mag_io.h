/*
 * mag_io.h
 *
 *  Created on: Mar 29, 2018
 *      Author: djdup
 */

#ifndef MAG_IO_H_
#define MAG_IO_H_

// TODO what should this be?
#define MAG_VALID_RANGE_T 5e-6

#define MAG1_I2CBUS I2CBus1
#define MAG2_I2CBUS I2CBus2

#include "core/debugtools.h"
#include "core/agglib.h"
#include "interfaces/canWrap.h"
#include "sensors/magnetometer.h"
#include "autocode/rtwtypes.h"

// telemetry segment for mag readings in nanoTeslas
TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    int16_t x;
    int16_t y;
    int16_t z;
} mag_segment;

typedef struct {
	hMag handle;
	MagnetometerData *data;
	real32_T *input;
	real32_T *output;
	aggVec_i agg_x;
	aggVec_i agg_y;
	aggVec_i agg_z;
	aggVec_i agg_valid;
	uint8_t last_phase
} MagIO;

void magioInit(MagIO *magio, real32_T *input, real32_T *output, bus_instance_i2c bus);
void magioUpdate(MagIO *magio);
void magioSendBackchannel(MagIO *magio, uint8_t tmlId, uint8_t tlmIdP);
void magioUpdatePhase(uint8_t phase);

void magioInit1();
void magioUpdate1();
void magioSendBackchannel1();
void magioSendCAN1();

void magioInit2();
void magioUpdate2();
void magioSendBackchannel2();
void magioSendCAN2();

// rollcall
void magio1RcPopulate6(rc_adcs_sp_6 *rc);
void magio2RcPopulate6(rc_adcs_sp_6 *rc);
void magio1RcPopulate7(rc_adcs_sp_7 *r);
void magio2RcPopulate8(rc_adcs_sp_8 *r);
void magio1RcPopulate9(rc_adcs_sp_9 *rc);
void magio1RcPopulate10(rc_adcs_sp_10 *rc);
void magio2RcPopulate11(rc_adcs_sp_11 *rc);
void magio2RcPopulate12(rc_adcs_sp_12 *rc);
void magio2RcPopulate13(rc_adcs_sp_13 *rc);

#endif /* MAG_IO_H_ */
