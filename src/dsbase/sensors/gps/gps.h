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

// size of byte buffer for uart interrupt to fill while processing GPS messages
#define GPS_RX_BUFFER_LENGTH 20

// --- GPIO pins ---

#define GPS_ENABLE_DIR P2DIR
#define GPS_ENABLE_OUT P2OUT
#define GPS_ENABLE_BIT BIT2

#define BUCK_ENABLE_DIR P3DIR
#define BUCK_ENABLE_OUT P3OUT
#define BUCK_ENABLE_BIT BIT7

#define RESET_DIR P1DIR
#define RESET_OUT P1OUT
#define RESET_BIT BIT6

#define BUCK_GOOD_DIR P3DIR
#define BUCK_GOOD_IN  P3IN
#define BUCK_GOOD_BIT BIT6

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
#define SATSYSTEM_GPS     0
#define SATSYSTEM_GLONASS 1
#define SATSYSTEM_SBAS    2
#define SATSYSTEM_GALILEO 5
#define SATSYSTEM_BEIDOU  6
#define SATSYSTEM_QZSS    7

// HWMONITOR codes, see table 98 of firmware reference
#define HW_TEMP    1
#define HW_ANTCUR  2
#define HW_SUPVOLT 6
#define HW_ANTVOLT 7
#define HW_DIGCORE 8
#define HW_REGVOLT 15 // called Supply Voltage
#define HW_ONEV8   17
#define HW_TEMP2   22
#define HW_PCORE   23

#include <stdbool.h>
#include <stdint.h>

#include "core/utils.h"

typedef struct PACKED_STRUCT {
    uint8_t skipped;     // # of skipped messages for lack of RX buffer memory
    uint8_t bad_crc;     // # of messages with incorrect CRCs
    uint8_t oversized;   // # of messages that are too big for memory
    uint8_t buck_status; // status of buck converter, 1 = good, 0 = not good
} gps_health;

// --- NovAtel logs and commands ---

// types used in NovAtel
typedef int32_t gps_ec;
typedef uint32_t gps_enum;
typedef uint16_t gps_message_id;
typedef uint32_t gps_event_id;

/**
 * the header used in each command and log
 */
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

/**
 * LOG command
 */
typedef struct PACKED_STRUCT
{
    gps_enum port; // output port. Set to 0xc0 for THIS_PORT
    uint16_t messageId;
    uint8_t messageType; // 0 for binary, 64 for abbreviated ASCII
    gps_enum trigger;
    double period;
    double offset;
    gps_enum hold;
} GPSCmdLog;

/**
 * Vector type used to represent position and velocity
 */
typedef struct PACKED_STRUCT
{
    double x;
    double y;
    double z;
} GPSVectorD;

/**
 * Vector type used to represent standard deviation of position and velocity
 */
typedef struct PACKED_STRUCT
{
    float x;
    float y;
    float z;
} GPSVectorF;

/**
 * BESTXYZ log
 */
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


/* TODO: GPSLonLat */
typedef struct PACKED_STRUCT {
    double lat;
    double lon;
} GPSLonLat;

typedef struct PACKED_STRUCT {
    double minDotDecimal;
    double decimalDegrees;
} SendLoc;

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
    GPSMeasurement mes[];
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
 * Switch the buck converter on or off
 */
void gpsSetBuck(uint8_t enable);

/**
 * Returns 1 iff the buck converter is enabled. 0 otherwise
 */
uint8_t gpsIsBuckEnabled();

/**
 * Returns 1 iff the buck converter status is good
 */
uint8_t gpsIsBuckGood();

/**
 * Powers the GPS on or off.
 *
 * Set reset off > 150 ms after powering on the GPS.
 *
 * Powering off the GPS will flush any leftover logs.
 */
void gpsSetPower(uint8_t on);

/**
 * Returns 1 iff the gps switch is enabled. 0 otherwise
 */
uint8_t gpsIsPowerEnabled();

/**
 * Set the RESET pin active or inactive (active low). An active RESET signal
 * will reboot the receiver without use of the power switch or buck converter.
 * RESET should be active for > 150 ms while the receiver is booting up, then
 * set inactive for normal operation.
 */
void gpsSetReset(uint8_t active);

/**
 * Returns 1 iff the RESET pin is active (low). 0 otherwise.
 */
uint8_t gpsIsResetActive();

/**
 * Returns a GPSPackage if one is available, otherwise NULL.
 */
GPSPackage *gpsRead();


void gpsSendData(uint8_t *data, uint16_t len);

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
