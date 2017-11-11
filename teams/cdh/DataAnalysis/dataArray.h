#pragma once
#ifndef DATAARRAY_H
#define DATAARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct buffer;

// initialize buffer struct
int init(uint16_t *userBuffer, uint16_t size);

// Print out given buffer
void printArray(int handle);

// Add new data to given buffer
void addData(int handle, uint16_t data);

// reset all data in the given buffer
void resetData(int handle);

// return uint16_t sum of data in the given buffer
uint16_t getSum(int handle);

// return uint16_t average of data in the given buffer
uint16_t getAvg(int handle);

// return uint16_t min of data in the given buffer
uint16_t getMin(int handle);

// return uint16_t max of data in the given buffer
uint16_t getMax(int handle);

#endif //DATAARRAY_H