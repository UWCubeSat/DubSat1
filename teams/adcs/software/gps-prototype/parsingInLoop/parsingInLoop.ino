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
#define PIN_RESET 2 // reset pin

// The log ID numbers for the logs we know how to parse.
// See page 364 of the firmware reference for a table of ID numbers.
#define ID_BESTPOS 42
#define ID_BESTXYZ 241
#define ID_RXSTATUS 93
#define ID_TIME 101

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

  // check the header length
  unsigned char headerLength = readByte(&gpsSerial);
  
  // parse the message ID for the log
  unsigned short messageID = readShort(&gpsSerial);

  // skip the rest of the header
  // using headerLength - 6 because it already read 3 sync bits + the length bit + 2 messageID bits
  skip(&gpsSerial, headerLength - 6);

  return messageID;
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
        parseBestpos(&gpsSerial);
        break;
      case ID_BESTXYZ:
        parseBestxyz(&gpsSerial);
        break;
      case ID_TIME:
        parseTime(&gpsSerial);
        break;
      default:
        Serial.print("unimplemented messageID: ");
        Serial.println(messageID);
        return -1;
    }
  }
}

