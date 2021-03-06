/*
 * mag_io.c
 *
 *  Created on: Mar 29, 2018
 *      Author: djdup
 */

#include "mag_io.h"
#include "adcs_sensorproc_ids.h"
#include "sensors/magnetometer.h"

#include "core/i2c.h"
#include "core/utils.h"
#include "core/agglib.h"
#include "interfaces/canwrap.h"

#include "autocode/MSP_SP.h"
#include "autocode/rtwtypes.h"

FILE_STATIC MagIO mag1;
FILE_STATIC MagIO mag2;

FILE_STATIC uint8_t isValidRange(float v)
{
	return v < MAG_VALID_RANGE_T && v > -MAG_VALID_RANGE_T;
}

void magioInit(MagIO *magio, real32_T *input, real32_T *output, bus_instance_i2c bus)
{
	magio->handle = magInit(bus);
	magio->input = input;
	magio->output = output;
	magio->last_phase = -1;
	aggVec_init_i_Var(&magio->agg_x);
	aggVec_init_i_Var(&magio->agg_y);
	aggVec_init_i_Var(&magio->agg_z);
	aggVec_init_i(&magio->agg_valid);
}



void magioUpdate(MagIO *magio)
{
	magio->data = magReadXYZData(magio->handle, ConvertToNone);
	float x = magConvertRawToTeslas(magio->data->rawX);
	float y = magConvertRawToTeslas(magio->data->rawY);
	float z = magConvertRawToTeslas(magio->data->rawZ);
	uint8_t valid = isValidRange(x) && isValidRange(y) && isValidRange(z);

	magio->input[0] = x;
	magio->input[1] = y;
	magio->input[2] = z;
	magio->input[3] = valid;
}

void magioSendBackchannel(MagIO *magio, uint8_t tlmId, uint8_t tlmIdP)
{
	mag_segment seg;
	seg.x = magio->data->rawX;
	seg.y = magio->data->rawY;
	seg.z = magio->data->rawZ;
	bcbinPopulateHeader(&seg.header, tlmId, sizeof(seg));
	bcbinSendPacket((uint8_t *) &seg, sizeof(seg));

	sensor_vector_segment s;
	s.x = magio->output[0];
	s.y = magio->output[1];
	s.z = magio->output[2];
	s.valid = magio->output[3];
	bcbinPopulateHeader(&s.header, tlmIdP, sizeof(s));
	bcbinSendPacket((uint8_t *) &s, sizeof(s));
}

void magioInit1()
{
	magioInit(&mag1, rtU.mag1_vec_body_T, rtY.mag1_body_processed_T, MAG1_I2CBUS);
}

void magioInit2()
{
	magioInit(&mag2, rtU.mag2_vec_body_T, rtY.mag2_body_processed_T, MAG2_I2CBUS);
}

void magioUpdate1()
{
	magioUpdate(&mag1);
}

void magioUpdate2()
{
	magioUpdate(&mag2);
}

void magioSendBackchannel1()
{
	magioSendBackchannel(&mag1, TLM_ID_MAG1_RAW, TLM_ID_MAG1_FILTERED);
}

void magioSendBackchannel2()
{
	magioSendBackchannel(&mag2, TLM_ID_MAG2_RAW, TLM_ID_MAG2_FILTERED);
}

FILE_STATIC void updateProcessedRollcall(MagIO *mag, int16_t x, int16_t y,
		int16_t z, uint8_t valid)
{
    aggVec_push_i(&mag->agg_x, x);
    aggVec_push_i(&mag->agg_y, y);
    aggVec_push_i(&mag->agg_z, z);
    aggVec_push_i(&mag->agg_valid, valid);
}

void magioSendCAN1()
{
    // send packet
	real32_T *output = mag1.output;

    sensorproc_mag p;
    p.sensorproc_mag_x = magConvertTeslasToRaw(output[0]);
    p.sensorproc_mag_y = magConvertTeslasToRaw(output[1]);
    p.sensorproc_mag_z = magConvertTeslasToRaw(output[2]);
    p.sensorproc_mag_valid = output[3];
    p.sensorproc_mag_bdot_valid = mag1.last_phase;
    CANPacket packet;
    encodesensorproc_mag(&p, &packet);
    canSendPacket(&packet);

    // update rollcall arrays
    updateProcessedRollcall(&mag1, p.sensorproc_mag_x, p.sensorproc_mag_y,
    		p.sensorproc_mag_z, p.sensorproc_mag_valid);
}

void magioSendCAN2()
{
    // send packet
	real32_T *output = mag2.output;

    sensorproc_mag2 p;
    p.sensorproc_mag2_x = magConvertTeslasToRaw(output[0]);
    p.sensorproc_mag2_y = magConvertTeslasToRaw(output[1]);
    p.sensorproc_mag2_z = magConvertTeslasToRaw(output[2]);
    p.sensorproc_mag2_valid = output[3];
    p.sensorproc_mag2_bdot_valid =mag2.last_phase;

    CANPacket packet;
    encodesensorproc_mag2(&p, &packet);
    canSendPacket(&packet);

    // update rollcall arrays
    updateProcessedRollcall(&mag2, p.sensorproc_mag2_x, p.sensorproc_mag2_y,
        		p.sensorproc_mag2_z, p.sensorproc_mag2_valid);
}

