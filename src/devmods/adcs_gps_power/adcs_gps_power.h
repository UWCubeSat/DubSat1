/*
 * adcs_sensorproc.h
 */

#ifndef ADCS_GPS_POWER_H_
#define ADCS_GPS_POWER_H_

// Debug LED
#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

#include <stdint.h>

#include "core/debugtools.h"

void handlePPTFiringNotification();
void handleRollCall();

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);

// general telemetry functions
void sendHealthSegment();
void sendMetaSegment();

#endif /* ADCS_GPS_POWER_H_ */
