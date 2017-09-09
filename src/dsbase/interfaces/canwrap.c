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


void canSetPacketType(uint8_t type, CANPacket *packet){
    packet->id |= (uint32_t) type << 20;
}

void canSetPacketDestination(uint32_t board, CANPacket *packet){
    packet->id |= board;
}

void canSetPacketParameter(uint64_t param, CANPacket *packet, uint8_t* value){
    // TODO: DBC Magic.
}

void canSendPacket(CANPacket *packet){
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

CANPacket *canConvertToPacket(uint32_t id, uint8_t* data){
    CANPacket *p;
    p -> id = id;
    uint8_t i;
    for (i = 7; i>=0; i++) p -> data[i] = data[i];
    return p;
}

uint16_t *canGetPacketParameter(uint64_t param, CANPacket *packet, uint8_t *value){
    // TODO: DBC Magic.
    uint16_t* ret;
    return ret;
}

/* BEGIN CAN PACKET AUTOGEN STUFF */

PPTMisfireCount* decodePPTMisfireCount(CANPacket *input){
    PPTMisfireCount *p;
    return p;
}

PPTTimingStatus* decodePPTTimingStatus(CANPacket *input){
    PPTTimingStatus *p;
    return p;
}

FiringStatus* decodeFiringStatus(CANPacket *input){
    FiringStatus *p;
    return p;
}

VoltageCurrentInfo* decodeVoltageCurrentInfo(CANPacket *input){
    VoltageCurrentInfo *p;
    return p;
}

BatteryStatus* decodeBatteryStatus(CANPacket *input){
    BatteryStatus *p;
    return p;
}

PowerStatus* decodePowerStatus(CANPacket *input){
    PowerStatus *p;
    return p;
}

MCUStatus* decodeMCUStatus(CANPacket *input){
    MCUStatus *p;
    return p;
}

CANPacket* encodePPTMisfireCount(PPTMisfireCount *input){
    CANPacket *p;
    p -> id = (uint64_t) 0x600000;
    p -> data[2] = (int16_t) ((input -> totalMisfires2 - 0) / 1.0);
    p -> data[0] = (int16_t) ((input -> totalMisfires1 - 0) / 1.0);
    return p;
}

CANPacket* encodePPTTimingStatus(PPTTimingStatus *input){
    CANPacket *p;
    p -> id = (uint64_t) 0x500000;
    p -> data[1] = (int8_t) ((input -> averageChargeTime - 0) / 2.0);
    p -> data[4] = (int16_t) ((input -> minutesSinceSuccessfulFire - 0) / 1.0);
    p -> data[2] = (int16_t) ((input -> minutesSinceAttemptedFire - 0) / 1.0);
    p -> data[0] = (int8_t) ((input -> medianChargeTime - 0) / 2.0);
    return p;
}

CANPacket* encodeFiringStatus(FiringStatus *input){
    CANPacket *p;
    p -> id = (uint64_t) 0x400000;
    p -> data[4] = (int16_t) ((input -> successfullFires1 - 0) / 1.0);
    p -> data[2] = (uint16_t) ((input -> successfulFires2 - 0) / 1.0);
    p -> data[1] = (int8_t) ((input -> numberOfMisfires - 0) / 1.0);
    p -> data[0] = (int8_t) ((input -> lastFiringRate - 0) / 1.0);
    return p;
}

CANPacket* encodeVoltageCurrentInfo(VoltageCurrentInfo *input){
    CANPacket *p;
    p -> id = (uint64_t) 0x300000;
    p -> data[4] = (int8_t) ((input -> adcCurrent - 0) / 25.0);
    p -> data[3] = (int8_t) ((input -> com1Current - 0) / 1.0);
    p -> data[2] = (int8_t) ((input -> com2Current - 0) / 25.0);
    p -> data[1] = (int8_t) ((input -> LineVoltage - 0) / 25.0);
    p -> data[0] = (int8_t) ((input -> rahsCurrent - 0) / 25.0);
    return p;
}

CANPacket* encodeBatteryStatus(BatteryStatus *input){
    CANPacket *p;
    p -> id = (uint64_t) 0x200000;
    p -> data[6] = (uint16_t) ((input -> batteryFullChargeCount - 0) / 1.0);
    p -> data[4] = (uint8_t) ((input -> batteryTemperature - 0) / 1.5);
    p -> data[3] = (int8_t) ((input -> batteryVoltage - 0) / 25.0);
    p -> data[2] = (int8_t) ((input -> LowestBatteryVoltage - 0) / 25.0);
    p -> data[0] = (int16_t) ((input -> underVoltageEvents - 0) / 1.0);
    return p;
}

CANPacket* encodePowerStatus(PowerStatus *input){
    CANPacket *p;
    p -> id = (uint64_t) 0x100000;
    p -> data[7] = (int8_t) ((input -> powerGeneration - 0) / 150.0);
    p -> data[6] = (int8_t) ((input -> overcurrent - 0) / 1.0);
    p -> data[5] = (int8_t) ((input -> outputPower - 0) / 150.0);
    p -> data[4] = (int8_t) ((input -> outputConfig - 0) / 1.0);
    p -> data[2] = (int8_t) ((input -> coulombCount - 0) / 15.0);
    p -> data[1] = (int8_t) ((input -> batteryFullyCharged - 0) / 1.0);
    p -> data[0] = (int8_t) ((input -> avePowerGeneration - 0) / 100.0);
    return p;
}

CANPacket* encodeMCUStatus(MCUStatus *input){
    CANPacket *p;
    p -> id = (uint64_t) 0x0;
    p -> data[4] = (int16_t) ((input -> numOfTurnons - 0) / 1.0);
    p -> data[2] = (int16_t) ((input -> minutesSinceTurnon - 0) / 1.0);
    p -> data[0] = (uint8_t) ((input -> MCUTemp - 0) / 1.0);
    return p;
}

/* END CAN PACKET AUTOGEN STUFF */

