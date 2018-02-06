#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "dataArray.h"
#include "unity.h"

/*
	This file contains that will test the dataArray library.
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
void test_initialization(void)
{
	TYPE myArray[10];
   uint16_t array1 = init(myArray, 10);
	TEST_ASSERT_EQUAL(0, getAvg(array1));
	TEST_ASSERT_EQUAL(0, getSum(array1));
	TEST_ASSERT_EQUAL(0, getMin(array1));
	TEST_ASSERT_EQUAL(0, getMax(array1));
}

/*
 * Function: test_adding_one_value
 * Initializes a buffer and adds one value to buffer.
 * Tests if buffer reports the right statistics.
 *
 *
 */
void test_adding_one_value(void)
{
	TYPE myArray[10];
   uint16_t array1 = init(myArray, 10);
	addData(array1, (TYPE)5);
	TEST_ASSERT_EQUAL(5, getAvg(array1));
	TEST_ASSERT_EQUAL(5, getSum(array1));
	TEST_ASSERT_EQUAL(5, getMin(array1));
	TEST_ASSERT_EQUAL(5, getMax(array1));
}

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
	TEST_ASSERT_EQUAL(141, getAvg(array1));
	TEST_ASSERT_EQUAL(1417, getSum(array1));
	TEST_ASSERT_EQUAL(3, getMin(array1));
	TEST_ASSERT_EQUAL(544, getMax(array1));
}

void test_reset(void) {
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	/* Reset with no added values */
	resetAll(array1);
	TEST_ASSERT_EQUAL(0, getAvg(array1));
	TEST_ASSERT_EQUAL(0, getSum(array1));
	TEST_ASSERT_EQUAL(0, getMin(array1));
	TEST_ASSERT_EQUAL(0, getMax(array1));
	/* Reset with one added value */
	addData(array1, (TYPE)5);
	TEST_ASSERT_EQUAL(5, getAvg(array1));
	TEST_ASSERT_EQUAL(5, getSum(array1));
	TEST_ASSERT_EQUAL(5, getMin(array1));
	TEST_ASSERT_EQUAL(5, getMax(array1));
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
	TEST_ASSERT_EQUAL(141, getAvg(array1));
	TEST_ASSERT_EQUAL(1417, getSum(array1));
	TEST_ASSERT_EQUAL(3, getMin(array1));
	TEST_ASSERT_EQUAL(544, getMax(array1));
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
	TEST_ASSERT_EQUAL(1, getMin(array1));
	TEST_ASSERT_EQUAL(544, getMax(array1));
	TEST_ASSERT_EQUAL(102, getAvg(array1));
	resetAll(array1);
	TEST_ASSERT_EQUAL(0, getAvg(array1));
	TEST_ASSERT_EQUAL(0, getSum(array1));
	TEST_ASSERT_EQUAL(0, getMin(array1));
	TEST_ASSERT_EQUAL(0, getMax(array1));

}

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
	TEST_ASSERT_EQUAL(1, getMin(array1));
	TEST_ASSERT_EQUAL(544, getMax(array1));
	TEST_ASSERT_EQUAL(102, getAvg(array1));
}

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
	TEST_ASSERT_EQUAL(2, getMin(array1));
	TEST_ASSERT_EQUAL(2, getMax(array1));
	TEST_ASSERT_EQUAL(2, getAvg(array1));
}

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
	TEST_ASSERT_EQUAL(2, getMin(array1));
	TEST_ASSERT_EQUAL(3423, getMax(array1));
	TEST_ASSERT_EQUAL(992, getAvg(array1));
}

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
	TEST_ASSERT_EQUAL(2, getMin(array1));
	TEST_ASSERT_EQUAL(3423, getMax(array1));
	TEST_ASSERT_EQUAL(992, getAvg(array1));
	resetAll(array1);
	addData(array1, (TYPE)444);
	addData(array1, (TYPE)100);
	addData(array1, (TYPE)5134);
	addData(array1, (TYPE)10000);
	addData(array1, (TYPE)50);
	addData(array1, (TYPE)76);
	addData(array1, (TYPE)90);
	addData(array1, (TYPE)51);
	TEST_ASSERT_EQUAL(50, getMin(array1));
	TEST_ASSERT_EQUAL(10000, getMax(array1));
	TEST_ASSERT_EQUAL(1993, getAvg(array1));
}

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

	TEST_ASSERT_EQUAL(1, getMin(array1));
	TEST_ASSERT_EQUAL(544, getMax(array1));
	TEST_ASSERT_EQUAL(102, getAvg(array1));

	resetAll(array1);
	addData(array1, (TYPE)1);
	addData(array1, (TYPE)2);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)4);
	addData(array1, (TYPE)5);
	addData(array1, (TYPE)6);

	TEST_ASSERT_EQUAL(1, getMin(array1));
	TEST_ASSERT_EQUAL(6, getMax(array1));
	TEST_ASSERT_EQUAL(3, getAvg(array1));
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

	TEST_ASSERT_EQUAL(1, getMin(array1));
	TEST_ASSERT_EQUAL(544, getMax(array1));
	TEST_ASSERT_EQUAL(102, getAvg(array1));

	TEST_ASSERT_EQUAL(99, getMin(array2));
	TEST_ASSERT_EQUAL(99, getMax(array2));
	TEST_ASSERT_EQUAL(99, getAvg(array2));
}

