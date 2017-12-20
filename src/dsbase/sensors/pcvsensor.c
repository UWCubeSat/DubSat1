#include <msp430.h>
#include "bsp/bsp.h"
#include "pcvsensor.h""


#define MAX_BUFF_SIZE   0x0A

FILE_STATIC uint8_t i2cInitialized = 0;
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC hDev hSensor;

PCVSensorData pcvsensordata;

/**
 * Initializes the current sensor. Will not do anything if this function
 * has run to completion before. Otherwise, it will reset all registers
 * and then write the configuration values to the calibration register.
 *
 * Returns 0 on success.
 * Returns 1 if sensor has been initialized before.
 */
int pcvsensorInit(uint8_t i2cAddr) {
    // don't do anything if we've run to completion before
    if (i2cInitialized != 0)
            return 1;

    // i2c initialization stuff
    i2cEnable(I2CBus2); // TODO bus 2 or 1?
    hSensor = i2cInit(I2CBus2, i2cAddr);

    // reset command, clears all registers on the INA219
    i2cBuff[0] = CUR_INA219_REG_ADDR_CONFIG;
    i2cBuff[1] = CUR_INA219_CONFIG_MAX_GAIN_MAX_SAMPLING_MSB;
    i2cBuff[2] = CUR_INA219_CONFIG_MAX_GAIN_MAX_SAMPLING_LSB;
    i2cMasterWrite(hSensor, i2cBuff, 3);

    // done
    i2cInitialized = 1;
    return 0;
}

PCVSensorData *pcvsensorRead()
{
    uint16_t temp;

    i2cMasterRegisterRead(hSensor, CUR_INA219_REG_ADDR_SHUNT_V, i2cBuff, 2 );
    pcvsensordata.rawShuntVoltage = (uint16_t)(i2cBuff[1] | ((uint16_t)i2cBuff[0] << 8));
    i2cMasterRegisterRead(hSensor, CUR_INA219_REG_ADDR_BUS_V, i2cBuff, 2 );
    temp = (uint16_t)(i2cBuff[1] | ((uint16_t)i2cBuff[0] << 8));
    pcvsensordata.rawBusVoltage = temp >> 3;   // value is only top 13 bits of register, gah

    // Perform conversions
    pcvsensordata.busVoltageV = 0.004 * pcvsensordata.rawBusVoltage;
    pcvsensordata.shuntVoltageV = 0.00001 * pcvsensordata.rawShuntVoltage;

    return &pcvsensordata;
}


