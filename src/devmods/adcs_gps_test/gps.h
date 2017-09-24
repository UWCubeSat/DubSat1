#ifndef GPS_H_
#define GPS_H_

#include <stdint.h>
#include "core/debugtools.h"

typedef int32_t GPSec;
typedef uint32_t GPS_ENUM;

typedef enum _message_type
{
    Message_BestXYZ = 241,
    Message_Time = 101,
} message_type;

void gpsInit(void);
void gpsSendCommand(uint8_t *command);
uint8_t gpsStatus(DebugMode mode);

#endif /* GPS_H_ */
