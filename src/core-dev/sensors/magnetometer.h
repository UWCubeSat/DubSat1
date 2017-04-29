/*
 * magnetometer.h
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */

#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_

#include <stdint.h>
#include "i2c_support.h"

#ifndef HW_MAGTOM
#warning  No magnetometer specified in HW_MAGTOM - defaulting to MAG3110.
#define HW_MAGTOM  3110
#endif  /* HW_MGTOM */

#if HW_MAGTOM == 5883   // Honeywell HMC5883L
#include "HMC5883L_Magnetometer.h"

#define MAG_I2C_7BIT_ADDRESS                        MAG_HMC5883L_I2C_7BIT_ADDRESS
#define MAG_XYZ_OUTPUT_REG_ADDR_START               MAG_HMC5883L_XYZ_OUTPUT_REG_ADDR_START
#define MAG_CONVERSION_FACTOR_RAW_TO_NANOTESLAS     HMC5883L_CONVERSION_FACTOR_RAW_TO_NANOTESLAS
#define MAG_CONVERSION_FACTOR_RAW_TO_TESLAS         HMC5883L_CONVERSION_FACTOR_RAW_TO_TESLAS

#elif HW_MAGTOM == 3110  // Freescale MAG3110

#include "MAG3110_Magnetometer.h"

#define MAG_I2C_7BIT_ADDRESS                        MAG_MAG3110_I2C_7BIT_ADDRESS
#define MAG_XYZ_OUTPUT_REG_ADDR_START               MAG_MAG3110_XYZ_OUTPUT_REG_ADDR_START
#define MAG_CONVERSION_FACTOR_RAW_TO_NANOTESLAS     MAG3110_CONVERSION_FACTOR_RAW_TO_NANOTESLAS
#define MAG_CONVERSION_FACTOR_RAW_TO_TESLAS         MAG3110_CONVERSION_FACTOR_RAW_TO_TESLAS

#else
#error Unknown - or no - magnetometer specified.  Use HW_MAGTOM to set valid type.
#endif  /* MAGNETOMETER */


// Common definitions, regardless of magnetometer device type
#define MAG_CONVERSION_FACTOR_DEFAULT               MAG_CONVERSION_FACTOR_RAW_TO_NANOTESLAS


typedef enum _unitConversionMode {
    ConvertToNanoTeslas,
    ConvertToTeslas,
} UnitConversionMode;

// Raw values are magnetometer-dependent
// Converted values have units, specified in conversionMode
typedef struct  {
    UnitConversionMode conversionMode;

    int16_t rawX;
    int16_t rawY;
    int16_t rawZ;
    double convertedX;
    double convertedY;
    double convertedZ;
} MagnetometerData;

// Main entry points
void magInit();
MagnetometerData *magReadXYZData(UnitConversionMode);

#endif /* MAGNETOMETER_H_ */
