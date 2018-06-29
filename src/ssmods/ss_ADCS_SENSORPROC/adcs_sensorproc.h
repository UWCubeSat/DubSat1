/*
 * adcs_sensorproc.h
 */

#ifndef ADCS_SENSORPROC_H_
#define ADCS_SENSORPROC_H_

// Debug LED
#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

#include <stdint.h>

#include "core/debugtools.h"
#include "interfaces/canwrap.h"

/* Sensor interfaces */

/**
 * Calls the init function of each sensor interface
 */
void initSensorInterfaces();

/**
 * Calls the update function of each sensor interface. This reads all sensors
 * and sets their corresponding autocode inputs. Call the individual update
 * functions instead when the sensors need to be updated at different rates
 * (i.e. most of the time).
 */
void updateSensorInterfaces();

/**
 * Sends data from the last update over the backchannel for each sensor
 * interface.
 */
void sendSensorBackchannel();

/**
 * Sends the autocode outputs from each sensor interface over CAN.
 */
void sendSensorCAN();


/* Commands */

void canRxCallback(CANPacket *packet);
uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr);


/* Backchannel */

void sendHealthSegment();
void sendMetaSegment();


/* Rollcall */
// Packets 7 and 8 are no longer used

void rcPopulate1(CANPacket *out);
void rcPopulate2(CANPacket *out);
void rcPopulate3(CANPacket *out);
void rcPopulate4(CANPacket *out);
void rcPopulate5(CANPacket *out);
void rcPopulate6(CANPacket *out);
void rcPopulate9(CANPacket *out);
void rcPopulate10(CANPacket *out);
void rcPopulate11(CANPacket *out);
void rcPopulate12(CANPacket *out);
void rcPopulate13(CANPacket *out);
void rcPopulate14(CANPacket *out);
void rcPopulate15(CANPacket *out);
void rcPopulate16(CANPacket *out);
void rcPopulate17(CANPacket *out);


/* Autocode */

/**
 * Trigger a call to step() outside of any interrupt.
 */
void triggerStep();

/**
 * Do one round of out-of-interrupt stuff. Sends backchannel, rollcall, and
 * takes one step of autocode.
 */
void step();

#endif /* ADCS_SENSORPROC_H_ */
