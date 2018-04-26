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
#include "adcs_sensorproc_ids.h"

#include "autocode/MSP_SP.h"

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

void sunsensorioInit()
{
#if ENABLE_SUNSENSOR
    sunSensorInit(I2C_BUS_SUNSENSOR);
#endif
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
    rtU.sun_vec_body_sunsensor[0] = angle->alpha;
    rtU.sun_vec_body_sunsensor[1] = angle->beta;
    rtU.sun_vec_body_sunsensor[2] = isValid();
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
    sensorproc_sun sun;
    sun.sensorproc_sun_x = UNIT_TO_INT16(rtY.sun_vec_body[0]);
    sun.sensorproc_sun_y = UNIT_TO_INT16(rtY.sun_vec_body[1]);
    sun.sensorproc_sun_z = UNIT_TO_INT16(rtY.sun_vec_body[2]);
    sun.sensorproc_sun_valid = rtY.sun_vec_body[3];
    CANPacket packet;
    encodesensorproc_sun(&sun, &packet);
    canSendPacket(&packet);
}
