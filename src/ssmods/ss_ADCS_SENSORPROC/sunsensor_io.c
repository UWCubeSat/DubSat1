/*
 * sunsensor_io.c
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#define SUNSENSOR_UPDATE_DELAY_MS 200

#include <math.h>

#include "sunsensor_io.h"
#include "sensors/sun_sensor.h"
#include "interfaces/canwrap.h"
#include "core/timer.h"
#include "adcs_sensorproc_ids.h"

FILE_STATIC sunsensor_segment sunsensorSeg;

FILE_STATIC int timerHandle;

void sunsensorioInit()
{
    initializeTimer();
    timerHandle = timerPollInitializer(SUNSENSOR_UPDATE_DELAY_MS);

    bcbinPopulateHeader(&sunsensorSeg.header, TLM_ID_SUNSENSOR, sizeof(sunsensorSeg));

    sunSensorInit(I2C_BUS_SUNSENSOR);
}

void sunsensorioUpdate()
{
    if (!checkTimer(timerHandle))
    {
        return;
    }
    timerHandle = timerPollInitializer(SUNSENSOR_UPDATE_DELAY_MS);

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

    sunsensorioSendData();
}

void sunsensorioSendData()
{
    // send backchannel telemetry
    bcbinSendPacket((uint8_t *) &sunsensorSeg, sizeof(sunsensorSeg));

    // send CAN packet
    // converting degrees to arcminutes
    sensorproc_sun sun = { sunsensorSeg.error,
                           (int32_t) round(sunsensorSeg.alpha * 60),
                           (int32_t) round(sunsensorSeg.beta * 60) };
    CANPacket packet;
    encodesensorproc_sun(&sun, &packet);
    canSendPacket(&packet);
}
