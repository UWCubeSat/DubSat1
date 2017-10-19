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

CANPacket *encodePPTTimingStatus(PPTTimingStatus *input);

CANPacket *encodeFiringStatus(FiringStatus *input);

CANPacket *encodeVoltageCurrentInfo(VoltageCurrentInfo *input);

CANPacket *encodeBatteryStatus(BatteryStatus *input);

CANPacket *encodePowerStatus(PowerStatus *input);

CANPacket *encodeMCUStatus(MCUStatus *input);

