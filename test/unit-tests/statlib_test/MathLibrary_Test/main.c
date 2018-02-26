#include <msp430.h> 
#include "TestingCode.h"

/**
 * main.c
 */
int main()
{
    test_initialization();
    test_adding_one_value();
    test_fill_array();
    test_reset();
    test_overloading_array();
    test_not_full_array_with_same_numbers();
    test_not_full_array_with_diff_numbers();
    test_not_full_array_with_diff_numbers_with_reset();
    test_overloading_array_with_reset();
    test_multiple_handles();
    test_1_before_overflow();
    test_overflow_uint16();
    test_resetAvg_and_resetMinMax();
    test_resetAvg_and_resetMinMax();

}
