#include "dataArray.h"

typedef struct buffer_TYPE {
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
} buffer_TYPE;

uint16_t handleNumber = 0;
buffer_TYPE bufferContainer_TYPE[10];

/**
 * Function: init
 * Initializes a buffer and fills the buffer with as many numbers as it can handle.
 * Tests to verify whether or not the buffer can accurately record statistics and manages the
 * numbers in an expected manner.
 * @param userBuffer is a pointer to the array where the buffer will manage.
 * @param size is the capacity of the buffer. This MUST match the size of the userBuffer array.
 *        otherwise it will cause an abort trap when running the library. Max value is 2000.
 * @return {@code uint16_t} A 16-bit int that gives an identifier as to which buffer it has initialized.
 */
uint16_t init_TYPE(TYPE *userBuffer, uint16_t size) {
                          /* MSP430 */
	buffer_TYPE *myBuffer = &bufferContainer_TYPE[handleNumber];
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

void printArray_TYPE(uint16_t handle) {
	buffer_TYPE *theirBuffer = &bufferContainer_TYPE[handle];
	for (int i = 0; i < theirBuffer->maxSize; i++) {
		TYPE number = theirBuffer->bufferPt[i];
		printf("Position %d: %u\n", i, number);
	}
}

void printAllStats_TYPE(uint16_t handle) {
	buffer_TYPE *theirBuffer = &bufferContainer_TYPE[handle];
	NEXT_TYPE sum = getSum_TYPE(handle);
	printf("Sum: %u\n", sum);
	TYPE min = theirBuffer->min;
	TYPE max = theirBuffer->max;
	printf("Min: %hu, Max: %hu\n", min, max);
	TYPE avg = getAvg_TYPE(handle);
	printf("Avg: %u\n", avg);
}

/**
 * Function: addData
 * Adds a data value to the buffer. If the buffer is full, data will be added at the beginning of the buffer and
 * overwrite that data. Any subsequent data will be added in the next spot, meaning that it has wrapped to the
 * beginning of the buffer.
 * @param handle the identifier for the buffer that will be manipulated
 * @param data value being added into the buffer
 *
 */
void addData_TYPE(uint16_t handle, TYPE data) {
	buffer_TYPE *theirBuffer = &bufferContainer_TYPE[handle];
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

	/* check if the array is full. If the array is full, will add
	 * value to the beginning of the buffer and overwrite that value.  */
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
}

/**
 * Function: resetAll
 * Resets all statistics of the buffer.
 * @param handle the identifier for the buffer that will be manipulated the identifier for the buffer that will be manipulated
 *
 */
void resetAll_TYPE(uint16_t handle) {
	/* Resets*/
	buffer_TYPE *theirBuffer = &bufferContainer_TYPE[handle];
	theirBuffer->currPt = theirBuffer->startPt;
	theirBuffer->endPt = theirBuffer->startPt;
	theirBuffer->currentSize = 0;
	theirBuffer->min = 0;
	theirBuffer->max = 0;
	theirBuffer->resetMinMaxFlag = 0;
}

/**
 * Function: resetAvg
 * Resets the average of the buffer by having the pointers point to the beginning of the buffer,
 * representing that the buffer is "flushed" of old values
 * @param handle the identifier for the buffer that will be manipulated
 *
 */
void resetAvg_TYPE(uint16_t handle) {
    buffer_TYPE *theirBuffer = &bufferContainer_TYPE[handle];
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

/**
 * Function: resetMinMax
 * Resets statistics of the min and max.
 * @param handle the identifier for the buffer that will be manipulated
 *
 */
void resetMinMax_TYPE(uint16_t handle) {
    buffer_TYPE *theirBuffer = &bufferContainer_TYPE[handle];
    theirBuffer->min = (TYPE)0;
    theirBuffer->max = (TYPE)0;
    theirBuffer->resetMinMaxFlag = 1;
    theirBuffer->resetAvgFlag = 0;
}

/**
 * Function: getSum
 * Returns the sum of all numbers stored in the array. If an overflow occurred during calculation,
 * returns zero.
 * @param handle the identifier for the buffer that will be manipulated.
 * @return TYPE sum of the values within
 */
TYPE getSum_TYPE(uint16_t handle) {
	TYPE sum = (TYPE)0;
	buffer_TYPE *theirBuffer = &bufferContainer_TYPE[handle];
	TYPE *curr = theirBuffer->startPt;
	while (curr != theirBuffer->endPt) {
		TYPE number = *curr;
		sum += number;
		curr++;
   }

    /* handles overflow */
	if (sum < getMax_TYPE(handle)) {
		sum = (TYPE)0;
	}
	return sum;
}

/**
 * Function: getAvg
 * Returns the average of value within the current buffer
 *
 * @return TYPE average of values stored within the buffer.
 */
TYPE getAvg_TYPE(uint16_t handle) {
    buffer_TYPE *theirBuffer = &bufferContainer_TYPE[handle];
    if (theirBuffer->currentSize == 0) {
        return 0;
    }
    TYPE sum = getSum_TYPE(handle);
    TYPE avg = sum / theirBuffer->currentSize;
    return avg;
}

/**
 * Function: getMin
 * Returns the smallest value stored in the buffer
 *  the number that is associated with a buffer
 * @return TYPE maximum number that is stored within the buffer
 */
TYPE getMin_TYPE(uint16_t handle) {
    buffer_TYPE *theirBuffer = &bufferContainer_TYPE[handle];
    return theirBuffer->min;
}

/**
 * Function: getMax
 * Retrieves the largest value that has been added to the buffer.
 * @return TYPE maximum number that is stored within the buffer
 */
TYPE getMax_TYPE(uint16_t handle) {
    buffer_TYPE *theirBuffer = &bufferContainer_TYPE[handle];
    return theirBuffer->max;
}
