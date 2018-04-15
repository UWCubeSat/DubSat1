/*
 * sunsensor_io.c
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#include <math.h>

#include "sunsensor_io.h"
#include "sensors/sun_sensor.h"
#include "interfaces/canwrap.h"
#include "adcs_sensorproc_ids.h"

#include "autocode/MSP_SP0.h"

FILE_STATIC SunSensorAngle *angle;

FILE_STATIC uint8_t isValid()
{
    // TODO implement a more useful validity check
    if (angle == NULLP)
    {
        return 0;
    }
    return 1;
}

void sunsensorioInit()
{
    sunSensorInit(I2C_BUS_SUNSENSOR);
}

void sunsensorioUpdate()
{
    angle = sunSensorReadAngle();

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

    sunsensor_segment sunsensorSeg;
    sunsensorSeg.alpha = angle->alpha;
    sunsensorSeg.beta = angle->beta;
    sunsensorSeg.error = angle->error;
    bcbinPopulateHeader(&sunsensorSeg.header, TLM_ID_SUNSENSOR, sizeof(sunsensorSeg));
    bcbinSendPacket((uint8_t *) &sunsensorSeg, sizeof(sunsensorSeg));
}

void sunsensorioSendCAN()
{
    // TODO use new CAN format when implemented
    sensorproc_sun sun = { 0 };
//    sensorproc_sun sun = { rtY.sun_vec_body[0],
//                           rtY.sun_vec_body[1],
//                           rtY.sun_vec_body[2],
//                           rtY.sun_vec_body[3] };
    CANPacket packet;
    encodesensorproc_sun(&sun, &packet);
    canSendPacket(&packet);
}
