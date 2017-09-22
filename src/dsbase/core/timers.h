/*
 * timers.h
 *
 *  Created on: Aug 22, 2017
 *      Author: jeffc
 */

#ifndef DSBASE_CORE_TIMERS_H_
#define DSBASE_CORE_TIMERS_H_

#include <stdint.h>

#define SEC   8000000   // Ideal number of cycles/second

typedef uint32_t met_time_s;  // = 136.2 years before rollover
typedef uint64_t met_time_ms; // = half a billion years before rollover

uint32_t timerCycleDiff16(uint16_t earlierTime, uint16_t laterTime);

#endif /* DSBASE_CORE_TIMERS_H_ */
