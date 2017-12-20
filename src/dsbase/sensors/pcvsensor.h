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

typedef struct _pcvSensorData {

    uint16_t rawBusVoltage;
    uint16_t rawShuntVoltage;

    float busVoltageV;
    float shuntVoltageV;

} PCVSensorData;

typedef struct _device_context_pcvsensor {
    BOOL initialized;
    bus_instance_i2c bus;
    uint8_t slaveaddr;
    hDev hI2CDevice;

    PCVSensorData sensordata;
} deviceContextPCVSensor;

hDev pcvsensorInit(bus_instance_i2c bus, uint8_t i2cAddr);
PCVSensorData *pcvsensorRead(hDev hSensor);

#endif /* POWER_CURRENT_SENSOR_H_ */
