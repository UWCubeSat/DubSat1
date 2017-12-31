#include <msp430.h>
#include <math.h>

#include "bsp/bsp.h"
#include "pcvsensor.h"


#define MAX_BUFF_SIZE   0x10

FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC hDev hSensor;

FILE_STATIC numSensors;
FILE_STATIC deviceContextPCVSensor sensors[MAX_NUM_PCVSENSORS];

FILE_STATIC currentLSB = 0;

/**
 * Initializes the current sensor. Will not do anything if this function
 * has run to completion before. Otherwise, it will reset all registers
 * and then write the configuration values to the calibration register.
 */
hDev pcvsensorInit(bus_instance_i2c bus, uint8_t i2cAddr, float shuntResistance, float maxCurrent) {

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

    __delay_cycles(0.25 * SEC);

    // Store per-sensor information
    uint8_t newIndex = numSensors++;
    sensors[newIndex].initialized = TRUE;
    sensors[newIndex].bus = bus;
    sensors[newIndex].slaveaddr = i2cAddr;  // probably don't need, already captured in handle to I2C device
    sensors[newIndex].hI2CDevice = hSensor;
    sensors[newIndex].shuntResistance = shuntResistance;
    if (maxCurrent == 0)
        maxCurrent = DEFAULT_MAX_CURRENT_AMPS;
    sensors[newIndex].maxCurrent = maxCurrent;

    // Configure sensor
    // Reminder:  INA219 has registers with ONE byte addresses, but TWO byte lengths (big-endian ordering)
    i2cBuff[0] = CUR_INA219_REG_ADDR_CONFIG;
    i2cBuff[1] = CUR_INA219_CONFIG_MAX_GAIN_MAX_SAMPLING_MSB;
    i2cBuff[2] = CUR_INA219_CONFIG_MAX_GAIN_MAX_SAMPLING_LSB;
    i2cMasterWrite(hSensor, i2cBuff, 3);

    //__delay_cycles(0.25 * SEC);
    //i2cMasterRegisterRead(hSensor, CUR_INA219_REG_ADDR_CONFIG, i2cBuff, 2 );

    // Calculate calibration info based on request
    sensors[newIndex].currentLSB = (maxCurrent/(INA219_CURRENTLSB_FACTOR));
    uint16_t cfactor = floor( (INA219_CALIBRATION_SET_CONSTANT / (sensors[newIndex].currentLSB * shuntResistance)) );


    // Perform the calibration configuration
    i2cBuff[0] = CUR_INA219_REG_ADDR_CALIB;

    uint16_t cfactor2 = (cfactor << 0);
    cfactor2 &= 0b1111111111111110;   // another stupid idiosyncrasy of the INA219 registers

    i2cBuff[2] = cfactor2 & 0x00FF;
    i2cBuff[1] = cfactor2 >> 8;



    sensors[newIndex].calibFactor = cfactor2;


    i2cMasterWrite(hSensor, i2cBuff, 3);
    //__delay_cycles(0.25 * SEC);
    //i2cMasterRegisterRead(hSensor, CUR_INA219_REG_ADDR_CALIB, i2cBuff, 2 );

    // Done
    return (hDev)newIndex;
}

PCVSensorData *pcvsensorRead(hDev hSensor, pcv_read_type rtype)
{
    uint8_t devIndex = (uint8_t)hSensor;
    hDev i2cdevice = sensors[devIndex].hI2CDevice;
    uint16_t temp;

    if (rtype != Read_CurrentOnly)
    {
        i2cMasterRegisterRead(i2cdevice, CUR_INA219_REG_ADDR_SHUNT_V, i2cBuff, 2 );
        sensors[devIndex].sensordata.rawShuntVoltage = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
        sensors[devIndex].sensordata.shuntVoltageV = INA219_SHUNT_VOLTAGE_CONVERSION_FACTOR * sensors[devIndex].sensordata.rawShuntVoltage;
    }

    if (rtype == Read_BusAndShuntV || rtype == Read_All)
    {
        i2cMasterRegisterRead(i2cdevice, CUR_INA219_REG_ADDR_BUS_V, i2cBuff, 2 );
        temp = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
        sensors[devIndex].sensordata.rawBusVoltage = temp >> 3;   // value is only top 13 bits of register, gah
        sensors[devIndex].sensordata.busVoltageV = INA219_BUS_VOLTAGE_CONVERSION_FACTOR * sensors[devIndex].sensordata.rawBusVoltage;
    }

    if (rtype == Read_CurrentOnly || rtype == Read_All)
    {
        i2cMasterRegisterRead(i2cdevice, CUR_INA219_REG_ADDR_CURRENT, i2cBuff, 2 );
        sensors[devIndex].sensordata.rawCurrent = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0] << 8));
        sensors[devIndex].sensordata.calcdCurrentA = sensors[devIndex].sensordata.rawCurrent * sensors[devIndex].currentLSB;
    }

    return &(sensors[devIndex].sensordata);
}

hDev pcvsensorGetUnderlyingDevice(hDev hSensor)
{
    uint8_t devIndex = (uint8_t)hSensor;
    return sensors[devIndex].hI2CDevice;
}


