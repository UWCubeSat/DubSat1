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
                                             "ADCS/RWheelZ",
                                             "PPT",
                                             "EPS/Dist",
                                             "EPS/Gen",
                                             "EPS/Batt",
                                             "ADCS/Estim",
                                             "ADCS/MPC",
                                             "ADCS/SensorProc",
                                             "ADCS/MTQ",
                                             "COM1",
                                             "COM2",
                                             "RAHS"};

FILE_STATIC SpacecraftInfo sc_info;


// Performs logic to "figure out" current state (1st boot? reboot? MET?)
// Hooks up sync pulse handlers
StartupType coreStartup(special_handler pptfiringhandler, special_handler rollcallhandler)
{

    // TODO:  populates sc_info as well?
    debugTraceF(1, "Completed core startup routine.\r\n");
    sc_info.scmode = SCMode_Unknown;
    return Startup_Unknown;
}

uint8_t *getSubsystemModulePath()
{
    return SubsystemModulePaths[(uint8_t)bspGetModule()];
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


