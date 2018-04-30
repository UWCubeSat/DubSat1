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
#include "interfaces/canwrap.h"

#include "autocode/MSP_SP.h"

FILE_STATIC IMUData *data;

#if !ENABLE_IMU
FILE_STATIC IMUData mockData;
#endif

void imuioInit()
{
#if ENABLE_IMU
    imuInit(IMU_I2CBUS, IMUUpdateRate_52Hz);
#endif
}

void imuioUpdate()
{
#if ENABLE_IMU
    data = imuReadGyroAccelData();
#else
    mockData.rawGyroX = 456;
    mockData.rawGyroY = 456;
    mockData.rawGyroZ = 456;
    data = &mockData;
#endif

    // TODO write validity check
    uint8_t valid = 1;

    // set autocode inputs
    rtU.omega_body_radps_gyro[0] = imuConvertRawToRPS(data->rawGyroX);
    rtU.omega_body_radps_gyro[1] = imuConvertRawToRPS(data->rawGyroY);
    rtU.omega_body_radps_gyro[2] = imuConvertRawToRPS(data->rawGyroZ);
    rtU.omega_body_radps_gyro[3] = valid;
}

void imuioSendBackchannel()
{
    // send raw vector
    imu_segment seg;
    seg.x = data->rawGyroX;
    seg.y = data->rawGyroY;
    seg.z = data->rawGyroZ;
    bcbinPopulateHeader(&seg.header, TLM_ID_IMU_RAW, sizeof(seg));
    bcbinSendPacket((uint8_t *) &seg, sizeof(seg));

    // send processed vector
    sensor_vector_segment v;
    v.x = rtY.omega_radps_processed[0];
    v.y = rtY.omega_radps_processed[1];
    v.z = rtY.omega_radps_processed[2];
    v.valid = rtY.omega_radps_processed[3];
    bcbinPopulateHeader(&v.header, TLM_ID_IMU_VECTOR, sizeof(v));
    bcbinSendPacket((uint8_t *) &v, sizeof(v));
}

void imuioSendCAN()
{
    sensorproc_imu gyro;
    gyro.sensorproc_imu_x = imuConvertRPSToRaw(rtY.omega_radps_processed[0]);
    gyro.sensorproc_imu_y = imuConvertRPSToRaw(rtY.omega_radps_processed[1]);
    gyro.sensorproc_imu_z = imuConvertRPSToRaw(rtY.omega_radps_processed[2]);
    gyro.sensorproc_imu_valid = rtY.omega_radps_processed[3];
    CANPacket packet;
    encodesensorproc_imu(&gyro, &packet);
    canSendPacket(&packet);
}
