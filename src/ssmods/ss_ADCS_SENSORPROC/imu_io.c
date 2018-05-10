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
#include "core/dataArray.h"
#include "interfaces/canwrap.h"

#include "autocode/MSP_SP.h"

// rollcall arrays
FILE_STATIC int16_t rc_imuxData[RC_BUFFER_SIZE];
FILE_STATIC int16_t rc_imuyData[RC_BUFFER_SIZE];
FILE_STATIC int16_t rc_imuzData[RC_BUFFER_SIZE];
FILE_STATIC int16_t rc_imupxData[RC_BUFFER_SIZE];
FILE_STATIC int16_t rc_imupyData[RC_BUFFER_SIZE];
FILE_STATIC int16_t rc_imupzData[RC_BUFFER_SIZE];
FILE_STATIC uint8_t rc_imupValidData[RC_BUFFER_SIZE];
FILE_STATIC uint16_t rc_imuyHandle;
FILE_STATIC uint16_t rc_imuxHandle;
FILE_STATIC uint16_t rc_imuzHandle;
FILE_STATIC uint16_t rc_imupxHandle;
FILE_STATIC uint16_t rc_imupyHandle;
FILE_STATIC uint16_t rc_imupzHandle;
FILE_STATIC uint16_t rc_imupValidHandle;

FILE_STATIC IMUData *data;

#if !ENABLE_IMU
FILE_STATIC IMUData mockData;
#endif

void imuioInit()
{
#if ENABLE_IMU
    imuInit(IMU_I2CBUS, IMUUpdateRate_52Hz);
#endif
    rc_imuxHandle = init_int16_t(rc_imuxData, RC_BUFFER_SIZE);
    rc_imuyHandle = init_int16_t(rc_imuyData, RC_BUFFER_SIZE);
    rc_imuzHandle = init_int16_t(rc_imuzData, RC_BUFFER_SIZE);
    rc_imupxHandle = init_int16_t(rc_imupxData, RC_BUFFER_SIZE);
    rc_imupyHandle = init_int16_t(rc_imupyData, RC_BUFFER_SIZE);
    rc_imupzHandle = init_int16_t(rc_imupzData, RC_BUFFER_SIZE);
    rc_imupValidHandle = init_uint8_t(rc_imupValidData, RC_BUFFER_SIZE);
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

    // update rollcall arrays
    addData_int16_t(rc_imuxHandle, data->rawGyroX);
    addData_int16_t(rc_imuyHandle, data->rawGyroY);
    addData_int16_t(rc_imuzHandle, data->rawGyroZ);
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
    // send CAN packet
    sensorproc_imu gyro;
    gyro.sensorproc_imu_x = imuConvertRPSToRaw(rtY.omega_radps_processed[0]);
    gyro.sensorproc_imu_y = imuConvertRPSToRaw(rtY.omega_radps_processed[1]);
    gyro.sensorproc_imu_z = imuConvertRPSToRaw(rtY.omega_radps_processed[2]);
    gyro.sensorproc_imu_valid = rtY.omega_radps_processed[3];
    CANPacket packet;
    encodesensorproc_imu(&gyro, &packet);
    canSendPacket(&packet);

    // update rollcall arrays
    addData_int16_t(rc_imupxHandle, gyro.sensorproc_imu_x);
    addData_int16_t(rc_imupyHandle, gyro.sensorproc_imu_y);
    addData_int16_t(rc_imupzHandle, gyro.sensorproc_imu_z);
    addData_uint8_t(rc_imupValidHandle, gyro.sensorproc_imu_valid);
}

void imuioRcPopulate2(rc_adcs_sp_2 *rc)
{
    rc->rc_adcs_sp_2_imup_x_min = getMin_int16_t(rc_imupxHandle);
    rc->rc_adcs_sp_2_imup_x_max = getMax_int16_t(rc_imupxHandle);
    rc->rc_adcs_sp_2_imup_x_avg = getAvg_int16_t(rc_imupxHandle);
    rc->rc_adcs_sp_2_imup_y_min = getMin_int16_t(rc_imupyHandle);
}

void imuioRcPopulate3(rc_adcs_sp_3 *rc)
{
    rc->rc_adcs_sp_3_imup_y_max = getMax_int16_t(rc_imupyHandle);
    rc->rc_adcs_sp_3_imup_y_avg = getAvg_int16_t(rc_imupyHandle);
    rc->rc_adcs_sp_3_imup_z_min = getMin_int16_t(rc_imupzHandle);
    rc->rc_adcs_sp_3_imup_z_max = getMax_int16_t(rc_imupzHandle);
}

void imuioRcPopulate4(rc_adcs_sp_4 *rc)
{
    rc->rc_adcs_sp_4_imup_z_avg = getAvg_int16_t(rc_imupzHandle);
}

void imuioRcPopulate15(rc_adcs_sp_15 *rc)
{
    rc->rc_adcs_sp_15_imu_x_min = getMin_int16_t(rc_imuxHandle);
    rc->rc_adcs_sp_15_imu_x_max = getMax_int16_t(rc_imuxHandle);
    rc->rc_adcs_sp_15_imu_x_avg = getAvg_int16_t(rc_imuxHandle);
    rc->rc_adcs_sp_15_imu_valid = getSum_uint8_t(rc_imupValidHandle);
}

void imuioRcPopulate16(rc_adcs_sp_16 *rc)
{
    rc->rc_adcs_sp_16_imu_y_min = getMin_int16_t(rc_imuyHandle);
    rc->rc_adcs_sp_16_imu_y_max = getMax_int16_t(rc_imuyHandle);
    rc->rc_adcs_sp_16_imu_y_avg = getAvg_int16_t(rc_imuyHandle);
    rc->rc_adcs_sp_16_imu_z_min = getMin_int16_t(rc_imuzHandle);
}

void imuioRcPopulate17(rc_adcs_sp_17 *rc)
{
    rc->rc_adcs_sp_17_imu_z_max = getMax_int16_t(rc_imuzHandle);
    rc->rc_adcs_sp_17_imu_z_avg = getAvg_int16_t(rc_imuzHandle);
}
