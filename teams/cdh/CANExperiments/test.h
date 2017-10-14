#ifndef CANDB_HEADER
#define CANDB_HEADER

#include <stdint.h>

typedef struct {
	int32_ttotalMisfires2;
	int32_ttotalMisfires1;
} PPTMisfireCount;

typedef struct {
	int16_taverageChargeTime;
	int32_tminutesSinceSuccessfulFire;
	int32_tminutesSinceAttemptedFire;
	int16_tmedianChargeTime;
} PPTTimingStatus;

typedef struct {
	int32_tsuccessfullFires1;
	int32_tsuccessfulFires2;
	int16_tnumberOfMisfires;
	int8_tlastFiringRate;
} FiringStatus;

typedef struct {
	int16_tadcCurrent;
	int8_tcom1Current;
	int16_tcom2Current;
	int16_tLineVoltage;
	int16_trahsCurrent;
} VoltageCurrentInfo;

typedef struct {
	int32_tbatteryFullChargeCount;
	int16_tbatteryTemperature;
	uint32_tbatteryVoltage;
	uint16_tLowestBatteryVoltage;
	int32_tunderVoltageEvents;
} BatteryStatus;

typedef struct {
	int32_tpowerGeneration;
	int16_tovercurrent;
	int32_toutputPower;
	int16_toutputConfig;
	int16_tcoulombCount;
	int8_tbatteryFullyCharged;
	int16_tavePowerGeneration;
} PowerStatus;

typedef struct {
	int32_tnumOfTurnons;
	int32_tminutesSinceTurnon;
	int16_tMCUTemp;
} MCUStatus;


#endif