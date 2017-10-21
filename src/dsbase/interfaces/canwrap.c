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

struct CANPacket *canConvertToPacket(uint32_t id, uint8_t* data){
    struct CANPacket *p;
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

// AUTOGEN STUFF HERE

PPTMisfireCount *decodePPTMisfireCount(CANPacket *input){
    uint64_t fullData = (uint64_t) (input -> data);
    PPTMisfireCount *output;
    output -> totalMisfires2 = (uint32_t) ((fullData & 0b1111111111111111 << 0) >> 0);
    output -> totalMisfires1 = (uint32_t) ((fullData & 0b1111111111111111 << 16) >> 16);
    return output;
}

PPTTimingStatus *decodePPTTimingStatus(CANPacket *input){
    uint64_t fullData = (uint64_t) (input -> data);
    PPTTimingStatus *output;
    output -> averageChargeTime = (uint16_t) ((fullData & 0b11111111 << 32) >> 32);
    output -> minutesSinceSuccessfulFire = (uint32_t) ((fullData & 0b1111111111111111 << 0) >> 0);
    output -> minutesSinceAttemptedFire = (uint32_t) ((fullData & 0b1111111111111111 << 16) >> 16);
    output -> medianChargeTime = (uint16_t) ((fullData & 0b11111111 << 40) >> 40);
    return output;
}

FiringStatus *decodeFiringStatus(CANPacket *input){
    uint64_t fullData = (uint64_t) (input -> data);
    FiringStatus *output;
    output -> successfullFires1 = (uint32_t) ((fullData & 0b1111111111111111 << 0) >> 0);
    output -> successfulFires2 = (uint32_t) ((fullData & 0b1111111111111111 << 16) >> 16);
    output -> numberOfMisfires = (uint16_t) ((fullData & 0b11111111 << 32) >> 32);
    output -> lastFiringRate = (uint8_t) ((fullData & 0b11111111 << 40) >> 40);
    return output;
}

VoltageCurrentInfo *decodeVoltageCurrentInfo(CANPacket *input){
    uint64_t fullData = (uint64_t) (input -> data);
    VoltageCurrentInfo *output;
    output -> adcCurrent = (uint16_t) ((fullData & 0b11111111 << 0) >> 0);
    output -> com1Current = (uint8_t) ((fullData & 0b11111111 << 8) >> 8);
    output -> com2Current = (uint16_t) ((fullData & 0b11111111 << 16) >> 16);
    output -> LineVoltage = (uint16_t) ((fullData & 0b11111111 << 24) >> 24);
    output -> rahsCurrent = (uint16_t) ((fullData & 0b11111111 << 32) >> 32);
    return output;
}

BatteryStatus *decodeBatteryStatus(CANPacket *input){
    uint64_t fullData = (uint64_t) (input -> data);
    BatteryStatus *output;
    output -> batteryFullChargeCount = (uint32_t) ((fullData & 0b1111111111111111 << 0) >> 0);
    output -> batteryTemperature = (int16_t) ((fullData & 0b11111111 << 24) >> 24);
    output -> batteryVoltage = (uint32_t) ((fullData & 0b11111111 << 32) >> 32);
    output -> LowestBatteryVoltage = (uint16_t) ((fullData & 0b11111111 << 40) >> 40);
    output -> underVoltageEvents = (uint32_t) ((fullData & 0b1111111111111111 << 48) >> 48);
    return output;
}

PowerStatus *decodePowerStatus(CANPacket *input){
    uint64_t fullData = (uint64_t) (input -> data);
    PowerStatus *output;
    output -> powerGeneration = (uint16_t) ((fullData & 0b11111111 << 0) >> 0);
    output -> overcurrent = (uint8_t) ((fullData & 0b11111111 << 8) >> 8);
    output -> outputPower = (uint16_t) ((fullData & 0b11111111 << 16) >> 16);
    output -> outputConfig = (uint8_t) ((fullData & 0b11111111 << 24) >> 24);
    output -> coulombCount = (uint16_t) ((fullData & 0b11111111 << 40) >> 40);
    output -> batteryFullyCharged = (uint8_t) ((fullData & 0b11111111 << 48) >> 48);
    output -> avePowerGeneration = (uint16_t) ((fullData & 0b11111111 << 56) >> 56);
    return output;
}

MCUStatus *decodeMCUStatus(CANPacket *input){
    uint64_t fullData = (uint64_t) (input -> data);
    MCUStatus *output;
    output -> numOfTurnons = (uint32_t) ((fullData & 0b1111111111111111 << 0) >> 0);
    output -> minutesSinceTurnon = (uint32_t) ((fullData & 0b1111111111111111 << 16) >> 16);
    output -> MCUTemp = (int8_t) ((fullData & 0b11111111 << 40) >> 40);
    return output;
}


