 /*
 * POWER_LTC2943_COULOMBCOUNTER.h
 *
 *  Created on: Dec 24, 2017
 *      Author: Sean Poulter
 */

#ifndef COULOMB_COUNTER_H_
#define COULOMB_COUNTER_H_

#include <msp430.h>

//#include "bsp/bsp.h"
//#include "core/utils.h"
#include <stdint.h>
#include "core/i2c.h"

//ADDRESS
#define LTC2943_ADDRESS 0X64

//Register addresses
#define LTC2943_ADDR_STATUS 0x00
#define LTC2943_ADDR_CONTROL 0x01
#define LTC2943_ADDR_ACCUMULATED_CHARGE_MSB 0x02
#define LTC2943_ADDR_VOLTAGE_MSB 0x08
#define LTC2943_ADDR_CURRENT_MSB 0x0E

//constant settings
#define LTC2943_PRESCALEFACTOR 64.0

//Preset shortcuts
#define ALERTDISABLED_PRESCALER256_ADCAUTOMATICMODE 0xD8

#define MAX_BUFF_SIZE 10

typedef struct _coulombCounterData {

    uint16_t rawBusVoltage;
    uint16_t rawShuntVoltage;
    uint16_t rawCurrent;
    uint16_t rawAccumCharge;

    float busVoltageV;
    float shuntVoltageV;
    float calcdCurrentA;
    float SOC;
    float totalAccumulatedCharge;
    float battCharge;

    uint8_t controlReg;
    uint8_t statusReg;

    BOOL fullCharge;

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

typedef enum {
    Automatic_Mode = 0x03,
    Scan_Mode = 0x02,
    Manual_Mode = 0x01,
    Sleep = 0x00,
} CC_Control_ADCmode;

typedef enum {
    M_1 = 0x00,
    M_4 = 0x01,
    M_16 = 0x02,
    M_64 = 0x03,
    M_256 = 0x04,
    M_1024 = 0x05,
    M_4096 = 0x07,
} CC_Control_PrescaleFactor;

typedef enum {
    Alert_Mode = 0x02,
    Charge_Complete_Mode = 0x01,
    ALCC_Disabled = 0x00,
} CC_Control_ALCCConfiguration;

typedef enum {
    Shutdown = 0x01,
    Active = 0x00,
} CC_Control_Shutdown;

void LTC2943Init(bus_instance_i2c bus, float rShunt);
CoulombCounterData readCoulombCounter();
void readCoulombCounterControl();
void readCoulombCounterStatus();
float rawBusVoltageToFloat(int16_t raw);
float rawCurrentToFloat(int16_t raw);

#endif /* POWER_LTC2943_COULOMBCOUNTER_H_ */
