#pragma once
#ifndef DATAARRAY_H
#define DATAARRAY_H

#define ARRAY_SIZE 10

struct dataArray;
typedef struct dataArray dataArray;

// Initialized dataArray
dataArray* initArray();

// Print out Array
void printArray(dataArray *self);

// Add single number to the data set. Return 1 if overflow, else return 0
int addValue(dataArray *self, int value);

// Add given value to the current running sum
void sum(dataArray *self, int value);

// Record max value
void maxValue(dataArray *self, int value);

// Record min value
void minValue(dataArray *self, int value);

// Return average value of given data
int getAvg(dataArray *self);

// Return max value recorded
int getMax(dataArray *self);

// Return min value recorded
int getMin(dataArray *self);


#endif //DATAARRAY_H
