#pragma once
#ifndef DATAARRAY_H
#define DATAARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>

#define NEXT_uint16_t uint32_t

struct buffer_uint16_t;

 /*initialize buffer struct*/
uint16_t init_uint16_t(uint16_t *userBuffer, uint16_t size);

/* Add new data to given buffer
*/
void addData_uint16_t(uint16_t handle, uint16_t data);

/* reset all data in the given buffer including min, max, and average
*/
void resetAll_uint16_t(uint16_t handle);

/* reset only average
*/
void resetAvg_uint16_t(uint16_t handle);

/* reset only min and max
*/
void resetMinMax_uint16_t(uint16_t handle);

/* return uint16_t sum of data in the given buffer
*/
uint32_t getSum_uint16_t(uint16_t handle);

/* return uint16_t average of data in the given buffer
*/
uint16_t getAvg_uint16_t(uint16_t handle);

/* return uint16_t min of data in the given buffer
No matter how many values are loaded into the array, min value stays until user call reset function
*/
uint16_t getMin_uint16_t(uint16_t handle);

/* return uint16_t max of data in the given buffer
No matter how many values are loaded into the array, max value stays until user call reset function */
uint16_t getMax_uint16_t(uint16_t handle);

/* Prints out given buffer
*/


/**
 * Prints out statistics of the buffer.
 */

struct buffer_float;

 /*initialize buffer struct*/
uint16_t init_float(float *userBuffer, uint16_t size);

/* Add new data to given buffer
*/
void addData_float(uint16_t handle, float data);

/* reset all data in the given buffer including min, max, and average
*/
void resetAll_float(uint16_t handle);

/* reset only average
*/
void resetAvg_float(uint16_t handle);

/* reset only min and max
*/
void resetMinMax_float(uint16_t handle);

/* return uint16_t sum of data in the given buffer
*/
float getSum_float(uint16_t handle);

/* return uint16_t average of data in the given buffer
*/
float getAvg_float(uint16_t handle);

/* return uint16_t min of data in the given buffer
No matter how many values are loaded into the array, min value stays until user call reset function
*/
float getMin_float(uint16_t handle);

/* return uint16_t max of data in the given buffer
No matter how many values are loaded into the array, max value stays until user call reset function */
float getMax_float(uint16_t handle);


#define NEXT_uint8_t uint16_t

struct buffer_uint8_t;

 /*initialize buffer struct*/
uint16_t init_uint8_t(uint8_t *userBuffer, uint16_t size);

/* Add new data to given buffer
*/
void addData_uint8_t(uint16_t handle, uint8_t data);

/* reset all data in the given buffer including min, max, and average
*/
void resetAll_uint8_t(uint16_t handle);

/* reset only average
*/
void resetAvg_uint8_t(uint16_t handle);

/* reset only min and max
*/
void resetMinMax_uint8_t(uint16_t handle);

/* return uint16_t sum of data in the given buffer
*/
uint8_t getSum_uint8_t(uint16_t handle);

/* return uint16_t average of data in the given buffer
*/
uint8_t getAvg_uint8_t(uint16_t handle);

/* return uint16_t min of data in the given buffer
No matter how many values are loaded into the array, min value stays until user call reset function
*/
uint8_t getMin_uint8_t(uint16_t handle);

/* return uint16_t max of data in the given buffer
No matter how many values are loaded into the array, max value stays until user call reset function */
uint8_t getMax_uint8_t(uint16_t handle);


struct buffer_int16_t;

 /*initialize buffer struct*/
uint16_t init_int16_t(int16_t *userBuffer, uint16_t size);

/* Add new data to given buffer
*/
void addData_int16_t(uint16_t handle, int16_t data);

/* reset all data in the given buffer including min, max, and average
*/
void resetAll_int16_t(uint16_t handle);

/* reset only average
*/
void resetAvg_int16_t(uint16_t handle);

/* reset only min and max
*/
void resetMinMax_int16_t(uint16_t handle);

/* return uint16_t sum of data in the given buffer
*/
int16_t getSum_int16_t(uint16_t handle);

/* return uint16_t average of data in the given buffer
*/
int16_t getAvg_int16_t(uint16_t handle);

/* return uint16_t min of data in the given buffer
No matter how many values are loaded into the array, min value stays until user call reset function
*/
int16_t getMin_int16_t(uint16_t handle);

/* return uint16_t max of data in the given buffer
No matter how many values are loaded into the array, max value stays until user call reset function */
int16_t getMax_int16_t(uint16_t handle);


#endif /*DATAARRAY_H*/