void magio1RcPopulate6(rc_adcs_sp_6 *r)
{
	r->rc_adcs_sp_6_mag1_valid = aggVec_sum_i(&mag1.agg_valid);
	aggVec_reset((aggVec *) &mag1.agg_valid);
	r->rc_adcs_sp_6_mag1_x_min = aggVec_min_i(&mag1.agg_x);
	aggVec_min_reset((aggVec *) &mag1.agg_x);
}

void magio2RcPopulate6(rc_adcs_sp_6 *r)
{
	r->rc_adcs_sp_6_mag2_valid = aggVec_sum_i(&mag2.agg_valid);
	aggVec_reset((aggVec *) &mag2.agg_valid);
}

void magio1RcPopulate7(rc_adcs_sp_7 *r)
{
    r->rc_adcs_sp_7_mag1_var_x = compressVariance(aggVec_var_i_f(&mag1.agg_x));
    r->rc_adcs_sp_7_mag1_var_y = compressVariance(aggVec_var_i_f(&mag1.agg_y));
    r->rc_adcs_sp_7_mag1_var_z = compressVariance(aggVec_var_i_f(&mag1.agg_z));
}

void magio2RcPopulate8(rc_adcs_sp_8 *r)
{
    r->rc_adcs_sp_8_mag2_var_x = compressVariance(aggVec_var_i_f(&mag2.agg_x));
    r->rc_adcs_sp_8_mag2_var_y = compressVariance(aggVec_var_i_f(&mag2.agg_y));
    r->rc_adcs_sp_8_mag2_var_z = compressVariance(aggVec_var_i_f(&mag2.agg_z));
}

void magio1RcPopulate9(rc_adcs_sp_9 *r)
{
    r->rc_adcs_sp_9_mag1_x_max = aggVec_max_i(&mag1.agg_x);
    aggVec_max_reset((aggVec *) &mag1.agg_x);
    r->rc_adcs_sp_9_mag1_x_avg = aggVec_avg_i_i(&mag1.agg_x);
    aggVec_as_reset((aggVec *) &mag1.agg_x);
    r->rc_adcs_sp_9_mag1_y_min = aggVec_min_i(&mag1.agg_y);
    r->rc_adcs_sp_9_mag1_y_max = aggVec_max_i(&mag1.agg_y);
    aggVec_mm_reset((aggVec *) &mag1.agg_y);
}

void magio1RcPopulate10(rc_adcs_sp_10 *r)
{
    r->rc_adcs_sp_10_mag1_y_avg = aggVec_avg_i_i(&mag1.agg_y);
    aggVec_as_reset((aggVec *) &mag1.agg_y);
    r->rc_adcs_sp_10_mag1_z_min = aggVec_min_i(&mag1.agg_z);
    r->rc_adcs_sp_10_mag1_z_max = aggVec_max_i(&mag1.agg_z);
    r->rc_adcs_sp_10_mag1_z_avg = aggVec_avg_i_i(&mag1.agg_z);
    aggVec_reset((aggVec *) &mag1.agg_z);
}

void magio2RcPopulate11(rc_adcs_sp_11 *r)
{
    r->rc_adcs_sp_11_mag2_x_min = aggVec_min_i(&mag2.agg_x);
    r->rc_adcs_sp_11_mag2_x_max = aggVec_max_i(&mag2.agg_x);
    r->rc_adcs_sp_11_mag2_x_avg = aggVec_avg_i_i(&mag2.agg_x);
    aggVec_reset((aggVec *) &mag2.agg_x);
    r->rc_adcs_sp_11_mag2_y_min = aggVec_min_i(&mag2.agg_y);
    aggVec_min_reset((aggVec *) &mag2.agg_y);
}

void magio2RcPopulate12(rc_adcs_sp_12 *r)
{
    r->rc_adcs_sp_12_mag2_y_max = aggVec_max_i(&mag2.agg_y);
    aggVec_max_reset((aggVec *) &mag2.agg_y);
    r->rc_adcs_sp_12_mag2_y_avg = aggVec_avg_i_i(&mag2.agg_y);
    aggVec_as_reset((aggVec *) &mag2.agg_y);
    r->rc_adcs_sp_12_mag2_z_min = aggVec_min_i(&mag2.agg_z);
    r->rc_adcs_sp_12_mag2_z_max = aggVec_max_i(&mag2.agg_z);
    aggVec_mm_reset((aggVec *) &mag2.agg_z);
}

void magio2RcPopulate13(rc_adcs_sp_13 *r)
{
    r->rc_adcs_sp_13_mag2_z_avg = aggVec_avg_i_i(&mag2.agg_z);
    aggVec_as_reset((aggVec *) &mag2.agg_z);
}

void magioUpdatePhase(uint8_t inphase) {
    mag1.last_phase = inphase;
    mag2.last_phase = inphase;
}
