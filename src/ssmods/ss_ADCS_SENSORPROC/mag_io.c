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

typedef MagnetometerData (* mag_read_fn)(hMag handle, UnitConversionMode desiredConversion);
#if __I2C_DONT_READ_MAG__
	FILE_STATIC mag_read_fn magRead = testing_magReadXYZData;
#else
	FILE_STATIC mag_read_fn magRead = magReadXYZData;
#endif

FILE_STATIC uint8_t isValidRange(int16_t v)
{
	return v < MAG_VALID_RANGE_RAW && v > -MAG_VALID_RANGE_RAW;
}

void magioInit(MagIO *magio, real32_T *input, real32_T *output, bus_instance_i2c bus)
{
	magio->handle = magInit(bus);
	magio->input = input;
	magio->output = output;
	aggVec_init_i(&magio->agg_x);
	aggVec_init_i(&magio->agg_y);
	aggVec_init_i(&magio->agg_z);
	aggVec_init_i(&magio->agg_px);
	aggVec_init_i(&magio->agg_py);
	aggVec_init_i(&magio->agg_pz);
}

void magioUpdate(MagIO *magio)
{
	magio->data = magRead(magio->handle, ConvertToNone);
	int16_t x = magConvertRawToTeslas(magio->data->rawX);
	int16_t y = magConvertRawToTeslas(magio->data->rawY);
	int16_t z = magConvertRawToTeslas(magio->data->rawZ);

	magio->input[0] = x;
	magio->input[1] = y;
	magio->input[2] = z;
	magio->input[3] = isValidRange(x) && isValidRange(y) && isValidRange(z);

	aggVec_push_i(&magio->agg_x, x);
	aggVec_push_i(&magio->agg_y, y);
	aggVec_push_i(&magio->agg_z, z);
}

void magioSendBackchannel(MagIO *magio, uint8_t tmlId, uint8_t tlmIdP)
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
	magioInit(&mag1, rtU.mag1_vec_body_T, rtY.mag_body_processed_T, MAG1_I2CBUS);
}

void magioInit2()
{
	// TODO use mag2 rtY
	magioInit(&mag2, rtU.mag2_vec_body_T, rtY.mag_body_processed_T, MAG2_I2CBUS);
}

void magioUpdate1()
{
	magioUpdate(&mag1);
}

void magioUpdate2()
{
	magioUpdate(&mag2);
}

magioSendBackchannel1()
{
	// TODO use mag1p backchannel
	magioSendBackchannel(&mag1, TLM_ID_MAG1_RAW, TLM_ID_MAG_VECTOR);
}

magioSendBackchannel2()
{
	// TODO use mag2p backchannel
	magioSendBackchannel(&mag2, TLM_ID_MAG2_RAW, TLM_ID_MAG_VECTOR);
}

void magioSendCAN1()
{
	// TODO
}

void magioHandleCAN1(CANPacket *p)
{
	// TODO
}

void magioSendCAN2()
{
	// TODO
}

void magioHandleCAN2(CANPacket *p)
{
	// TODO
}

void magioSendCAN()
{
    // send packet
    sensorproc_mag mag;
    mag.sensorproc_mag_x = magConvertTeslasToRaw(rtY.mag_body_processed_T[0]);
    mag.sensorproc_mag_y = magConvertTeslasToRaw(rtY.mag_body_processed_T[1]);
    mag.sensorproc_mag_z = magConvertTeslasToRaw(rtY.mag_body_processed_T[2]);
    mag.sensorproc_mag_valid = rtY.mag_body_processed_T[3];
    CANPacket packet;
    encodesensorproc_mag(&mag, &packet);
    canSendPacket(&packet);

    // update rollcall arrays
    aggVec_push_i(&rc_magpx, mag.sensorproc_mag_x);
    aggVec_push_i(&rc_magpy, mag.sensorproc_mag_y);
    aggVec_push_i(&rc_magpz, mag.sensorproc_mag_z);
    aggVec_push_i(&rc_magValid, mag.sensorproc_mag_valid);
}

void magioHandleCAN(CANPacket *p)
{
	mtq_ack ack;
	if (p->id == CAN_ID_MTQ_ACK)
	{
		decodemtq_ack(p, &ack);
		mtqPhase = ack.mtq_ack_phase;
	}
}

void magioRcPopulate6(rc_adcs_sp_6 *r)
{
    r->rc_adcs_sp_6_magp_x_min = aggVec_min_i(&rc_magpx);
    r->rc_adcs_sp_6_magp_x_max = aggVec_max_i(&rc_magpx);
    aggVec_mm_reset((aggVec *) &rc_magpx);
}

