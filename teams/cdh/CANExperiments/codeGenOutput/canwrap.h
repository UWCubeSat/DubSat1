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


// BEGIN GENERATOR MACROS

#define CAN_ID_MESSAGE5VALUETABLESIGS 4
#define CAN_ID_MESSAGE4ODDSIZES 3
#define CAN_ID_MESSAGE3 2
#define CAN_ID_MESSAGE2SMALLER 1
#define CAN_ID_MESSAGE1 0

#define CAN_ENUM_VALUETABLESIGNAL1_INVALIDGARBAGE 2
#define CAN_ENUM_VALUETABLESIGNAL1_FINISHED 1
#define CAN_ENUM_VALUETABLESIGNAL1_STARTED 0

#define CAN_ENUM_SIMPLEVALUETABLE_INVALIDGARBAGE 2
#define CAN_ENUM_SIMPLEVALUETABLE_FINISHED 1
#define CAN_ENUM_SIMPLEVALUETABLE_STARTED 0

typedef struct CANPacket {
   uint32_t id; // Actual physical ID of the packet
   uint8_t data[8]; // Data
   uint8_t bufferNum; // Only applicable for Rx, which buffer it landed in
   uint8_t length; // Only applies to sending packets. We don't know how long incoming packets are.
} CANPacket;

void canWrapInit();

// Global function pointer to point to the function
// when a packet is received through CAN
void (*CANPacketReceived)(CANPacket *);

void canSendPacket(CANPacket *packet);

void setCANPacketRxCallback(void (*ReceiveCallbackArg)(CANPacket *packet));
typedef struct Message5ValueTableSigs {
    uint8_t ValueTableSignal1; //  (No Units)
} Message5ValueTableSigs;

typedef struct Message4OddSizes {
    uint64_t OddSize3; //  (No Units)
    int16_t OddSize2; //  (No Units)
    uint8_t OddSize1; //  (No Units)
} Message4OddSizes;

typedef struct Message3 {
    double NormalDouble; //  (No Units)
} Message3;

typedef struct Message2Smaller {
    float NormalFloat; // 2.752
} Message2Smaller;

typedef struct Message1 {
    int8_t NormalSignedInt; //  (No Units)
    int32_t IntFactorOffset; //  (No Units)
    float FloatFactor; // FFGFGF!!!
    uint8_t NormalUint; // m/s
} Message1;

void encodeMessage5ValueTableSigs(Message5ValueTableSigs *input, CANPacket* output);
void decodeMessage5ValueTableSigs(CANPacket *input, Message5ValueTableSigs *output);

void encodeMessage4OddSizes(Message4OddSizes *input, CANPacket* output);
void decodeMessage4OddSizes(CANPacket *input, Message4OddSizes *output);

void encodeMessage3(Message3 *input, CANPacket* output);
void decodeMessage3(CANPacket *input, Message3 *output);

void encodeMessage2Smaller(Message2Smaller *input, CANPacket* output);
void decodeMessage2Smaller(CANPacket *input, Message2Smaller *output);

void encodeMessage1(Message1 *input, CANPacket* output);
void decodeMessage1(CANPacket *input, Message1 *output);

#endif /* DSBASE_INTERFACES_CANWRAP_H_ */
