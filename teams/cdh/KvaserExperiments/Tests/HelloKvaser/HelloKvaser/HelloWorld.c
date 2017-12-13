#include <stdio.h>
#include "canlib.h"



//Checks if the status is an error code and displays its error message
void Check(char* id, canStatus stat)
{
  if (stat != canOK) {
    char buf[50];
    buf[0] = '\0';
    canGetErrorText(stat, buf, sizeof(buf));
    printf("%s: failed, stat=%d (%s)\n", id, (int)stat, buf);
  }
}

void main(int argc, char* argv[]){
  //Holds a handle to the CAN channel
  canHandle hnd;
  //Status returned by the Canlib calls
  canStatus stat;

  int Bitrate = canBITRATE_250K;
  char msg[8] = {1,2,3,4,5,6,7,8};

  canInitializeLibrary();
  printf("Opening channel 0\n");
  hnd = canOpenChannel(0, 0);
  if(hnd < 0){
    Check("canOpenChannel", hnd);
    exit(1);
  }

  printf("Setting bitrate and going bus on\n");
  stat = canSetBusParams(hnd, Bitrate, 0, 0, 0, 0, 0);
  Check("canSetBusParams", stat);
  stat = canBusOn(hnd);
  Check("canBusOn", stat);

  printf("Writing a message to the channel and waiting for it to be sent \n");
  stat = canWrite(hnd, 123, msg, 8, 0);
  Check("canWrite", stat);
  stat = canWriteSync(hnd, 100);
  Check("canWriteSync", stat);

  printf("Going off bus and closing channel");
  stat = canBusOff(hnd);
  Check("canBusOff", stat);
  stat = canClose(hnd);
  Check("canClose", stat);

}
