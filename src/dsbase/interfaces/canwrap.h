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
    uint32_t totalMisfires2; //  (No Units)
    uint32_t totalMisfires1; //  (No Units)
} PPTMisfireCount;

typedef struct PPTTimingStatus {
    uint16_t averageChargeTime; //  (No Units)
    uint32_t minutesSinceSuccessfulFire; // min
    uint32_t minutesSinceAttemptedFire; // min
    uint16_t medianChargeTime; // ms
} PPTTimingStatus;

typedef struct FiringStatus {
    uint32_t successfullFires1; //  (No Units)
    uint32_t successfulFires2; //  (No Units)
    uint16_t numberOfMisfires; //  (No Units)
    uint8_t lastFiringRate; //  (No Units)
} FiringStatus;

typedef struct VoltageCurrentInfo {
    uint16_t adcCurrent; // mA
    uint8_t com1Current; // mA
    uint16_t com2Current; // mA
    uint16_t LineVoltage; // mV
    uint16_t rahsCurrent; // mA
} VoltageCurrentInfo;

typedef struct BatteryStatus {
    uint32_t batteryFullChargeCount; //  (No Units)
    int16_t batteryTemperature; // C
    uint32_t batteryVoltage; // mV
    uint16_t LowestBatteryVoltage; // mV
    uint32_t underVoltageEvents; //  (No Units)
} BatteryStatus;

typedef struct PowerStatus {
    uint16_t powerGeneration; // mW
    uint8_t overcurrent; // bools
    uint16_t outputPower; // mW
    uint8_t outputConfig; // bools
    uint16_t coulombCount; // mA*hr
    uint8_t batteryFullyCharged; // Boolean
    uint16_t avePowerGeneration; // mW
} PowerStatus;

typedef struct MCUStatus {
    uint32_t numOfTurnons; //  (No Units)
    uint32_t minutesSinceTurnon; // min/16
    int8_t MCUTemp; // C
} MCUStatus;

CANPacket *encodePPTMisfireCount(PPTMisfireCount *input);
PPTMisfireCount *decodePPTMisfireCount(CANPacket *input);

CANPacket *encodePPTTimingStatus(PPTTimingStatus *input);
PPTTimingStatus *decodePPTTimingStatus(CANPacket *input);

CANPacket *encodeFiringStatus(FiringStatus *input);
FiringStatus *decodeFiringStatus(CANPacket *input);

CANPacket *encodeVoltageCurrentInfo(VoltageCurrentInfo *input);
VoltageCurrentInfo *decodeVoltageCurrentInfo(CANPacket *input);

void encodeBatteryStatus(BatteryStatus *input, CANPacket *output);
void decodeBatteryStatus(CANPacket *input, BatteryStatus *output);

CANPacket *encodePowerStatus(PowerStatus *input);
PowerStatus *decodePowerStatus(CANPacket *input);

CANPacket *encodeMCUStatus(MCUStatus *input);
MCUStatus *decodeMCUStatus(CANPacket *input);


#endif /* DSBASE_INTERFACES_CANWRAP_H_ */
