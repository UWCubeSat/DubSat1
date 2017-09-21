#ifndef GPS_H_
#define GPS_H_

// See page 364 of the firmware reference for a table of log ID numbers
#define ID_BESTXYZ 241
#define ID_TIME 101

// See page 734 of the firmware reference for response IDs
#define RESPONSE_ID_OK 1

#include <stdint.h>
#include "core/debugtools.h"

typedef int32_t GPSec;
typedef uint32_t GPS_ENUM;

void gpsInit(void);
void sendCommand(uint8_t *command);
uint8_t gpsStatus(DebugMode mode);

#endif /* GPS_H_ */
