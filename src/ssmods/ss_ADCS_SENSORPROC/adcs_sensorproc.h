/*
 * adcs_sensorproc.h
 */

#ifndef ADCS_SENSORPROC_H_
#define ADCS_SENSORPROC_H_

// Debug LED
#define LED_DIR P1DIR
#define LED_OUT P1OUT
#define LED_BIT BIT0

#include <stdint.h>

#include "core/debugtools.h"
#include "interfaces/canwrap.h"

void handlePPTFiringNotification();
void handleRollCall();

void canRxCallback(CANPacket *packet);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

// general telemetry functions
void sendHealthSegment();
void sendMetaSegment();

#endif /* ADCS_SENSORPROC_H_ */
