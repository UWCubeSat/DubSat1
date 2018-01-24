#ifndef GPS_H_
#define GPS_H_

// use OEM719 if no hardware is specified
#ifndef __BSP_HW_GPS_OEM719__
#ifndef __BSP_HW_GPS_OEM615__
#define __BSP_HW_GPS_OEM719__
#endif
#endif

// number of bytes allocated to array members of GPS messages
#define FLEX_ARRAY_BUFFER_LENGTH 500

// --- GPIO pins ---

#define GPS_ENABLE_DIR P2DIR
#define GPS_ENABLE_OUT P2OUT
#define GPS_ENABLE_BIT BIT2

#define BUCK_ENABLE_DIR P3DIR
#define BUCK_ENABLE_OUT P3OUT
#define BUCK_ENABLE_BIT BIT7

// --- NovAtel IDs and enums ---

#define MSGID_RXSTATUS 93
#define MSGID_RXSTATUSEVENT 94
#define MSGID_TIME 101
#define MSGID_BESTXYZ 241
#define MSGID_HWMONITOR 963
#define MSGID_SATVIS2 1043
#define MSGID_RANGE 43

#define EVTID_POSITION 19
#define EVTID_UTC 18
#define EVTID_CLOCK 22

#define EVTTYPE_CLEAR 0
#define EVTTYPE_SET 1

// see table 97 of firmware reference
#define SATSYSTEM_GPS 0
#define SATSYSTEM_GLONASS 1
#define SATSYSTEM_SBAS 2

#include <stdbool.h>
#include <stdint.h>

#include "core/utils.h"

typedef struct PACKED_STRUCT {
    uint8_t skipped;   // # of skipped messages due to lack of RX buffer memory
    uint8_t bad_crc;   // # of messages with incorrect CRCs
    uint8_t oversized; // # of messages that are too big for memory
} gps_health;

// --- NovAtel logs and commands ---

// types used in NovAtel
typedef int32_t gps_ec;
typedef uint32_t gps_enum;
typedef uint16_t gps_message_id;
typedef uint32_t gps_event_id;

typedef struct PACKED_STRUCT
{
    uint8_t sync[3];
    uint8_t headerLength;
    gps_message_id messageId;
    int8_t messageType;
    uint8_t portAddress;
    uint16_t messageLength;
    uint16_t sequence;
    uint8_t idleTime;
    uint8_t timeStatus;
    uint16_t week;
    gps_ec ms;
    uint32_t rxStatus; // receiver status
    uint16_t reserved;
    uint16_t rxVersion; // receiver S/W version
} GPSHeader;

typedef struct PACKED_STRUCT
{
    double x;
    double y;
    double z;
} GPSVectorD;

typedef struct PACKED_STRUCT
{
    float x;
    float y;
    float z;
} GPSVectorF;

typedef struct PACKED_STRUCT
{
    gps_enum pSolStatus; // position solution status
    gps_enum posType;
    GPSVectorD pos;
    GPSVectorF posStdDev;
    gps_enum vSolStatus; // velocity solution status
    gps_enum velType;
    GPSVectorD vel;
    GPSVectorF velStdDev;
    int8_t stationId[4];
    float velLatency;
    float diffAge;
    float solAge;
    uint8_t numSVs;
    uint8_t numSolnSVs;
    uint8_t numGGL1;
    uint8_t numSolnMultiSVs;
    int8_t reserved;
    uint8_t extSolStat;
    uint8_t sigMaskGalBeiDou;
    uint8_t sigMaskGPSGLO;
} GPSBestXYZ;

typedef struct PACKED_STRUCT
{
    gps_enum clockStatus;
    double offset;
    double offsetStdDev;
    double utcOffset;
    uint32_t utcYear;
    uint8_t utcMonth;
    uint8_t utcDay;
    uint8_t utcHour;
    uint8_t utcMin;
    uint32_t utcMs;
    gps_enum utcStatus; // 0 = invalid, 1 = valid, 2 = warning
} GPSTime;

