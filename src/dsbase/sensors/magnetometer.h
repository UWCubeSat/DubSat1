/*
 * magnetometer.h
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */

#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_

#include <stdint.h>

#include "../core/i2c.h"
#include "../core/utils.h"


#if defined(__BSP_HW_MAGTOM_HMC5883L__)   // Honeywell HMC5883L

#include "HMC5883L_Magnetometer.h"

#define MAG_I2C_7BIT_ADDRESS                        MAG_HMC5883L_I2C_7BIT_ADDRESS
#define MAG_XYZ_OUTPUT_REG_ADDR_START               MAG_HMC5883L_XYZ_OUTPUT_REG_ADDR_START
#define MAG_CONVERSION_FACTOR_RAW_TO_NANOTESLAS     HMC5883L_CONVERSION_FACTOR_RAW_TO_NANOTESLAS
#define MAG_CONVERSION_FACTOR_RAW_TO_TESLAS         HMC5883L_CONVERSION_FACTOR_RAW_TO_TESLAS

#elif defined(__BSP_HW_MAGTOM_MAG3110__)  // Freescale MAG3110

#include "MAG3110_Magnetometer.h"

#define MAG_I2C_7BIT_ADDRESS                        MAG_MAG3110_I2C_7BIT_ADDRESS
#define MAG_XYZ_OUTPUT_REG_ADDR_START               MAG_MAG3110_XYZ_OUTPUT_REG_ADDR_START
#define MAG_CONVERSION_FACTOR_RAW_TO_NANOTESLAS     MAG3110_CONVERSION_FACTOR_RAW_TO_NANOTESLAS
#define MAG_CONVERSION_FACTOR_RAW_TO_TESLAS         MAG3110_CONVERSION_FACTOR_RAW_TO_TESLAS

#else

#error  Unknown - or no - magnetometer specified.  Please see magnetometer.h for definition options.

#endif  /* MAGNETOMETER */


// Common definitions, regardless of magnetometer device type
#define MAG_CONVERSION_FACTOR_DEFAULT               MAG_CONVERSION_FACTOR_RAW_TO_NANOTESLAS


typedef enum _unitConversionMode {
    ConvertToNanoTeslas,
    ConvertToTeslas,
    ConvertToNone, // skip conversions
} UnitConversionMode;

// Raw values are magnetometer-dependent
// Converted values have units, specified in conversionMode
typedef struct  {
    UnitConversionMode conversionMode;

    int16_t rawX;
    int16_t rawY;
    int16_t rawZ;
    float convertedX;
    float convertedY;
    float convertedZ;
} MagnetometerData;

typedef uint8_t hMag;

/*
 * Initializes a magnetometer on a specific I2C bus.
 * Initialize no more than one per bus.
 *
 * Returns the magnetometer's handle to be passed to other mag. functions
 */
hMag magInit(bus_instance_i2c bus);

MagnetometerData *magReadXYZData(hMag handle, UnitConversionMode mode);
void mag_self_test_config(hMag handle);
void mag_normal_reading_operation_config(hMag handle);

/**
 * Convert a raw mag reading to Teslas
 */
float magConvertRawToTeslas(int16_t raw);

/**
 * Convert Teslas to the same units that the raw readings use
 */
int16_t magConvertTeslasToRaw(float teslas);

#endif /* MAGNETOMETER_H_ */
