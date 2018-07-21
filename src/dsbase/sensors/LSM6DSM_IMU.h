/*
 * LSM6DSM_IMU.h
 *
 *  Created on: Oct 23, 2017
 *      Author: SatLab
 */

#ifndef DSBASE_SENSORS_LSM6DSM_IMU_H_
#define DSBASE_SENSORS_LSM6DSM_IMU_H_

#define IMU_LSM6DSM_I2C_7BIT_ADDRESS        0x6A

// The interesting command registers
#define IMU_LSM6DSM_CTRL2_G                 0x11
#define IMU_LSM6DSM_CTRL7_G                 0x16

// Relevant settings for command registers
#define IMU_LSM6DSM_ODR_12p5_FS125          0x12
#define IMU_LSM6DSM_ODR_26_FS125            0x22
#define IMU_LSM6DSM_ODR_52_FS125            0x32
#define IMU_LSM6DSM_ODR_104_FS125           0x42
#define IMU_LSM6DSM_HIGH_PERF_OFF           0x80
#define IMU_LSM6DSM_HIGH_PERF_ON            0x00

// Output register info
#define IMU_LSM6DSM_OUTPUT_DATA_REGS        0x22
#define IMU_LSM6DSM_DATA_NUM_BYTES          6
#define IMU_LSM6DSM_WHOAMI_REGS             0x0f

#endif /* DSBASE_SENSORS_LSM6DSM_IMU_H_ */
