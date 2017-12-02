#include <stdint.h>
#include <stdlib.h>
#include <printf.h>
#include "CanWrap.h"
#include "inttypes.h"
void reverseArray(uint8_t arr[], int start, int end)
{
    uint8_t temp;
    if (start >= end)
        return;
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    reverseArray(arr, start+1, end-1);
}
//
// Created by Emory Eng on 10/21/17.
//
PPTMisfireCount *decodePPTMisfireCount(CANPacket *input){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    PPTMisfireCount *output = malloc(sizeof(PPTMisfireCount));
    output -> totalMisfires2 = (uint32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 0)) >> 0) * 1.0 + 0.0);
    output -> totalMisfires1 = (uint32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 16)) >> 16) * 1.0 + 0.0);
    return output;
}

CANPacket *encodePPTMisfireCount(PPTMisfireCount *input){
    CANPacket *output = malloc(sizeof(CANPacket));
    output -> id = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> totalMisfires2 - 0.0) / 1.0)) << 32;
    fullPacketData |= ((uint64_t)((input -> totalMisfires1 - 0.0) / 1.0)) << 48;
    uint64_t *thePointer = (uint64_t *) output -> data;
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
    return output;
}

PPTTimingStatus *decodePPTTimingStatus(CANPacket *input){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    PPTTimingStatus *output = malloc(sizeof(PPTTimingStatus));
    output -> averageChargeTime = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 32)) >> 32) * 2.0 + 0.0);
    output -> minutesSinceSuccessfulFire = (uint32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 0)) >> 0) * 1.0 + 0.0);
    output -> minutesSinceAttemptedFire = (uint32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 16)) >> 16) * 1.0 + 0.0);
    output -> medianChargeTime = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 40)) >> 40) * 2.0 + 0.0);
    return output;
}

CANPacket *encodePPTTimingStatus(PPTTimingStatus *input){
    CANPacket *output = malloc(sizeof(CANPacket));
    output -> id = 5;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> averageChargeTime - 0.0) / 2.0)) << 48;
    fullPacketData |= ((uint64_t)((input -> minutesSinceSuccessfulFire - 0.0) / 1.0)) << 16;
    fullPacketData |= ((uint64_t)((input -> minutesSinceAttemptedFire - 0.0) / 1.0)) << 32;
    fullPacketData |= ((uint64_t)((input -> medianChargeTime - 0.0) / 2.0)) << 56;
    uint64_t *thePointer = (uint64_t *) output -> data;
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
    return output;
}

FiringStatus *decodeFiringStatus(CANPacket *input){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    FiringStatus *output = malloc(sizeof(FiringStatus));
    output -> successfullFires1 = (uint32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 0)) >> 0) * 1.0 + 0.0);
    output -> successfulFires2 = (uint32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 16)) >> 16) * 1.0 + 0.0);
    output -> numberOfMisfires = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 32)) >> 32) * 1.0 + 0.0);
    output -> lastFiringRate = (uint8_t) (((fullData & ((uint64_t) 0b11111111 << 40)) >> 40) * 1.0 + 0.0);
    return output;
}

CANPacket *encodeFiringStatus(FiringStatus *input){
    CANPacket *output = malloc(sizeof(CANPacket));
    output -> id = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> successfullFires1 - 0.0) / 1.0)) << 16;
    fullPacketData |= ((uint64_t)((input -> successfulFires2 - 0.0) / 1.0)) << 32;
    fullPacketData |= ((uint64_t)((input -> numberOfMisfires - 0.0) / 1.0)) << 48;
    fullPacketData |= ((uint64_t)((input -> lastFiringRate - 0.0) / 1.0)) << 56;
    uint64_t *thePointer = (uint64_t *) output -> data;
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
    return output;
}

VoltageCurrentInfo *decodeVoltageCurrentInfo(CANPacket *input){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    VoltageCurrentInfo *output = malloc(sizeof(VoltageCurrentInfo));
    output -> adcCurrent = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 0)) >> 0) * 25.0 + 0.0);
    output -> com1Current = (uint8_t) (((fullData & ((uint64_t) 0b11111111 << 8)) >> 8) * 1.0 + 0.0);
    output -> com2Current = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 16)) >> 16) * 25.0 + 0.0);
    output -> LineVoltage = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 24)) >> 24) * 25.0 + 0.0);
    output -> rahsCurrent = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 32)) >> 32) * 25.0 + 0.0);
    return output;
}

CANPacket *encodeVoltageCurrentInfo(VoltageCurrentInfo *input){
    CANPacket *output = malloc(sizeof(CANPacket));
    output -> id = 3;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> adcCurrent - 0.0) / 25.0)) << 24;
    fullPacketData |= ((uint64_t)((input -> com1Current - 0.0) / 1.0)) << 32;
    fullPacketData |= ((uint64_t)((input -> com2Current - 0.0) / 25.0)) << 40;
    fullPacketData |= ((uint64_t)((input -> LineVoltage - 0.0) / 25.0)) << 48;
    fullPacketData |= ((uint64_t)((input -> rahsCurrent - 0.0) / 25.0)) << 56;
    uint64_t *thePointer = (uint64_t *) output -> data;
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
    return output;
}

