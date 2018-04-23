/*
 * Copied from https://e2e.ti.com/support/development_tools/compiler/f/343/t/580473
 */

#include <stdint.h>
#include "stack_checker.h"

#if defined(STACK_CHECK)
#define STACK_INIT  0xBEEF // Pattern to use to initially poison the stack
extern uint16_t  _stack;   // Start of stack (low address)
#endif

#if defined(__cplusplus)
extern "C"
{
#endif
#if defined(__TI_COMPILER_VERSION__) || \
    defined(__GNUC__)
int _system_pre_init( void )
#elif defined(__IAR_SYSTEMS_ICC__)
int __low_level_init( void )
#endif
{
    //... stuff...

#if defined(STACK_CHECK)
    //
    // Poison the stack, word by word, with a defined pattern
    //
    // Note that _system_pre_init is the earliest that we can
    // do this and that it may not be possible in TI-RTOS
    //
    // When we call the __get_SP_register intrinsic (same on IAR & CCS), it will return the address
    // of the RET address for the caller of this routine. Make sure that we don't trash it!!
    //
    register uint16_t *stack = &_stack; // Address of lowest address in .stack section
    register uint16_t *stack_top = (uint16_t *) __get_SP_register();

    do {
        *stack++ = STACK_INIT;          // Poison stack addresses
    } while (stack < stack_top);     // Stop before top of stack to leave RET address
#endif

    return 1;
}
#if defined(__cplusplus)
}
#endif

#if defined(STACK_CHECK)
/**
 * Check how deep the stack usage has been
 *
 * \return  \c uint16_t     Minimum number of bytes to bottom of stack
 */

extern uint16_t __STACK_END;        // End of data
extern uint16_t __STACK_SIZE;       // Linker-set size of stack

uint16_t stackMinFreeCount(void)
{
    const uint16_t *stack = &_stack;
    uint16_t freeCount = 0;

    while (*stack == STACK_INIT && stack++ <= &__STACK_END)
    {
        freeCount++;
    }
    return freeCount << 1;
}

/**
 * Return size of C++ stack
 *
 * Set by the linker --stack_size option
 *
 * \return  \c uint16_t     Configued maximum size of the stack in bytes
 */
uint16_t stackMaxSize(void)
{
    return (uint16_t) _symval(&__STACK_SIZE);
}
#endif