void magioRcPopulate7(rc_adcs_sp_7 *r)
{
    r->rc_adcs_sp_7_magp_x_avg = aggVec_avg_i_i(&rc_magpx);
    aggVec_as_reset((aggVec *) &rc_magpx);
    r->rc_adcs_sp_7_magp_y_min = aggVec_min_i(&rc_magpy);
    r->rc_adcs_sp_7_magp_y_max = aggVec_max_i(&rc_magpy);
    r->rc_adcs_sp_7_magp_y_avg = aggVec_avg_i_i(&rc_magpy);
    aggVec_reset((aggVec *) &rc_magpx);
}

void magioRcPopulate8(rc_adcs_sp_8 *r)
{
    r->rc_adcs_sp_8_magp_z_min = aggVec_min_i(&rc_magpz);
    r->rc_adcs_sp_8_magp_z_max = aggVec_max_i(&rc_magpz);
    r->rc_adcs_sp_8_magp_z_avg = aggVec_avg_i_i(&rc_magpz);
    aggVec_reset((aggVec *) &rc_magpz);
    r->rc_adcs_sp_8_mag1_x_min = aggVec_min_i(&rc_mag1x);
    aggVec_min_reset((aggVec *) &rc_mag1x);
}

void magioRcPopulate9(rc_adcs_sp_9 *r)
{
    r->rc_adcs_sp_9_mag1_x_max = aggVec_max_i(&rc_mag1x);
    aggVec_max_reset((aggVec *) &rc_mag1x);
    r->rc_adcs_sp_9_mag1_x_avg = aggVec_avg_i_i(&rc_mag1x);
    aggVec_as_reset((aggVec *) &rc_mag1x);
    r->rc_adcs_sp_9_mag1_y_min = aggVec_min_i(&rc_mag1y);
    r->rc_adcs_sp_9_mag1_y_max = aggVec_max_i(&rc_mag1y);
    aggVec_mm_reset((aggVec *) &rc_mag1y);
}

void magioRcPopulate10(rc_adcs_sp_10 *r)
{
    r->rc_adcs_sp_10_mag1_y_avg = aggVec_avg_i_i(&rc_mag1y);
    aggVec_as_reset((aggVec *) &rc_mag1y);
    r->rc_adcs_sp_10_mag1_z_min = aggVec_min_i(&rc_mag1z);
    r->rc_adcs_sp_10_mag1_z_max = aggVec_max_i(&rc_mag1z);
    r->rc_adcs_sp_10_mag1_z_avg = aggVec_avg_i_i(&rc_mag1z);
    aggVec_reset((aggVec *) &rc_mag1z);
}

void magioRcPopulate11(rc_adcs_sp_11 *r)
{
    r->rc_adcs_sp_11_mag2_x_min = aggVec_min_i(&rc_mag2x);
    r->rc_adcs_sp_11_mag2_x_max = aggVec_max_i(&rc_mag2x);
    r->rc_adcs_sp_11_mag2_x_avg = aggVec_avg_i_i(&rc_mag2x);
    aggVec_reset((aggVec *) &rc_mag2x);
    r->rc_adcs_sp_11_mag2_y_min = aggVec_min_i(&rc_mag2y);
    aggVec_min_reset((aggVec *) &rc_mag2y);
}

void magioRcPopulate12(rc_adcs_sp_12 *r)
{
    r->rc_adcs_sp_12_mag2_y_max = aggVec_max_i(&rc_mag2y);
    aggVec_max_reset((aggVec *) &rc_mag2y);
    r->rc_adcs_sp_12_mag2_y_avg = aggVec_avg_i_i(&rc_mag2y);
    aggVec_as_reset((aggVec *) &rc_mag2y);
    r->rc_adcs_sp_12_mag2_z_min = aggVec_min_i(&rc_mag2z);
    r->rc_adcs_sp_12_mag2_z_max = aggVec_max_i(&rc_mag2z);
    aggVec_mm_reset((aggVec *) &rc_mag2z);
}

void magioRcPopulate13(rc_adcs_sp_13 *r)
{
    r->rc_adcs_sp_13_mag2_z_avg = aggVec_avg_i_i(&rc_mag2z);
    aggVec_as_reset((aggVec *) &rc_mag2z);
}

void magioRcPopulate14(rc_adcs_sp_14 *r)
{
    r->rc_adcs_sp_14_magp_valid = aggVec_sum_i(&rc_magValid);
    aggVec_reset((aggVec *) &rc_magValid);
}
