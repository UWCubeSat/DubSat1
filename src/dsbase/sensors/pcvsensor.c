#include <msp430.h>
#include "bsp/bsp.h"
#include "pcvsensor.h""


#define MAX_BUFF_SIZE   0x0A

FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC hDev hSensor;

FILE_STATIC numSensors;
FILE_STATIC deviceContextPCVSensor sensors[MAX_NUM_PCVSENSORS];


/**
 * Initializes the current sensor. Will not do anything if this function
 * has run to completion before. Otherwise, it will reset all registers
 * and then write the configuration values to the calibration register.
 */
hDev pcvsensorInit(bus_instance_i2c bus, uint8_t i2cAddr) {

    hDev hSensor;

    if (numSensors == MAX_NUM_PCVSENSORS)
    {
        debugPrintF("Maximum number of PCV sensors already reached.\r\n");
        return MAX_NUM_PCVSENSORS;
    }

    if (sensors[numSensors].initialized)
    {
        debugPrintF("Sensor being initialized twice.\r\n");
        return MAX_NUM_PCVSENSORS+1;
    }

    // We will rely on I2C initialization to handle the fact that the
    // I2C bus shouldn't be initialized more than once
    hSensor = i2cInit(bus, i2cAddr);

    // Store per-sensor information
    uint8_t newIndex = numSensors++;
    sensors[newIndex].initialized = TRUE;
    sensors[newIndex].bus = bus;
    sensors[newIndex].slaveaddr = i2cAddr;  // probably don't need, already captured in handle to I2C device
    sensors[newIndex].hI2CDevice = hSensor;

    // Configure sensor
    // Reminder:  INA219 has registers with ONE byte addresses, but TWO byte lengths (big-endian ordering)
    i2cBuff[0] = CUR_INA219_REG_ADDR_CONFIG;
    i2cBuff[1] = CUR_INA219_CONFIG_MAX_GAIN_MAX_SAMPLING_MSB;
    i2cBuff[2] = CUR_INA219_CONFIG_MAX_GAIN_MAX_SAMPLING_LSB;
    i2cMasterWrite(hSensor, i2cBuff, 3);

    // done
    return (hDev)newIndex;
}

PCVSensorData *pcvsensorRead(hDev hSensor)
{
    uint8_t devIndex = (uint8_t)hSensor;
    hDev i2cdevice = sensors[devIndex].hI2CDevice;
    uint16_t temp;

    i2cMasterRegisterRead(i2cdevice, CUR_INA219_REG_ADDR_SHUNT_V, i2cBuff, 2 );
    sensors[devIndex].sensordata.rawShuntVoltage = (uint16_t)(i2cBuff[1] | ((uint16_t)i2cBuff[0] << 8));
    i2cMasterRegisterRead(i2cdevice, CUR_INA219_REG_ADDR_BUS_V, i2cBuff, 2 );
    temp = (uint16_t)(i2cBuff[1] | ((uint16_t)i2cBuff[0] << 8));
    sensors[devIndex].sensordata.rawBusVoltage = temp >> 3;   // value is only top 13 bits of register, gah

    // Perform conversions
    sensors[devIndex].sensordata.busVoltageV = 0.004 * sensors[devIndex].sensordata.rawBusVoltage;
    sensors[devIndex].sensordata.shuntVoltageV = 0.00001 * sensors[devIndex].sensordata.rawShuntVoltage;

    return &(sensors[devIndex].sensordata);
}


