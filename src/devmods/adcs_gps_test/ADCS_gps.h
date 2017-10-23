#ifndef ADCS_GPS_H_
#define ADCS_GPS_H_

#include "core/debugtools.h"

uint8_t gpsStatus(DebugMode mode);

void gpsSendCommand(uint8_t *command);

uint8_t actionHandler(DebugMode mode, uint8_t *command);

#endif /* ADCS_GPS_H_ */
