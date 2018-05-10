/*
 * sensorInterface.h
 *
 * SensorInterface is a class-like struct for managing multiple sensors on one
 * microcontroller. Each interface should implement an init function and an
 * update function.
 *
 *  Created on: May 9, 2018
 *      Author: djdup
 */

#ifndef SENSORINTERFACE_H_
#define SENSORINTERFACE_H_

typedef void (* sensorio_init_fn)(void);
typedef void (* sensorio_update_fn)(void);
typedef void (* sensorio_sendBc_fn)(void);
typedef void (* sensorio_sendCan_fn)(void);

typedef struct {
    // initialize the sensor
    sensorio_init_fn init;

    // read from the sensor and set autocode inputs
    sensorio_update_fn update;

    // send raw sensor telemetry over the backchannel (uart)
    sensorio_sendBc_fn sendBackchannel;

    // send autocode outputs over CAN
    sensorio_sendCan_fn sendCan;
} SensorInterface;

#endif /* SENSORINTERFACE_H_ */
