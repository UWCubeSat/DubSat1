#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

/*
 * Basic test to observe stack usage.
 * DEBUG is turned off to more closely control the stack
 *
 * TODO does setting the max stack size in the linker options have any effect?
 * TODO does the "breakpoint on stack overflow" tool actually work?
 */

#include <msp430.h> 
#include <stdint.h>

#include "bsp/bsp.h"

#include "stack_checker.h"

hBus handle;

void logstack()
{
    uint16_t used = stackMinFreeCount();
    uint16_t total = stackMaxSize();

    // put a breakpoint here to read the variables
    LED_OUT |= LED_BIT;
}

void littleStack()
{
    uint8_t bytes[20];
    bytes[0] = 0;
    logstack();
}

void bigStack()
{
    uint8_t bytes[400];
    bytes[0] = 0;
    logstack();
}

int main(void)
{
    bspInit(__SUBSYSTEM_MODULE__);

    littleStack();
    bigStack();
    return 0;
}
