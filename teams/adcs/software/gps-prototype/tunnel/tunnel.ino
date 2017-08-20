/*
   David Dupre
   djdupre@uw.edu

   This program pipes data between the Arduino's hardware serial and a software 
   serial. This can be used to send messages to and read from the GPS receiver.
*/

#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

SoftwareSerial mySerial(rxPin, txPin);

void setup() {
  Serial.begin(9600);
  while(!Serial);

  mySerial.begin(9600);
}

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if(Serial.available()) {
     mySerial.write(Serial.read());
  }
}
