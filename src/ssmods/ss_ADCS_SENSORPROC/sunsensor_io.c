/*
 * sunsensor_io.c
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#define UNIT_TO_INT16(U) ((int16_t) ((U) * INT16_MAX))

#include <math.h>

#include "sunsensor_io.h"
#include "sensors/sun_sensor.h"
#include "interfaces/canwrap.h"
#include "core/dataArray.h"
#include "adcs_sensorproc_ids.h"

#include "autocode/MSP_SP.h"

// rollcall arrays
FILE_STATIC int16_t rc_sunaData[RC_BUFFER_SIZE];
FILE_STATIC int16_t rc_sunbData[RC_BUFFER_SIZE];
FILE_STATIC int16_t rc_sunxData[RC_BUFFER_SIZE];
FILE_STATIC int16_t rc_sunyData[RC_BUFFER_SIZE];
FILE_STATIC int16_t rc_sunzData[RC_BUFFER_SIZE];
FILE_STATIC uint8_t rc_sunValidData[RC_BUFFER_SIZE];
FILE_STATIC uint16_t rc_sunaHandle;
FILE_STATIC uint16_t rc_sunbHandle;
FILE_STATIC uint16_t rc_sunxHandle;
FILE_STATIC uint16_t rc_sunyHandle;
FILE_STATIC uint16_t rc_sunzHandle;
FILE_STATIC uint16_t rc_sunValidHandle;

FILE_STATIC SunSensorAngle *angle;
#if !ENABLE_SUNSENSOR
    FILE_STATIC SunSensorAngle mockAngle;
#endif

FILE_STATIC uint8_t isValid()
{
    // TODO implement a more useful validity check
    if (angle == NULLP)
    {
        return 0;
    }
    return angle->error == 0 ? 1 : 0;
}

FILE_STATIC int16_t rawToShort(float a)
{
    return a * (((float) INT16_MAX) / 120.0);
}

void sunsensorioInit()
{
#if ENABLE_SUNSENSOR
    sunSensorInit(I2C_BUS_SUNSENSOR);
#endif
    rc_sunaHandle = init_int16_t(rc_sunaData, RC_BUFFER_SIZE);
    rc_sunbHandle = init_int16_t(rc_sunbData, RC_BUFFER_SIZE);
    rc_sunxHandle = init_int16_t(rc_sunxData, RC_BUFFER_SIZE);
    rc_sunyHandle = init_int16_t(rc_sunyData, RC_BUFFER_SIZE);
    rc_sunzHandle = init_int16_t(rc_sunzData, RC_BUFFER_SIZE);
    rc_sunValidHandle = init_uint8_t(rc_sunValidData, RC_BUFFER_SIZE);
}

void sunsensorioUpdate()
{
#if ENABLE_SUNSENSOR
    angle = sunSensorReadAngle();
#else
    mockAngle.alpha = 20;
    mockAngle.beta = -10;
    mockAngle.error = 0;
    angle = &mockAngle;
#endif

    // set autocode inputs
    uint8_t valid = isValid();
    if (valid)
    {
        // only update if the angle is valid
        rtU.sun_vec_body_sunsensor[0] = angle->alpha;
        rtU.sun_vec_body_sunsensor[1] = angle->beta;
        rtU.sun_vec_body_sunsensor[2] = valid;
    }

    // update rollcall arrays
    addData_int16_t(rc_sunaHandle, rawToShort(angle->alpha));
    addData_int16_t(rc_sunbHandle, rawToShort(angle->beta));
}

void sunsensorioSendBackchannel()
{
    if (angle == NULL)
    {
        return;
    }

    // send raw readings
    sunsensor_segment sunsensorSeg;
    sunsensorSeg.alpha = angle->alpha;
    sunsensorSeg.beta = angle->beta;
    sunsensorSeg.error = angle->error;
    bcbinPopulateHeader(&sunsensorSeg.header, TLM_ID_SUNSENSOR_RAW, sizeof(sunsensorSeg));
    bcbinSendPacket((uint8_t *) &sunsensorSeg, sizeof(sunsensorSeg));

    // send processed vector
    sensor_vector_segment v;
    v.x = rtY.sun_vec_body[0];
    v.y = rtY.sun_vec_body[1];
    v.z = rtY.sun_vec_body[2];
    v.valid = rtY.sun_vec_body[3];
    bcbinPopulateHeader(&v.header, TLM_ID_SUNSENSOR_VECTOR, sizeof(v));
    bcbinSendPacket((uint8_t *) &v, sizeof(v));
}

void sunsensorioSendCAN()
{
    // send packet
    sensorproc_sun sun;
    sun.sensorproc_sun_x = UNIT_TO_INT16(rtY.sun_vec_body[0]);
    sun.sensorproc_sun_y = UNIT_TO_INT16(rtY.sun_vec_body[1]);
    sun.sensorproc_sun_z = UNIT_TO_INT16(rtY.sun_vec_body[2]);
    sun.sensorproc_sun_valid = rtY.sun_vec_body[3];
    CANPacket packet;
    encodesensorproc_sun(&sun, &packet);
    canSendPacket(&packet);

    // update rollcall arrays
    addData_int16_t(rc_sunxHandle, sun.sensorproc_sun_x);
    addData_int16_t(rc_sunyHandle, sun.sensorproc_sun_y);
    addData_int16_t(rc_sunzHandle, sun.sensorproc_sun_z);
    addData_uint8_t(rc_sunValidHandle, sun.sensorproc_sun_valid);
}

void sunsensorioRcPopulate4(rc_adcs_sp_4 *rc)
{
    rc->rc_adcs_sp_4_sun_x_min = getMin_int16_t(rc_sunxHandle);
    rc->rc_adcs_sp_4_sun_x_max = getMax_int16_t(rc_sunxHandle);
    rc->rc_adcs_sp_4_sun_x_avg = getAvg_int16_t(rc_sunxHandle);
}

void sunsensorioRcPopulate5(rc_adcs_sp_5 *rc)
{
    rc->rc_adcs_sp_5_sun_z_min = getMin_int16_t(rc_sunzHandle);
    rc->rc_adcs_sp_5_sun_y_min = getMin_int16_t(rc_sunyHandle);
    rc->rc_adcs_sp_5_sun_y_max = getMax_int16_t(rc_sunyHandle);
    rc->rc_adcs_sp_5_sun_y_avg = getAvg_int16_t(rc_sunyHandle);
}

void sunsensorioRcPopulate6(rc_adcs_sp_6 *rc)
{
    rc->rc_adcs_sp_6_sun_z_max = getMax_int16_t(rc_sunzHandle);
    rc->rc_adcs_sp_6_sun_z_avg = getAvg_int16_t(rc_sunzHandle);
}

void sunsensorioRcPopulate13(rc_adcs_sp_13 *rc)
{
    rc->rc_adcs_sp_13_suna_min = getMin_int16_t(rc_sunaHandle);
    rc->rc_adcs_sp_13_suna_max = getMax_int16_t(rc_sunaHandle);
    rc->rc_adcs_sp_13_suna_avg = getAvg_int16_t(rc_sunaHandle);
}

void sunsensorioRcPopulate14(rc_adcs_sp_14 *rc)
{
    rc->rc_adcs_sp_14_sunb_min = getMin_int16_t(rc_sunbHandle);
    rc->rc_adcs_sp_14_sunb_max = getMax_int16_t(rc_sunbHandle);
    rc->rc_adcs_sp_14_sunb_avg = getAvg_int16_t(rc_sunbHandle);
    rc->rc_adcs_sp_14_sun_valid = getSum_uint8_t(rc_sunValidHandle);
}
