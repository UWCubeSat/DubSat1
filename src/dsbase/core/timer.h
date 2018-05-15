/*
 * timer.h
 *
 *  Created on: Jan 15, 2018
 *      Author: Thu Phan, Nathan Wacker
 */
#include <stdint.h>

#ifndef TIMER_H_
#define TIMER_H_

typedef int8_t TIMER_HANDLE;

/*********** Defines the number of polling/callback timer supported by the timer library***********/
#define NUM_SUPPORTED_DURATIONS_POLLING   8
#define NUM_SUPPORTED_DURATIONS_CALLBACK  2
/*************************************************************************************************/


typedef struct
{
    TIMER_HANDLE timer_id;
    uint16_t user_id;
    uint8_t inUse;
    uint16_t start_timer_counter;
    uint16_t start_TAR;
    uint16_t counter_dif;
    uint16_t tar_dif;
} user_timer_polling_info;

typedef struct
{
    TIMER_HANDLE timer_id;
    uint8_t inUse;
    uint16_t count;
    uint16_t current_count;
    uint16_t tar;
    uint16_t user_id;
} user_timer_callback_info;

void initializeTimer();
TIMER_HANDLE timerPollInitializer(uint16_t ms);
int checkTimer(TIMER_HANDLE timerNumber);
void endPollingTimer(TIMER_HANDLE timerNumber);
TIMER_HANDLE timerCallbackInitializer(void (*waitFunc)(), uint32_t us);
void startCallback(TIMER_HANDLE n);
void stopCallback(TIMER_HANDLE n);

//user_timer_polling_info getPollingTimerInfo(uint16_t user_id);
//user_timer_callback_info getCallbackTimerInfo(uint16_t user_id);
TIMER_HANDLE timerCallbackInitializer_userID(void (*waitFunc)(), uint32_t us, uint16_t user_id);
TIMER_HANDLE timerPollInitializer_userID(uint16_t ms, uint16_t user_id);
void get_polling_timer_info(user_timer_polling_info * user_timer_info);
void get_callback_timer_info(user_timer_callback_info * user_timer_info);
#endif /* TIMER_H_ */
