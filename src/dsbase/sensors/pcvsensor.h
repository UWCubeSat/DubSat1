/*
 * POWER_CURRENT_SENSOR.h
 *
 *  Created on: November 21, 2017
 *      Author: Mohammad Kayali
 */

#ifndef POWER_CURRENT_SENSOR_H_
#define POWER_CURRENT_SENSOR_H_

#include "INA219_pcvsensor.h"
#include "../core/i2c.h"

#define MAX_NUM_PCVSENSORS  16
#define DEFAULT_MAX_CURRENT_AMPS    16  // If max=0 is selected

typedef struct _pcvSensorData {

    uint16_t rawBusVoltage;
    uint16_t rawShuntVoltage;
    uint16_t rawCurrent;

    float busVoltageV;
    float shuntVoltageV;
    float calcdCurrentA;

} PCVSensorData;

typedef struct _device_context_pcvsensor {
    BOOL initialized;
    bus_instance_i2c bus;
    uint8_t slaveaddr;
    hDev hI2CDevice;

    float shuntResistance;
    float maxCurrent;
    float currentLSB;
    uint16_t calibFactor;

    PCVSensorData sensordata;
} deviceContextPCVSensor;

hDev pcvsensorInit(bus_instance_i2c bus, uint8_t i2cAddr, float shuntResistance, float maxCurrent);
PCVSensorData *pcvsensorRead(hDev hSensor);

#endif /* POWER_CURRENT_SENSOR_H_ */
