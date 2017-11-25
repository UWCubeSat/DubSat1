#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dataArray.h"

struct buffer {
	uint16_t *bufferPt;
	uint16_t maxSize;
	uint16_t *startPt;
	uint16_t *currPt;
	uint16_t *endPt;
	uint16_t currentSize;
};

typedef struct buffer buffer;

uint16_t handleNumber = 0;
buffer bufferContainer[10];

uint16_t init(uint16_t *userBuffer, uint16_t size) {
	buffer *myBuffer = &bufferContainer[handleNumber];
	myBuffer->bufferPt = userBuffer;
	myBuffer->maxSize = size;
	myBuffer->startPt = userBuffer;
	myBuffer->currPt = userBuffer;
	myBuffer->endPt = userBuffer;
	myBuffer->currentSize = 0;
	handleNumber += 1;
	return handleNumber - 1;
}

void printArray(uint16_t handle) {
	buffer *theirBuffer = &bufferContainer[handle];
	for (int i = 0; i < theirBuffer->maxSize; i++) {
		uint16_t number = theirBuffer->bufferPt[i];
		printf("Position %d: %u\n", i, number);
	}
}

void addData(uint16_t handle, uint16_t data) {
	buffer *theirBuffer = &bufferContainer[handle];
	uint16_t maxSize = theirBuffer->maxSize;

	// check if the array is full
	if (theirBuffer->currPt == theirBuffer->startPt) {
		*theirBuffer->currPt = data;
	}
	else if (theirBuffer->currPt == ((theirBuffer->bufferPt) + maxSize))
	{
		theirBuffer->currPt = theirBuffer->startPt;
		*theirBuffer->currPt = data;
	}
	else
	{
		*theirBuffer->currPt = data;
	}

	theirBuffer->currPt++;

	// update end pointer for filling in the array the first time
	if (theirBuffer->endPt != ((theirBuffer->bufferPt) + maxSize)) {
		theirBuffer->endPt++;
		theirBuffer->currentSize++;
	}

}

void resetData(uint16_t handle) {
	buffer *theirBuffer = &bufferContainer[handle];
	theirBuffer->currPt = theirBuffer->startPt;
	theirBuffer->endPt = theirBuffer->startPt;
	theirBuffer->currentSize = 0;
}

uint16_t getSum(uint16_t handle) {
	uint16_t sum = 0;
	buffer *theirBuffer = &bufferContainer[handle];
	uint16_t *curr = theirBuffer->startPt;
	while (curr != theirBuffer->endPt) {
		uint16_t number = *curr;
		sum += number;
		curr++;
	}
	if (sum < getMax(handle)) {
		sum = 0;
	}
	return sum;
}

uint16_t getAvg(uint16_t handle) {
	uint16_t sum = getSum(handle);
	uint16_t avg = sum / bufferContainer[handle].currentSize;
	return avg;
}

uint16_t getMin(uint16_t handle) {
	buffer *theirBuffer = &bufferContainer[handle];
	uint16_t *curr = theirBuffer->startPt;
	uint16_t min = *curr;
	while (curr != theirBuffer->endPt) {
		uint16_t number = *curr;
		if (number < min) {
			min = number;
		}
		curr++;
	}
	return min;
}

uint16_t getMax(uint16_t handle) {
	buffer *theirBuffer = &bufferContainer[handle];
	uint16_t *curr = theirBuffer->startPt;
	uint16_t max = *curr;
	while (curr != theirBuffer->endPt) {
		uint16_t number = *curr;
		if (number > max) {
			max = number;
		}
		curr++;
	}
	return max;
}