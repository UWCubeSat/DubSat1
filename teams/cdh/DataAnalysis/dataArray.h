#pragma once
#ifndef DATAARRAY_H
#define DATAARRAY_H


// Print out Array
void printArray(int *theirArray, int size);

// Add given value to the current running sum
int getSum(int *theirArray, int size);

// Return average value of given data
int getAvg(int *theirArray, int size);

// Return max value recorded
int getMax(int *theirArray, int size);

// Return min value recorded
int getMin(int *theirArray, int size);

#endif //DATAARRAY_H
