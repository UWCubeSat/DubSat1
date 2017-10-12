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

#define MODE_ON 1
#define MODE_OFF 0

#define OPCODE_DELAY 1
#define OPCODE_MODE  2 

#define MINDELAY  15

struct soh_t
{
  byte length;
  byte id;
  byte delay;
  byte mode;

  byte cmds_accepted;
  byte cmds_received;
};

soh_t mySoh;

// the setup function runs once when you press reset or power the board
void setup() {
  mySoh.id = 1;
  mySoh.delay = 250;
  mySoh.mode = MODE_ON;
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  processCmds();
  if (mySoh.mode == MODE_ON) 
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(mySoh.delay);                      
  if (mySoh.mode == MODE_ON) 
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(mySoh.delay);
  sendTlm();                   
}

// TODO:  Obviously, we only handle one command type currently ... 
void processCmds()
{
  byte opcode;
  byte param;
  
  bool recOne = false;
  
  while (Serial.available())
  {
    opcode = Serial.read();
    switch (opcode)
    {
      case OPCODE_DELAY:
        while (!Serial.available()) {}
        param = Serial.read();
        param = (param < MINDELAY) ? MINDELAY : param;
        mySoh.delay = param;
        recOne = true;
        mySoh.cmds_received++;
        break;
      case OPCODE_MODE:
        while (!Serial.available()) {}
        mySoh.mode = Serial.read();
        recOne = true;
        mySoh.cmds_received++;
        break;
      default:
        break;  
    }    
  }

  if (recOne==true)
  {
    mySoh.cmds_accepted++;
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


