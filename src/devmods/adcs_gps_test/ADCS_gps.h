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

#ifdef __DEBUG__
const char *GPS_ERROR[] = {
    "Error (use RXSTATUS for details)",
    "Temperature warning",
    "Voltage supply warning",
    "Antenna not powered",
    "LNA Failure",
    "Antenna open",
    "Antenna shorted",
    "CPU overload",
    "COM1 buffer overrun",
    "COM2 buffer overrun",
    "COM3 buffer overrun",
    "Link overrun",
    "",
    "Aux transmit overrun",
    "AGC out of range",
    "",
    "INS reset",
    "",
    "GPS Almanac/UTC invalid",
    "Position invalid",
    "Position fixed",
    "Clock steering disabled",
    "Clock model invalid",
    "External oscillator locked",
    "Software resource warning",
    "",
    "",
    "",
    "",
    "Aux 3 status event",
    "Aux 2 status event",
    "Aux 1 status event"
};
#else
const char *GPS_ERROR[];
#endif /* __DEBUG__ */

#endif /* ADCS_GPS_H_ */
