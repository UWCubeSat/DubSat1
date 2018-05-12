/*
 * rollcall.h
 *
 *  Created on: May 9, 2018
 *      Author: djdup
 *
 * This manages the bulk sending of rollcall packets. It ensures that the
 * packets go through even if the CAN bus is busy when the rollcall command is
 * received.
 */

#ifndef ROLLCALL_H_
#define ROLLCALL_H_

#include <stdint.h>

#include "interfaces/canwrap.h"

typedef void (* rollcall_fn)(CANPacket *out);

/**
 * Initialize rollcall with an array of functions. Each function populates a
 * CAN packet to send (but does not send it).
 */
void rollcallInit(const rollcall_fn *functions, uint8_t numFunctions);

/**
 * Trigger a round of rollcall. Should be called when a rollcall command packet
 * is received.
 */
void rollcallStart();

/**
 * Populate and send the next rollcall packets if there are any left to send
 * since the last start.
 */
void rollcallUpdate();

#endif /* ROLLCALL_H_ */