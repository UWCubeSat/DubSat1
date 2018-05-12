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
#include "core/agglib.h"
#include "interfaces/canwrap.h"

#include "autocode/MSP_SP.h"

// rollcall arrays
FILE_STATIC aggVec_i rc_imux;
FILE_STATIC aggVec_i rc_imuy;
FILE_STATIC aggVec_i rc_imuz;
FILE_STATIC aggVec_i rc_imupx;
FILE_STATIC aggVec_i rc_imupy;
FILE_STATIC aggVec_i rc_imupz;
FILE_STATIC aggVec_i rc_imuValid;

FILE_STATIC IMUData *data;

#if !ENABLE_IMU
FILE_STATIC IMUData mockData;
#endif

void imuioInit()
{
#if ENABLE_IMU
    imuInit(IMU_I2CBUS, IMUUpdateRate_52Hz);
#endif
    aggVec_init(&rc_imux);
    aggVec_init(&rc_imuy);
    aggVec_init(&rc_imuz);
    aggVec_init(&rc_imupx);
    aggVec_init(&rc_imupy);
    aggVec_init(&rc_imupz);
    aggVec_init(&rc_imuValid);
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

    // update rollcall data
    aggVec_i_push(&rc_imux, data->rawGyroX);
    aggVec_i_push(&rc_imuy, data->rawGyroY);
    aggVec_i_push(&rc_imuz, data->rawGyroZ);
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

    // update rollcall data
    aggVec_i_push(&rc_imupx, gyro.sensorproc_imu_x);
    aggVec_i_push(&rc_imupy, gyro.sensorproc_imu_y);
    aggVec_i_push(&rc_imupz, gyro.sensorproc_imu_z);
    aggVec_i_push(&rc_imuValid, gyro.sensorproc_imu_valid);
}

void imuioRcPopulate2(rc_adcs_sp_2 *rc)
{
    rc->rc_adcs_sp_2_imup_x_min = aggVec_i_min(&rc_imupx);
    rc->rc_adcs_sp_2_imup_x_max = aggVec_i_max(&rc_imupx);
    rc->rc_adcs_sp_2_imup_x_avg = aggVec_i_avg_i(&rc_imupx);
    aggVec_reset(&rc_imupx);
    rc->rc_adcs_sp_2_imup_y_min = aggVec_i_min(&rc_imupy);
    aggVec_min_reset(&rc_imupy);
}

void imuioRcPopulate3(rc_adcs_sp_3 *rc)
{
    rc->rc_adcs_sp_3_imup_y_max = aggVec_i_max(&rc_imupy);
    aggVec_max_reset(&rc_imupy);
    rc->rc_adcs_sp_3_imup_y_avg = aggVec_i_avg_i(&rc_imupy);
    aggVec_as_reset(&rc_imupy);
    rc->rc_adcs_sp_3_imup_z_min = aggVec_i_min(&rc_imupz);
    aggVec_min_reset(&rc_imupz);
    rc->rc_adcs_sp_3_imup_z_max = aggVec_i_max(&rc_imupz);
    aggVec_max_reset(&rc_imupz);
}

void imuioRcPopulate4(rc_adcs_sp_4 *rc)
{
    rc->rc_adcs_sp_4_imup_z_avg = aggVec_i_avg_i(&rc_imupz);
    aggVec_as_reset(&rc_imupz);
}

void imuioRcPopulate15(rc_adcs_sp_15 *rc)
{
    rc->rc_adcs_sp_15_imu_x_min = aggVec_i_min(&rc_imux);
    rc->rc_adcs_sp_15_imu_x_max = aggVec_i_max(&rc_imux);
    rc->rc_adcs_sp_15_imu_x_avg = aggVec_i_avg_i(&rc_imux);
    aggVec_reset(&rc_imux);
    rc->rc_adcs_sp_15_imu_valid = aggVec_i_sum(&rc_imuValid);
    aggVec_reset(&rc_imuValid);
}

void imuioRcPopulate16(rc_adcs_sp_16 *rc)
{
    rc->rc_adcs_sp_16_imu_y_min = aggVec_i_min(&rc_imuy);
    rc->rc_adcs_sp_16_imu_y_max = aggVec_i_max(&rc_imuy);
    rc->rc_adcs_sp_16_imu_y_avg = aggVec_i_avg_i(&rc_imuy);
    aggVec_reset(&rc_imuy);
    rc->rc_adcs_sp_16_imu_z_min = aggVec_i_min(&rc_imuz);
    aggVec_min_reset(&rc_imuz);
}

void imuioRcPopulate17(rc_adcs_sp_17 *rc)
{
    rc->rc_adcs_sp_17_imu_z_max = aggVec_i_max(&rc_imuz);
    aggVec_max_reset(&rc_imuz);
    rc->rc_adcs_sp_17_imu_z_avg = aggVec_i_avg_i(&rc_imuz);
    aggVec_as_reset(&rc_imuz);
}
