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
int checkTimer(TIMER_HANDLE timerNumber);
void endPollingTimer(TIMER_HANDLE timerNumber);

int timerCallbackInitializer(void (*waitFunc)(), uint32_t us);
int timerCallbackInitializer2(uint16_t ms);
void startCallback(TIMER_HANDLE n);
void stopCallback(TIMER_HANDLE n);

#endif /* TIMER_H_ */
