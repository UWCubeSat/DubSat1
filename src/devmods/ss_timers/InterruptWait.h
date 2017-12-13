#include <msp430.h>
#include <stdint.h>

//calls *waitFunc* after *timerVal* timer ticks (1000 ms =
void createInterrupt(void (*waitFunc)(), uint16_t timerVal);

//calls *waitFunc* after *timerVal* timer ticks and *countVal* timer overflows (
void createInterruptWithOverflow(void (*waitFunc)(), uint16_t timerVal, int countVal);

//calls *waitFunc* after *duration* ms
void wait(void (*waitFunc)(), uint32_t duration);
