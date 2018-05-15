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

typedef struct
{
    TIMER_HANDLE timer_id;
    uint16_t inUse;
    uint16_t start_timer_counter;
    uint16_t start_TAR;
    uint16_t counter_dif;
    uint16_t tar_dif;
} user_timer_polling_info;


typedef struct
{
    uint16_t inUse;
    uint16_t count;
    uint16_t current_count;
    uint16_t tar;
    uint16_t user_id;
    void (*fxPtr)();
} user_timer_callback_info;

void initializeTimer();
TIMER_HANDLE timerPollInitializer(uint16_t ms);
int checkTimer(TIMER_HANDLE timerNumber);
void endPollingTimer(TIMER_HANDLE timerNumber);

TIMER_HANDLE timerCallbackInitializer(void (*waitFunc)(), uint32_t us);
void startCallback(TIMER_HANDLE n);
void stopCallback(TIMER_HANDLE n);

#endif /* TIMER_H_ */
