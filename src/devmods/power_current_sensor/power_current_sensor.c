#include <msp430.h>
#include "bsp/bsp.h"
#include <power_current_sensor.h>
#include <INA219_current_sensor.h>

#define MAX_BUFF_SIZE   0x0A

FILE_STATIC uint8_t i2cInitialized = 0;
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC hDev hSensor;

/**
 * Initializes the current sensor. Will not do anything if this function
 * has run to completion before. Otherwise, it will reset all registers
 * and then write the configuration values to the calibration register.
 *
 * Returns 0 on success.
 * Returns 1 if sensor has been initialized before.
 */
int currentSensorInit() {
    // don't do anything if we've run to completion before
    if (i2cInitialized != 0)
            return 1;

    // i2c initialization stuff
    i2cEnable(I2CBus2); // TODO bus 2 or 1?
    hSensor = i2cInit(I2CBus2, INA219_CURRENT_SENSOR_I2C_ADDRESS);

    // reset command, clears all registers on the INA219
    i2cBuff[0] = CUR_INA219_REG_ADDR_CONFIG;
    i2cBuff[1] = CUR_INA219_REG_CONFIG_RESET_LSB;
    i2cBuff[2] = CUR_INA219_REG_ADDR_CONFIG;
    i2cBuff[3] = CUR_INA219_REG_CONFIG_RESET_MSB;
    i2cMasterWrite(hSensor, i2cBuff, 4);

    // done
    i2cInitialized = 1;
    return 0;
}

int main(void) {
    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
    currentSensorInit();
}
