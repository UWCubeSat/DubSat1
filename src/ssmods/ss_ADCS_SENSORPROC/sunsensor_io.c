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

#include "autocode/MSP_SP.h"

FILE_STATIC sunsensor_segment sunsensorSeg;

void sunsensorioInit()
{
    bcbinPopulateHeader(&sunsensorSeg.header, TLM_ID_SUNSENSOR, sizeof(sunsensorSeg));

    sunSensorInit(I2C_BUS_SUNSENSOR);
}

void sunsensorioUpdate()
{
    SunSensorAngle *angle = sunSensorReadAngle();
    if (angle != NULLP)
    {
        sunsensorSeg.alpha = angle->alpha;
        sunsensorSeg.beta = angle->beta;
        sunsensorSeg.error = angle->error;
    }
    else
    {
        // TODO log read error
    }

    // TODO set autocode inputs
    rtU.sun_vec_body_sunsensor[0] = 123.0;
}

void sunsensorioSendBackchannel()
{
    bcbinSendPacket((uint8_t *) &sunsensorSeg, sizeof(sunsensorSeg));
}

void sunsensorioSendCAN()
{
    // TODO use autocode outputs instead of segment
    // converting degrees to arcminutes
    sensorproc_sun sun = { sunsensorSeg.error,
                           (int32_t) round(sunsensorSeg.alpha * 60),
                           (int32_t) round(sunsensorSeg.beta * 60) };
    CANPacket packet;
    encodesensorproc_sun(&sun, &packet);
    canSendPacket(&packet);
}
