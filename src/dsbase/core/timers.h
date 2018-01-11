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
#define MSEC  8000      // Idealy number of cycles/millisecond

typedef uint32_t met_time_s;  // = 136.2 years before rollover
typedef uint64_t met_time_ms; // = half a billion years before rollover

uint32_t timerCycleDiff16(uint16_t earlierTime, uint16_t laterTime);

// This function serves to both encapsulate ms->cycles conversion, and because __delay_cycles()
// can only take a constant (determinable at compile time)
void spinWait_ms(uint16_t ms);

// Mission Elapse Timer (MET) management and usage functions



#endif /* DSBASE_CORE_TIMERS_H_ */
