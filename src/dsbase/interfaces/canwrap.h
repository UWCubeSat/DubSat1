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

//void canPacketInit(uint8_t boardNum);

//void canSetPacketType(uint8_t type, CANPacket *packet);

//void canSetPacketDestination(uint32_t board, CANPacket *packet);

//void canSetPacketParameter(uint64_t param, CANPacket *packet, uint8_t* value);

void canSendPacket(CANPacket *packet);

CANPacket *canConvertToPacket(uint32_t id, uint8_t* data);

//uint16_t *canGetPacketParameter(uint64_t param, CANPacket *packet, uint8_t *value);


// BEGIN AUTOGEN

typedef struct PPTMisfireCount {
    uint32_t totalMisfires2;
    uint32_t totalMisfires1;
} PPTMisfireCount;

typedef struct PPTTimingStatus {
    uint16_t averageChargeTime;
    uint32_t minutesSinceSuccessfulFire;
    uint32_t minutesSinceAttemptedFire;
    uint16_t medianChargeTime;
} PPTTimingStatus;

typedef struct FiringStatus {
    uint32_t successfullFires1;
    uint32_t successfulFires2;
    uint16_t numberOfMisfires;
    uint8_t lastFiringRate;
} FiringStatus;

typedef struct VoltageCurrentInfo {
    uint16_t adcCurrent;
    uint8_t com1Current;
    uint16_t com2Current;
    uint16_t LineVoltage;
    uint16_t rahsCurrent;
} VoltageCurrentInfo;

typedef struct BatteryStatus {
    uint32_t batteryFullChargeCount;
    int16_t batteryTemperature;
    uint32_t batteryVoltage;
    uint16_t LowestBatteryVoltage;
    uint32_t underVoltageEvents;
} BatteryStatus;

typedef struct PowerStatus {
    uint16_t powerGeneration;
    uint8_t overcurrent;
    uint16_t outputPower;
    uint8_t outputConfig;
    uint16_t coulombCount;
    uint8_t batteryFullyCharged;
    uint16_t avePowerGeneration;
} PowerStatus;

typedef struct MCUStatus {
    uint32_t numOfTurnons;
    uint32_t minutesSinceTurnon;
    int8_t MCUTemp;
} MCUStatus;

CANPacket *encodePPTMisfireCount(PPTMisfireCount *input);
PPTMisfireCount *decodePPTMisfireCount(CANPacket *input);

CANPacket *encodePPTTimingStatus(PPTTimingStatus *input);
PPTTimingStatus *decodePPTTimingStatus(CANPacket *input);

CANPacket *encodeFiringStatus(FiringStatus *input);
FiringStatus *decodeFiringStatus(CANPacket *input);

CANPacket *encodeVoltageCurrentInfo(VoltageCurrentInfo *input);
VoltageCurrentInfo *decodeVoltageCurrentInfo(CANPacket *input);

CANPacket *encodeBatteryStatus(BatteryStatus *input);
BatteryStatus *decodeBatteryStatus(CANPacket *input);

CANPacket *encodePowerStatus(PowerStatus *input);
PowerStatus *decodePowerStatus(CANPacket *input);

CANPacket *encodeMCUStatus(MCUStatus *input);
MCUStatus *decodeMCUStatus(CANPacket *input);


#endif /* DSBASE_INTERFACES_CANWRAP_H_ */
