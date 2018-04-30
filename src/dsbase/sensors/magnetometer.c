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

typedef struct {
    hDev hSensor;
    MagnetometerData data;
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
    //i2cBuff[1] = MAG_HMC5883L_AVERAGE_1_SAMPLE | MAG_HMC5883L_CONTINUOUS_OUTPUT_RATE_75 | MAG_HMC5883L_MEASURE_MODE_NORMAL;
    i2cBuff[1] = MAG_HMC5883L_AVERAGE_8_SAMPLE | MAG_HMC5883L_CONTINUOUS_OUTPUT_RATE_30 | MAG_HMC5883L_MEASURE_MODE_DEFAULT | MAG_HMC5883L_TEMP_COMP_MODE;
    i2cBuff[2] = MAG_HMC5883L_GAIN_1370;
    i2cBuff[3] = MAG_HMC5883L_OPERATING_MODE_CONTINUOUS;
    i2cMasterWrite(hSensor, i2cBuff, 4);
}

MagnetometerData *magReadXYZData(hMag handle, UnitConversionMode desiredConversion)
{
    hDev hSensor = mags[handle].hSensor;
    MagnetometerData *mdata = &(mags[handle].data);

    mdata->conversionMode = desiredConversion;
    i2cMasterRegisterRead(hSensor, MAG_XYZ_OUTPUT_REG_ADDR_START, i2cBuff, 6 );
    i2cMasterRegisterRead(hSensor, MAG_HMC5883L_REG_ADDR_TORA, &i2cBuff[6], 2 );

#if defined(__BSP_HW_MAGTOM_HMC5883L__)

    // NOTE:  Order of X/Z/Y on HMC5883 is, unfortunately, intentional ...

    mdata->rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
    mdata->rawZ = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
    mdata->rawY = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));

    mdata->rawTempA = (int8_t)i2cBuff[6];
    mdata->rawTempB = (int8_t)i2cBuff[7];

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



