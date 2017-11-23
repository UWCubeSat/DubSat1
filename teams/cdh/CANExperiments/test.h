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

void encodePPTMisfireCount(PPTMisfireCount *input, CANPacket* output);
void decodePPTMisfireCount(CANPacket *input, PPTMisfireCount *output);

void encodePPTTimingStatus(PPTTimingStatus *input, CANPacket* output);
void decodePPTTimingStatus(CANPacket *input, PPTTimingStatus *output);

void encodeFiringStatus(FiringStatus *input, CANPacket* output);
void decodeFiringStatus(CANPacket *input, FiringStatus *output);

void encodeVoltageCurrentInfo(VoltageCurrentInfo *input, CANPacket* output);
void decodeVoltageCurrentInfo(CANPacket *input, VoltageCurrentInfo *output);

void encodeBatteryStatus(BatteryStatus *input, CANPacket* output);
void decodeBatteryStatus(CANPacket *input, BatteryStatus *output);

void encodePowerStatus(PowerStatus *input, CANPacket* output);
void decodePowerStatus(CANPacket *input, PowerStatus *output);

void encodeMCUStatus(MCUStatus *input, CANPacket* output);
void decodeMCUStatus(CANPacket *input, MCUStatus *output);

