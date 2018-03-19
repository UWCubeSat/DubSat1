/*
 * timer.h
 *
 *  Created on: Jan 15, 2018
 *      Author: Thu Phan, Nathan Wacker
 */
#include <stdint.h>

#ifndef TIMER_H_
#define TIMER_H_

void initializeTimer();
int timerPollInitializer(uint16_t desired_counter_dif, uint16_t desired_TAR_dif);
int checkTimer(uint16_t timerNumber);
void endPollingTimer(uint16_t timerNumber);

int timerCallbackInitializer(void (*waitFunc)(), uint32_t us);
void startCallback(unsigned int n);
void stopCallback(unsigned int n);

#endif /* TIMER_H_ */
