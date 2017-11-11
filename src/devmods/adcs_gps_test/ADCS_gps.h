#ifndef ADCS_GPS_H_
#define ADCS_GPS_H_

#include <stdint.h>
#include "core/debugtools.h"

typedef int32_t GPSec;
typedef uint32_t GPS_ENUM;

typedef enum _message_type
{
    Message_BestXYZ = 241,
    Message_Time = 101,
} message_type;

#endif /* ADCS_GPS_H_ */
