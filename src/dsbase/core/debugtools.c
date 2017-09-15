/*
 * debugtools.c
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#include "debugtools.h"
#include "uart.h"
#include "utils.h"
#include "config/config.h"
#include "bsp/bsp.h"

// NOTE:  Virtually everything in this file should be inside of the
// __DEBUG__ #ifdef, as the implementation of all these things
// should go away when compiled for release
#ifdef __DEBUG__

FILE_STATIC char debugWriteOutputBuff[CONFIGM_debug_outputbuffsize];
FILE_STATIC char debugConsoleInputBuff[CONFIGM_debug_consoleinputbuffsize];

FILE_STATIC uint8_t consoleBytesRead = 0;
FILE_STATIC uint8_t consoleBuildingCommand = 0;

FILE_STATIC volatile uint8_t debugTraceLevel = 0;
FILE_STATIC volatile DebugMode debugMode = InteractiveMode;

FILE_STATIC hBus handle;

void debugInit()
{
    handle = uartInit(BackchannelUART);
    uartRegisterRxCallback(handle, debugReadCallback);
}

void debugPrint(uint8_t * buff, uint8_t szBuff)
{
    uartTransmit(handle, buff, szBuff);
}

void debugPrintF(const char *_format, ...)
{
    int numBytes;

    va_list argptr;
    va_start(argptr, _format);
    numBytes = vsnprintf(debugWriteOutputBuff, CONFIGM_debug_outputbuffsize, _format, argptr);
    uartTransmit(handle, (uint8_t *)debugWriteOutputBuff, numBytes + 1);
}

void debugTrace(uint8_t level, uint8_t * buff, uint8_t szBuff)
{
    if (validTraceLevel(level) <= debugTraceLevel)
        debugPrint(buff, szBuff);
}

void debugTraceF(uint8_t level, const char *_format, ...)
{
    int numBytes;

    if (validTraceLevel(level) <= debugTraceLevel)
    {
        va_list argptr;
        va_start(argptr, _format);
        numBytes = vsnprintf(debugWriteOutputBuff, CONFIGM_debug_outputbuffsize, _format, argptr);
        uartTransmit(handle, (uint8_t *)debugWriteOutputBuff, numBytes + 1);
    }
}

void debugReadCallback(uint8_t rcvdbyte)
{
    // New command starting
    if (consoleBuildingCommand == 0)
    {
        consoleBuildingCommand = 1;
        consoleBytesRead = 0;
    }

    // Keep building command string until enter is hit
    if (rcvdbyte == '\r')
    {
        consoleBytesRead++;
        consoleBuildingCommand = 0;
        processCommand((uint8_t *)debugConsoleInputBuff, consoleBytesRead);
    }
    else
    {
        debugConsoleInputBuff[consoleBytesRead] = rcvdbyte;
        consoleBytesRead++;
    }
}

void processCommand(uint8_t * cmdbuff, uint8_t cmdlength)
{
    // Someone hits just enter - turn "off" anything noisy going on
    if (cmdlength == 1)
    {
        debugTraceLevel = 0;
        displayPrompt();
        return;
    }
    switch(cmdbuff[0])
    {
        case 't':    // Change TRACE level
            // Poor-man's atoi for single digits!
            debugTraceLevel = validTraceLevel(((uint8_t)cmdbuff[1]) - 48);
            break;
        case 'i':   // Call report* functions on all entities
            debugTraceLevel = 0;
            infoReport(debugMode);
            __delay_cycles(1000000);
            displayPrompt();
            break;
    }


    displayPrompt();
}

uint8_t validTraceLevel(uint8_t lvl)
{
    uint8_t vallvl = lvl;

    // Normalize to proper range (note:  it's unsigned, so don't need to check for < 0)
    if (vallvl > MAX_TRACE_LEVEL)
        vallvl = MAX_TRACE_LEVEL;
    return vallvl;
}

void displayPrompt()
{
    if (debugMode == InteractiveMode)
    {
        //debugPrint("\r\n[DEBUG]>>", 9);
        debugPrintF("\r\n[%s]>>", getSubsystemModulePath());
    }
}

#else  /* __DEBUG__ not specified, therefore nop debug operations */

void debugInit() {}
void debugPrint(uint8_t * buff, uint8_t szBuff) {}
void debugPrintF(const char *_format, ...) {}
void debugReadCallback(uint8_t rcvdbyte) {}

#endif /* __DEBUG__ */
