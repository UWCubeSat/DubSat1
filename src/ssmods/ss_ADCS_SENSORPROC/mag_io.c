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
#include "core/timer.h"

FILE_STATIC int timerHandle;

FILE_STATIC void resetTimer()
{
    timerHandle = timerPollInitializer(DELAY_MAG_UPDATE_MS);
}

void magioInit()
{
    initializeTimer();
    resetTimer();

    magInit(MAG_I2CBUS);
    normalOperationConfig();
}

void magioUpdate()
{
    if (!checkTimer(timerHandle))
    {
        return;
    }
    resetTimer();

    MagnetometerData *data = magReadXYZData(ConvertToNanoTeslas);

    // send backchannel data
    mag_segment seg;
    seg.x = data->convertedX;
    seg.y = data->convertedY;
    seg.z = data->convertedZ;
    bcbinPopulateHeader(&seg.header, TLM_ID_MAG, sizeof(seg));
    bcbinSendPacket((uint8_t *) &seg, sizeof(seg));

    // TODO send CAN data
}