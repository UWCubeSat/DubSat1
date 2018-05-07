/*
 * timer.h
 *
 *  Created on: Jan 15, 2018
 *      Author: Thu Phan, Nathan Wacker
 */
#include <stdint.h>

#ifndef TIMER_H_
#define TIMER_H_

typedef uint8_t TIMER_HANDLE;

void initializeTimer();
int timerPollInitializer(uint16_t ms);
int checkTimer(uint16_t timerNumber);
void endPollingTimer(uint16_t timerNumber);

int timerCallbackInitializer(void (*waitFunc)(), uint32_t us);
int timerCallbackInitializer2(uint16_t ms);
void startCallback(unsigned int n);
void stopCallback(unsigned int n);

#endif /* TIMER_H_ */
