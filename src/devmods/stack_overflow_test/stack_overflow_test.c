#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

/*
 * Basic test to observe stack usage.
 * DEBUG is turned off to more closely control the stack.
 *
 * Use "breakpoint on stack overflow" and refresh it after changing the stack
 * size in the linker options.
 */

#include <msp430.h> 
#include <stdint.h>

#include "bsp/bsp.h"

#include "stack_checker.h"

hBus handle;

void logstack()
{
    uint16_t unused = stackMinFreeCount();
    uint16_t total = stackMaxSize();

    // doing an AND so the variables don't get optimized out
    if (unused && total)
    {
        // put a breakpoint here to read the variables
        LED_OUT |= LED_BIT;
    }
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
    logstack();
    littleStack();
    bigStack();
    return 0;
}
