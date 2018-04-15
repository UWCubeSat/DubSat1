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

#include "autocode/MSP_SP0.h"

FILE_STATIC IMUData *data;

void imuioInit()
{
    imuInit(IMU_I2CBUS);
}

void imuioUpdate()
{
    data = imuReadGyroAccelData();

    // TODO write validity check
    uint8_t valid = 1;

    // set autocode inputs
    // TODO verfiy units
    rtU.omega_body_radps_gyro[0] = data->rawGyroX;
    rtU.omega_body_radps_gyro[1] = data->rawGyroY;
    rtU.omega_body_radps_gyro[2] = data->rawGyroZ;
    rtU.omega_body_radps_gyro[3] = valid;
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
    // TODO send CAN packet when implemented
//    rtY.omega_radps_processed...
}
