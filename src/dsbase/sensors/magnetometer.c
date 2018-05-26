/*
 * magnetometer.c
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */

#include <stdint.h>
#include <math.h>
#include "magnetometer.h"

#define MAX_BUFF_SIZE   0x25
#define MAX_NUM_MAGNETOMETERS 2 // one for each i2c bus
#define GLITCH_FILTER_MAX_DIFF 10000
typedef struct {
    hDev hSensor;
    MagnetometerData data;
    MagnetometerData prev_data;
} MagInternalData;


// TODO:  Need some const decorations
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];

FILE_STATIC MagInternalData mags[MAX_NUM_MAGNETOMETERS];
FILE_STATIC uint8_t numRegistered = 0;

hMag magInit(bus_instance_i2c bus)
{
    i2cEnable(bus);
    hDev hSensor = i2cInit(bus, MAG_I2C_7BIT_ADDRESS);
    mags[numRegistered].hSensor = hSensor;

#if defined(__BSP_HW_MAGTOM_HMC5883L__)  /* */

    // HMC5883 pattern is to address

   // selfTestConfig(numRegistered);
   normalOperationConfig(numRegistered);

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

    return numRegistered++;
}

void selfTestConfig(hMag handle)
{
    hDev hSensor = mags[handle].hSensor;

    i2cBuff[0] = MAG_HMC5883L_REG_ADDR_CRA;
    i2cBuff[1] = MAG_HMC5883L_CONFIG_REGISTER_A_SELF_TEST;
    i2cBuff[2] = MAG_HMC5883L_CONFIG_REGISTER_B_SELF_TEST;
    i2cBuff[3] = MAG_HMC5883L_MODE_REGISTER_SELF_TEST;
    i2cMasterWrite(hSensor, i2cBuff, 4);
}


void normalOperationConfig(hMag handle)
{
    hDev hSensor = mags[handle].hSensor;

    // HMC5883 pattern is to address
    i2cBuff[0] = MAG_HMC5883L_REG_ADDR_CRA;
    i2cBuff[1] = MAG_HMC5883L_AVERAGE_8_SAMPLE | MAG_HMC5883L_CONTINUOUS_OUTPUT_RATE_30 | MAG_HMC5883L_MEASURE_MODE_DEFAULT | MAG_HMC5883L_TEMP_COMP_MODE;
    i2cBuff[2] = MAG_HMC5883L_GAIN_1370;
    i2cBuff[3] = MAG_HMC5883L_OPERATING_MODE_CONTINUOUS;
    i2cMasterWrite(hSensor, i2cBuff, 4);
}

MagnetometerData *magReadXYZData(hMag handle, UnitConversionMode desiredConversion)
{
    hDev hSensor = mags[handle].hSensor;
    MagnetometerData *mdata = &(mags[handle].data);
    MagnetometerData *prevData = &(mags[handle].prev_data);

#if defined(__BSP_HW_MAGTOM_HMC5883L__)

#if defined(__HIL_AA_NOEXTRAREADS__)
    i2cMasterRead(hSensor, i2cBuff, 6 );
#else
    i2cMasterRegisterRead(hSensor, MAG_XYZ_OUTPUT_REG_ADDR_START, i2cBuff, 6 );
#endif
    // NOTE:  Order of X/Z/Y on HMC5883 is, unfortunately, intentional ...

    prevData->conversionMode = mdata->conversionMode;
    prevData->rawX = mdata->rawX;
    prevData->rawY = mdata->rawY;
    prevData->rawZ = mdata->rawZ;

    mdata->conversionMode = desiredConversion;
    mdata->rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
    mdata->rawZ = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
    mdata->rawY = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));

    mdata->rawTempA = (int8_t)i2cBuff[6];
    mdata->rawTempB = (int8_t)i2cBuff[7];

#if defined(__HIL_AA_GLITCHFILTER__)

    if(abs(mdata->rawX - prevData->rawX) || abs(mdata->rawY - prevData->rawY) || abs(mdata->rawZ - prevData->rawZ)  && (mdata->conversionMode == prevData->conversionMode))
    {
        mdata->rawX = prevData->rawX;
        mdata->rawY = prevData->rawY;
        mdata->rawZ = prevData->rawZ;
    }
#endif

#elif defined( __BSP_HW_MAGTOM_MAG3110__)

    mdata->rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
    mdata->rawY = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
    mdata->rawZ = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));

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
        case ConvertToNone:
            // return early to skip conversions
            return mdata;
        default:
            conversionFactor = MAG_CONVERSION_FACTOR_DEFAULT;
            break;
    }

    // (MSB * 2^8 + LSB) / (2^4 * 8) + 25 in C

    mdata->convertedX = mdata->rawX * conversionFactor;
    mdata->convertedY = mdata->rawY * conversionFactor;
    mdata->convertedZ = mdata->rawZ * conversionFactor;
    mdata->convertedTemp = (double)((((double)mdata->rawTempA * 256.0)  +  (double) mdata->rawTempB * 1.0) / (8.0 * 16.0) + 25.0);

    return mdata;
}

float magConvertRawToTeslas(int16_t raw)
{
    return raw * MAG_CONVERSION_FACTOR_RAW_TO_TESLAS;
}

int16_t magConvertTeslasToRaw(float teslas)
{
    return roundf(teslas / MAG_CONVERSION_FACTOR_RAW_TO_TESLAS);
}


