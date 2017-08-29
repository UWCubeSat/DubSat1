/*
 * canwrap.c
 *
 *  Created on: Aug 14, 2017
 *      Author: emoryeng
 */

#include "canwrap.h"
#include "../core/can.h"

void canPacketInit(uint8_t boardNum){
    canInit();
    setTheFilter(CAN_MASK_0, 0x01 << boardNum);
    setTheFilter(CAN_FILTER_0, 0x01 << boardNum);
    setTheFilter(CAN_FILTER_1, 0x01 << boardNum);
    setTheFilter(CAN_MASK_1, 0x01 << boardNum);
    setTheFilter(CAN_FILTER_2, 0x01 << boardNum);
    setTheFilter(CAN_FILTER_3, 0x01 << boardNum);
    setTheFilter(CAN_FILTER_4, 0x01 << boardNum);
    setTheFilter(CAN_FILTER_5, 0x01 << boardNum);
}


void canSetPacketType(uint8_t type, struct CANPacket *packet){
    packet->id |= (uint32_t) type << 20;
}

void canSetPacketDestination(uint32_t board, struct CANPacket *packet){
    packet->id |= board;
}

void canSetPacketParameter(uint64_t param, struct CANPacket *packet, uint8_t* value){
    // TODO: DBC Magic.
}

void canSendPacket(struct CANPacket *packet){
    const uint8_t length = 0x08;
    uint8_t tech[5] = {
       (uint8_t) (packet->id >> 21),
       (uint8_t) (packet->id >> 16) & 0x03 | (uint8_t) (packet->id >> 13) & 0xE0 | 0x08,
       (uint8_t) (packet->id >> 8),
       (uint8_t) packet->id,
       length
    };
    canSend(0,tech, packet->data);
}

struct CANPacket *canConvertToPacket(uint32_t id, uint8_t* data){
    struct CANPacket *p;
    p -> id = id;
    uint8_t i;
    for (i = 7; i>=0; i++) p -> data[i] = data[i];
    return p;
}

uint16_t *canGetPacketParameter(uint64_t param, struct CANPacket *packet, uint8_t *value){
    // TODO: DBC Magic.
    uint16_t* ret;
    return ret;
}



