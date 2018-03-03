#include "TestingCode.h"
/*#include "unity.h"*/



/*
	This file contains tests that will test the dataArray library.

*/

/*
	some tests were written by TinTin Patana-

*/

/* sometimes you may want to get at local data in a module.
 * for example: If you plan to pass by reference, this could be useful
 * however, it should often be avoided */

void setUp(void)
{
  /* This is run before EACH TEST */
}

void tearDown(void)
{
}

/*
	TODO: Write methods that will test
	The following methods will define tests
*/
/**
 * Function: test_initialization
 * Initializes a buffer and tests if the library initializes it properly.
 * This means that statistics should all be zero since no values have been added.
 *
 */
void test_initialization(void)
{
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	assert(0 == getAvg(array1));
	assert(0 == getSum(array1));
	assert(0 == getMin(array1));
	assert(0 == getMax(array1));
}

/**
 * Function: test_adding_one_value
 * Initializes a buffer and adds one value to buffer.
 *
 */
void test_adding_one_value(void)
{
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	addData(array1, (TYPE)5);
	assert(5 == getAvg(array1));
	assert(5 == getSum(array1));
	assert(5 == getMin(array1));
	assert(5 == getMax(array1));
}

/**
 * Function: test_fill_array
 * Initializes a buffer and fills the buffer with as many numbers as it can handle.
 * Tests to verify whether or not the buffer can accurately record statistics and manages the
 * numbers in an expected manner.
 *
 */
void test_fill_array(void)
{
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	addData(array1, (TYPE)5);
	addData(array1, (TYPE)11);
	addData(array1, (TYPE)544);
	addData(array1, (TYPE)245);
	addData(array1, (TYPE)123);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)324);
	addData(array1, (TYPE)89);
	addData(array1, (TYPE)53);
	addData(array1, (TYPE)20);
	assert(141 == getAvg(array1));
	assert(1417 == getSum(array1));
	assert(3 == getMin(array1));
	assert(544 == getMax(array1));
}

/**
 * Function: test_reset
 * Initializes a buffer and fills it with values, then performs resets.
 *
 */
void test_reset(void) {
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	/* Reset with no added values */
	resetAll(array1);
	assert(0 == getAvg(array1));
	assert(0 == getSum(array1));
	assert(0 == getMin(array1));
	assert(0 == getMax(array1));
	/* Reset with one added value */
	addData(array1, (TYPE)5);
	assert(5 == getAvg(array1));
	assert(5 == getSum(array1));
	assert(5 == getMin(array1));
	assert(5 == getMax(array1));
	/* Reset and then fill buffer */
	resetAll(array1);
	addData(array1, (TYPE)5);
	addData(array1, (TYPE)11);
	addData(array1, (TYPE)544);
	addData(array1, (TYPE)245);
	addData(array1, (TYPE)123);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)324);
	addData(array1, (TYPE)89);
	addData(array1, (TYPE)53);
	addData(array1, (TYPE)20);
	assert(141 == getAvg(array1));
	assert(1417 == getSum(array1));
	assert(3 == getMin(array1));
	assert(544 == getMax(array1));
	/* Reset then overflow buffer capacity */
	resetAll(array1);
	addData(array1, (TYPE)1);
	addData(array1, (TYPE)11);
	addData(array1, (TYPE)544);
	addData(array1, (TYPE)245);
	addData(array1, (TYPE)123);
	addData(array1, (TYPE)5);
	addData(array1, (TYPE)324);
	addData(array1, (TYPE)89);
	addData(array1, (TYPE)53);
	addData(array1, (TYPE)20);
	addData(array1, (TYPE)100);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)58);
	assert(1 == getMin(array1));
	assert(544 == getMax(array1));
	assert(102 == getAvg(array1));
	resetAll(array1);
	assert(0 == getAvg(array1));
	assert(0 == getSum(array1));
	assert(0 == getMin(array1));
	assert(0 == getMax(array1));

}

/**
 * Function: test_overloading_array
 * Initializes a buffer and fills the buffer with as many numbers as it can handle.
 * Tests to verify whether or not the buffer can accurately record statistics and manages the
 * numbers in an expected manner.
 *
 */
void test_overloading_array(void)
{
	/*Initializing array to be tested */
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	addData(array1, (TYPE)1);
	addData(array1, (TYPE)11);
	addData(array1, (TYPE)544);
	addData(array1, (TYPE)245);
	addData(array1, (TYPE)123);
	addData(array1, (TYPE)5);
	addData(array1, (TYPE)324);
	addData(array1, (TYPE)89);
	addData(array1, (TYPE)53);
	addData(array1, (TYPE)20);
	addData(array1, (TYPE)100);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)58);
	assert(1 == getMin(array1));
	assert(544 == getMax(array1));
	assert(102 == getAvg(array1));
}

