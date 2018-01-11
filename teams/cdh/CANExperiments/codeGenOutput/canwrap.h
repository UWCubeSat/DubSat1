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

#define CAN_ID_VECTOR__INDEPENDENT_SIG_MSG 1073741824
#define CAN_ID_ADCS_MTQ_BDOT_CMD 514
#define CAN_ID_ADCS_MTQ_STATEST_CMD 513
#define CAN_ID_ADCS_RW_CMD 512

#define CAN_ENUM_MTQ_COMMAND_Z_MTQ_CMD_ASSERTCONTROL 1
#define CAN_ENUM_MTQ_COMMAND_Z_MTQ_CMD_CEDECONTROL 0
#define CAN_ENUM_MTQ_COMMAND_Y_MTQ_CMD_ASSERTCONTROL 1
#define CAN_ENUM_MTQ_COMMAND_Y_MTQ_CMD_CEDECONTROL 0
#define CAN_ENUM_MTQ_COMMAND_X_MTQ_CMD_ASSERTCONTROL 1
#define CAN_ENUM_MTQ_COMMAND_X_MTQ_CMD_CEDECONTROL 0
#define CAN_ENUM_MTQ_COMMAND_Z_MTQ_CMD_ASSERTCONTROL 1
#define CAN_ENUM_MTQ_COMMAND_Z_MTQ_CMD_CEDECONTROL 0
#define CAN_ENUM_MTQ_COMMAND_Y_MTQ_CMD_ASSERTCONTROL 1
#define CAN_ENUM_MTQ_COMMAND_Y_MTQ_CMD_CEDECONTROL 0
#define CAN_ENUM_MTQ_COMMAND_X_MTQ_CMD_ASSERTCONTROL 1
#define CAN_ENUM_MTQ_COMMAND_X_MTQ_CMD_CEDECONTROL 0
#define CAN_ENUM_RW_COMMAND_Z_RW_CMD_OPENLOOP 2
#define CAN_ENUM_RW_COMMAND_Z_RW_CMD_PID 1
#define CAN_ENUM_RW_COMMAND_Z_RW_CMD_STOP 0
#define CAN_ENUM_RW_COMMAND_Y_RW_CMD_OPENLOOP 2
#define CAN_ENUM_RW_COMMAND_Y_RW_CMD_PID 1
#define CAN_ENUM_RW_COMMAND_Y_RW_CMD_STOP 0
#define CAN_ENUM_RW_COMMAND_X_RW_CMD_OPENLOOP 2
#define CAN_ENUM_RW_COMMAND_X_RW_CMD_PID 1
#define CAN_ENUM_RW_COMMAND_X_RW_CMD_STOP 0

#define CAN_ENUM_MTQCOMMAND_MTQ_CMD_ASSERTCONTROL 1
#define CAN_ENUM_MTQCOMMAND_MTQ_CMD_CEDECONTROL 0
#define CAN_ENUM_REACTIONWHEELCOMMAND_RW_CMD_OPENLOOP 2
#define CAN_ENUM_REACTIONWHEELCOMMAND_RW_CMD_PID 1
#define CAN_ENUM_REACTIONWHEELCOMMAND_RW_CMD_STOP 0

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
typedef struct VECTOR__INDEPENDENT_SIG_MSG {
    uint8_t CDH_GroupingID; //  (No Units)
} VECTOR__INDEPENDENT_SIG_MSG;

typedef struct ADCS_MTQ_BDOT_CMD {
    uint8_t MTQ_Command_Z; //  (No Units)
    uint8_t MTQ_Command_Y; //  (No Units)
    uint8_t MTQ_Command_X; //  (No Units)
    int16_t MTQ_Strength_Z; //  (No Units)
    int16_t MTQ_Strength_Y; //  (No Units)
    int16_t MTQ_Strength_X; //  (No Units)
} ADCS_MTQ_BDOT_CMD;

typedef struct ADCS_MTQ_STATEST_CMD {
    uint8_t MTQ_Command_Z; //  (No Units)
    uint8_t MTQ_Command_Y; //  (No Units)
    uint8_t MTQ_Command_X; //  (No Units)
    int16_t MTQ_Strength_Z; //  (No Units)
    int16_t MTQ_Strength_Y; //  (No Units)
    int16_t MTQ_Strength_X; //  (No Units)
} ADCS_MTQ_STATEST_CMD;

typedef struct ADCS_RW_CMD {
    uint8_t RW_Command_Z; //  (No Units)
    uint8_t RW_Command_Y; //  (No Units)
    uint8_t RW_Command_X; //  (No Units)
    int16_t RW_SetPoint_Z; // rpm
    int16_t RW_SetPoint_Y; // rpm
    int16_t RW_SetPoint_X; // rpm
} ADCS_RW_CMD;

void encodeVECTOR__INDEPENDENT_SIG_MSG(VECTOR__INDEPENDENT_SIG_MSG *input, CANPacket* output);
void decodeVECTOR__INDEPENDENT_SIG_MSG(CANPacket *input, VECTOR__INDEPENDENT_SIG_MSG *output);

void encodeADCS_MTQ_BDOT_CMD(ADCS_MTQ_BDOT_CMD *input, CANPacket* output);
void decodeADCS_MTQ_BDOT_CMD(CANPacket *input, ADCS_MTQ_BDOT_CMD *output);

void encodeADCS_MTQ_STATEST_CMD(ADCS_MTQ_STATEST_CMD *input, CANPacket* output);
void decodeADCS_MTQ_STATEST_CMD(CANPacket *input, ADCS_MTQ_STATEST_CMD *output);

void encodeADCS_RW_CMD(ADCS_RW_CMD *input, CANPacket* output);
void decodeADCS_RW_CMD(CANPacket *input, ADCS_RW_CMD *output);

#endif /* DSBASE_INTERFACES_CANWRAP_H_ */
