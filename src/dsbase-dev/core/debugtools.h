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

#define MAX_DEBUG_BUFFER_SIZE  250

#ifndef __DEBUG__
#warning  Header debugtools.h included, but __DEBUG__ flag not set.
#endif  /* __DEBUG__ */

void debugInit();
void debugPrint(uint8_t * buff, uint8_t szBuff);
void debugPrintF(const char *_format, ...);


#endif /* DEBUGTOOLS_H_ */