/**
 * Function: test_not_full_array_with_same_numbers
 * Initializes a buffer and fills the buffer with as many numbers as it can handle.
 * Tests to verify whether or not the buffer can accurately record statistics and manages the
 * numbers in an expected manner.
 *
 */
void test_not_full_array_with_same_numbers(void)
{
	/* Initializing array to be tested */
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	addData(array1, (TYPE)2);
	addData(array1, (TYPE)2);
	addData(array1, (TYPE)2);
	addData(array1, (TYPE)2);
	addData(array1, (TYPE)2);
	assert(10 == getSum(array1));
	assert(2 == getMin(array1));
	assert(2 == getMax(array1));
	assert(2 == getAvg(array1));
}

/**
 * Function: test_not_full_array_with_same_numbers
 * Initializes a buffer and fills the buffer with as many numbers as it can handle.
 * Tests to verify whether or not the buffer can accurately record statistics and manages the
 * numbers in an expected manner.
 *
 */
void test_not_full_array_with_diff_numbers(void)
{
	/* Initializing array to be tested */
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	addData(array1, (TYPE)22);
	addData(array1, (TYPE)1000);
	addData(array1, (TYPE)513);
	addData(array1, (TYPE)3423);
	addData(array1, (TYPE)2);
	assert(2 == getMin(array1));
	assert((22 + 1000 + 513 + 3423 + 2) == getSum(array1));
	assert(3423 == getMax(array1));
	assert(992 == getAvg(array1));
}


/**
 * Function: test_not_full_array_with_diff_numbers_with_reset
 * Initializes a buffer and fills the buffer with as many numbers as it can handle.
 * Tests to verify whether or not the buffer can accurately record statistics and manages the
 * numbers in an expected manner.
 *
 */
void test_not_full_array_with_diff_numbers_with_reset(void)
{
	/* Initializing array to be tested */
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	addData(array1, (TYPE)22);
	addData(array1, (TYPE)1000);
	addData(array1, (TYPE)513);
	addData(array1, (TYPE)3423);
	addData(array1, (TYPE)2);
	assert(2 == getMin(array1));
	assert(3423 == getMax(array1));
	assert(992 == getAvg(array1));
	resetAll(array1);
	addData(array1, (TYPE)444);
	addData(array1, (TYPE)100);
	addData(array1, (TYPE)5134);
	addData(array1, (TYPE)10000);
	addData(array1, (TYPE)50);
	addData(array1, (TYPE)76);
	addData(array1, (TYPE)90);
	addData(array1, (TYPE)51);
	assert(50 == getMin(array1));
	assert(10000 == getMax(array1));
	assert(1993 == getAvg(array1));
}

/**
 * Function: test_overloading_array_with_reset
 * Initializes a buffer and fills the buffer with as many numbers as it can handle.
 * Tests to verify whether or not the buffer can accurately record statistics and manages the
 * numbers in an expected manner.
 *
 */
void test_overloading_array_with_reset(void)
{
	/* Initializing array to be tested */
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	addData(array1, (TYPE)1);
	addData(array1, (TYPE)11);
	addData(array1, (TYPE)544);
	addData(array1, (TYPE)245);
	addData(array1, (TYPE)123);
	addData(array1, (TYPE)5);
	addData(array1, (TYPE)324);
	addData(array1, (TYPE)89);
	addData(array1, (TYPE)53);
	addData(array1, (TYPE)20);
	addData(array1, (TYPE)100);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)58);

	assert(1 == getMin(array1));
	assert(544 == getMax(array1));
	assert(102 == getAvg(array1));

	resetAll(array1);
	addData(array1, (TYPE)1);
	addData(array1, (TYPE)2);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)4);
	addData(array1, (TYPE)5);
	addData(array1, (TYPE)6);

	assert(1 == getMin(array1));
	assert(6 == getMax(array1));
	assert(3 == getAvg(array1));
}

void test_multiple_handles(void)
{
	/* Initializing array to be tested */
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	addData(array1, (TYPE)1);
	addData(array1, (TYPE)11);
	addData(array1, (TYPE)544);
	addData(array1, (TYPE)245);
	addData(array1, (TYPE)123);
	addData(array1, (TYPE)5);
	addData(array1, (TYPE)324);
	addData(array1, (TYPE)89);
	addData(array1, (TYPE)53);
	addData(array1, (TYPE)20);
	addData(array1, (TYPE)100);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)58);
	/* Initializing second array to be tested */
	TYPE myArray2[5];
	uint16_t array2 = init(myArray2, 5);
	addData(array2, (TYPE)99);
	addData(array2, (TYPE)99);
	addData(array2, (TYPE)99);
	addData(array2, (TYPE)99);

	assert(1 == getMin(array1));
	assert(544 == getMax(array1));
	assert(102 == getAvg(array1));

	assert(99 == getMin(array2));
	assert(99 == getMax(array2));
	assert(99 == getAvg(array2));
}

