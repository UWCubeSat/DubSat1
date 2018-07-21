/*
 * imu.c
 *
 *  Created on: Jul 4, 2017
 *      Author: jeffc
 */

#include <math.h>

#include "imu.h"

#define GLITCH_FILTER_MAX_DIFF 800

FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC uint8_t imuInitialized = 0;
FILE_STATIC hDev hSensor;

FILE_STATIC IMUData idata;
FILE_STATIC uint8_t hasRead = 0;



uint8_t imuInit(bus_instance_i2c i2cbus, IMUUpdateRate rate)
{
    if (imuInitialized != 0)
        return 0;

    imuInitialized = 1;
    i2cEnable(i2cbus);
    hSensor = i2cInit(i2cbus, IMU_I2C_7BIT_ADDRESS);

#if defined (__BSP_HW_IMU_BMI160__)

    // TODO:  Add initialization code
    i2cBuff[0] = 0x7E; // CMD register
    i2cBuff[1] = 0x15;  // SHOULD turn on gyro, and only gyro
    if(i2cMasterWrite(hSensor, i2cBuff, 2))
        return 1;

    // Turn on accel for now
    i2cBuff[0] = 0x7E;
    i2cBuff[1] = 0x11;
    return i2cMasterWrite(hSensor, i2cBuff, 2);

#elif defined(__BSP_HW_IMU_LSM6DSM__)

    uint8_t rateVal;
    switch (rate)
    {
    case IMUUpdateRate_12p5Hz:
        rateVal = IMU_LSM6DSM_ODR_12p5_FS125;
        break;
    case IMUUpdateRate_26Hz:
        rateVal = IMU_LSM6DSM_ODR_26_FS125;
        break;
    case IMUUpdateRate_52Hz:
        rateVal = IMU_LSM6DSM_ODR_52_FS125;
        break;
    case IMUUpdateRate_104Hz:
        rateVal = IMU_LSM6DSM_ODR_104_FS125;
        break;
    default:
        rateVal = IMU_LSM6DSM_ODR_26_FS125;
        break;
    }

    i2cBuff[0] = IMU_LSM6DSM_CTRL2_G;
    i2cBuff[1] = rateVal;
    i2cBuff[2] = IMU_LSM6DSM_CTRL7_G;
    i2cBuff[3] = IMU_LSM6DSM_HIGH_PERF_ON;
    return i2cMasterWrite(hSensor, i2cBuff, 4);

#else

#error Unknown - or no - IMU hardware selected.

#endif  // IMU type

}

IMUData *imuReadGyroAccelData()
{

#if defined (__BSP_HW_IMU_BMI160__)

    // TODO:  Add state read code, populate IMU data struct
    if(i2cMasterRegisterRead(hSensor, 0x0C, i2cBuff, 12) == 0)  // read data bytes, little-endian pairs
    {
        idata.rawGyroX = (int16_t)(i2cBuff[0] | ((int16_t)i2cBuff[1] << 8));
        idata.rawGyroY = (int16_t)(i2cBuff[2] | ((int16_t)i2cBuff[3] << 8));
        idata.rawGyroZ = (int16_t)(i2cBuff[4] | ((int16_t)i2cBuff[5] << 8));
    }
    else
        return &idata;

    //idata.rawAccelX = (int16_t)(i2cBuff[6] | ((int16_t)i2cBuff[7] << 8));
    //idata.rawAccelY = (int16_t)(i2cBuff[8] | ((int16_t)i2cBuff[9] << 8));
    //idata.rawAccelZ = (int16_t)(i2cBuff[10] | ((int16_t)i2cBuff[11] << 8));


    //i2cCombinedAddressWriteThenRead(0x1B, i2cBuff, 1);  // read status byte

#elif defined(__BSP_HW_IMU_LSM6DSM__)

#ifdef __I2C_DONT_WRITE_IMU__
    i2cMasterRead(hSensor, i2cBuff, IMU_LSM6DSM_DATA_NUM_BYTES);
#else
    i2cMasterRegisterRead(hSensor, IMU_LSM6DSM_OUTPUT_DATA_REGS, i2cBuff, IMU_LSM6DSM_DATA_NUM_BYTES);
#endif /* __I2C_DONT_WRITE_IMU__ */

#if defined(__HIL_AA_GLITCHFILTER__)
    int16_t prevX = idata.rawGyroX;
    int16_t prevY = idata.rawGyroY;
    int16_t prevZ = idata.rawGyroZ;
#endif /* __HIL_AA_GLITCHFILTER__ */

    // the datasheet says it's little-endian, but testing shows big-endian
    idata.rawGyroX = (int16_t)(i2cBuff[1] | ((uint16_t)i2cBuff[0] << 8));
    idata.rawGyroY = (int16_t)(i2cBuff[3] | ((uint16_t)i2cBuff[2] << 8));
    idata.rawGyroZ = (int16_t)(i2cBuff[5] | ((uint16_t)i2cBuff[4] << 8));

#if defined(__HIL_AA_GLITCHFILTER__)
    if (hasRead && abs(idata.rawGyroX - prevX) > GLITCH_FILTER_MAX_DIFF
    		|| abs(idata.rawGyroY - prevY) > GLITCH_FILTER_MAX_DIFF
			|| abs(idata.rawGyroZ - prevZ) > GLITCH_FILTER_MAX_DIFF)
    {
    	idata.rawGyroX = prevX;
    	idata.rawGyroY = prevY;
    	idata.rawGyroZ = prevZ;
    }

    hasRead = 1;
#endif /* __HIL_AA_GLITCHFILTER__ */

#else

#error Unknown - or no - IMU hardware selected.

#endif  // IMU type

    return &idata;
}

uint8_t imuWhoami()
{
#if defined(__BSP_HW_IMU_LSM6DSM__)
    i2cMasterRegisterRead(hSensor, IMU_LSM6DSM_WHOAMI_REGS, i2cBuff, 1);
#else

#error whoami not implemented for other IMUs

#endif  // IMU type
    return i2cBuff[0];
}

float imuConvertRawToRPS(int16_t raw)
{
    return raw * (IMU_RAW_TO_DPS * DEG_TO_RAD);
}

int16_t imuConvertRPSToRaw(float rps)
{
    return roundf(rps / (IMU_RAW_TO_DPS * DEG_TO_RAD));
}
