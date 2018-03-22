/*
 * imu.c
 *
 *  Created on: Jul 4, 2017
 *      Author: jeffc
 */

#include "imu.h"

FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC uint8_t imuInitialized = 0;
FILE_STATIC hDev hSensor;

IMUData idata;



void imuInit(bus_instance_i2c i2cbus)
{
    if (imuInitialized != 0)
        return;

    imuInitialized = 1;
    i2cEnable(i2cbus);
    hSensor = i2cInit(i2cbus, IMU_I2C_7BIT_ADDRESS);

#if defined (__BSP_HW_IMU_BMI160__)

    // TODO:  Add initialization code
    i2cBuff[0] = 0x7E; // CMD register
    i2cBuff[1] = 0x15;  // SHOULD turn on gyro, and only gyro
    i2cMasterWrite(hSensor, i2cBuff, 2);

    // Turn on accel for now
    i2cBuff[0] = 0x7E;
    i2cBuff[1] = 0x11;
    i2cMasterWrite(hSensor, i2cBuff, 2);

#elif defined(__BSP_HW_IMU_LSM6DSM__)

    i2cBuff[0] = IMU_LSM6DSM_CTRL2_G;
    i2cBuff[1] = IMU_LSM6DSM_ODR_26_FS125;
    i2cBuff[2] = IMU_LSM6DSM_CTRL7_G;
    i2cBuff[3] = IMU_LSM6DSM_HIGH_PERF_ON;
    i2cMasterWrite(hSensor, i2cBuff, 4);

#else

#error Unknown - or no - IMU hardware selected.

#endif  // IMU type

}

IMUData *imuReadGyroAccelData()
{

#if defined (__BSP_HW_IMU_BMI160__)

    // TODO:  Add state read code, populate IMU data struct
    i2cMasterRegisterRead(hSensor, 0x0C, i2cBuff, 12);  // read data bytes, little-endian pairs

    idata.rawGyroX = (int16_t)(i2cBuff[0] | ((int16_t)i2cBuff[1] << 8));
    idata.rawGyroY = (int16_t)(i2cBuff[2] | ((int16_t)i2cBuff[3] << 8));
    idata.rawGyroZ = (int16_t)(i2cBuff[4] | ((int16_t)i2cBuff[5] << 8));

    //idata.rawAccelX = (int16_t)(i2cBuff[6] | ((int16_t)i2cBuff[7] << 8));
    //idata.rawAccelY = (int16_t)(i2cBuff[8] | ((int16_t)i2cBuff[9] << 8));
    //idata.rawAccelZ = (int16_t)(i2cBuff[10] | ((int16_t)i2cBuff[11] << 8));


    //i2cCombinedAddressWriteThenRead(0x1B, i2cBuff, 1);  // read status byte

#elif defined(__BSP_HW_IMU_LSM6DSM__)

    i2cMasterRegisterRead(hSensor, IMU_LSM6DSM_OUTPUT_DATA_REGS, i2cBuff, IMU_LSM6DSM_DATA_NUM_BYTES);

    idata.rawGyroX = (int16_t)(i2cBuff[0] | ((int16_t)i2cBuff[1] << 8));
    idata.rawGyroY = (int16_t)(i2cBuff[2] | ((int16_t)i2cBuff[3] << 8));
    idata.rawGyroZ = (int16_t)(i2cBuff[4] | ((int16_t)i2cBuff[5] << 8));

#else

#error Unknown - or no - IMU hardware selected.

#endif  // IMU type

    return &idata;
}