void test_1_before_overflow(void)
{
	/*Initializing array to be tested */
	TYPE myArray[2000];
	uint16_t array1 = init(myArray, 2000);
	addData(array1, (TYPE)60000);
	addData(array1, (TYPE)5000);
	addData(array1, (TYPE)500);
	addData(array1, (TYPE)30);
	addData(array1, (TYPE)5);

	assert(5 == getMin(array1));
	assert(60000 == getMax(array1));
	assert(13107 == getAvg(array1));
}

void test_overflow_uint16(void)
{
	/*Initializing array to be tested */
	/* Tests overflow of uint16_t */
    TYPE myArray[2000];
    uint16_t array1 = init(myArray, 2000);
	/* 2^16 = 65536 is the max overflow value */
	addData(array1, (TYPE)60000);
	addData(array1, (TYPE)5000);
	addData(array1, (TYPE)500);
	addData(array1, (TYPE)30);
	addData(array1, (TYPE)6);
	addData(array1, (TYPE)5000);
	assert(6 == getMin(array1));
	assert(60000 == getMax(array1));
	assert(0 == getAvg(array1));
}

//void test_overflow_float(void)
//{
//	/*Initializing array to be tested */
//	/* Tests overflow of float */
//	// TODO: Finish up test
//	TYPE myArray[10026];
//	uint16_t array1 = init(myArray, 10026);
//	addData(array1, (TYPE)FLT_MIN);
//	addData(array1, (TYPE)1.0);
//	assert(1.0 == getMin(array1));
//	assert_FLOAT(FLT_MIN == getMax(array1));
//	assert(0.0 == getAvg(array1));
//}

void test_resetAvg_and_resetMinMax(void)
{
	/*Initializing array to be tested */
	TYPE myArray[123];
	uint16_t array1 = init(myArray, 123);

	/* test no data inserted */
	assert(0 == getMin(array1));
	assert(0 == getMax(array1));
	assert(0 == getAvg(array1));

	addData(array1, (TYPE)11);
	addData(array1, (TYPE)13);
	addData(array1, (TYPE)45);
	addData(array1, (TYPE)330);
	addData(array1, (TYPE)59);
	addData(array1, (TYPE)712);

	/* test normal functionalities */
	assert(11 == getMin(array1));
	assert(712 == getMax(array1));
	assert(195 == getAvg(array1));

	/* test resetAvg */
	resetAvg(array1);
	assert(11 == getMin(array1));
	assert(712 == getMax(array1));
	assert(0 == getAvg(array1));

	addData(array1, (TYPE)222);
	addData(array1, (TYPE)3456);
	addData(array1, (TYPE)42);
	addData(array1, (TYPE)4459);
	addData(array1, (TYPE)70);

	/* test with new data after calling resetAvg */
	assert(11 == getMin(array1));
	assert(4459 == getMax(array1));
	assert(1649 == getAvg(array1));

	/* test resetMinMax */
	resetMinMax(array1);
	assert(0 == getMin(array1));
	assert(0 == getMax(array1));
	assert(1649 == getAvg(array1));

	addData(array1, (TYPE)5);
	addData(array1, (TYPE)500);

	/* test add 1 min and 1 max value and check min max */
	assert(5 == getMin(array1));
	assert(500 == getMax(array1));
	assert(1250 == getAvg(array1));

	addData(array1, (TYPE)3);
	addData(array1, (TYPE)999);

	/* test update new min and max */
	assert(3 == getMin(array1));
	assert(999 == getMax(array1));
	assert(1084 == getAvg(array1));


	addData(array1, (TYPE)5);
	addData(array1, (TYPE)500);
	addData(array1, (TYPE)5);

	addData(array1, (TYPE)500);
	addData(array1, (TYPE)5);
	addData(array1, (TYPE)500);

	resetAvg(array1);
	resetMinMax(array1);

	/* test calling resetAvg() and resetMinMax() right after each other */
	assert(0 == getMin(array1));
	assert(0 == getMax(array1));
	assert(0 == getAvg(array1));

	addData(array1, (TYPE)123);
	addData(array1, (TYPE)23);
	addData(array1, (TYPE)555);
	addData(array1, (TYPE)556);
	addData(array1, (TYPE)453);
	addData(array1, (TYPE)96);

	/* find min/max/avg after calling resetAvg() and resetMinMax() */
	assert(23 == getMin(array1));
	assert(556 == getMax(array1));
	assert(301 == getAvg(array1));

	resetMinMax(array1);
	resetAvg(array1);

	/* test calling resetMinMax() and resetAvg() right after each other */
	assert(0 == getMin(array1));
	assert(0 == getMax(array1));
	assert(0 == getAvg(array1));

	addData(array1, (TYPE)1);
	addData(array1, (TYPE)2);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)2);
	addData(array1, (TYPE)1);

	/* find min/max/avg after calling resetMinMax() and resetAvg() */
	assert(1 == getMin(array1));
	assert(3 == getMax(array1));
	assert(2 == getAvg(array1));
}
