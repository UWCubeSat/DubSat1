#ifndef CANDB_HEADER
#define CANDB_HEADER

#include <stdint.h>

typedef struct PPTMisfireCount {
	uint32_ttotalMisfires2;
	uint32_ttotalMisfires1;
} PPTMisfireCount;

typedef struct PPTTimingStatus {
	uint16_taverageChargeTime;
	uint32_tminutesSinceSuccessfulFire;
	uint32_tminutesSinceAttemptedFire;
	uint16_tmedianChargeTime;
} PPTTimingStatus;

typedef struct FiringStatus {
	uint32_tsuccessfullFires1;
	uint32_tsuccessfulFires2;
	uint16_tnumberOfMisfires;
	uint8_tlastFiringRate;
} FiringStatus;

typedef struct VoltageCurrentInfo {
	uint16_tadcCurrent;
	uint8_tcom1Current;
	uint16_tcom2Current;
	uint16_tLineVoltage;
	uint16_trahsCurrent;
} VoltageCurrentInfo;

typedef struct BatteryStatus {
	uint32_tbatteryFullChargeCount;
	int16_tbatteryTemperature;
	uint32_tbatteryVoltage;
	uint16_tLowestBatteryVoltage;
	uint32_tunderVoltageEvents;
} BatteryStatus;

typedef struct PowerStatus {
	uint16_tpowerGeneration;
	uint8_tovercurrent;
	uint16_toutputPower;
	uint8_toutputConfig;
	uint16_tcoulombCount;
	uint8_tbatteryFullyCharged;
	uint16_tavePowerGeneration;
} PowerStatus;

typedef struct MCUStatus {
	uint32_tnumOfTurnons;
	uint32_tminutesSinceTurnon;
	int8_tMCUTemp;
} MCUStatus;


#endif