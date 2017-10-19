/*
 * canwrap.h
 *
 *  Created on: Aug 14, 2017
 *      Author: emoryeng
 */

#include <inttypes.h>

#ifndef DSBASE_INTERFACES_CANWRAP_H_
#define DSBASE_INTERFACES_CANWRAP_H_

#define PARAM_ADCS_STATUS_VELOCITY_RPM 0x4201;

typedef struct CANPacket {
   uint32_t id;
   uint8_t data[8];
} CANPacket;

void canPacketInit(uint8_t boardNum);

void canSetPacketType(uint8_t type, CANPacket *packet);

void canSetPacketDestination(uint32_t board, CANPacket *packet);

void canSetPacketParameter(uint64_t param, CANPacket *packet, uint8_t* value);

void canSendPacket(CANPacket *packet);

CANPacket *canConvertToPacket(uint32_t id, uint8_t* data);

uint16_t *canGetPacketParameter(uint64_t param, CANPacket *packet, uint8_t *value);

#endif /* DSBASE_INTERFACES_CANWRAP_H_ */
