/*
 * magnetometer.c
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */

#include <stdint.h>
#include "magnetometer.h"

#define MAX_BUFF_SIZE   0x25

// TODO:  Need some const decorations
FILE_STATIC uint8_t szBuff;
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC uint8_t i2cInitialized = 0;

MagnetometerData mdata;

void magInit()
{
    // Initialize once and only once for this device
    if (i2cInitialized != 0)
        return;

    i2cInitialized = 1;
    i2cEnable();
    i2cInit(I2CBus2, MAG_I2C_7BIT_ADDRESS);

#if defined(__BSP_HW_MAGTOM_HMC5883L__)  /* */

    // HMC5883 pattern is to address
    i2cBuff[0] = MAG_HMC5883L_REG_ADDR_CRA;
    //i2cBuff[1] = MAG_HMC5883L_AVERAGE_1_SAMPLE | MAG_HMC5883L_CONTINUOUS_OUTPUT_RATE_75 | MAG_HMC5883L_MEASURE_MODE_NORMAL;
    i2cBuff[1] = MAG_HMC5883L_AVERAGE_8_SAMPLE | MAG_HMC5883L_CONTINUOUS_OUTPUT_RATE_30 | MAG_HMC5883L_MEASURE_MODE_NORMAL;
    i2cBuff[2] = MAG_HMC5883L_REG_ADDR_CRB;
    i2cBuff[3] = MAG_HMC5883L_GAIN_1370;
    i2cBuff[4] = MAG_HMC5883L_REG_ADDR_MR;
    i2cBuff[5] = MAG_HMC5883L_OPERATING_MODE_CONTINUOUS;

    i2cMasterWrite(i2cBuff, 6);

#elif defined( __BSP_HW_MAGTOM_MAG3110__)

    // MAG3110 auto increments the address pointer, so no need to tweak address as long as you do things in sequence
    i2cBuff[0] = MAG_MAG3110_REG_ADDR_CTRL_REG2;
    i2cBuff[1] = MAG_MAG3110_CTRL_REG2_AUTO_MAG_SENSOR_RESET | MAG_MAG3110_CTRL_REG2_RAW_MODE;
    i2cRawWrite(i2cBuff, 2);

    i2cBuff[0] = MAG_MAG3110_REG_ADDR_CTRL_REG1;
    i2cBuff[1] = (MAG_MAG3110_CTRL_REG1_FAST_READ_OFF | MAG_MAG3110_CTRL_REG1_AUTO_MODE | \
            MAG_MAG3110_CTRL_REG1_OSDR_COMBINATION_80HZ | MAG_MAG3110_CTRL_REG1_MODE_SELECT_ACTIVE);
    i2cMasterWrite(i2cBuff, 2);

#else

#error Unknown - or no - magnetometer specified.  Use HW_MAGTOM definition to set type.

#endif  /* HW_MAGTOM */
}

MagnetometerData *magReadXYZData(UnitConversionMode desiredConversion)
{
    mdata.conversionMode = desiredConversion;
    i2cMasterRegisterRead(MAG_XYZ_OUTPUT_REG_ADDR_START, i2cBuff, 6 );

#if defined(__BSP_HW_MAGTOM_HMC5883L__)

    // NOTE:  Order of X/Z/Y on HMC5883 is, unfortunately, intentional ...
    mdata.rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
    mdata.rawZ = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
    mdata.rawY = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));

#elif defined( __BSP_HW_MAGTOM_MAG3110__)

    mdata.rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
    mdata.rawY = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
    mdata.rawZ = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));

#else

#error Unknown - or no - magnetometer specified.  Use HW_MAGTOM definition to set type.

#endif /* HW_MAGTOM */


    // todo:  Ultimately, this logic needs to move into the device-specific file
    // (though some of it will be able to stay here, hopefully? just lookup conversion in a table
    // in the device header?
    double conversionFactor;
    switch (desiredConversion)
    {
        case ConvertToNanoTeslas:
            conversionFactor = MAG_CONVERSION_FACTOR_RAW_TO_NANOTESLAS;
            break;
        case ConvertToTeslas:
            conversionFactor = MAG_CONVERSION_FACTOR_RAW_TO_TESLAS;
            break;
        default:
            conversionFactor = MAG_CONVERSION_FACTOR_DEFAULT;
            break;
    }

    mdata.convertedX = mdata.rawX * conversionFactor;
    mdata.convertedY = mdata.rawY * conversionFactor;
    mdata.convertedZ = mdata.rawZ * conversionFactor;

    return &mdata;
}


