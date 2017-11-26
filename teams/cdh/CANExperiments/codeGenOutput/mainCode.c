void decodeMessage5ValueTableSigs(CANPacket *input, Message5ValueTableSigs *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> ValueTableSignal1 = (uint8_t) (((fullData & ((uint64_t) 0b11111111))));
}

void encodeMessage5ValueTableSigs(Message5ValueTableSigs *input, CANPacket *output){
    output -> id = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> ValueTableSignal1))) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeMessage4OddSizes(CANPacket *input, Message4OddSizes *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> OddSize3 = (uint64_t) (((fullData & ((uint64_t) 0b1111111111111111111111111111111111111 << 3)) >> 3));
    output -> OddSize2 = (int16_t) (((fullData & ((uint64_t) 0b111111111 << 43)) >> 43));
    output -> OddSize1 = (uint8_t) (((fullData & ((uint64_t) 0b111 << 58)) >> 58));
}

void encodeMessage4OddSizes(Message4OddSizes *input, CANPacket *output){
    output -> id = 3;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> OddSize3))) << 3;
    fullPacketData |= ((uint64_t)((input -> OddSize2))) << 43;
    fullPacketData |= ((uint64_t)((input -> OddSize1))) << 58;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeMessage3(CANPacket *input, Message3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> NormalDouble = (uint8_t) (((fullData & ((uint64_t) 0b1111111111111111111111111111111111111111111111111111111111111111))));
}

void encodeMessage3(Message3 *input, CANPacket *output){
    output -> id = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> NormalDouble)));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeMessage2Smaller(CANPacket *input, Message2Smaller *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> NormalFloat = (uint8_t) (((fullData & ((uint64_t) 0b11111111111111111111111111111111))));
}

void encodeMessage2Smaller(Message2Smaller *input, CANPacket *output){
    output -> id = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> NormalFloat))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeMessage1(CANPacket *input, Message1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> NormalSignedInt = (int8_t) (((fullData & ((uint64_t) 0b11111111))));
    output -> IntFactorOffset = (int32_t) (((fullData & ((uint64_t) 0b1111111111111111 << 8)) >> 8) * 3 + 357);
    output -> FloatFactor = (uint8_t) (((fullData & ((uint64_t) 0b11111111111111111111111111111111 << 24)) >> 24) * 2.75);
    output -> NormalUint = (uint8_t) (((fullData & ((uint64_t) 0b11111111 << 56)) >> 56));
}

void encodeMessage1(Message1 *input, CANPacket *output){
    output -> id = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= ((uint64_t)((input -> NormalSignedInt)));
    fullPacketData |= ((uint64_t)((input -> IntFactorOffset - 357) / 3)) << 8;
    fullPacketData |= ((uint64_t)((input -> FloatFactor) / 2.75)) << 24;
    fullPacketData |= ((uint64_t)((input -> NormalUint))) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

