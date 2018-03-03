#pragma once
#ifndef DATAARRAY_H
#define DATAARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>

#define TYPE uint16_t

struct buffer;

 /*initialize buffer struct*/
uint16_t init(TYPE *userBuffer, uint16_t size);

/* Add new data to given buffer
*/
void addData(uint16_t handle, TYPE data);

/* reset all data in the given buffer including min, max, and average
*/
void resetAll(uint16_t handle);

/* reset only average
*/
void resetAvg(uint16_t handle);

/* reset only min and max
*/
void resetMinMax(uint16_t handle);

/* return uint16_t sum of data in the given buffer
*/
TYPE getSum(uint16_t handle);

/* return uint16_t average of data in the given buffer
*/
TYPE getAvg(uint16_t handle);

/* return uint16_t min of data in the given buffer
No matter how many values are loaded into the array, min value stays until user call reset function
*/
TYPE getMin(uint16_t handle);

/* return uint16_t max of data in the given buffer
No matter how many values are loaded into the array, max value stays until user call reset function */
TYPE getMax(uint16_t handle);

/* Prints out given buffer
*/
void printArray(uint16_t handle);


/**
 * Prints out statistics of the buffer.
 */
void printAllStats(uint16_t handle);

#endif /*DATAARRAY_H*/
