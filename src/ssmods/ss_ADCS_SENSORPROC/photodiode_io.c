/*
 * photodiode_io.c
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#include "photodiode_io.h"
#include "sensors/photodiode.h"
#include "interfaces/canwrap.h"
#include "adcs_sensorproc_ids.h"

// backchannel telemetry segment
FILE_STATIC photodiode_segment photodiodeSeg;

// photodiode handles
FILE_STATIC uint8_t pdCenter;
FILE_STATIC uint8_t pdRight;
FILE_STATIC uint8_t pdLeft;

void photodiodeioInit()
{
    bcbinPopulateHeader(&photodiodeSeg.header, TLM_ID_PHOTODIODE, sizeof(photodiodeSeg));

    pdCenter = photodiodeInit(PD_ADDR_HH, I2C_BUS_PHOTODIODES);
    pdRight = photodiodeInit(PD_ADDR_FF, I2C_BUS_PHOTODIODES);
    pdLeft = photodiodeInit(PD_ADDR_HF, I2C_BUS_PHOTODIODES);
}

void photodiodeioUpdate()
{
    photodiodeSeg.center = photodiodeRead(defaultWrite, pdCenter);
    photodiodeSeg.right = photodiodeRead(defaultWrite, pdRight);
    photodiodeSeg.left = photodiodeRead(defaultWrite, pdLeft);
}

void photodiodeioSendData()
{
    // send backchannel telemetry
    bcbinSendPacket((uint8_t *) &photodiodeSeg, sizeof(photodiodeSeg));

    // send CAN packet
    // TODO verify the order on these is correct
    // TODO how are these supposed to fit in a uint8_t?
    sensorproc_photodiode pd = { (uint8_t) photodiodeSeg.center,
                                 (uint8_t) photodiodeSeg.right,
                                 (uint8_t) photodiodeSeg.left };
    CANPacket packet;
    encodesensorproc_photodiode(&pd, &packet);
    canSendPacket(&packet);
}
