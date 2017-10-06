#ifndef ADCS_GPS_H_
#define ADCS_GPS_H_

// See page 364 of the firmware reference for a table of log ID numbers
#define ID_BESTPOS 42
#define ID_BESTVEL 99
#define ID_BESTXYZ 241
#define ID_TIME 101

// See page 734 of the firmware reference for response IDs
#define RESPONSE_ID_OK 1

#include <stdint.h>

typedef int32_t GPSec;
typedef uint32_t GPS_ENUM;

#endif /* ADCS_GPS_H_ */