void test_1_before_overflow(void)
{
	/*Initializing array to be tested */
	TYPE myArray[10026];
	uint16_t array1 = init(myArray, 10026);
	addData(array1, (TYPE)60000);
	addData(array1, (TYPE)5000);
	addData(array1, (TYPE)500);
	addData(array1, (TYPE)30);
	addData(array1, (TYPE)5);

	TEST_ASSERT_EQUAL(5, getMin(array1));
	TEST_ASSERT_EQUAL(60000, getMax(array1));
	TEST_ASSERT_EQUAL(13107, getAvg(array1));
}

void test_overflow_uint16(void)
{
	/*Initializing array to be tested */
	/* Tests overflow of uint16_t */
	TYPE myArray[10026];
	uint16_t array1 = init(myArray, 10026);
	/* 2^16 = 65536 is the max overflow value */
	addData(array1, (TYPE)60000);
	addData(array1, (TYPE)5000);
	addData(array1, (TYPE)500);
	addData(array1, (TYPE)30);
	addData(array1, (TYPE)6);
	addData(array1, (TYPE)5000);
	TEST_ASSERT_EQUAL(6, getMin(array1));
	TEST_ASSERT_EQUAL(60000, getMax(array1));
	TEST_ASSERT_EQUAL(0, getAvg(array1));
}

void test_overflow_float(void)
{
	/*Initializing array to be tested */
	/* Tests overflow of float */
	// TODO: Finish up test
	TYPE myArray[10026];
	uint16_t array1 = init(myArray, 10026);
	addData(array1, (TYPE)FLT_MIN);
	addData(array1, (TYPE)1.0);
	TEST_ASSERT_EQUAL(1.0, getMin(array1));
	TEST_ASSERT_EQUAL_FLOAT(FLT_MIN, getMax(array1));
	TEST_ASSERT_EQUAL(0.0, getAvg(array1));
}

