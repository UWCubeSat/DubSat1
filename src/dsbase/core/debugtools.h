/*
 * debugtools.h
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#ifndef DEBUGTOOLS_H_
#define DEBUGTOOLS_H_

#include <stdint.h>

#ifndef __DEBUG__
#warning  Header debugtools.h included, but __DEBUG__ flag not set.
#endif  /* __DEBUG__ */

void debug_print(uint8_t * buff, uint8_t szBuff);

#endif /* DEBUGTOOLS_H_ */
