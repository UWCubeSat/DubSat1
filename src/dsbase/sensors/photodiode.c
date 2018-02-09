/*
 * photodiode.c
 *
 *  Created on: Oct 8, 2017
 *      Author: jerrolde
 */

#include <stdint.h>
#include "../core/utils.h"
#include "photodiode.h"

FILE_STATIC uint8_t read[3];
FILE_STATIC PhotodiodeData pdData[8] = { 0 };
FILE_STATIC uint8_t currIndex = 0;

uint8_t photodiodeInit(uint8_t addr, bus_instance_i2c i2cbus)
{
//Connect to the LTC2481 at addr and initialize default settings
    i2cEnable(i2cbus);
    hDev hSensor = i2cInit(i2cbus, addr);
    pdData[currIndex].addr = addr;
    pdData[currIndex].handle = hSensor;
    i2cMasterWrite(hSensor, defaultWrite, 1);

    // wait for the conversion period to end
    __delay_cycles(PHOTODIODE_DELAY_S * SEC);

    return currIndex++;
}
uint32_t photodiodeVoltage(uint8_t handle, uint8_t gain)
{
    uint32_t voltage = photodiodeRead(defaultWrite | gain, handle);
    pdData[handle].lastVoltage = voltage;
    return voltage;
}
double photodiodeVoltageDouble(uint8_t handle, uint8_t gain)
{
//Return voltage reading from the LTC2481 as a double
    double temp;
    temp = photodiodeReadDouble(defaultWrite | gain, handle);
    pdData[handle].lastVoltage = temp;
    return temp;
}

double photodiodeTemperature(uint8_t handle)
{
//Return temperature reading from the LTC2481 as a double
    double temp = photodiodeRead(temperatureWrite, handle);
    temp = (temp / 0.0014) - 273.15;
    pdData[handle].lastTemperature = temp;
    return temp;
}

// TODO use the timer library to prevent multiple reads in the space of PHOTODIODE_DELAY
uint32_t photodiodeRead(uint8_t write, uint8_t handle)
{
//select adc with handle and read mode with write condfiguration
    uint32_t x; //x contains the positive reading in the format of [16 zeroes,16-bit number]
    i2cMasterCombinedWriteRead(pdData[handle].handle, &write, 1, &read[0], 3);
    read[2] &= 0b10000000;
    read[0] &= 0b01111111;
    x = 0;
    x |= read[0];
    x = x << 8;
    x |= read[1];
    x = x << 1;
    x |= (read[2] >> 7);
    if (read[0] >> 6)
    {
        x = ~x + 1;
        x &= 0x0000FFFF;
    }
    return x;
}

double photodiodeReadDouble(uint8_t write, uint8_t handle)
{
//call photodiodeRead and convert result into a double
//binary reading * Vcc / 2^16 = double reading
    double reading;
    uint32_t x;
    x = photodiodeRead(write, handle);
    reading = (double) x;
    reading = reading * 3.3 / 65536.0;
    return reading;
}
