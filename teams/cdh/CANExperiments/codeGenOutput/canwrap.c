/*
 * canwrap.c
 *
 *  Created on: Aug 14, 2017
 *      Author: emoryeng
 */

#include "canwrap.h"
#include "../core/can.h"

//void canPacketInit(uint8_t boardNum){
//    canInit();
//    setTheFilter(CAN_MASK_0, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_0, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_1, 0x01 << boardNum);
//    setTheFilter(CAN_MASK_1, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_2, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_3, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_4, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_5, 0x01 << boardNum);
//}
void setMaskOrFilter(uint8_t addr, uint32_t filter){
    setTheFilter(addr, filter);
}

void wrapCB0(uint8_t length, uint8_t* data, uint32_t id){
    CANPacket packet = {0};
    CANPacket *p = &packet;
    p -> id = id;
    uint8_t i;
    for(i = 0 ; i < length; i++){
        p -> data[i] = data[i];
    }
    p -> bufferNum = 0;
    CANPacketReceived(p);
}

void wrapCB1(uint8_t length, uint8_t* data, uint32_t id){
    CANPacket packet = {0};
    CANPacket *p = &packet;
    p -> id = id;
    uint8_t i;
    for(i = 0 ; i < length; i++){
        p -> data[i] = data[i];
    }
    p -> bufferNum = 1;
    CANPacketReceived(p);
}

void canWrapInit(){
    canInit();
    setReceiveCallback0(wrapCB0);
    setReceiveCallback1(wrapCB1);
}

void reverseArray(uint8_t arr[], uint8_t start, uint8_t end)
{
    uint8_t temp;
    if (start >= end)
        return;
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    reverseArray(arr, start+1, end-1);
}

void canSendPacket(CANPacket *packet){
    uint8_t tech[5] = {
       (uint8_t) (packet->id >> 21),
       (uint8_t) (packet->id >> 16) & 0x03 | (uint8_t) (packet->id >> 13) & 0xE0 | 0x08,
       (uint8_t) (packet->id >> 8),
       (uint8_t) packet->id,
       packet->length
    };
    canSend(0,tech, packet->data);
}

void setCANPacketRxCallback(void (*ReceiveCallbackArg)(CANPacket *packet)) {
    CANPacketReceived = ReceiveCallbackArg;
}

// AUTOGEN STUFF HERE

void decodeVECTOR__INDEPENDENT_SIG_MSG(CANPacket *input, VECTOR__INDEPENDENT_SIG_MSG *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> CDH_GroupingID = (uint8_t) (((fullData & ((uint64_t) 0xf << -4)) >> -4));
}

void encodeVECTOR__INDEPENDENT_SIG_MSG(VECTOR__INDEPENDENT_SIG_MSG *input, CANPacket *output){
    output -> id = 1073741824;
    output -> length = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> CDH_GroupingID))) & 0xf) << 60;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeADCS_MTQ_BDOT_CMD(CANPacket *input, ADCS_MTQ_BDOT_CMD *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> MTQ_Command_Z = (uint8_t) (((fullData & ((uint64_t) 0x3 << 2)) >> 2));
    output -> MTQ_Command_Y = (uint8_t) (((fullData & ((uint64_t) 0x3 << 4)) >> 4));
    output -> MTQ_Command_X = (uint8_t) (((fullData & ((uint64_t) 0x3 << 6)) >> 6));
    output -> MTQ_Strength_Z = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
    output -> MTQ_Strength_Y = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> MTQ_Strength_X = (int16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
}

void encodeADCS_MTQ_BDOT_CMD(ADCS_MTQ_BDOT_CMD *input, CANPacket *output){
    output -> id = 514;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> MTQ_Command_Z))) & 0x3) << 10;
    fullPacketData |= (((uint64_t)((input -> MTQ_Command_Y))) & 0x3) << 12;
    fullPacketData |= (((uint64_t)((input -> MTQ_Command_X))) & 0x3) << 14;
    fullPacketData |= (((uint64_t)((input -> MTQ_Strength_Z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> MTQ_Strength_Y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> MTQ_Strength_X))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeADCS_MTQ_STATEST_CMD(CANPacket *input, ADCS_MTQ_STATEST_CMD *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> MTQ_Command_Z = (uint8_t) (((fullData & ((uint64_t) 0x3 << 2)) >> 2));
    output -> MTQ_Command_Y = (uint8_t) (((fullData & ((uint64_t) 0x3 << 4)) >> 4));
    output -> MTQ_Command_X = (uint8_t) (((fullData & ((uint64_t) 0x3 << 6)) >> 6));
    output -> MTQ_Strength_Z = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
    output -> MTQ_Strength_Y = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> MTQ_Strength_X = (int16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
}

void encodeADCS_MTQ_STATEST_CMD(ADCS_MTQ_STATEST_CMD *input, CANPacket *output){
    output -> id = 513;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> MTQ_Command_Z))) & 0x3) << 10;
    fullPacketData |= (((uint64_t)((input -> MTQ_Command_Y))) & 0x3) << 12;
    fullPacketData |= (((uint64_t)((input -> MTQ_Command_X))) & 0x3) << 14;
    fullPacketData |= (((uint64_t)((input -> MTQ_Strength_Z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> MTQ_Strength_Y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> MTQ_Strength_X))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeADCS_RW_CMD(CANPacket *input, ADCS_RW_CMD *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> RW_Command_Z = (uint8_t) (((fullData & ((uint64_t) 0xf << 52)) >> 52));
    output -> RW_Command_Y = (uint8_t) (((fullData & ((uint64_t) 0xf << 56)) >> 56));
    output -> RW_Command_X = (uint8_t) (((fullData & ((uint64_t) 0xf << 60)) >> 60));
    output -> RW_SetPoint_Z = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> RW_SetPoint_Y = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> RW_SetPoint_X = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
}

void encodeADCS_RW_CMD(ADCS_RW_CMD *input, CANPacket *output){
    output -> id = 512;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> RW_Command_Z))) & 0xf) << 52;
    fullPacketData |= (((uint64_t)((input -> RW_Command_Y))) & 0xf) << 56;
    fullPacketData |= (((uint64_t)((input -> RW_Command_X))) & 0xf) << 60;
    fullPacketData |= (((uint64_t)((input -> RW_SetPoint_Z))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> RW_SetPoint_Y))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> RW_SetPoint_X))) & 0xffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

