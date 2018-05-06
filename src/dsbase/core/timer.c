/**
 * timer.c
 *
 *  Created on: Jan 13, 2018
 *      Author: Thu Phan, Nathan Wacker
 *
 *  Description: This source file contains two ways to implement the timer.
 *  1. Interrupt based / Callback timer
 *  2. Non-interrupt based / Polling-based timer
 *  This timer uses Timer A0.
 *
 *  Procedure to use non-interrupt (polling) based timer:
 *      1. Initialize timer with the function initalizeTimer()
 *      2. When user wants to start timer, call the function timerPollbackInitializer(). It has 1 parameter,
 *         the desired milliseconds user wants to set the timer at. Function will return a timer identification number.
 *      3. Call on checkTimer(identification number). Will return 1 when timer goes off, 0 otherwise.
 *      4. To start timer again, repeat step 2-4.
 *      Ex:
 *          initializeTimer();
 *          int timerID = timerPollbackInitializer(2, 0); // 4 seconds timer
 *          while(!checkTimer(timerID)) {}  // stays in loop till checkTimer returns 1
 *          LED1 ^= 1; // Do whatever
 *      5. To end timer before checkTimer() returns 1, call on endPollingTimer(uint16_t timerNumber).
 *
 *
 *  Procedure to use interrupt (callback) based timer:
 *      1. Initialize timer with the function initializeTimer()     // NOTE: initializeTimer should only be called ONCE in main loop
 *      2. Call on timerCallbackInitializer(). Function has two parameters: A pointer to desired function, and "us", the number of
 *         desired microseconds. Function will return a timer identification number.
 *      3. To start timer, call on startCallback(timer ID). When timer goes off, interrupt will automatically execute function that the function pointer
 *         points to. Function will continuously be called every desired "us".
 *      4. To stop calling timer from resetting and calling the function, call on stopCallback(timer ID).
 *      Ex:
 *          initializeTimer();
 *          int timerID = timerCallbackInitializer(&blinkLED, 2000); // timer will call on blinkLED every 2000 us.
 *          startCallback(timerID);
 *          ...// Do whatever you want
 *          // When you want to stop blinkLED;
 *          stopCallback(timerID);
 *
 *          void blinkLED()
 *          {
 *              blink the LED;
 *          }
 */
#include <msp430.h>
#include "timer.h"
#include <stdint.h>

static const int NUM_SUPPORTED_DURATIONS_POLLING = 8;
static const int NUM_SUPPORTED_DURATIONS_CALLBACK = 2;


typedef struct
{
    uint16_t inUse;
    uint16_t durationMS;   // Always indicate what the units are in struct names
    uint16_t start_timer_counter;
    uint16_t start_TAR;
    uint16_t counter_dif;
    uint16_t tar_dif;
} polling_info;

typedef struct
{
    uint16_t inUse;
    uint16_t count;
    uint16_t current_count;
    uint16_t tar;
    void (*fxPtr)();
} callback_info;

typedef struct
{
    uint16_t counter;
    uint16_t TARval;
} desired_time;

static polling_info polling[NUM_SUPPORTED_DURATIONS_POLLING];
static int initialized = 0;

/* IMPORTANT: callback[1] == TA0CCTL2, callback[0] == TA0CCTL1 */
static callback_info callback[NUM_SUPPORTED_DURATIONS_CALLBACK];
static uint16_t timer_counter = 0;


void initializeTimer()
{
    if(initialized)
    {
        return;
    }
//    // ACLK has frequency of 32768 Hz, which means one "tick" is .00003051757 s = 30.51757 us
    TA0CTL = TASSEL__ACLK | TAIE | MC__CONTINUOUS | ID__1;
    __bis_SR_register(GIE);
    uint16_t i;
    for (i = 0; i < NUM_SUPPORTED_DURATIONS_POLLING; i++)
    {
        polling[i].inUse = 0;
        polling[i].durationMS = 0;
        polling[i].start_timer_counter = 0;
        polling[i].start_TAR = 0;
        polling[i].counter_dif = 0;
        polling[i].tar_dif = 0;
    }
    for (i = 0; i < NUM_SUPPORTED_DURATIONS_CALLBACK; i++)
    {
        callback[i].inUse = 0;
        callback[i].count = 0;
        callback[i].current_count = 0;
        callback[i].tar = 0;
//        callback[i].fxPtr = 0; // TODO: Ask what to set to
    }
    initialized = 1;
}

