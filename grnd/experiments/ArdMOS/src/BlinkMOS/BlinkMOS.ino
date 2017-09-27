/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

struct soh_t
{
  byte length;
  byte id;
  byte delay;
};

soh_t mySoh;

// the setup function runs once when you press reset or power the board
void setup() {
  mySoh.id = 1;
  mySoh.delay = 250;
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  processCmds();
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(mySoh.delay);                      
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(mySoh.delay);
  sendTlm();                   
}

// TODO:  Obviously, we only handle one command type currently ... 
void processCmds()
{
  while (Serial.available())
  {
    mySoh.delay = Serial.read();
  }
}

void sendTlm()
{
  mySoh.length = sizeof(mySoh);
  writeTlm( (const char*) &mySoh, sizeof(mySoh) );
}

void writeTlm( const char* pkt, byte size)
{
  for (int i=0; i < size; i++)
  {
    Serial.write(pkt[i]);
  }
}


