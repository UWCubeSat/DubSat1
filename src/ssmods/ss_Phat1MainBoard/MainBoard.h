/*
 * Phat1Board.h
 *
 *  Created on: May 1, 2019
 *      Author: Kyle S
 */

#ifndef PHAT1BOARD_H_
#define PHAT1BOARD_H_

#include <stdint.h>

#include "bsp/bsp.h"
#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"
#include "sensors/altimeter.h"
#include "gps_io.h"
#include "adcs_sensorproc_ids.h"
#include "core/timer.h"

//Structs go here
typedef struct rx_data_holder{
    uint8_t data_set[16];
    uint16_t length;

} rxDataHolder;

typedef struct PACKED_STRUCT
{
    AltimeterData altimeter;
    GPSDegMin gps;
} UartTXData;


// A struct for storing various interesting info about the subsystem module
typedef struct _module_status {
    StartupType startup_type;

    uint16_t state_transition_errors;
    uint16_t in_unknown_state;
} ModuleStatus;

//method headers go here
void initAltimeter();
void readAltimeterData();
void i2cLED();
void uartLED();
void initLEDs();
void rxCallBack(uint8_t data);
void initMagnetometer();

uint8_t handleDebugInfoCallback(DebugMode mode);
uint8_t handleDebugStatusCallback(DebugMode mode);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

#endif /* PHAT1BOARD_H_ */
