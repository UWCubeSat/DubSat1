#ifndef MET_H
#define MET_H

typedef struct timeStamp{
	uint8_t count1;
	uint8_t count2;
	uint8_t count3;
	uint8_t count4;
	uint8_t count5;
} timeStamp;

void METInit();
timeStamp getTimeStamp();

#endif
