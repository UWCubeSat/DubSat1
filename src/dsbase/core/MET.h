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

//returns the first four counter registers as a 32-bit integer
uint32_t getPrimaryTime();

//returns the overflow time as an 8-bit integer
uint8_t getOverflowTime();

//returns a timestamp for the given primary and overflow time
timeStamp constructTimestamp(uint32_t primary, uint8_t overflow);

//updates MET to timeStamp and confirms
void updateMET(timeStamp newTime);

#endif
