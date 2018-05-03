/*
 * SUBSYSTEMNAME_MODULENAME.h
 *
 *  Created on: Jul 12, 2017
 *      Author: jeffc
 */

#ifndef ADCS_MPC_H_
#define ADCS_MPC_H_

// Debug LED
#define LED_DIR PJDIR
#define LED_OUT PJOUT
#define LED_BIT BIT1

#define TLM_ID_OUTPUT 2
#define TLM_ID_MTQCMD 3

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

TLM_SEGMENT {
    BcTlmHeader header;

    double sc_quat[4];
    double body_rates[3];
    int8_t sc_mode;
    uint8_t point_true;
    uint8_t sc_above_gs;
} output_segment;

TLM_SEGMENT {
    BcTlmHeader header;

    int8_t sc_mode;
    int8_t cmd_MT_fsw_dv[3];
} mtqcmd_segment;

// Autocode steps and timing
void triggerStep();
FILE_STATIC void rt_OneStep();
void acceptInputs();

// CAN input
void canRxCallback(CANPacket *p);

// CAN output
void sendCANVelocityPointing();
void sendCANMtqCmd();

// Backchannel telemerty
void sendHealthSegment();
void sendMetaSegment();
void sendBackchannelTelem();

void handlePPTFiringNotification();
void handleRollCall();

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

#endif /* ADCS_MPC_H_ */
