#include <msp430.h> 

#include "bsp/bsp.h"

/*
 * main.c
 */
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);
	
	return 0;
}
