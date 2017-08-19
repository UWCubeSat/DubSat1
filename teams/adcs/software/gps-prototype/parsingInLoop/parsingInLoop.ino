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

/*
  The maximum number of bytes in a message this program is allowed to read
  before it gives up. Some messages may be longer than 76 bytes.
*/
#define MAX_BUFFER 76

// The log ID numbers for the logs we know how to parse.
// See page 364 of the firmware reference for a table of ID numbers.
#define ID_BESTPOS 42
#define ID_BESTXYZ 93
#define ID_TIME 101

// The Arduino's hardware serial is already occupied by the USB cable.
// This uses some digital pins on the Arduino to get a second serial.
SoftwareSerial gpsSerial(PIN_RX, PIN_TX);

// buf is the byte array that gpsSerial will read into.
char buf[MAX_BUFFER];

// setup is run once when the Arduino boots
void setup() {
  // reset the GPS receiver
//  boot();

  // open Serial communication with the computer
  Serial.begin(9600);
  while (!Serial);

  Serial.println("we up");

  // open Serial communication with the receiver
  gpsSerial.begin(9600);

  // request a repeated log
  // remember to use bestposb (binary) and not bestposa (ASCII)
  gpsSerial.println("log bestposb ontime 1");
}

/**
   This function resets the receiver. The receiver must be reset after it is
   powered on.
*/
void boot() {
  /*
     To reset the receiver, its RESET pin must be set to LOW for > 150 ms,
     then let it float during normal operation. The Arduino will accomplish this
     using a mosfet transistor.
  */
  // assuming N channel
  pinMode(PIN_RESET, OUTPUT);
  digitalWrite(PIN_RESET, HIGH); // flow -- sets reset to low
  delay(200); // low for > 150 ms
  digitalWrite(PIN_RESET, LOW); // stop flow -- sets reset to floating
  // TODO consider adding another delay to let it finish booting?
}

/**
   Reads serial bytes until they match with the 3 sync bytes which designate
   the start of a header (0xAA, 0x44, 0x12).
*/
void sync() {
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
  gpsSerial.readBytes(buf, 1);
  unsigned char headerLength = buf[0];

  // read in the rest of the header
  // using headerLength - 4 because it already read 3 sync bits + the length bit
  gpsSerial.readBytes(buf, headerLength - 4);

  // parse the message ID for the log
  unsigned short messageID = *((unsigned short*) buf);

  // parse the message length
  unsigned short messageLength = *((unsigned short*) (buf + 4));
  if (messageLength > MAX_BUFFER) {
    // use 0 for error reporting because 0 is not a valid message ID.
    // See page 364 of the firmware reference for message IDs.
    return 0;
  }

  // read the message into the buffer
  gpsSerial.readBytes(buf, messageLength);

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
        parseBestpos();
        break;
      case ID_BESTXYZ:
        parseBestxyz();
        break;
      case ID_TIME:
        parseTime();
        break;
      case 0: // nextMessage error
        Serial.println("message size exceeded limits");
        return -1;
      default:
        Serial.print("unimplemented messageID: ");
        Serial.println(messageID);
        return -1;
    }
  }
}

