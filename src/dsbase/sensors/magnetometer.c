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
#define MAX_CALIBRATION_SAMPLES 15
#define CALIBRATION_IGNORE_SAMPLES 5
#define MAX_NUM_MAGNETOMETERS 2 // one for each i2c bus
#define MIN_VALID_MAG_READING 0xF800
#define MAX_VALID_MAG_READING 0x07FF

#define NORMAL_OPERATION 1
#define SELF_TEST_OPERATION 0

FILE_STATIC uint16_t SELF_TEST_NOMINAL_X = MAG_HMC5883L_SELF_TEST_GAUSS_FACTOR_X * MAG_HMC5883L_GAIN_1370_LSB_GAUSS;
FILE_STATIC uint16_t SELF_TEST_NOMINAL_Y = MAG_HMC5883L_SELF_TEST_GAUSS_FACTOR_Y * MAG_HMC5883L_GAIN_1370_LSB_GAUSS;
FILE_STATIC uint16_t SELF_TEST_NOMINAL_Z = MAG_HMC5883L_SELF_TEST_GAUSS_FACTOR_Z * MAG_HMC5883L_GAIN_1370_LSB_GAUSS;

#if defined(__HIL_AA_GLITCHFILTER__)
#define GLITCH_FILTER_MAX_DIFF 50
typedef struct {
    hDev hSensor;
    MagnetometerData data;
    MagnetometerData prev_data;
    uint16_t prev_difference_x;
    uint16_t prev_difference_y;
    uint16_t prev_difference_z;
    uint16_t glitter_count;
} MagInternalData;
FILE_STATIC uint8_t first_read = 1;
#else
typedef struct {
    hDev hSensor;
    MagnetometerData data;
    uint16_t calibration_x[MAX_CALIBRATION_SAMPLES];
    uint16_t calibration_y[MAX_CALIBRATION_SAMPLES];
    uint16_t calibration_z[MAX_CALIBRATION_SAMPLES];
    uint8_t curr_calibration_index;
    uint8_t operation_mode;
} MagInternalData;
#endif

// TODO:  Need some const decorations
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];

FILE_STATIC MagInternalData mags[MAX_NUM_MAGNETOMETERS];
FILE_STATIC uint8_t numRegistered = 0;

