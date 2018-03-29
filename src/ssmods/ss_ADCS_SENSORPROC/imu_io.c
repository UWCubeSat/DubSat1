/*
 * imu_io.c
 *
 *  Created on: Mar 29, 2018
 *      Author: djdup
 */

#include "imu_io.h"
#include "adcs_sensorproc_ids.h"
#include "sensors/imu.h"

#include "core/i2c.h"
#include "core/utils.h"
#include "core/timer.h"

FILE_STATIC int timerHandle;

FILE_STATIC void resetTimer()
{
    timerHandle = timerPollInitializer(DELAY_IMU_UPDATE_MS);
}

void imuioInit()
{
    initializeTimer();
    resetTimer();

    imuInit(IMU_I2CBUS);
}

void imuioUpdate()
{
    if (!checkTimer(timerHandle))
    {
        return;
    }
    resetTimer();

    IMUData *data = imuReadGyroAccelData();

    // send backchannel data
    imu_segment seg;
    seg.x = data->rawGyroX;
    seg.y = data->rawGyroY;
    seg.z = data->rawGyroZ;
    bcbinPopulateHeader(&seg.header, TLM_ID_IMU, sizeof(seg));
    bcbinSendPacket((uint8_t *) &seg, sizeof(seg));

    // TODO send CAN data
}
