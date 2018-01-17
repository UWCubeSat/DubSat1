#define PACKAGE_BUFFER_LENGTH 3
#define HANDLER_BUFFER_LENGTH 10

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "core/uart.h"
#include "core/debugtools.h"

#include "gps.h"
#include "GPSPackage.h"
#include "GPSReader.h"
#include "GPSIDs.h"
#include "crc.h"

FILE_STATIC GPSPackage packageBuffer[PACKAGE_BUFFER_LENGTH];

typedef struct message_handler_entity
{
    gps_message_handler handler;
    gps_message_id id;
} message_handler_entity;
FILE_STATIC message_handler_entity messageHandlers[HANDLER_BUFFER_LENGTH];
FILE_STATIC uint8_t numMessageHandlers = 0;

typedef struct event_handler_entity
{
    gps_event_handler handler;
    gps_event_id eventId;
    gps_enum eventType;
} event_handler_entity;
FILE_STATIC event_handler_entity eventHandlers[HANDLER_BUFFER_LENGTH];
FILE_STATIC uint8_t numEventHandlers = 0;

FILE_STATIC hBus uartHandle;

FILE_STATIC gps_header_handler genericMsgHandler;

FILE_STATIC gps_health health;

FILE_STATIC void handleRxStatusEvent(const GPSPackage *package);

void gpsInit(gps_header_handler messageHandler)
{
    // zero out health struct
    health.registration_errors = 0;
    health.unknown_event_ids = 0;
    health.unknown_msg_ids = 0;

    // initialize the reader
    uartHandle = uartInit(ApplicationUART, 0, Speed_9600);
    GPSReaderInit(uartHandle, packageBuffer, PACKAGE_BUFFER_LENGTH);

    // register default message handlers
    gpsRegisterMessageHandler(MSGID_RXSTATUSEVENT, handleRxStatusEvent);
    genericMsgHandler = messageHandler;

    // configure power GPIO to be output
    GPS_POWER_DIR |= GPS_POWER_BIT;
}

void gpsPowerOn()
{
    GPS_POWER_OUT |= GPS_POWER_BIT;
}

void gpsPowerOff()
{
    GPS_POWER_OUT &= ~GPS_POWER_BIT;
    gpsFlush();
}

bool gpsUpdate()
{
    GPSPackage *package = ReadGPSPackage();
    if (package == NULL)
    {
        return false;
    }

    // call the generic message handler if it exists
    if (genericMsgHandler != NULL)
    {
        (genericMsgHandler)(&package->header);
    }

    // filter out response messages
    /*
     * TODO start checking these for confirmation (see LOG command).
     * Replies are in the same form as the command, so we need to switch to
     * sending binary commands if we want confirmation messages.
     */
    if (package->header.messageType & 0b1000000)
    {
        // the receiver responds to commands with a confirmation message,
        // which can be ignored
        debugPrintF("received command: %u\r\n", package->header.messageId);
    }
    else
    {
        // delegate the package to its handlers
        uint8_t i = numMessageHandlers;
        uint8_t numRegistered = 0;
        while (i-- != 0)
        {
            if (messageHandlers[i].id == package->header.messageId)
            {
                (messageHandlers[i].handler)(package);
                numRegistered++;
            }
        }
        if (numRegistered == 0)
        {
            debugPrintF("unregistered message ID: %u\r\n",
                        package->header.messageId);
            health.unknown_msg_ids++;
        }
    }

    PopGPSPackage();

    return true;
}

bool gpsFlush()
{
    bool updated = false;
    while (gpsUpdate())
    {
        updated = true;
    }
    return updated;
}

void gpsSendCommand(uint8_t *command)
{
    uartTransmit(uartHandle, command, strlen((char*) command));
}

void gpsSendBinaryCommand(gps_message_id messageId,
                          uint8_t *message,
                          uint16_t messageLength)
{
    // populate header
    GPSHeader header;
    header.sync[0] = 0xAA;
    header.sync[1] = 0x44;
    header.sync[2] = 0x12;
    header.headerLength = sizeof(header);
    header.messageId = messageId;
    header.messageType = 0; // ???
    header.portAddress = 0xc0; // 0xc0: THISPORT
    header.messageLength = messageLength;
    // all other header fields are either ignored or recommended 0 on input
    header.sequence = 0;
    header.idleTime = 0;
    header.timeStatus = 0;
    header.week = 0;
    header.ms = 0;
    header.rxStatus = 0;
    header.reserved = 0;
    header.rxVersion = 0;

    // calculate crc
    uint32_t crc = calculateBlockCrc32(sizeof(header), (uint8_t *) &header);
    uint16_t i;
    for (i = 0; i < messageLength; i++)
    {
        crc = continueCrc32(crc, message[i]);
    }

    // transmit command
    uartTransmit(uartHandle, (uint8_t *) &header, sizeof(header));
    uartTransmit(uartHandle, message, messageLength);
    uartTransmit(uartHandle, (uint8_t *) &crc, sizeof(crc));
}

bool gpsRegisterMessageHandler(gps_message_id messageId,
                               gps_message_handler handler)
{
    if (numMessageHandlers >= HANDLER_BUFFER_LENGTH)
    {
        debugPrintF("too many message handlers\r\n");
        health.registration_errors++;
        return false;
    }
    messageHandlers[numMessageHandlers++]
                    = (message_handler_entity){ handler, messageId };
    return true;
}

bool gpsRegisterEventHandler(gps_event_id eventId,
                             gps_event_handler handler,
                             gps_enum eventType)
{
    if (numEventHandlers >= HANDLER_BUFFER_LENGTH)
    {
        debugPrintF("too many event handlers\r\n");
        health.registration_errors++;
        return false;
    }
    eventHandlers[numEventHandlers++]
                  = (event_handler_entity){ handler, eventId, eventType };
    return true;
}

void gpsGetHealth(gps_health *h)
{
    GPSReaderGetHealth(&h->reader_health);
    h->registration_errors = health.registration_errors;
    h->unknown_event_ids = health.unknown_event_ids;
    h->unknown_msg_ids = health.unknown_msg_ids;
}

FILE_STATIC void handleRxStatusEvent(const GPSPackage *package)
{
    const GPSRXStatusEvent e = package->message.rxstatusEvent;

    // only read the status events
    // TODO read and deal with error/aux events too
    if (e.word != 1)
    {
        return;
    }

    uint8_t i = numEventHandlers;
    uint8_t foundAtLeastOne = 0;
    while (i-- != 0)
    {
        event_handler_entity h = eventHandlers[i];
        if (h.eventId == e.bitPosition && h.eventType == e.event)
        {
            (eventHandlers[i].handler)(&e);
            foundAtLeastOne = 1;
        }
    }
    if (!foundAtLeastOne)
    {
        health.unknown_event_ids++;
        debugPrintF("unregistered event ID: %u\r\n", package->header.messageId);
    }
}
