/* AUTOGENERATED FILE. DO NOT EDIT. */

/*=======Test Runner Used To Run Each Test Below=====*/
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.CurrentTestLineNumber = TestLineNum; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      setUp(); \
      TestFunc(); \
  } \
  if (TEST_PROTECT()) \
  { \
    tearDown(); \
  } \
  UnityConcludeTest(); \
}

/*=======Automagically Detected Files To Include=====*/
#ifdef __WIN32__
#define UNITY_INCLUDE_SETUP_STUBS
#endif
#include "unity.h"
#ifndef UNITY_EXCLUDE_SETJMP_H
#include <setjmp.h>
#endif
#include <stdio.h>
#include "dataArray.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/*=======External Functions This Runner Calls=====*/
extern void setUp(void);
extern void tearDown(void);
extern void test_initialization(void);
extern void test_adding_one_value(void);
extern void test_fill_array(void);
extern void test_reset(void);
extern void test_overloading_array(void);
extern void test_not_full_array_with_same_numbers(void);
extern void test_not_full_array_with_diff_numbers(void);
extern void test_not_full_array_with_diff_numbers_with_reset(void);
extern void test_overloading_array_with_reset(void);
extern void test_multiple_handles(void);
extern void test_1_before_overflow(void);
extern void test_overflow_uint16(void);
extern void test_overflow_float(void);
extern void test_resetAvg_and_resetMinMax(void);


/*=======Suite Setup=====*/
static void suite_setup(void)
{
#if defined(UNITY_WEAK_ATTRIBUTE) || defined(UNITY_WEAK_PRAGMA)
  suiteSetUp();
#endif
}

/*=======Suite Teardown=====*/
static int suite_teardown(int num_failures)
{
#if defined(UNITY_WEAK_ATTRIBUTE) || defined(UNITY_WEAK_PRAGMA)
  return suiteTearDown(num_failures);
#else
  return num_failures;
#endif
}

/*=======Test Reset Option=====*/
void resetTest(void);
void resetTest(void)
{
  tearDown();
  setUp();
}


/*=======MAIN=====*/
int main(void)
{
  suite_setup();
  UnityBegin("test/TestingCode.c");
  RUN_TEST(test_initialization, 35);
  RUN_TEST(test_adding_one_value, 52);
  RUN_TEST(test_fill_array, 63);
  RUN_TEST(test_reset, 83);
  RUN_TEST(test_overloading_array, 140);
  RUN_TEST(test_not_full_array_with_same_numbers, 163);
  RUN_TEST(test_not_full_array_with_diff_numbers, 178);
  RUN_TEST(test_not_full_array_with_diff_numbers_with_reset, 193);
  RUN_TEST(test_overloading_array_with_reset, 220);
  RUN_TEST(test_multiple_handles, 256);
  RUN_TEST(test_1_before_overflow, 291);
  RUN_TEST(test_overflow_float, 325);
  RUN_TEST(test_resetAvg_and_resetMinMax, 339);

  return suite_teardown(UnityEnd());
}
