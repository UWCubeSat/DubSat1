/*
 * debugtools.h
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#ifndef DEBUGTOOLS_H_
#define DEBUGTOOLS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#ifndef __DEBUG__
#warning  Header debugtools.h included, but __DEBUG__ flag not set.
#endif  /* __DEBUG__ */

#define MAX_TRACE_LEVEL  4

void debugInit();

void debugPrint(uint8_t * buff, uint8_t szBuff);
void debugPrintF(const char *_format, ...);
void debugTrace(uint8_t level, uint8_t * buff, uint8_t szBuff);
void debugTraceF(uint8_t level, const char *_format, ...);

void debugReadCallback(uint8_t rcvdbyte);
void processCommand(uint8_t * cmdbuff, uint8_t cmdlength);
uint8_t validTraceLevel(uint8_t lvl);
void displayPrompt();

typedef enum _debugmode {
    InteractiveMode,
    HeadlessMode,
} DebugMode;


#endif /* DEBUGTOOLS_H_ */
