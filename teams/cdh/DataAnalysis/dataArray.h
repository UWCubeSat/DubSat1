#pragma once
#ifndef DATAARRAY_H
#define DATAARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct buffer;

// initialize buffer struct
uint16_t init(uint16_t *userBuffer, uint16_t size);

// Print out given buffer
void printArray(uint16_t handle);

// Add new data to given buffer
void addData(uint16_t handle, uint16_t data);

// reset all data in the given buffer including min, max, and average
void resetAll(uint16_t handle);

// reset only average
void resetAvg(uint16_t handle);

// reset only min and max
void resetMinMax(uint16_t handle);  

// return uint16_t sum of data in the given buffer
uint16_t getSum(uint16_t handle);

// return uint16_t average of data in the given buffer
uint16_t getAvg(uint16_t handle);

// return uint16_t min of data in the given buffer
// No matter how many values are loaded into the array, min value stays until user call reset function
uint16_t getMin(uint16_t handle);

// return uint16_t max of data in the given buffer
// No matter how many values are loaded into the array, max value stays until user call reset function
uint16_t getMax(uint16_t handle);

#endif //DATAARRAY_H
