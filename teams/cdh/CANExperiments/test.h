typedef struct PPTMisfireCount {
	uint16_t totalMisfires2;
	uint16_t totalMisfires1;
} PPTMisfireCount;

typedef struct PPTTimingStatus {
	uint8_t averageChargeTime;
	uint16_t minutesSinceSuccessfulFire;
	uint16_t minutesSinceAttemptedFire;
	uint8_t medianChargeTime;
} PPTTimingStatus;

typedef struct FiringStatus {
	uint16_t successfullFires1;
	int16_t successfulFires2;
	uint8_t numberOfMisfires;
	uint8_t lastFiringRate;
} FiringStatus;

typedef struct VoltageCurrentInfo {
	uint8_t adcCurrent;
	uint8_t com1Current;
	uint8_t com2Current;
	uint8_t LineVoltage;
	uint8_t rahsCurrent;
} VoltageCurrentInfo;

typedef struct BatteryStatus {
	int16_t batteryFullChargeCount;
	int8_t batteryTemperature;
	uint8_t batteryVoltage;
	uint8_t LowestBatteryVoltage;
	uint16_t underVoltageEvents;
} BatteryStatus;

typedef struct PowerStatus {
	uint8_t powerGeneration;
	uint8_t overcurrent;
	uint8_t outputPower;
	uint8_t outputConfig;
	uint8_t coulombCount;
	uint8_t batteryFullyCharged;
	uint8_t avePowerGeneration;
} PowerStatus;

typedef struct MCUStatus {
	uint16_t numOfTurnons;
	uint16_t minutesSinceTurnon;
	int8_t MCUTemp;
} MCUStatus;

PPTMisfireCount* decodePPTMisfireCount(CANPacket *input);

PPTTimingStatus* decodePPTTimingStatus(CANPacket *input);

FiringStatus* decodeFiringStatus(CANPacket *input);

VoltageCurrentInfo* decodeVoltageCurrentInfo(CANPacket *input);

BatteryStatus* decodeBatteryStatus(CANPacket *input);

PowerStatus* decodePowerStatus(CANPacket *input);

MCUStatus* decodeMCUStatus(CANPacket *input);

CANPacket* encodePPTMisfireCount(PPTMisfireCount *input);

CANPacket* encodePPTTimingStatus(PPTTimingStatus *input);

CANPacket* encodeFiringStatus(FiringStatus *input);

CANPacket* encodeVoltageCurrentInfo(VoltageCurrentInfo *input);

CANPacket* encodeBatteryStatus(BatteryStatus *input);

CANPacket* encodePowerStatus(PowerStatus *input);

CANPacket* encodeMCUStatus(MCUStatus *input);

