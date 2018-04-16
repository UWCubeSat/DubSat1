/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef ADCS_ESTIM_H_
#define ADCS_ESTIM_H_

// Debug LED
#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

#include <stdint.h>

#include "core/utils.h"
#include "core/timers.h"
#include "interfaces/systeminfo.h"
#include "core/debugtools.h"
#include "interfaces/canwrap.h"

// A struct for storing various interesting info about the subsystem module
typedef struct _module_status {
    StartupType startup_type;
} ModuleStatus;

void handlePPTFiringNotification();
void handleRollCall();

void canRxCallback(CANPacket *packet);

#endif /* ADCS_ESTIM_H_ */
