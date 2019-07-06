/*
 * imu.h
 *
 *  Created on: Jul 11, 2017
 *      Author: jeffc
 */

#ifndef SENSORS_IMU_H_
#define SENSORS_IMU_H_

#include <stdint.h>

// assuming FS = +-125
#define IMU_RAW_TO_DPS 0.004375

// degrees to radians
#define DEG_TO_RAD     (3.14159265358979323846 / 180.0)


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

typedef enum {
    IMUUpdateRate_12p5Hz,
    IMUUpdateRate_26Hz,
    IMUUpdateRate_52Hz,
    IMUUpdateRate_104Hz,
} IMUUpdateRate;

// Main entry points
uint8_t imuInit(bus_instance_i2c i2cbus, IMUUpdateRate rate);
IMUData *imuReadGyroAccelData();

/**
 * Returns the IMU slave address
 */
uint8_t imuWhoami();

/**
 * Convert a raw angular rate reading to radians per second
 */
float imuConvertRawToRPS(int16_t raw);

/**
 * Convert angular rate in radians per second to the same units used in raw
 * readings.
 */
int16_t imuConvertRPSToRaw(float rps);

#endif /* SENSORS_IMU_H_ */