BatteryStatus *decodeBatteryStatus(CANPacket *input){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    BatteryStatus *output = malloc(sizeof(BatteryStatus));
    output -> batteryFullChargeCount = (uint32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 0)) >> 0) * 1.0 + 0.0);
    output -> batteryTemperature = (int16_t) (((fullData & ((uint64_t) 0b11111111 << 24)) >> 24) * 1.5 + 0.0);
    output -> batteryVoltage = (uint32_t) (((fullData & ((uint64_t) 0b11111111 << 32)) >> 32) * 25.0 + 0.0);
    output -> LowestBatteryVoltage = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 40)) >> 40) * 25.0 + 0.0);
    output -> underVoltageEvents = (uint32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 48)) >> 48) * 1.0 + 0.0);
    return output;
}

CANPacket *encodeBatteryStatus(BatteryStatus *input){
    CANPacket *output = malloc(sizeof(CANPacket));
    output -> id = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> batteryFullChargeCount - 0.0) / 1.0)) << 0;
    fullPacketData |= ((uint64_t)((input -> batteryTemperature - 0.0) / 1.5)) << 24;
    fullPacketData |= ((uint64_t)((input -> batteryVoltage - 0.0) / 25.0)) << 32;
    fullPacketData |= ((uint64_t)((input -> LowestBatteryVoltage - 0.0) / 25.0)) << 40;
    fullPacketData |= ((uint64_t)((input -> underVoltageEvents - 0.0) / 1.0)) << 48;
    uint64_t *thePointer = (uint64_t *) output -> data;
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
    return output;
}

PowerStatus *decodePowerStatus(CANPacket *input){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    PowerStatus *output = malloc(sizeof(PowerStatus));
    output -> powerGeneration = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 0)) >> 0) * 150.0 + 0.0);
    output -> overcurrent = (uint8_t) (((fullData & ((uint64_t) 0b11111111 << 8)) >> 8) * 1.0 + 0.0);
    output -> outputPower = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 16)) >> 16) * 150.0 + 0.0);
    output -> outputConfig = (uint8_t) (((fullData & ((uint64_t) 0b11111111 << 24)) >> 24) * 1.0 + 0.0);
    output -> coulombCount = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 40)) >> 40) * 15.0 + 0.0);
    output -> batteryFullyCharged = (uint8_t) (((fullData & ((uint64_t) 0b11111111 << 48)) >> 48) * 1.0 + 0.0);
    output -> avePowerGeneration = (uint16_t) (((fullData & ((uint64_t) 0b11111111 << 56)) >> 56) * 100.0 + 0.0);
    return output;
}

CANPacket *encodePowerStatus(PowerStatus *input){
    CANPacket *output = malloc(sizeof(CANPacket));
    output -> id = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> powerGeneration - 0.0) / 150.0)) << 0;
    fullPacketData |= ((uint64_t)((input -> overcurrent - 0.0) / 1.0)) << 8;
    fullPacketData |= ((uint64_t)((input -> outputPower - 0.0) / 150.0)) << 16;
    fullPacketData |= ((uint64_t)((input -> outputConfig - 0.0) / 1.0)) << 24;
    fullPacketData |= ((uint64_t)((input -> coulombCount - 0.0) / 15.0)) << 40;
    fullPacketData |= ((uint64_t)((input -> batteryFullyCharged - 0.0) / 1.0)) << 48;
    fullPacketData |= ((uint64_t)((input -> avePowerGeneration - 0.0) / 100.0)) << 56;
    uint64_t *thePointer = (uint64_t *) output -> data;
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
    return output;
}

MCUStatus *decodeMCUStatus(CANPacket *input){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    MCUStatus *output = malloc(sizeof(MCUStatus));
    output -> numOfTurnons = (uint32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 0)) >> 0) * 1.0 + 0.0);
    output -> minutesSinceTurnon = (uint32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 16)) >> 16) * 1.0 + 0.0);
    output -> MCUTemp = (int8_t) (((fullData & ((uint64_t) 0b11111111 << 40)) >> 40) * 1.0 + 0.0);
    return output;
}

CANPacket *encodeMCUStatus(MCUStatus *input){
    CANPacket *output = malloc(sizeof(CANPacket));
    output -> id = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> numOfTurnons - 0.0) / 1.0)) << 16;
    fullPacketData |= ((uint64_t)((input -> minutesSinceTurnon - 0.0) / 1.0)) << 32;
    fullPacketData |= ((uint64_t)((input -> MCUTemp - 0.0) / 1.0)) << 56;
    uint64_t *thePointer = (uint64_t *) output -> data;
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
    return output;
}

