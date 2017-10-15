#include <msp430.h> 
#include "i2c_test.h"

static char debugOutputFormatString[] = "%d,%d,%d,%2.10f,%2.10f,%2.10f\r\n";

/*
 * main.c
 */
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);
    magInit();

    while (TRUE)
    {
        pmagdata = magReadXYZData(ConvertToTeslas);
        debugPrintF(debugOutputFormatString,
                    pmagdata->rawX, pmagdata->rawY, pmagdata->rawZ,
                    pmagdata->convertedX*1000, pmagdata->convertedY*1000, pmagdata->convertedZ*1000);

        __delay_cycles(.25 * SEC);
    }

	return 0;
}
