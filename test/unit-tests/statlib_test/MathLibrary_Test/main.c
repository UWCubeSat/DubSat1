#include <msp430.h> 
#include "TestingCode.h"

/**
 * main.c
 */
int main()
{
    test_1_before_overflow();
    test_overflow_uint16();
    test_resetAvg_and_resetMinMax();
    test_resetAvg_and_resetMinMax();

}
