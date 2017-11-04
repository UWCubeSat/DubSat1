#ifndef GPS_H_
#define GPS_H_

#include <stdbool.h>

#include "core/utils.h"
#include "GPSPackage.h"

void gpsInit();

/**
 * returns true if anything changed
 */
bool gpsUpdate();

void gpsSendCommand(uint8_t *command);

// the following structs are for telemetry. The raw data is in GPSPackage.h

typedef struct PACKED_STRUCT _rxstatus_info {
    BcTlmHeader header;
    uint32_t error;
    uint32_t status;
    uint32_t aux1;
    uint32_t aux2;
    uint32_t aux3;
    uint32_t invalidMessages; // this should maybe move somewhere else
} RXStatusInfo;

typedef struct PACKED_STRUCT _bestxyz_info {
    BcTlmHeader header;
    double pX;
    double pY;
    double pZ;
    double vX;
    double vY;
    double vZ;
    uint16_t week;
    int32_t ms;
} BestXYZInfo;

typedef struct PACKED_STRUCT _time_info {
    BcTlmHeader header;
    double offset;
    int32_t ms;
    uint16_t week;
} TimeInfo;

typedef struct PACKED_STRUCT _hwmonitor_info {
    BcTlmHeader header;
    float temp;
    uint8_t status;
} HWMonitorInfo;

#endif /* GPS_H_ */
