//Always include unity.h as it holds the testing functions
#include "unity.h"

/******************************************************************************
 *     Include header file(s) to test
 *****************************************************************************/
#include "can.h" 

/******************************************************************************
 *     Include header file(s) to mock (see wiki for full explanation)
 *****************************************************************************/
#include "mock_spi.h"
#include "mock_msp430.h"


void setUp(void)
{
    //This function will be called before the test runs.
    //
    //Set up any test variables you need here.
    //
    //This function will often be empty.
}

void tearDown(void)
{
    //Perform any cleanup after the test finishes here. Often you won't need 
    //to put anything in this function.
}



/******************************************************************************
 *     TESTS
 *****************************************************************************/

//To write a test just prepend test_ to a function that returns void and takes
//void as an argument. Ceedling will look through our test files for such 
//functions and automatically test them.
void test_requestToSend(void)
{
    spiTransceive_Ignore(); //we're mocking this function, so ignore it being called
    TEST_ASSERT_EQUAL(requestToSend(1),0);
}
