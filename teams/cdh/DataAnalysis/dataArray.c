#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataArray.h"

void sum(dataArray *self, int value);
void maxValue(dataArray *self, int value);
void minValue(dataArray *self, int value);

struct dataArray {
	int myArray[ARRAY_SIZE];
	int current;	// current position in the array
	int sum;
	int max;
	int min;
	int reset;		// flag reset data collected
	int counter;	// count number of data
	int overflow;	// overflow flag
};

dataArray* initArray() {
	dataArray *newArray = (dataArray*) malloc(sizeof(dataArray));
	
	for (int i = 0; i < ARRAY_SIZE; i++) {
		newArray->myArray[i] = 0;
	}

	newArray->current = 0;
	newArray->sum = 0;
	newArray->min = 0;
	newArray->max = 0;
	newArray->reset = 1;
	newArray->counter = 0;
	newArray->overflow = 0;
	
	return newArray;
}

void printArray(dataArray *self) {
	for (int i = 0; i < ARRAY_SIZE; i++) {
		printf("Pos:%d is %d\n", i, self->myArray[i]);
	}
}

int addValue(dataArray *self, int value) {
	if (self->current >= ARRAY_SIZE) {
		self->current = 0;
	}

	// do this when enter value for the first time
	if (self->reset == 1) {
		self->min = value;
		self->max = value;
		self->reset = 0;
	}

	self->myArray[self->current] = value;
	sum(self, value);
	maxValue(self, value);
	minValue(self, value);
	self->current++;
	self->counter++;
	return self->overflow;
}

void sum(dataArray *self, int value) {
	if ((value > 0 && self->sum > INT_MAX - value) || (value < 0 && self->sum < INT_MAX - value)) {
		self->overflow = 1;
	} else {
		self->sum += value;
	}
}

void maxValue(dataArray *self, int value) {
	if (self->max < value) {
		self->max = value;
	}
}

void minValue(dataArray *self, int value) {
	if (self->min > value) {
		self->min = value;
	}
}

int getAvg(dataArray *self) {
	if (self->counter == 0) {
		return 0;
	} else {
		int avg = self->sum / self->counter;
		return avg;
	}
}

int getMax(dataArray *self) {
	int max = self->max;
	return max;
}

int getMin(dataArray *self) {
	int min = self->min;
	return min;
}
