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

FILE_STATIC IMUData *data;

void imuioInit()
{
    imuInit(IMU_I2CBUS);
}

void imuioUpdate()
{
    data = imuReadGyroAccelData();

    // TODO set autocode inputs
}

void imuioSendBackchannel()
{
    imu_segment seg;
    seg.x = data->rawGyroX;
    seg.y = data->rawGyroY;
    seg.z = data->rawGyroZ;
    bcbinPopulateHeader(&seg.header, TLM_ID_IMU, sizeof(seg));
    bcbinSendPacket((uint8_t *) &seg, sizeof(seg));
}

void imuioSendCAN()
{
    // TODO
}