// a status code for use in RXStatus
typedef struct PACKED_STRUCT
{
    uint32_t word;
    uint32_t pri;  // priority mask
    uint32_t set;  // set mask
    uint32_t clear;  // clear mask
} GPSStatusCode;

typedef struct PACKED_STRUCT
{
    uint32_t error;
    uint32_t numStats;
    GPSStatusCode rxstat;
    GPSStatusCode aux1stat;
    GPSStatusCode aux2stat;
    GPSStatusCode aux3stat;
} GPSRXStatus;

// for use in GPSHWMonitor
typedef struct PACKED_STRUCT
{
    float reading;

    // these three fields are combined as "status" in the firmware guide, but
    // it makes more sense for parsing to treat them as separate.
    uint8_t status;
    uint8_t type;
    uint8_t pad[2];
} GPSMeasurement;

typedef struct PACKED_STRUCT
{
    uint32_t numMeasurements;
#if defined(__BSP_HW_GPS_OEM615__)
    GPSMeasurement temp;
#elif defined(__BSP_HW_GPS_OEM719__)
    GPSMeasurement temp;
    GPSMeasurement antCurrent;
    GPSMeasurement supVolt; // firmware guide claims this measurement isn't
                            // supported but it is
    GPSMeasurement antVolt;
    GPSMeasurement digCoreVolt;
    GPSMeasurement regVolt;
    GPSMeasurement oneVEight;
    GPSMeasurement secTemp;
    GPSMeasurement periphCoreVolt;
#else

#error GPS HW unspecified

#endif
} GPSHWMonitor;

typedef struct PACKED_STRUCT
{
    gps_enum word;
    gps_event_id bitPosition;
    gps_enum event;
    uint8_t description[32];
} GPSRXStatusEvent;

// array element of a SATVIS2 message
typedef struct PACKED_STRUCT
{
    uint32_t satId;
    uint32_t health;
    double elev;
    double az;
    double trueDop;
    double appDop;
} GPSSatvis2Member;

typedef struct PACKED_STRUCT
{
    gps_enum system;
    gps_enum isValid;
    gps_enum usedGNSSAlmanac;
    uint32_t numSats;
    GPSSatvis2Member sats[];  // flexible array member
} GPSSatvis2;

// array element of a RANGE message
typedef struct PACKED_STRUCT
{
    uint16_t prn;
    uint16_t gloFreq;
    double psr; // pseudorange measurement (m)
    float psrStdDev;
    double adr; // carier phase, in cycles
    float adrStdDev;
    float dopp; // instantaneous carrier Doppler frequency (Hz)
    float cToNo; // carrier to noise density ratio
    float locktime; // number of seconds of continuous tracking
    uint32_t status; // tracking status
} GPSRangeMember;

typedef struct PACKED_STRUCT
{
    uint32_t numObs;
    GPSRangeMember obs[];
} GPSRange;

typedef union
{
    GPSBestXYZ bestXYZ;
    GPSTime time;
    GPSRXStatus rxstatus;
    GPSRXStatusEvent rxstatusEvent;
    GPSHWMonitor hwMonitor;
    GPSSatvis2 satvis2;
    GPSRange range;
} GPSMessage;

typedef struct
{
    GPSHeader header;
    GPSMessage message;

    /*
     * "_array" is a buffer for array parts of GPS messages.
     * It must be directly after "message" so that array parts from the message
     * naturally overflow into _array.
     */
    uint8_t _array[FLEX_ARRAY_BUFFER_LENGTH];
} GPSPackage;

// --- Function declarations ---

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
 * Returns a GPSPackage if one is available, otherwise NULL.
 */
GPSPackage *gpsRead();

/**
 * Send an ASCII command directly to the GPS
 */
void gpsSendCommand(uint8_t *command);

/**
 * Send a binary command directly to the GPS
 */
void gpsSendBinaryCommand(gps_message_id messageId,
                          uint8_t *message,
                          uint16_t messageLength);

/**
 * Populate health struct
 */
void gpsGetHealth(gps_health *health);

#endif /* GPS_H_ */
