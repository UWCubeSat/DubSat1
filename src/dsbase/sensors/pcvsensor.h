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

typedef struct _pcvSensorData {

    uint16_t rawBusVoltage;
    uint16_t rawShuntVoltage;

    float busVoltageV;
    float shuntVoltageV;

} PCVSensorData;

#endif /* POWER_CURRENT_SENSOR_H_ */
