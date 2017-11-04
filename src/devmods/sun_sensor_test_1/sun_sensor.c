#include <stdint.h>

#include "sun_sensor.h"
#include "core/utils.h"
#include "core/i2c.h"
#include "core/debugTools.h"

#define MAX_BUFF_SIZE 18

#define CMD_CODE_UNFILTERED_CELL_VOLTAGES 0x01
#define CMD_CODE_FILTERED_CELL_VOLTAGES   0x03
#define CMD_CODE_ANGULAR_POSITION         0x04

#define RESPONSE_LENGTH_UNFILTERED_CELL_VOLTAGES 18
#define RESPONSE_LENGTH_FILTERED_CELL_VOLTAGES   18
#define RESPONSE_LENGTH_ANGULAR_POSITION         10

FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC uint8_t i2cInitialized = 0;
FILE_STATIC hDev hSensor;

FILE_STATIC sun_sensor_voltage vUnfiltered[4];
FILE_STATIC sun_sensor_voltage vFiltered[4];
FILE_STATIC SunSensorAngle angleData;

void sunSensorInit()
{
    // Initialize once and only once for this device
    if (i2cInitialized != 0)
    {
        return;
    }

    i2cInitialized = 1;
    i2cEnable(I2CBus2);
    hSensor = i2cInit(I2CBus2, SUN_I2C_7BIT_ADDRESS);
}

/*
 * print and error message and return 1 if the size and ID in i2cBuff match
 */
FILE_STATIC uint8_t checkSizeAndId(uint8_t id, uint8_t size)
{
    uint8_t res = 0;
    if (i2cBuff[0] != id)
    {
        debugPrintF("wrong id. expected %u, actual %u", id, i2cBuff[0]);
        res = 1;
    }
    if (i2cBuff[1] != size)
    {
        debugPrintF("wrong size. expected %u, actual %u", size, i2cBuff[1]);
        res = 1;
    }
    return res;
}

sun_sensor_voltage *sunSensorReadUnfiltered()
{
    i2cMasterRegisterRead(hSensor, CMD_CODE_UNFILTERED_CELL_VOLTAGES, i2cBuff,
                          RESPONSE_LENGTH_UNFILTERED_CELL_VOLTAGES);
    if (checkSizeAndId(CMD_CODE_UNFILTERED_CELL_VOLTAGES,
                       RESPONSE_LENGTH_UNFILTERED_CELL_VOLTAGES))
    {
        return NULLP;
    }
    memcpy(vUnfiltered, i2cBuff + 2, 16);
    return vUnfiltered;
}

sun_sensor_voltage *sunSensorReadFiltered()
{
    i2cMasterRegisterRead(hSensor, CMD_CODE_FILTERED_CELL_VOLTAGES, i2cBuff,
                          RESPONSE_LENGTH_FILTERED_CELL_VOLTAGES);
    if (checkSizeAndId(CMD_CODE_FILTERED_CELL_VOLTAGES,
                       RESPONSE_LENGTH_FILTERED_CELL_VOLTAGES))
    {
        return NULLP;
    }
    memcpy(vFiltered, i2cBuff + 2, 16);
    return vFiltered;
}

SunSensorAngle *sunSensorReadAngle()
{
    i2cMasterRegisterRead(hSensor, CMD_CODE_ANGULAR_POSITION, i2cBuff,
                          RESPONSE_LENGTH_ANGULAR_POSITION);
    if (checkSizeAndId(CMD_CODE_ANGULAR_POSITION,
                       RESPONSE_LENGTH_ANGULAR_POSITION))
    {
        return NULLP;
    }
    angleData.alpha = *((float *) (i2cBuff + 2));
    angleData.beta = *((float *) (i2cBuff + 6));
    angleData.error = i2cBuff[10];
    return &angleData;
}
