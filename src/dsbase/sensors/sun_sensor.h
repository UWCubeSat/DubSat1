#ifndef SUN_SENSOR_H_
#define SUN_SENSOR_H_

#define SUN_I2C_7BIT_ADDRESS 0x60

#include <stdint.h>

#include "core/i2c.h"

typedef struct SunSensorAngle {
    float alpha;
    float beta;
    uint8_t error;
} SunSensorAngle;

typedef float sun_sensor_voltage;

void sunSensorInit(bus_instance_i2c i2cbus);

/*
 * return an array of unfiltered voltage values for the four photocells, or
 * NULLP if there is a read error
 */
sun_sensor_voltage *sunSensorReadUnfiltered();

/*
 * return an array of filtered voltage values for the four photocells or NULLP
 * if there is a read error
 */
sun_sensor_voltage *sunSensorReadFiltered();

/*
 * return angle data or NULLP if there is a read error
 */
SunSensorAngle *sunSensorReadAngle();

#endif /* SUN_SENSOR_H_ */
