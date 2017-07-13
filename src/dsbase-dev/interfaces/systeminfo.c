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
        debugPrintF("**Subsystem Module: %s\r\n", getSubsystemModulePath());
        debugPrintF("Compiler Version: %f\r\n", 17.0);
    }
    else
    {
        debugPrintF("Stuff without as many words (e.g. just CSV)");
    }
    return 1;
}
