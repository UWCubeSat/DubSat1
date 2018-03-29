/*
 * photodiode_io.c
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#include "photodiode_io.h"
#include "sensors/photodiode.h"
#include "interfaces/canwrap.h"
#include "core/timer.h"
#include "adcs_sensorproc_ids.h"

// backchannel telemetry segment
FILE_STATIC photodiode_segment photodiodeSeg;

// photodiode handles
FILE_STATIC uint8_t pdCenter;
FILE_STATIC uint8_t pdRight;
FILE_STATIC uint8_t pdLeft;

// timer handle
FILE_STATIC int timerHandle;

void photodiodeioInit()
{
    initializeTimer();
    timerHandle = timerPollInitializer(PHOTODIODE_DELAY_MS);

    bcbinPopulateHeader(&photodiodeSeg.header, TLM_ID_PHOTODIODE, sizeof(photodiodeSeg));

    pdCenter = photodiodeInit(PD_ADDR_HH, I2C_BUS_PHOTODIODES);
    pdRight = photodiodeInit(PD_ADDR_FF, I2C_BUS_PHOTODIODES);
    pdLeft = photodiodeInit(PD_ADDR_HF, I2C_BUS_PHOTODIODES);
}

FILE_STATIC uint8_t compressReading(uint32_t reading)
{
    if (reading >= PD_MAX_VAL)
    {
        return 255;
    }
    return reading >> 7;
}

void photodiodeioUpdate()
{
    if (!checkTimer(timerHandle))
    {
        return;
    }
    timerHandle = timerPollInitializer(PHOTODIODE_DELAY_MS);

    photodiodeSeg.center = photodiodeRead(defaultWrite, pdCenter);
    photodiodeSeg.right = photodiodeRead(defaultWrite, pdRight);
    photodiodeSeg.left = photodiodeRead(defaultWrite, pdLeft);
    photodiodeSeg.cCenter = compressReading(photodiodeSeg.center);
    photodiodeSeg.cRight = compressReading(photodiodeSeg.right);
    photodiodeSeg.cLeft = compressReading(photodiodeSeg.left);

    photodiodeioSendData();
}

void photodiodeioSendData()
{
    // send backchannel telemetry
    bcbinSendPacket((uint8_t *) &photodiodeSeg, sizeof(photodiodeSeg));

    // send CAN packet
    // TODO verify the order on these is correct
    sensorproc_photodiode pd = { compressReading(photodiodeSeg.center),
                                 compressReading(photodiodeSeg.right),
                                 compressReading(photodiodeSeg.left) };
    CANPacket packet;
    encodesensorproc_photodiode(&pd, &packet);
    canSendPacket(&packet);
}
