/*
 * general_timer.h
 *
 *  Created on: Nov 1, 2017
 *      Author: Thu Phan
 */

#ifndef GENERAL_TIMER_H_
#define GENERAL_TIMER_H_
#include <stdint.h>

uint64_t checkTime_microSeconds();

uint64_t checkTime_seconds();

uint64_t setTimertoZero();

uint64_t checkTimer(uint64_t startTime);


#endif /* GENERAL_TIMER_H_ */
