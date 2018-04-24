#ifndef MET_H
#define MET_H

//used to get and set MET
typedef struct timeStamp{
	uint8_t count1;
	uint8_t count2;
	uint8_t count3;
	uint8_t count4;
	uint8_t count5;
} timeStamp;

void METInit(uint8_t _isDist);

//returns the current MET timeStamp
timeStamp getTimeStamp();

// returns the current MET in units of 2^-8 seconds
uint64_t getTimeStampInt();

// returns the current MET in seconds
double getTimeStampSeconds();

//returns if MET has been confirmed
uint8_t isConfirmed();

//updates MET to timeStamp and confirms
void updateMET(timeStamp newTime);

#endif
