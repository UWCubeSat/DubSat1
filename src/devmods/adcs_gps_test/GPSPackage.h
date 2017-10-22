#ifndef GPSPACKAGE_H_
#define GPSPACKAGE_H_

#include <stdbool.h>

#include "core/utils.h"

typedef int32_t gps_ec;
typedef uint32_t gps_enum;

typedef struct PACKED_STRUCT GPSHeader {
    uint8_t sync[3];
    uint8_t headerLength;
    uint16_t messageId;
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

typedef struct PACKED_STRUCT GPSVectorD {
    double x;
    double y;
    double z;
} GPSVectorD;

typedef struct PACKED_STRUCT GPSVectorF {
    float x;
    float y;
    float z;
} GPSVectorF;

typedef struct PACKED_STRUCT GPSBestXYZ {
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

typedef struct PACKED_STRUCT GPSTime {
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
typedef struct PACKED_STRUCT GPSStatusCode {
    uint32_t word;
    uint32_t pri;  // priority mask
    uint32_t set;  // set mask
    uint32_t clear;  // clear mask
} GPSStatusCode;

typedef struct PACKED_STRUCT GPSRXStatus {
    uint32_t error;
    uint32_t numStats;
    GPSStatusCode rxstat;
    GPSStatusCode aux1stat;
    GPSStatusCode aux2stat;
    GPSStatusCode aux3stat;
} GPSRXStatus;

// for use in GPSHWMonitor
typedef struct PACKED_STRUCT GPSMeasurement {
    float reading;
    uint32_t status;
} GPSMeasurement;

// TODO This is by far the biggest message and most of the space is wasted. If
// the message size becomes a problem this will probably be why.
typedef struct PACKED_STRUCT GPSHWMonitor {
    uint32_t numMeasurements;
    GPSMeasurement measurements[23];
} GPSHWMonitor;

typedef struct PACKED_STRUCT GPSRXStatusEvent {
    gps_enum word;
    uint32_t bitPosition;
    gps_enum event;
    uint8_t description[32];
} GPSRXStatusEvent;

typedef union GPSMessage {
    GPSBestXYZ bestXYZ;
    GPSTime time;
    GPSRXStatus rxstatus;
    GPSRXStatusEvent rxstatusEvent;
    GPSHWMonitor hwMonitor;
} GPSMessage;

typedef struct PACKED_STRUCT GPSPackage {
    GPSHeader header;
    GPSMessage message;
    uint32_t crc;
} GPSPackage;

bool isGPSPackageValid(GPSPackage *p);

#endif /* GPSPACKAGE_H_ */
