/*
   David Dupre
   djdupre@uw.edu

   This file requests and then parses a synchronous (repeated) log from the
   NovAtel OEM615 receiver.

   Firmware reference:
   https://www.novatel.com/assets/Documents/Manuals/om-20000129.pdf

   User manual:
   https://www.novatel.com/assets/Documents/Manuals/om-20000128.pdf
*/

#include <SoftwareSerial.h>

#define PIN_RX 10 // receive pin
#define PIN_TX 11 // transmit pin

// The log ID numbers for the logs we know how to parse.
// See page 364 of the firmware reference for a table of ID numbers.
#define ID_BESTPOS 42
#define ID_BESTXYZ 241
#define ID_TIME 101

// See page 734 for response IDs
#define RESPONSE_ID_OK 1

typedef long GPSec;
typedef unsigned long ENUM;

typedef struct {
  unsigned char headerLength;
  unsigned short messageId;
  char messageType;
  unsigned char portAddr;
  unsigned short messageLength;
  unsigned short sequence;
  unsigned char idleTime;
  unsigned char timeStatus;
  unsigned short week;
  GPSec ms;
  unsigned long rxStatus;
  unsigned short reserved;
  unsigned short rxVersion;
} Header;

// The Arduino's hardware serial is already occupied by the USB cable.
// This uses some digital pins on the Arduino to get a second serial.
SoftwareSerial gpsSerial(PIN_RX, PIN_TX);

// setup is run once when the Arduino boots
void setup() {
  // open Serial communication with the computer
  Serial.begin(9600);
  while (!Serial);

  Serial.println("connected");

  // open Serial communication with the receiver
  gpsSerial.begin(9600);

  // request a repeated log
  // remember to use bestposb (binary) and not bestposa (ASCII)
  gpsSerial.println("interfacemode com1 novatel novatelbinary on");
  gpsSerial.println("unlogall");
  gpsSerial.println("log timeb ontime 1");
//  gpsSerial.println("log bestposb ontime 1");
//  gpsSerial.println("log bestxyzb ontime 1");
}

/**
   Reads serial bytes until they match with the 3 sync bytes which designate
   the start of a header (0xAA, 0x44, 0x12).
*/
void sync() {
  char buf[3];
  while (buf[0] != '\xAA' || buf[1] != '\x44' || buf[2] != '\x12') {
    if (gpsSerial.available()) {
      buf[0] = buf[1];
      buf[1] = buf[2];
      buf[2] = gpsSerial.read();
    }
  }
}

/**
   Reads a header and fills the buffer with the log that follows.
   Returns the messageID for the log, or 0 if the message length exceeds the
   max buffer size.
   See page 23 of the firmware reference for header details.
*/
unsigned short nextMessage() {
  // read until we reach the start of a header (just past the sync bits)
  sync();

  // read in the header and get messageId
  Header *header = malloc(sizeof(Header));
  readThing(&gpsSerial, header, sizeof(Header));
  unsigned short messageId = header->messageId;
  free(header);

  // read the response ID and check if the command was received correctly
  ENUM responseId = readEnum(&gpsSerial);
  if (responseId != RESPONSE_ID_OK) {
    Serial.print("bad response id: ");
    Serial.println(responseId);
    return 0; // error code
  }

  return messageId;
}

/**
   Runs repeatedly after setup is called
*/
void loop() {
  if (gpsSerial.available()) {
    // read in the next message and parse according to the type of message
    unsigned short messageID = nextMessage();
    switch (messageID) {
      case ID_BESTPOS:
        parseBestPos(&gpsSerial);
        break;
      case ID_BESTXYZ:
        parseBestXYZ(&gpsSerial);
        break;
      case ID_TIME:
        parseTime(&gpsSerial);
        break;
      case 0: // error case
        return -1;
      default:
        Serial.print("unimplemented messageID: ");
        Serial.println(messageID);
        return -1;
    }
  }
}