///*
// * magnetometer.c
// *
// *  Created on: Apr 9, 2017
// *      Author: jeffc
// */
//
//#include <stdint.h>
//#include "magnetometer.h"
//
//#define MAX_BUFF_SIZE   0x25
//
//// TODO:  Need some const decorations
//FILE_STATIC uint8_t szBuff;
//FILE_STATIC uint8_t tempArr[2];
//FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
//FILE_STATIC hDev hSensor;
//
//MagnetometerData mdata;
//
//void magInit(bus_instance_i2c bus)
//{
//    i2cEnable(bus);
//    hSensor = i2cInit(bus, MAG_I2C_7BIT_ADDRESS);
//
//#if defined(__BSP_HW_MAGTOM_HMC5883L__)  /* */
//
//    // HMC5883 pattern is to address
//    i2cBuff[0] = MAG_HMC5883L_REG_ADDR_CRA; // Configuration pin A
//    //i2cBuff[1] = MAG_HMC5883L_AVERAGE_1_SAMPLE | MAG_HMC5883L_CONTINUOUS_OUTPUT_RATE_75 | MAG_HMC5883L_MEASURE_MODE_NORMAL;
//    i2cBuff[1] = MAG_HMC5883L_AVERAGE_8_SAMPLE | MAG_HMC5883L_CONTINUOUS_OUTPUT_RATE_30 | MAG_HMC5883L_MEASURE_MODE_NORMAL | MAG_HMC5883L_TEMP_COMP_MODE;
//  //  | MAG_HMC5883L_TEMP_COMP_MODE
//    i2cBuff[2] = MAG_HMC5883L_REG_ADDR_CRB;
//    i2cBuff[3] = MAG_HMC5883L_GAIN_1370;
//    i2cBuff[4] = MAG_HMC5883L_REG_ADDR_MR;
//    i2cBuff[5] = MAG_HMC5883L_OPERATING_MODE_CONTINUOUS;
//
//    i2cMasterWrite(hSensor, i2cBuff, 6);
//
//#elif defined( __BSP_HW_MAGTOM_MAG3110__)
//
//    // MAG3110 auto increments the address pointer, so no need to tweak address as long as you do things in sequence
//    i2cBuff[0] = MAG_MAG3110_REG_ADDR_CTRL_REG2;
//    i2cBuff[1] = MAG_MAG3110_CTRL_REG2_AUTO_MAG_SENSOR_RESET | MAG_MAG3110_CTRL_REG2_RAW_MODE;
//    i2cRawWrite(i2cBuff, 2);
//
//    i2cBuff[0] = MAG_MAG3110_REG_ADDR_CTRL_REG1;
//    i2cBuff[1] = (MAG_MAG3110_CTRL_REG1_FAST_READ_OFF | MAG_MAG3110_CTRL_REG1_AUTO_MODE | \
//            MAG_MAG3110_CTRL_REG1_OSDR_COMBINATION_80HZ | MAG_MAG3110_CTRL_REG1_MODE_SELECT_ACTIVE);
//    i2cMasterWrite(i2cBuff, 2);
//
//#else
//
//#error Unknown - or no - magnetometer specified.  Use HW_MAGTOM definition to set type.
//
//#endif  /* HW_MAGTOM */
//}
//
//MagnetometerData *magReadXYZData(UnitConversionMode desiredConversion)
//{
//    mdata.conversionMode = desiredConversion;
//
//    i2cMasterRegisterRead(hSensor, MAG_XYZ_OUTPUT_REG_ADDR_START, i2cBuff, 6 );
////    i2cMasterRegisterRead(hSensor, MAG_TMP_OUTPUT_REG_ADDR_START, tempArr, 2 );
//
//#if defined(__BSP_HW_MAGTOM_HMC5883L__)
//
//    // NOTE:  Order of X/Z/Y on HMC5883 is, unfortunately, intentional ...
//    uint8_t rawX_A = i2cBuff[0];
//    uint8_t rawX_B = i2cBuff[1];
//    uint8_t rawZ_A = i2cBuff[2];
//    uint8_t rawZ_B = i2cBuff[3];
//    uint8_t rawY_A = i2cBuff[4];
//    uint8_t rawY_B = i2cBuff[5];
//    mdata.rawX = (int16_t)(rawX_B | ((int16_t)rawX_A << 8));
//    mdata.rawZ = (int16_t)(rawZ_B | ((int16_t)rawZ_A << 8));
//    mdata.rawY = (int16_t)(rawY_B | ((int16_t)rawY_A << 8));
////    mdata.rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
////    mdata.rawZ = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
////    mdata.rawY = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));
//
//    //    mdata.rawTemp = (int16_t)(i2cBuff[7] | ((int16_t)i2cBuff[6] << 8));
//    mdata.rawTemp = (int16_t)(i2cBuff[7] | ((int16_t)i2cBuff[6] << 8));
//    mdata.rawTempA = i2cBuff[6];
//    mdata.rawTempB = i2cBuff[7];
//
//#elif defined( __BSP_HW_MAGTOM_MAG3110__)
//
//    mdata.rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
//    mdata.rawY = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
//    mdata.rawZ = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));
//
//#else
//
//#error Unknown - or no - magnetometer specified.  Use HW_MAGTOM definition to set type.
//
//#endif /* HW_MAGTOM */
//
//
//    // todo:  Ultimately, this logic needs to move into the device-specific file
//    // (though some of it will be able to stay here, hopefully? just lookup conversion in a table
//    // in the device header?
//    double conversionFactor;
//    switch (desiredConversion)
//    {
//        case ConvertToNanoTeslas:
//            conversionFactor = MAG_CONVERSION_FACTOR_RAW_TO_NANOTESLAS;
//            break;
//        case ConvertToTeslas:
//            conversionFactor = MAG_CONVERSION_FACTOR_RAW_TO_TESLAS;
//            break;
//        default:
//            conversionFactor = MAG_CONVERSION_FACTOR_DEFAULT;
//            break;
//    }
//    // (MSB * 2^8 + LSB) / (2^4 * 8) + 25 in C
//
//    mdata.convertedX = mdata.rawX * conversionFactor;
//    mdata.convertedY = mdata.rawY * conversionFactor;
//    mdata.convertedZ = mdata.rawZ * conversionFactor;
//    mdata.convertedTemp = (float)((((float)mdata.rawTempA * 256.0)  +  (float) mdata.rawTempB * 1.0) / (8.0 * 16.0) + 25.0);
//
//    return &mdata;
//}


