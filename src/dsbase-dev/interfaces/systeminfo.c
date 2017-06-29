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

void infoReport(DebugMode mode)
{
    if (mode == InteractiveMode)
    {
        debugPrintF("\r\nSubsystem Module: %s", getSubsystemModulePath());
        debugPrintF("\r\nCompiler Version: %s", __TI_COMPILER_VERSION__);
    }
    else
    {
        debugPrintF("Stuff without as many words (e.g. just CSV)");
    }
}
