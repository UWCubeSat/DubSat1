/*
 * stack_checker.h
 *
 *  Created on: Mar 25, 2018
 *      Author: djdup
 */

#ifndef STACK_CHECKER_H_
#define STACK_CHECKER_H_

#ifdef STACK_CHECK
uint16_t stackMinFreeCount(void);
uint16_t stackMaxSize(void);
#else
uint16_t stackMinFreeCount(void) { return 0; }
uint16_t stackMaxSize(void) { return 0; }
#endif

#endif /* STACK_CHECKER_H_ */
