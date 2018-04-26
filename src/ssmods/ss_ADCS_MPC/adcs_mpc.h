/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef ADCS_MPC_H_
#define ADCS_MPC_H_

// Debug LED
#define LED_DIR P1DIR
#define LED_OUT P1OUT
#define LED_BIT BIT0

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"

// A struct for storing various interesting info about the subsystem module
typedef struct _module_status {
    StartupType startup_type;
} ModuleStatus;

void handlePPTFiringNotification();
void handleRollCall();

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

#endif /* ADCS_MPC_H_ */
