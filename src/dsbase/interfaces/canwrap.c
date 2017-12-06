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
    packet -> data[1] = packet->length;
    canSend(0,tech, packet->data);
}

struct CANPacket *canConvertToPacket(uint32_t id, uint8_t* data){
    CANPacket ps = {0};
    struct CANPacket *p = &ps;
    p -> id = id;
    uint8_t i;
    for (i = 8; i; i--) p -> data[i-1] = data[i-1];
    return p;
}

void setCANPacketRxCallback(void (*ReceiveCallbackArg)(CANPacket *packet)) {
    CANPacketReceived = ReceiveCallbackArg;
}

// AUTOGEN STUFF HERE

void decodeMessage5ValueTableSigs(CANPacket *input, Message5ValueTableSigs *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> ValueTableSignal1 = (uint8_t) (((fullData & ((uint64_t) 0xff))));
}

void encodeMessage5ValueTableSigs(Message5ValueTableSigs *input, CANPacket *output){
    output -> id = 4;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> ValueTableSignal1))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeMessage4OddSizes(CANPacket *input, Message4OddSizes *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> OddSize3 = (uint64_t) (((fullData & ((uint64_t) 0x1fffffffff << 3)) >> 3));
    output -> OddSize2 = (int16_t) (((fullData & ((uint64_t) 0x1ff << 43)) >> 43));
    output -> OddSize1 = (uint8_t) (((fullData & ((uint64_t) 0x7 << 58)) >> 58));
}

void encodeMessage4OddSizes(Message4OddSizes *input, CANPacket *output){
    output -> id = 3;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> OddSize3))) & 0x1fffffffff) << 3;
    fullPacketData |= (((uint64_t)((input -> OddSize2))) & 0x1ff) << 43;
    fullPacketData |= (((uint64_t)((input -> OddSize1))) & 0x7) << 58;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeMessage3(CANPacket *input, Message3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempNormalDouble = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> NormalDouble = (*((double *)(&(tempNormalDouble))));
}

void encodeMessage3(Message3 *input, CANPacket *output){
    output -> id = 2;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempNormalDouble = ((input -> NormalDouble));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempNormalDouble)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeMessage2Smaller(CANPacket *input, Message2Smaller *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempNormalFloat = (uint32_t) ((fullData & ((uint64_t) 0xffffffff)));
    output -> NormalFloat = (*((float *)(&(tempNormalFloat))));
}

void encodeMessage2Smaller(Message2Smaller *input, CANPacket *output){
    output -> id = 1;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempNormalFloat = ((input -> NormalFloat));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempNormalFloat))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeMessage1(CANPacket *input, Message1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> NormalSignedInt = (int8_t) (((fullData & ((uint64_t) 0xff))));
    output -> IntFactorOffset = (int32_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8) * 3 + 357);
    uint32_t tempFloatFactor = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 24)) >> 24);
    output -> FloatFactor = (*((float *)(&(tempFloatFactor)))) * 2.75;
    output -> NormalUint = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodeMessage1(Message1 *input, CANPacket *output){
    output -> id = 0;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> NormalSignedInt))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> IntFactorOffset - 357) / 3)) & 0xffff) << 8;
    const float tempFloatFactor = ((input -> FloatFactor)) / 2.75;
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempFloatFactor))))) << 24;
    fullPacketData |= (((uint64_t)((input -> NormalUint))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}


