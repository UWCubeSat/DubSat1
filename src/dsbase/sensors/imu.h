/*
 * imu.h
 *
 *  Created on: Jul 11, 2017
 *      Author: jeffc
 */

#ifndef SENSORS_IMU_H_
#define SENSORS_IMU_H_

#include <stdint.h>

#include "../core/i2c.h"
#include "../core/utils.h"
#include "../core/debugtools.h"


#if defined(__BSP_HW_IMU_BMI160__)  // Bosch BMI160

#include "BMI160_IMU.h"

#define IMU_I2C_7BIT_ADDRESS                        IMU_BMI160_I2C_7BIT_ADDRESS
#define IMU_FULL_OUTPUT_REG_ADDR_START              IMU_BMI160_FULL_OUTPUT_REG_ADDR_START
#define IMU_GYRO_OUTPUT_REG_ADDR_START              IMU_BMI160_GYRO_OUTPUT_REG_ADDR_START
#define IMU_ACCEL_OUTPUT_REG_ADDR_START             IMU_BMI160_ACCEL_OUTPUT_REG_ADDR_START

#elif defined(__BSP_HW_IMU_LSM6DSM__)

#include "LSM6DSM_IMU.h"

#define IMU_I2C_7BIT_ADDRESS                        IMU_LSM6DSM_I2C_7BIT_ADDRESS

#else

#error Unknown - or no - IMU hardware selected.

#endif  // IMU type

#define MAX_BUFF_SIZE   0x25

typedef struct  {
    int16_t rawGyroX;
    int16_t rawGyroY;
    int16_t rawGyroZ;
    // TODO:  add timestamp?
} IMUData;

// Main entry points
void imuInit();
IMUData *imuReadGyroAccelData();

#endif /* SENSORS_IMU_H_ */
