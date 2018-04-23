#pragma once
#ifndef DATAARRAY_H
#define DATAARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>

#define NEXT_TYPE uint16_t

struct buffer_TYPE;

 /*initialize buffer struct*/
uint16_t init_TYPE(TYPE *userBuffer, uint16_t size);

/* Add new data to given buffer
*/
void addData_TYPE(uint16_t handle, TYPE data);

/* reset all data in the given buffer including min, max, and average
*/
void resetAll_TYPE(uint16_t handle);

/* reset only average
*/
void resetAvg_TYPE(uint16_t handle);

/* reset only min and max
*/
void resetMinMax_TYPE(uint16_t handle);

/* return uint16_t sum of data in the given buffer
*/
TYPE getSum_TYPE(uint16_t handle);

/* return uint16_t average of data in the given buffer
*/
TYPE getAvg_TYPE(uint16_t handle);

/* return uint16_t min of data in the given buffer
No matter how many values are loaded into the array, min value stays until user call reset function
*/
TYPE getMin_TYPE(uint16_t handle);

/* return uint16_t max of data in the given buffer
No matter how many values are loaded into the array, max value stays until user call reset function */
TYPE getMax_TYPE(uint16_t handle);

/* Prints out given buffer
*/
void printArray_TYPE(uint16_t handle);


/**
 * Prints out statistics of the buffer.
 */
void printAllStats_TYPE(uint16_t handle);

#endif /*DATAARRAY_H*/
