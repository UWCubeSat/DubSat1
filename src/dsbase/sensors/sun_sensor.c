#include <stdint.h>

#include "sensors/sun_sensor.h"
#include "core/utils.h"
#include "core/i2c.h"
#include "core/debugtools.h"

#define MAX_BUFF_SIZE 17

#define CMD_CODE_UNFILTERED_CELL_VOLTAGES 0x01
#define CMD_CODE_FILTERED_CELL_VOLTAGES   0x03
#define CMD_CODE_ANGULAR_POSITION         0x04

/*
 * Manuel claims the voltage messages have size 18, but they're really 17
 */
#define RESPONSE_LENGTH_UNFILTERED_CELL_VOLTAGES 17
#define RESPONSE_LENGTH_FILTERED_CELL_VOLTAGES   17
#define RESPONSE_LENGTH_ANGULAR_POSITION         10

#define GLITCH_FILTER_MAX_VAL 180

FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC uint8_t i2cInitialized = 0;
FILE_STATIC hDev hSensor;

FILE_STATIC sun_sensor_voltage vUnfiltered[4];
FILE_STATIC sun_sensor_voltage vFiltered[4];
FILE_STATIC SunSensorAngle angleData;

void sunSensorInit(bus_instance_i2c i2cbus)
{
    // Initialize once and only once for this device
    if (i2cInitialized != 0)
    {
        return;
    }

    i2cInitialized = 1;
    i2cEnable(i2cbus);
    hSensor = i2cInit(i2cbus, SUN_I2C_7BIT_ADDRESS);
}

/*
 * print an error message and return 1 if the size does not match
 */
FILE_STATIC uint8_t checkSize(uint8_t size)
{
    if (i2cBuff[0] != size)
    {
        debugPrintF("wrong size. expected %u, actual %u\r\n", size, i2cBuff[0]);
        return 1;
    }
    return 0;
}

sun_sensor_voltage *sunSensorReadUnfiltered()
{
    i2cMasterRegisterRead(hSensor, CMD_CODE_UNFILTERED_CELL_VOLTAGES, i2cBuff,
                          RESPONSE_LENGTH_UNFILTERED_CELL_VOLTAGES);
    if (checkSize(RESPONSE_LENGTH_UNFILTERED_CELL_VOLTAGES))
    {
        return NULLP;
    }
    memcpy(vUnfiltered, i2cBuff + 1, 16);
    return vUnfiltered;
}

sun_sensor_voltage *sunSensorReadFiltered()
{
    i2cMasterRegisterRead(hSensor, CMD_CODE_FILTERED_CELL_VOLTAGES, i2cBuff,
                          RESPONSE_LENGTH_FILTERED_CELL_VOLTAGES);
    if (checkSize(RESPONSE_LENGTH_FILTERED_CELL_VOLTAGES))
    {
        return NULLP;
    }
    memcpy(vFiltered, i2cBuff + 1, 16);
    return vFiltered;
}

SunSensorAngle *sunSensorReadAngle()
{
#ifdef __I2C_DONT_WRITE_SUN__
    i2cMasterRead(hSensor, i2cBuff, RESPONSE_LENGTH_ANGULAR_POSITION);
#else
    i2cMasterRegisterRead(hSensor, CMD_CODE_ANGULAR_POSITION, i2cBuff,
                          RESPONSE_LENGTH_ANGULAR_POSITION);
#endif /* __I2C_DONT_WRITE_SUN__ */
    if (checkSize(RESPONSE_LENGTH_ANGULAR_POSITION))
    {
        return NULLP;
    }

#if defined(__HIL_AA_GLITCHFILTER__)
    float prevAlpha = angleData.alpha;
    float prevBeta = angleData.beta;
    uint8_t prevError = angleData.error;
#endif /* __HIL_AA_GLITCHFILTER__ */

    memcpy(&angleData.alpha, i2cBuff + 1, 4);
    memcpy(&angleData.beta, i2cBuff + 5, 4);
    angleData.error = i2cBuff[9];

#if defined(__HIL_AA_GLITCHFILTER__)
    if (abs(angleData.alpha) > GLITCH_FILTER_MAX_VAL
    		|| abs(angleData.beta) > GLITCH_FILTER_MAX_VAL
			|| angleData.error > 14)
    {
    	angleData.alpha = prevAlpha;
    	angleData.beta = prevBeta;
    	angleData.error = prevError;
    }
#endif /* __HIL_AA_GLITCHFILTER__ */

    return &angleData;
}
