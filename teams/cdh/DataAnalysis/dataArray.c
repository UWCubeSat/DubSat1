#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dataArray.h"
#define TYPE uint16_t

typedef struct buffer {
	TYPE *bufferPt;
	TYPE maxSize;
	TYPE *startPt;
	TYPE *currPt;
	TYPE *endPt;
	uint16_t currentSize;
	TYPE min;
	TYPE max;
	uint16_t resetMinMaxFlag;
	uint16_t resetAvgFlag;
} buffer;

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
	myBuffer->min = (TYPE)0;
	myBuffer->max = (TYPE)0;
	myBuffer->resetMinMaxFlag = 0;
	myBuffer->resetAvgFlag = 0;
	return handleNumber++;
}

void printArray(uint16_t handle) {
	buffer *theirBuffer = &bufferContainer[handle];
	for (int i = 0; i < theirBuffer->maxSize; i++) {
		TYPE number = theirBuffer->bufferPt[i];
		printf("Position %d: %u\n", i, number);
	}
}

void printAllStats(uint16_t handle) {
	buffer *theirBuffer = &bufferContainer[handle];
	uint16_t sum = getSum(handle);
	printf("Sum: %u\n", sum);
	uint16_t min = theirBuffer->min;
	uint16_t max = theirBuffer->max;
	printf("Min: %hu, Max: %hu\n", min, max);
	uint16_t avg = getAvg(handle);
	printf("Avg: %u\n", avg);
}

void addData(uint16_t handle, uint16_t data) {
	buffer *theirBuffer = &bufferContainer[handle];
	printf("Adding %u\n", data);
	printf("Start: %p\n", theirBuffer->startPt);
	printf("current: %p\n", theirBuffer->currPt);
	printf("End: %p\n", theirBuffer->endPt);
	uint16_t maxSize = theirBuffer->maxSize;

	/* it will store new min and max even the array overflows or overloads */
	if ((theirBuffer->endPt == theirBuffer->startPt
	      || theirBuffer->resetMinMaxFlag == 1) && theirBuffer->resetAvgFlag != 1) {
	  theirBuffer->min = data;
	  theirBuffer->max = data;
	  theirBuffer->resetMinMaxFlag = 0;
	} else {
	  if (data < theirBuffer->min) {
	      theirBuffer->min = data;
	  }

	  if (data > theirBuffer->max) {
	      theirBuffer->max = data;
	  }

	  if (theirBuffer->resetAvgFlag == 1) {
	      theirBuffer->resetAvgFlag = 0;
	  }
	}

	/* check if the array is full */
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

	/* update end pointer for filling in the array the first time*/
	if (theirBuffer->endPt != ((theirBuffer->bufferPt) + maxSize)) {
		theirBuffer->endPt++;
		theirBuffer->currentSize++;
	}

	printf("Start: %p\n", theirBuffer->startPt);
	printf("current: %p\n", theirBuffer->currPt);
	printf("End: %p\n", theirBuffer->endPt);

}

void resetAll(uint16_t handle) {
	/* Resets*/
	buffer *theirBuffer = &bufferContainer[handle];
	theirBuffer->currPt = theirBuffer->startPt;
	theirBuffer->endPt = theirBuffer->startPt;
	theirBuffer->currentSize = 0;
	theirBuffer->min = 0;
	theirBuffer->max = 0;
	theirBuffer->resetMinMaxFlag = 0;
}

void resetAvg(uint16_t handle) {
    buffer *theirBuffer = &bufferContainer[handle];
    theirBuffer->currPt = theirBuffer->startPt;
	 theirBuffer->endPt = theirBuffer->startPt;
    theirBuffer->currentSize = 0;

    /* check case for situation when user calls resetMinMax() then resetAvg()
     make sure that calling those two functions in a row won't mess up min/max/avg
	 */
    if (theirBuffer->resetMinMaxFlag != 1) {
        theirBuffer->resetAvgFlag = 1;
    }
}

void resetMinMax(uint16_t handle) {
    buffer *theirBuffer = &bufferContainer[handle];
    theirBuffer->min = (TYPE)0;
    theirBuffer->max = (TYPE)0;
    theirBuffer->resetMinMaxFlag = 1;
    theirBuffer->resetAvgFlag = 0;
}

uint16_t getSum(uint16_t handle) {
	TYPE sum = (TYPE)0;
	buffer *theirBuffer = &bufferContainer[handle];
	uint16_t *curr = theirBuffer->startPt;
	while (curr != theirBuffer->endPt) {
		uint16_t number = *curr;
		sum += number;
		curr++;
   }

    /* handles overflow */
	if (sum < getMax(handle)) {
		sum = (TYPE)0;
	}
	return sum;
}

TYPE getAvg(uint16_t handle) {
	buffer *theirBuffer = &bufferContainer[handle];
	if (theirBuffer->currentSize == 0) {
		return 0;
	}
	TYPE sum = getSum(handle);
	uint16_t avg = sum / theirBuffer->currentSize;
	return avg;
}

TYPE getMin(uint16_t handle) {
	buffer *theirBuffer = &bufferContainer[handle];
	return theirBuffer->min;
}

TYPE getMax(uint16_t handle) {
	buffer *theirBuffer = &bufferContainer[handle];
	return theirBuffer->max;
}
