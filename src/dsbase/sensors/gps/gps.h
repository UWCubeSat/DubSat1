#ifndef GPS_H_
#define GPS_H_

#define GPS_ENABLE_DIR P2DIR
#define GPS_ENABLE_OUT P2OUT
#define GPS_ENABLE_BIT BIT2

#include <stdbool.h>

#include "core/utils.h"
#include "GPSPackage.h"
#include "GPSReader.h"

#ifdef __DEBUG__

#if defined(__BSP_HW_GPS_OEM615__)

FILE_STATIC const char *GPS_STATUS_DESC[] =
{
    "Error (use RXSTATUS for details)", "Temperature warning",
    "Voltage supply warning", "Antenna not powered", "LNA Failure",
    "Antenna open", "Antenna shorted", "CPU overload",
    "COM1 buffer overrun", "COM2 buffer overrun", "COM3 buffer overrun",
    "Link overrun", "", "Aux transmit overrun", "AGC out of range", "",
    "INS reset", "", "GPS Almanac/UTC invalid", "Position invalid",
    "Position fixed", "Clock steering disabled", "Clock model invalid",
    "External oscillator locked", "Software resource warning", "", "", "",
    "", "Aux 3 status event", "Aux 2 status event", "Aux 1 status event" };

#elif defined(__BSP_HW_GPS_OEM719__)

FILE_STATIC const char *GPS_STATUS_DESC[] =
{
    "Error (use RXSTATUS for details)", "Temperature warning",
    "Voltage supply warning", "Antenna not powered", "LNA Failure",
    "Antenna open", "Antenna shorted", "CPU overload",
    "COM1 buffer overrun", "", "", "Link overrun", "Input overrun",
    "Aux transmit overrun", "AGC out of range", "Jammer detected",
    "INS reset", "IMU communication failure", "GPS Almanac/UTC invalid",
    "Position invalid", "Position fixed", "Clock steering disabled",
    "Clock model invalid", "External oscillator locked",
    "Software resource warning", "Version bit 0", "Version bit 1",
    "HDR tracking", "Digital filtering enabled", "Aux 3 status event",
    "Aux 2 status event", "Aux 1 status event" };

#else

#error GPS hardware unspecified

#endif /* __BPS_HW_GPS_*__ */

#else

FILE_STATIC const char *GPS_STATUS_DESC[];

#endif /* __DEBUG__ */

typedef void (*gps_message_handler)(const GPSPackage *package);
typedef void (*gps_event_handler)(const GPSRXStatusEvent *event);
typedef void (*gps_header_handler)(const GPSHeader *header);

typedef struct PACKED_STRUCT {
    uint8_t unknown_msg_ids;
    uint8_t unknown_event_ids;
    uint8_t registration_errors;
    gpsreader_health reader_health;
} gps_health;

/**
 * Initializes MSP for IO with GPS. Should be called before powering on.
 *
 * genericMsgHandler: called every time a message is received
 */
void gpsInit(gps_header_handler messageHandler);

/**
 * Powers on and configures the GPS
 */
void gpsPowerOn();

/**
 * Powers off the GPS and finishes processing any leftover logs
 */
void gpsPowerOff();

/**
 * Returns true if anything changed
 */
bool gpsUpdate();

/**
 * Continue to update until there are no incoming messages
 */
bool gpsFlush();

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
 * Register a callback associated with a message ID. Callbacks are called during
 * calls to gpsUpdate(). Returns true iff success.
 */
bool gpsRegisterMessageHandler(gps_message_id messageId,
                               gps_message_handler handler);

/**
 * Register a callback to be called upon receiving a RXSTATUSEVENT message.
 * eventId corresponds to the bit position of the status word. Returns true iff
 * success.
 */
bool gpsRegisterEventHandler(gps_event_id eventId,
                             gps_event_handler handler,
                             gps_enum eventType);

/**
 * Populate health struct
 */
void gpsGetHealth(gps_health *health);

#endif /* GPS_H_ */