desired_time convertTime(uint16_t ms)
{
    // 1 desired_counter_dif = 2 seconds; 1 desired_TAR_dif = 1/32768 = 30.5us.
    desired_time convert;
    uint16_t calc_counter;
    uint16_t calc_TAR;
    float microSec;
    uint16_t new_ms = ms;
    calc_counter = new_ms / 2000;
    new_ms = new_ms - calc_counter * 2000;
    // ms --> us
    microSec = (float) new_ms * 1000.0;
    calc_TAR = (uint16_t) (microSec / 30.517);
    convert.counter = calc_counter;
    convert.TARval = calc_TAR;
    return convert;
}

int timerPollInitializer(uint16_t ms)
{
    desired_time convert = convertTime(ms);
    uint16_t start_counter = timer_counter;
    uint16_t start_TAR_ = TA0R;
    int i;
    for (i = NUM_SUPPORTED_DURATIONS_POLLING - 1; i >= 0; i--)
    {
        if (!polling[i].inUse)
        {
            polling[i].inUse = 1;
            polling[i].counter_dif = convert.counter;
            polling[i].tar_dif = convert.TARval;
            polling[i].start_timer_counter = start_counter;
            polling[i].start_TAR = start_TAR_;
            return i;
        }
    }
    return -1;
}

int timerCallbackInitializer(void (*waitFunc)(), uint32_t us)
{
    uint32_t newTime = us / 30.517578125;           //(ms * 1.00150225338) - 40; //TODO: check this factor
    uint16_t overflows = 0;
    while (newTime > 65535)
    {
        newTime -= 65535;
        overflows++;
    }
    uint16_t i;
    for (i = 0; i < 2; i++)
    {
        if (!callback[i].inUse)
        {
            callback[i].inUse = 1;
            callback[i].count = overflows;
            callback[i].current_count = overflows;
            callback[i].tar = newTime;
            callback[i].fxPtr = waitFunc;
            return i;
        }
    }
    return -1;
}

/** \fn startCallback() starts callback for specified interrupt
 * \param n the interrupt to start
 * \return 1 if interrupt is used, otherwise 0; if interrupt is used, does not override previously set value
 */
void startCallback(unsigned int n)
{
    uint16_t tarDiff = 0;
    if(65535 - callback[n].tar < TA0R)
    {
        tarDiff = callback[n].tar - 65535 + TA0R;
        callback[n].current_count = callback[n].count + 1;
    }
    else
    {
        callback[n].current_count = callback[n].count;
        tarDiff = callback[n].tar + TA0R;
    }

    if(n == 1)
    {
        TA0CCR2 = tarDiff;
        TA0CCTL2 = CCIE;
    } else if (n == 0)
    {
        TA0CCR1 = tarDiff;
        TA0CCTL1 = CCIE;
    }
}

/** \fn stopCallback() stops callback for specified interrupt
 * \param n the interrupt to stop
 * also clears the callback values set in setCallback
 */
void stopCallback(unsigned int n)
{

    //TODO: don't enable CCIE if it is already disabled
    if(n == 0)
    {
        TA0CCTL1 &= ~CCIE;
    }
    else if(n == 2)
    {
        TA0CCTL2 &= ~CCIE;
    }
    callback[n].inUse = 0;
    callback[n].count = 0;
    callback[n].current_count = 0;
    callback[n].tar = 0;
    callback[n].fxPtr = 0;
}


