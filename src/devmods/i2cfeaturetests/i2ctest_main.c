#include <msp430.h> 
#include "i2c_test.h"

/*
 * main.c
 */
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);
    magInit();

    while (TRUE)
    {

    }

	return 0;
}