hMag magInit(bus_instance_i2c bus)
{
    i2cEnable(bus);
    hDev hSensor = i2cInit(bus, MAG_I2C_7BIT_ADDRESS);
    mags[numRegistered].hSensor = hSensor;
    mags[numRegistered].curr_calibration_index = 0;
    mags[numRegistered].operation_mode = NORMAL_OPERATION;
    mags[numRegistered].data.calibration_factor_x = 1.0;
    mags[numRegistered].data.calibration_factor_y = 1.0;
    mags[numRegistered].data.calibration_factor_z = 1.0;

#if defined(__BSP_HW_MAGTOM_HMC5883L__)  /* */

    // HMC5883 pattern is to address

   // selfTestConfig(numRegistered);
    mag_normal_reading_operation_config(numRegistered);

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

/* Gain = 5
   Expected Results for self test
   For x and y data output registers: 390 * 1.16 = +452 --> 452 * 73 = 32996;
   For z data output registers: 390 & 1.08 = +421 * 73 = 30733; */
void mag_self_test_config(hMag handle)
{
    hDev hSensor = mags[handle].hSensor;

    i2cBuff[0] = MAG_HMC5883L_REG_ADDR_CRA;
    i2cBuff[1] = MAG_HMC5883L_CONFIG_REGISTER_A_SELF_TEST;
    i2cBuff[2] = MAG_HMC5883L_CONFIG_REGISTER_B_SELF_TEST;
    i2cBuff[3] = MAG_HMC5883L_MODE_REGISTER_SELF_TEST;
    i2cMasterWrite(hSensor, i2cBuff, 4);
}


void mag_normal_reading_operation_config(hMag handle)
{
    hDev hSensor = mags[handle].hSensor;

    // HMC5883 pattern is to address
    i2cBuff[0] = MAG_HMC5883L_REG_ADDR_CRA;
    // Configuration Register A: Average of 8 samples, 30Hz sample rate, normal measurement configuration (no bias)
    i2cBuff[1] = MAG_HMC5883L_AVERAGE_8_SAMPLE | MAG_HMC5883L_CONTINUOUS_OUTPUT_RATE_30 | MAG_HMC5883L_MEASURE_MODE_DEFAULT;
    // Configuration Register B: Gain (Lsb/Gauss) = 1370 --> nT gain factor of 73.0; Output range: 0xF800 - 0x07ff (-2048 - 2047)
    i2cBuff[2] = MAG_HMC5883L_GAIN_1370; // Gain of 73
    i2cBuff[3] = MAG_HMC5883L_OPERATING_MODE_CONTINUOUS;
    i2cMasterWrite(hSensor, i2cBuff, 4);
}

MagnetometerData *magReadXYZData(hMag handle, UnitConversionMode desiredConversion)
{
    hDev hSensor = mags[handle].hSensor;
    MagnetometerData *mdata = &(mags[handle].data);
#if defined(__BSP_HW_MAGTOM_HMC5883L__)

#if defined(__HIL_AA_NOEXTRAREADS__)
    i2cMasterRead(hSensor, i2cBuff, 6 );
#else
    i2cMasterRegisterRead(hSensor, MAG_XYZ_OUTPUT_REG_ADDR_START, i2cBuff, 6 );
#endif

#if defined(__HIL_AA_GLITCHFILTER__)

    MagnetometerData *prevData = &(mags[handle].prev_data);
    uint16_t * prev_dif_x = &(mags[handle].prev_difference_x);
    uint16_t * prev_dif_y = &(mags[handle].prev_difference_y);
    uint16_t * prev_dif_z = &(mags[handle].prev_difference_z);
    uint16_t * glitter_count = &(mags[handle].glitter_count);

    // NOTE:  Order of X/Z/Y on HMC5883 is, unfortunately, intentional ...
    if(!first_read)
    {
        *prev_dif_x = abs((int32_t)prevData->rawX - (int32_t)mdata->rawX);
        *prev_dif_y = abs((int32_t)prevData->rawY - (int32_t)mdata->rawY);
        *prev_dif_z = abs((int32_t)prevData->rawZ - (int32_t)mdata->rawZ);
        prevData->conversionMode = mdata->conversionMode;
        prevData->rawX = mdata->rawX;
        prevData->rawY = mdata->rawY;
        prevData->rawZ = mdata->rawZ;
    }

    mdata->conversionMode = desiredConversion;
    mdata->rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
    mdata->rawZ = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
    mdata->rawY = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));

    if(first_read)
    {
        prevData->conversionMode = mdata->conversionMode;
        prevData->rawX = mdata->rawX;
        prevData->rawY = mdata->rawY;
        prevData->rawZ = mdata->rawZ;
        *glitter_count = 0;
        *prev_dif_x = 0;
        *prev_dif_y = 0;
        *prev_dif_z = 0;
        first_read = 0;
    }

    if(abs((int32_t)mdata->rawX - (int32_t)prevData->rawX) > GLITCH_FILTER_MAX_DIFF || abs((int32_t)mdata->rawY - (int32_t)prevData->rawY) > GLITCH_FILTER_MAX_DIFF || abs((int32_t)mdata->rawZ - (int32_t)prevData->rawZ) > GLITCH_FILTER_MAX_DIFF)
    {
        if(*prev_dif_x > GLITCH_FILTER_MAX_DIFF || *prev_dif_y > GLITCH_FILTER_MAX_DIFF || *prev_dif_z > GLITCH_FILTER_MAX_DIFF)
        {
            prevData->rawX = mdata->rawX;
            prevData->rawY = mdata->rawY;
            prevData->rawZ = mdata->rawZ;
        } else
        {
            (*glitter_count)++;
        }
    }

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
            return prevData;
        default:
            conversionFactor = MAG_CONVERSION_FACTOR_DEFAULT;
            break;
    }

    // (MSB * 2^8 + LSB) / (2^4 * 8) + 25 in C

    prevData->convertedX = prevData->rawX * conversionFactor;
    prevData->convertedY = prevData->rawY * conversionFactor;
    prevData->convertedZ = prevData->rawZ * conversionFactor;

    return prevData;
