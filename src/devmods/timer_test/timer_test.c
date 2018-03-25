#define MAX_TIMERS 8

#define LED_DIR P1DIR
#define LED_OUT P1OUT
#define LED_BIT BIT0

#include <msp430.h> 

#include "bsp/bsp.h"

#include "core/timer.h"
#include "core/debugtools.h"

void assertTrue(uint8_t value, const char *msg)
{
    if (!value)
    {
        debugPrintF(msg);
    }
}

void assertEqual(int actual, int expected, const char *msg)
{
    if (expected != actual)
    {
        debugPrintF(msg);
        debugPrintF("\texpected: %d\r\n", expected);
        debugPrintF("\tactual:   %d\r\n", actual);
    }
}

int main(void)
{
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>

    LED_DIR |= LED_BIT;

    initializeTimer();

    // basic timer test
    int t1 = timerPollInitializer(500);
    assertTrue(t1 != -1, "timer init failed\r\n");
    assertEqual(t1, MAX_TIMERS - 1, "unexpected handle value\r\n");

    // end timer test
    endPollingTimer(t1);
    t1 = timerPollInitializer(500);
    assertTrue(t1 != -1, "second timer init failed\r\n");
    assertEqual(t1, MAX_TIMERS - 1, "free timer handle wasn't used\r\n");

    // check timer test
    while (!checkTimer(t1));
    t1 = timerPollInitializer(500);
    assertTrue(t1 != -1, "third timer init failed\r\n");
    assertEqual(t1, MAX_TIMERS - 1, "checkTimer did not free previous timer\r\n");

    endPollingTimer(t1);

    // fill up polling timers
    int handles[10];
    uint8_t i = 0;
    do
    {
        handles[i] = timerPollInitializer(500);
    } while (handles[i++] != -1);
    assertEqual(i - 1, MAX_TIMERS, "wrong number of max timers\r\n");

    // check all the timers
    while (i-- > 0)
    {
        while (!checkTimer(handles[i]));
    }

    // try multiple different timers with resets
    t1 = timerPollInitializer(2000);
    uint8_t j;
    for (j = 0; j < 10; j++)
    {
        int t = timerPollInitializer(100);
        assertTrue(!checkTimer(t1), "timer ended prematurely (before endPollingTimer)\r\n");
        endPollingTimer(t);
        assertTrue(!checkTimer(t1), "timer ended prematurely (after endPollingTimer)\r\n");
    }
    while (!checkTimer(t1));

    // try other stuff I guess
    t1 = timerPollInitializer(2000);
    uint8_t k;
    for (k = 0; k < 10; k++)
    {
        int t = timerPollInitializer(100);
        assertTrue(!checkTimer(t1), "timer ended prematurely (before checkTimer)\r\n");
        while (!checkTimer(t));
        assertTrue(!checkTimer(t1), "timer ended prematurely (after checkTimer)\r\n");
    }
    while (!checkTimer(t1));

    // long timer test
    t1 = timerPollInitializer(3000);
    int t2 = timerPollInitializer(1000);
    while (!checkTimer(t2)) {
        assertTrue(!checkTimer(t1), "longer timer ended first");
    }
    while (!checkTimer(t1));

    LED_OUT |= LED_BIT;
    debugPrintF("test complete\r\n");

	return 0;
}