int checkTimerOverflow(uint16_t timerNumber, uint16_t end_counter, uint16_t end_TAR)
{
    uint16_t calc_tar;
    uint16_t calc_counter = 65535 - polling[timerNumber].start_timer_counter + end_counter;
    if (end_TAR >= polling[timerNumber].start_TAR)
    {
        calc_tar = end_TAR - polling[timerNumber].start_TAR;
    } else
    {
        if (calc_counter == 0)
        {
            return 0;
        }
        calc_counter--;
        calc_tar = 65535 - polling[timerNumber].start_TAR + end_TAR;
    }
    if(calc_counter > polling[timerNumber].counter_dif)
    {
        polling[timerNumber].inUse = 0;
        return 1;
    }
    if(calc_counter == polling[timerNumber].counter_dif && calc_tar >= polling[timerNumber].tar_dif)
    {
        polling[timerNumber].inUse = 0;
        return 1;
    }
    return 0;
}

int checkTimer(uint16_t timerNumber)
{
    if(!checkValidPollingID(timerNumber))
    {
        return 0;
    }
    uint16_t end_counter = timer_counter;
    uint16_t end_TAR = TA0R;

    // timer overflow already happened, will deal with this case later
    if(end_counter < polling[timerNumber].start_timer_counter) {
        checkTimerOverflow(timerNumber, end_counter, end_TAR);
    }

    uint16_t calc_tar;
    uint16_t calc_counter = end_counter - polling[timerNumber].start_timer_counter;
    if (end_TAR >= polling[timerNumber].start_TAR)
    {
        calc_tar = end_TAR - polling[timerNumber].start_TAR;
    } else
    {
        if (calc_counter == 0)
        {
            return 0;
        }
        calc_counter--;
        calc_tar = 65535 - polling[timerNumber].start_TAR + end_TAR;
    }
    if(calc_counter > polling[timerNumber].counter_dif)
    {
        polling[timerNumber].inUse = 0;
        return 1;
    }
    if(calc_counter == polling[timerNumber].counter_dif && calc_tar >= polling[timerNumber].tar_dif)
    {
        polling[timerNumber].inUse = 0;
        return 1;
    }
    return 0;
}

void endPollingTimer(uint16_t timerNumber)
{
    polling[timerNumber].inUse = 0;
}

int checkValidPollingID(uint16_t timerNumber)
{
    if(timerNumber > NUM_SUPPORTED_DURATIONS_POLLING || timerNumber < 0)
    {
        return 0;
    }
    if(!polling[timerNumber].inUse)
    {
        return 0;
    }
    return 1;
}

int checkValidCallbackID(uint16_t timerNumber)
{
    if (timerNumber > NUM_SUPPORTED_DURATIONS_CALLBACK || timerNumber < 0)
    {
        return 0;
    }
    if (!callback[timerNumber].inUse)
    {
        return 0;
    }
    return 1;
}


#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void)
{
    switch(__even_in_range(TA0IV, TAIV__TAIFG))
    {
        case TAIV__NONE:   break;           // No interrupt
        case TAIV__TACCR1:
            if(callback[0].current_count <= 0)
            {
                startCallback(0);
                callback[0].fxPtr();

            }
            break;           // CCR1 not used
        case TAIV__TACCR2:
            if(callback[1].current_count <= 0)
            {
                startCallback(1);
                callback[1].fxPtr();
            }
            break;           // CCR2 not used]
        case TAIV__TACCR3: break;           // reserved
        case TAIV__TACCR4: break;           // reserved
        case TAIV__TACCR5: break;           // reserved
        case TAIV__TACCR6: break;           // reserved
        case TAIV__TAIFG:                   // overflow
            timer_counter++;
            uint16_t i;
            for(i = 0; i < NUM_SUPPORTED_DURATIONS_CALLBACK; i++)
            {
                if(callback[i].current_count > 0)
                    callback[i].current_count--;
            }
            break;
        default: break;
    }
}



