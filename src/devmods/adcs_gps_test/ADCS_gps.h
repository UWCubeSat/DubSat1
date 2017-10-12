#ifndef ADCS_GPS_H_
#define ADCS_GPS_H_

#include <stdint.h>
#include "core/debugtools.h"
#include "core/utils.h"

typedef int32_t gps_ec;
typedef uint32_t gps_enum;

typedef struct PACKED_STRUCT GPSHeader {
    int8_t sync[3];
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
    GPSHeader header;
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
    GPSHeader header;
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

#endif /* ADCS_GPS_H_ */