#else

    mdata->conversionMode = desiredConversion;
    mdata->rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
    mdata->rawZ = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
    mdata->rawY = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));
    mdata->isValid = mag_is_reading_valid(mdata);

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
    mdata->isValid = mag_is_reading_valid(mdata) && (mags[handle].operation_mode == NORMAL_OPERATION);

    return mdata;
#endif

#elif defined( __BSP_HW_MAGTOM_MAG3110__)

    mdata->rawX = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
    mdata->rawY = (int16_t)(i2cBuff[3] | ((int16_t)i2cBuff[2] << 8));
    mdata->rawZ = (int16_t)(i2cBuff[5] | ((int16_t)i2cBuff[4] << 8));

#else

#error Unknown - or no - magnetometer specified.  Use HW_MAGTOM definition to set type.

#endif /* HW_MAGTOM */
}

float magConvertRawToTeslas(int16_t raw)
{
    return raw * MAG_CONVERSION_FACTOR_RAW_TO_TESLAS;
}

int16_t magConvertTeslasToRaw(float teslas)
{
    return roundf(teslas / MAG_CONVERSION_FACTOR_RAW_TO_TESLAS);
}

uint8_t mag_is_reading_valid(MagnetometerData* data)
{
    if(data->rawX > MAX_VALID_MAG_READING || data->rawX < MIN_VALID_MAG_READING)
    {
        return 0;
    }
    if(data->rawY > MAX_VALID_MAG_READING || data->rawY < MIN_VALID_MAG_READING)
    {
        return 0;
    }
    if(data->rawZ > MAX_VALID_MAG_READING || data->rawZ < MIN_VALID_MAG_READING)
    {
        return 0;
    }
    return 1;
}

void start_self_test_calibration(hMag handle)
{
    mag_self_test_config(handle);
    mags[handle].operation_mode = SELF_TEST_OPERATION;
    mags[handle].curr_calibration_index = 0;
}

void self_test_add_samples(hMag handle, MagnetometerData* data)
{
    if(mags[handle].operation_mode != SELF_TEST_OPERATION) return;
    uint8_t curr_index = mags[handle].curr_calibration_index;
    if(curr_index >= MAX_CALIBRATION_SAMPLES) return;
    mags[handle].calibration_x[curr_index] = data->rawX;
    mags[handle].calibration_y[curr_index] = data->rawY;
    mags[handle].calibration_z[curr_index] = data->rawZ;
    mags[handle].curr_calibration_index++;
}

/* returns 1 if a new calibration number was generated, else 0 */
uint8_t end_self_test_calibration(hMag handle)
{
    mag_normal_reading_operation_config(handle);
    uint8_t curr_index = mags[handle].curr_calibration_index;
    MagnetometerData* data = &(mags[handle].data);
    uint8_t i;
    if(curr_index != MAX_CALIBRATION_SAMPLES) return 0;
    float avg_x = 0;
    float avg_y = 0;
    float avg_z = 0;

    for(i = CALIBRATION_IGNORE_SAMPLES; i < curr_index; i++)
    {
        avg_x += mags[handle].calibration_x[i];
        avg_y += mags[handle].calibration_y[i];
        avg_z += mags[handle].calibration_z[i];
    }
    avg_x = avg_x / (MAX_CALIBRATION_SAMPLES - CALIBRATION_IGNORE_SAMPLES);
    avg_y = avg_y / (MAX_CALIBRATION_SAMPLES - CALIBRATION_IGNORE_SAMPLES);
    avg_z = avg_z / (MAX_CALIBRATION_SAMPLES - CALIBRATION_IGNORE_SAMPLES);
    data->calibration_factor_x = SELF_TEST_NOMINAL_X / avg_x;
    data->calibration_factor_y = SELF_TEST_NOMINAL_Y / avg_y;
    data->calibration_factor_z = SELF_TEST_NOMINAL_Z / avg_z;
    return 1;
}
