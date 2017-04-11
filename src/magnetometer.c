/*
 * magnetometer.c
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */

#include <stdint.h>
#include "magnetometer.h"

#define MAX_BUFF_SIZE   0x20

// TODO:  Need some const decorations
uint8_t szBuff;
uint8_t i2cBuff[MAX_BUFF_SIZE];

MagnetometerData mdata;

void magInit()
{
#if HW_MAGTOM == 5883
    // Ready for configuration data
    i2cBuff[0] = MAG_ADDRESS_CRA;
    i2cBuff[1] = MAG_AVERAGE_1_SAMPLE | MAG_CONTINUOUS_OUTPUT_RATE_75 | MAG_MEASURE_MODE_NORMAL;
    i2cBuff[2] = MAG_ADDRESS_CRB;
    i2cBuff[3] = MAG_GAIN_1370;
    i2cBuff[4] = MAG_ADDRESS_MR;
    i2cBuff[5] = MAG_OPERATING_MODE_CONTINUOUS;
    szBuff = 6;
#elif HW_MAGTOM == 3110
    // TODO:  Do the MAG3110 setup
    // ...
    // szBuff = whatever;
#else
#error Unknown - or no - magnetometer specified.  Use MAGNETOMETER to set type.
#endif  /* MAGNETOMETER == HMC5883 */

    i2cRawWrite(i2cBuff, szBuff);
}

MagnetometerData *magReadXZYData()
{
    i2cCombinedAddressWriteThenRead(MAG_DATA_OUTPUT_ADDRESS_START, i2cBuff, 6 );

    // NOTE:  Order of X/Z/Y is, unfortunately, intentional ...
    mdata.rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
    mdata.rawZ = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
    mdata.rawY = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));

    return &mdata;
}
