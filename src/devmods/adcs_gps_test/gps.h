#ifndef GPS_H_
#define GPS_H_

#define OPCODE_SEND_ASCII 1

#include <stdbool.h>

#include "core/utils.h"
#include "GPSPackage.h"

typedef void (*gps_message_handler)(const GPSPackage *package);
typedef void (*gps_event_handler)(const GPSRXStatusEvent *event);

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

/**
 * Register a callback associated with a message ID. Callbacks are called during
 * calls to gpsUpdate(). Returns true iff success.
 */
bool gpsRegisterMessageHandler(gps_message_id messageId,
                               gps_message_handler handler);

/**
 * Register a callback to be called upon receiving a RXSTATUSEVENT message.
 * eventId corresponds to the bit position of the status word. Returns true iff
 * success.
 */
bool gpsRegisterEventHandler(gps_event_id eventId,
                             gps_event_handler handler,
                             gps_enum eventType);

// the following structs are for telemetry. The raw data is in GPSPackage.h

typedef struct PACKED_STRUCT _rxstatus_info {
    BcTlmHeader header;
    uint32_t error;
    uint32_t status;
    uint32_t aux1;
    uint32_t aux2;
    uint32_t aux3;
} RXStatusInfo;

typedef struct PACKED_STRUCT _bestxyz_info {
    BcTlmHeader header;
    gps_enum posStatus;
    GPSVectorD pos;
    GPSVectorF posStdDev;
    gps_enum velStatus;
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
    gps_enum clockStatus;
    gps_enum utcStatus;
} TimeInfo;

typedef struct PACKED_STRUCT _hwmonitor_info {
    BcTlmHeader header;
    float temp;
    uint8_t tempStatus;
} HWMonitorInfo;

typedef struct PACKED_STRUCT _satvis2_info {
    BcTlmHeader header;
    uint32_t numGPS;
    uint32_t numGLONASS;
    uint32_t numSBAS;
} Satvis2Info;

#endif /* GPS_H_ */
