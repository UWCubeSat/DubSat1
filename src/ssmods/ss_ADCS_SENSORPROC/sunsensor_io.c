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
#include "core/agglib.h"
#include "adcs_sensorproc_ids.h"

#include "autocode/MSP_SP.h"

// rollcall
FILE_STATIC aggVec_i rc_suna;
FILE_STATIC aggVec_i rc_sunb;
FILE_STATIC aggVec_i rc_sunx;
FILE_STATIC aggVec_i rc_suny;
FILE_STATIC aggVec_i rc_sunz;
FILE_STATIC aggVec_i rc_sunValid;

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
    return a * (((float) INT16_MAX) / 60.0);
}

void sunsensorioInit()
{
#if ENABLE_SUNSENSOR
    sunSensorInit(I2C_BUS_SUNSENSOR);
#endif
    aggVec_init_i(&rc_suna);
    aggVec_init_i(&rc_sunb);
    aggVec_init_i(&rc_sunx);
    aggVec_init_i(&rc_suny);
    aggVec_init_i(&rc_sunz);
    aggVec_init_i(&rc_sunValid);
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
    }
    rtU.sun_vec_body_sunsensor[2] = valid;

    // update rollcall arrays
    aggVec_push_i(&rc_suna, rawToShort(angle->alpha));
    aggVec_push_i(&rc_sunb, rawToShort(angle->beta));
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

    // update rollcall data
    aggVec_push_i(&rc_sunx, sun.sensorproc_sun_x);
    aggVec_push_i(&rc_suny, sun.sensorproc_sun_y);
    aggVec_push_i(&rc_sunz, sun.sensorproc_sun_z);
    aggVec_push_i(&rc_sunValid, sun.sensorproc_sun_valid);
}

void sunsensorioRcPopulate4(rc_adcs_sp_4 *rc)
{
    rc->rc_adcs_sp_4_sun_x_min = aggVec_min_i(&rc_sunx);
    rc->rc_adcs_sp_4_sun_x_max = aggVec_max_i(&rc_sunx);
    rc->rc_adcs_sp_4_sun_x_avg = aggVec_avg_i_i(&rc_sunx);
    aggVec_reset((aggVec * ) &rc_sunx);
}

void sunsensorioRcPopulate5(rc_adcs_sp_5 *rc)
{
    rc->rc_adcs_sp_5_sun_z_min = aggVec_min_i(&rc_sunz);
    aggVec_min_reset((aggVec * ) &rc_sunz);
    rc->rc_adcs_sp_5_sun_y_min = aggVec_min_i(&rc_suny);
    rc->rc_adcs_sp_5_sun_y_max = aggVec_max_i(&rc_suny);
    rc->rc_adcs_sp_5_sun_y_avg = aggVec_avg_i_i(&rc_suny);
    aggVec_reset((aggVec * ) &rc_suny);
}

void sunsensorioRcPopulate6(rc_adcs_sp_6 *rc)
{
    rc->rc_adcs_sp_6_sun_z_max = aggVec_max_i(&rc_sunz);
    aggVec_max_reset((aggVec * ) &rc_sunz);
    rc->rc_adcs_sp_6_sun_z_avg = aggVec_avg_i_i(&rc_sunz);
    aggVec_as_reset((aggVec * ) &rc_sunz);
}

void sunsensorioRcPopulate13(rc_adcs_sp_13 *rc)
{
    rc->rc_adcs_sp_13_suna_min = aggVec_min_i(&rc_suna);
    rc->rc_adcs_sp_13_suna_max = aggVec_max_i(&rc_suna);
    rc->rc_adcs_sp_13_suna_avg = aggVec_avg_i_i(&rc_suna);
    aggVec_reset((aggVec * ) &rc_suna);
}

void sunsensorioRcPopulate14(rc_adcs_sp_14 *rc)
{
    rc->rc_adcs_sp_14_sunb_min = aggVec_min_i(&rc_sunb);
    rc->rc_adcs_sp_14_sunb_max = aggVec_max_i(&rc_sunb);
    rc->rc_adcs_sp_14_sunb_avg = aggVec_avg_i_i(&rc_sunb);
    aggVec_reset((aggVec * ) &rc_sunb);
    rc->rc_adcs_sp_14_sun_valid = (aggVec_sum_i(&rc_sunValid) / ((float) aggVec_as_count((aggVec *) &rc_sunValid))) * 255;
    aggVec_reset((aggVec * ) &rc_sunValid);
}