void test_resetAvg_and_resetMinMax(void)
{
	/*Initializing array to be tested */
	TYPE myArray[123];
	uint16_t array1 = init(myArray, 123);

	/* test no data inserted */
	TEST_ASSERT_EQUAL(0, getMin(array1));
	TEST_ASSERT_EQUAL(0, getMax(array1));
	TEST_ASSERT_EQUAL(0, getAvg(array1));

	addData(array1, (TYPE)11);
	addData(array1, (TYPE)13);
	addData(array1, (TYPE)45);
	addData(array1, (TYPE)330);
	addData(array1, (TYPE)59);
	addData(array1, (TYPE)712);

	/* test normal functionalities */
	TEST_ASSERT_EQUAL(11, getMin(array1));
	TEST_ASSERT_EQUAL(712, getMax(array1));
	TEST_ASSERT_EQUAL(195, getAvg(array1));

	/* test resetAvg */
	resetAvg(array1);
	TEST_ASSERT_EQUAL(11, getMin(array1));
	TEST_ASSERT_EQUAL(712, getMax(array1));
	TEST_ASSERT_EQUAL(0, getAvg(array1));

	addData(array1, (TYPE)222);
	addData(array1, (TYPE)3456);
	addData(array1, (TYPE)42);
	addData(array1, (TYPE)4459);
	addData(array1, (TYPE)70);

	/* test with new data after calling resetAvg */
	TEST_ASSERT_EQUAL(11, getMin(array1));
	TEST_ASSERT_EQUAL(4459, getMax(array1));
	TEST_ASSERT_EQUAL(1649, getAvg(array1));

	/* test resetMinMax */
	resetMinMax(array1);
	TEST_ASSERT_EQUAL(0, getMin(array1));
	TEST_ASSERT_EQUAL(0, getMax(array1));
	TEST_ASSERT_EQUAL(1649, getAvg(array1));

	addData(array1, (TYPE)5);
	addData(array1, (TYPE)500);

	/* test add 1 min and 1 max value and check min max */
	TEST_ASSERT_EQUAL(5, getMin(array1));
	TEST_ASSERT_EQUAL(500, getMax(array1));
	TEST_ASSERT_EQUAL(1250, getAvg(array1));

	addData(array1, (TYPE)3);
	addData(array1, (TYPE)999);

	/* test update new min and max */
	TEST_ASSERT_EQUAL(3, getMin(array1));
	TEST_ASSERT_EQUAL(999, getMax(array1));
	TEST_ASSERT_EQUAL(1084, getAvg(array1));


	addData(array1, (TYPE)5);
	addData(array1, (TYPE)500);
	addData(array1, (TYPE)5);
	/*
		Abort trap: 6 occurs here
	*/
	addData(array1, (TYPE)500);
	addData(array1, (TYPE)5);
	addData(array1, (TYPE)500);

	resetAvg(array1);
	resetMinMax(array1);

	/* test calling resetAvg() and resetMinMax() right after each other */
	TEST_ASSERT_EQUAL(0, getMin(array1));
	TEST_ASSERT_EQUAL(0, getMax(array1));
	TEST_ASSERT_EQUAL(0, getAvg(array1));

	addData(array1, (TYPE)123);
	addData(array1, (TYPE)23);
	addData(array1, (TYPE)555);
	addData(array1, (TYPE)556);
	addData(array1, (TYPE)453);
	addData(array1, (TYPE)96);

	/* find min/max/avg after calling resetAvg() and resetMinMax() */
	TEST_ASSERT_EQUAL(23, getMin(array1));
	TEST_ASSERT_EQUAL(556, getMax(array1));
	TEST_ASSERT_EQUAL(301, getAvg(array1));

	resetMinMax(array1);
	resetAvg(array1);

	/* test calling resetMinMax() and resetAvg() right after each other */
	TEST_ASSERT_EQUAL(0, getMin(array1));
	TEST_ASSERT_EQUAL(0, getMax(array1));
	TEST_ASSERT_EQUAL(0, getAvg(array1));

	addData(array1, (TYPE)1);
	addData(array1, (TYPE)2);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)3);
	addData(array1, (TYPE)2);
	addData(array1, (TYPE)1);

	/* find min/max/avg after calling resetMinMax() and resetAvg() */
	TEST_ASSERT_EQUAL(1, getMin(array1));
	TEST_ASSERT_EQUAL(3, getMax(array1));
	TEST_ASSERT_EQUAL(2, getAvg(array1));
}
