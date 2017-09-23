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
                                             "ADCS/BDot",
                                             "ADCS/RWheelX",
                                             "ADCS/RWheelY",
                                             "ADCS/RWheelZ"};

FILE_STATIC SpacecraftInfo sc_info;

// Performs logic to "figure out" current state (1st boot? reboot? MET?)
// Hooks up sync pulse handlers
StartupType coreStartup(sync_pulse_handler sync1, sync_pulse_handler sync2)
{

    // TODO:  populates sc_info as well?
    debugTraceF(1, "Completed core startup routine.\r\n");
    return Startup_Unknown;
}

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

uint8_t coreGetPrecautionLevel()
{
    // TODO:  Do some fancy mapping between perceived PPT state and action
    // TODO:  Maybe add a parameter the says what you're trying to do, and this will send back a yes/no
    return 0;
}



// TODO:  maybe have the precaution level thing work with this function instead ...?
uint8_t coreIsPrecautionaryInhibitActive(ActivityType activity)
{
    return 1;
}


