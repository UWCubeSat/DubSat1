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

