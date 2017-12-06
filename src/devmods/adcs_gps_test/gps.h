#ifndef GPS_H_
#define GPS_H_

#include <stdbool.h>

#include "core/utils.h"
#include "GPSPackage.h"

/**
 * Initializes MSP for IO with GPS. Should be called before powering on.
 */
void gpsInit();

/**
 * Powers on and configures the GPS
 */
void gpsPowerOn();

/**
 * Powers off the GPS and finishes processing any leftover logs
 */
void gpsPowerOff();

/**
 * Returns true if anything changed
 */
bool gpsUpdate();

/**
 * Continue to update until there are no incoming messages
 */
bool gpsFlush();

/**
 * Send an ASCII command directly to the GPS
 */
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
    GPSVectorD pos;
    GPSVectorF posStdDev;
    GPSVectorD vel;
    GPSVectorF velStdDev;
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
    uint8_t tempStatus;
} HWMonitorInfo;

#endif /* GPS_H_ */
