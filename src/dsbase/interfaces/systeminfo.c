/*
 * systeminfo.c
 *
 *  Created on: Jun 29, 2017
 *      Author: jeffc
 */
#include <stdint.h>
#include "systeminfo.h"
#include "core/utils.h"
#include "core/debugtools.h"
#include "bsp/bsp.h"

// NOTE:  KEEP PATH STRINGS IN SYNC WITH ENUM (in systeminfo.h)
FILE_STATIC uint8_t *SubsystemModulePaths[] =
                                           { "TestModule",
                                             "ADCS/BDot" };

uint8_t *getSubsystemModulePath()
{
    return SubsystemModulePaths[(uint8_t)bspGetModule()];
}

uint8_t infoReport(DebugMode mode)
{
    if (mode == InteractiveMode)
    {
        debugPrintF("**Subsystem Module: \t%s\r\n", getSubsystemModulePath());
        debugPrintF("Compiler Version:\t%d\r\n", __TI_COMPILER_VERSION__);
        debugPrintF("Standard C Version:\t%d\r\n", __STDC_VERSION__);
        debugPrintF("Date Compiled:\t\t%s\r\n", __DATE__);
        debugPrintF("Time Compiled:\t\t%s\r\n", __TIME__);

    }
    else
    {
        // Output raw data in CSV form, without field names
    }
    return 1;
}
