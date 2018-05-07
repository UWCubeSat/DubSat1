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
TIMER_HANDLE timerPollInitializer(uint16_t ms);
int checkTimer(TIMER_HANDLE timerNumber);
void endPollingTimer(TIMER_HANDLE timerNumber);

TIMER_HANDLE timerCallbackInitializer(void (*waitFunc)(), uint32_t us);
void startCallback(TIMER_HANDLE n);
void stopCallback(TIMER_HANDLE n);

#endif /* TIMER_H_ */
