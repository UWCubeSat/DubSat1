/*
 * analogsensor.h
 *
 *  Created on: Nov 8, 2017
 *      Author: SatLab
 */

#ifndef DSBASE_SENSORS_ANALOGSENSOR_H_
#define DSBASE_SENSORS_ANALOGSENSOR_H_

#include <stdint.h>

#include "core/utils.h"
#include "config/config_sensor_analog.h"

typedef struct _analog_sensor_context {
    uint16_t something;
    uint16_t somethingelse;
} analog_sensor_context;


hDev analogInit();
uint16_t analogRawRead(hDev handle);

#endif /* DSBASE_SENSORS_ANALOGSENSOR_H_ */
