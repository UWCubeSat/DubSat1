 /*
 * POWER_LTC2943_COULOMBCOUNTER.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Sean Poulter
 */
#include <msp430.h>

#include "bsp/bsp.h"
#include "core/utils.h"
#include <stdint.h>
#include "core/i2c.h"

#ifndef POWER_LTC2943_COULOMBCOUNTER_H_
#define POWER_LTC2943_COULOMBCOUNTER_H_

//ADDRESS
#define LTC2943_ADDRESS 0X64

//Register addresses
#define LTC2943_ADDR_STATUS 0x00
#define LTC2943_ADDR_CONTROL 0x01
#define LTC2943_ADDR_ACCUMULATED_CHARGE_MSB 0x02
#define LTC2943_ADDR_VOLTAGE_MSB 0x08
#define LTC2943_ADDR_CURRENT_MSB 0x0E

//constant settings
#define LTC2943_PRESCALEFACTOR 64

//Preset shortcuts
#define ALERTDISABLED_PRESCALER256_ADCAUTOMATICMODE 0xE0

#define MAX_BUFF_SIZE 0x10

typedef struct _coulombCounterData {

    int16_t rawBusVoltage;
    int16_t rawShuntVoltage;
    int16_t rawCurrent;
    int16_t rawAccumCharge;

    float busVoltageV;
    float shuntVoltageV;
    float calcdCurrentA;
    float SOC;

} CoulombCounterData;

typedef struct _device_context_coulombCounter {
    BOOL initialized;
    bus_instance_i2c bus;
    uint8_t slaveaddr;
    hDev hI2CDevice;
    float shuntResistance; //milli ohms
    float chargeLSB; //mAh
    uint16_t accumChargeFull;
    uint16_t accumChargeEmpty;

    CoulombCounterData sensorData;
} deviceContextCoulombCounter;

void LTC2943Init(bus_instance_i2c bus, float rShunt);
CoulombCounterData readCoulombCounter();
float rawBusVoltageToFloat(int16_t raw);
float rawCurrentToFloat(int16_t raw);

#endif /* POWER_LTC2943_COULOMBCOUNTER_H_ */
