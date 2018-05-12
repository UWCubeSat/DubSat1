#include "dataArray.h"
typedef struct buffer_float {
	float *bufferPt;
	float maxSize;
	float *startPt;
	float *currPt;
	float *endPt;
	uint16_t currentSize;
	float min;
	float max;
	uint16_t resetMinMaxFlag;
	uint16_t resetAvgFlag;
} buffer_float;

uint16_t handleNumber_float = 0;
#ifdef __DATA_ARRAYS_COUNT__
	buffer_float bufferContainer_float[__DATA_ARRAYS_COUNT__];
	const uint8_t bufferContainerLength_float = __DATA_ARRAYS_COUNT__;
#else
	#warning need to define how many data arrays you are using 20 is default
	buffer_float bufferContainer_float[20];
	const uint8_t bufferContainerLength_float = 20;
#endif
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
uint16_t init_float(float *userBuffer, uint16_t size) {
                          /* MSP430 */
	buffer_float *myBuffer = &bufferContainer_float[handleNumber_float];
	myBuffer->bufferPt = userBuffer;
	myBuffer->maxSize = size;
	myBuffer->startPt = userBuffer;
	myBuffer->currPt = userBuffer;
	myBuffer->endPt = userBuffer;
	myBuffer->currentSize = 0;
	myBuffer->min = (float)0;
	myBuffer->max = (float)0;
	myBuffer->resetMinMaxFlag = 0;
	myBuffer->resetAvgFlag = 0;
	return handleNumber_float++;
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
void addData_float(uint16_t handle, float data) {
	buffer_float *theirBuffer = &bufferContainer_float[handle];
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
void resetAll_float(uint16_t handle) {
	/* Resets*/
	buffer_float *theirBuffer = &bufferContainer_float[handle];
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
void resetAvg_float(uint16_t handle) {
    buffer_float *theirBuffer = &bufferContainer_float[handle];
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
void resetMinMax_float(uint16_t handle) {
    buffer_float *theirBuffer = &bufferContainer_float[handle];
    theirBuffer->min = (float)0;
    theirBuffer->max = (float)0;
    theirBuffer->resetMinMaxFlag = 1;
    theirBuffer->resetAvgFlag = 0;
}

/**
 * Function: getSum
 * Returns the sum of all numbers stored in the array. If an overflow occurred during calculation,
 * returns zero.
 * @param handle the identifier for the buffer that will be manipulated.
 * @return float sum of the values within
 */
NEXT_float getSum_float(uint16_t handle) {
	NEXT_float sum = (float)0;
	buffer_float *theirBuffer = &bufferContainer_float[handle];
	float *curr = theirBuffer->startPt;
	while (curr != theirBuffer->endPt) {
		float number = *curr;
		sum += number;
		curr++;
   }

    /* handles overflow */

	return sum;
}

/**
 * Function: getAvg
 * Returns the average of value within the current buffer
 *
 * @return float average of values stored within the buffer.
 */
float getAvg_float(uint16_t handle) {
    buffer_float *theirBuffer = &bufferContainer_float[handle];
    if (theirBuffer->currentSize == 0) {
        return 0;
    }
    NEXT_float sum = getSum_float(handle);
    float avg = sum / theirBuffer->currentSize;
    return avg;
}

/**
 * Function: getMin
 * Returns the smallest value stored in the buffer
 *  the number that is associated with a buffer
 * @return float maximum number that is stored within the buffer
 */
float getMin_float(uint16_t handle) {
    buffer_float *theirBuffer = &bufferContainer_float[handle];
    return theirBuffer->min;
}

/**
 * Function: getMax
 * Retrieves the largest value that has been added to the buffer.
 * @return float maximum number that is stored within the buffer
 */
float getMax_float(uint16_t handle) {
    buffer_float *theirBuffer = &bufferContainer_float[handle];
    return theirBuffer->max;
}
typedef struct buffer_int16_t {
	int16_t *bufferPt;
	int16_t maxSize;
	int16_t *startPt;
	int16_t *currPt;
	int16_t *endPt;
	uint16_t currentSize;
	int16_t min;
	int16_t max;
	uint16_t resetMinMaxFlag;
	uint16_t resetAvgFlag;
} buffer_int16_t;

uint16_t handleNumber_int16_t = 0;
#ifdef __DATA_ARRAYS_COUNT__
	buffer_int16_t bufferContainer_int16_t[__DATA_ARRAYS_COUNT__];
	const uint8_t bufferContainerLength = __DATA_ARRAYS_COUNT__;
#else
	#warning need to define how many data arrays you are using 20 is default
	buffer_int16_t bufferContainer_int16_t[20];

#endif
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
uint16_t init_int16_t(int16_t *userBuffer, uint16_t size) {
                          /* MSP430 */
	buffer_int16_t *myBuffer = &bufferContainer_int16_t[handleNumber_int16_t];
	myBuffer->bufferPt = userBuffer;
	myBuffer->maxSize = size;
	myBuffer->startPt = userBuffer;
	myBuffer->currPt = userBuffer;
	myBuffer->endPt = userBuffer;
	myBuffer->currentSize = 0;
	myBuffer->min = (int16_t)0;
	myBuffer->max = (int16_t)0;
	myBuffer->resetMinMaxFlag = 0;
	myBuffer->resetAvgFlag = 0;
	return handleNumber_int16_t++;
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
void addData_int16_t(uint16_t handle, int16_t data) {
	buffer_int16_t *theirBuffer = &bufferContainer_int16_t[handle];
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
void resetAll_int16_t(uint16_t handle) {
	/* Resets*/
	buffer_int16_t *theirBuffer = &bufferContainer_int16_t[handle];
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
void resetAvg_int16_t(uint16_t handle) {
    buffer_int16_t *theirBuffer = &bufferContainer_int16_t[handle];
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
void resetMinMax_int16_t(uint16_t handle) {
    buffer_int16_t *theirBuffer = &bufferContainer_int16_t[handle];
    theirBuffer->min = (int16_t)0;
    theirBuffer->max = (int16_t)0;
    theirBuffer->resetMinMaxFlag = 1;
    theirBuffer->resetAvgFlag = 0;
}

/**
 * Function: getSum
 * Returns the sum of all numbers stored in the array. If an overflow occurred during calculation,
 * returns zero.
 * @param handle the identifier for the buffer that will be manipulated.
 * @return int16_t sum of the values within
 */
NEXT_int16_t getSum_int16_t(uint16_t handle) {
	NEXT_int16_t sum = (int16_t)0;
	buffer_int16_t *theirBuffer = &bufferContainer_int16_t[handle];
	int16_t *curr = theirBuffer->startPt;
	while (curr != theirBuffer->endPt) {
		int16_t number = *curr;
		sum += number;
		curr++;
   }

    /* handles overflow */

	return sum;
}

/**
 * Function: getAvg
 * Returns the average of value within the current buffer
 *
 * @return int16_t average of values stored within the buffer.
 */
int16_t getAvg_int16_t(uint16_t handle) {
    buffer_int16_t *theirBuffer = &bufferContainer_int16_t[handle];
    if (theirBuffer->currentSize == 0) {
        return 0;
    }
    NEXT_int16_t sum = getSum_int16_t(handle);
    int16_t avg = sum / theirBuffer->currentSize;
    return avg;
}

/**
 * Function: getMin
 * Returns the smallest value stored in the buffer
 *  the number that is associated with a buffer
 * @return int16_t maximum number that is stored within the buffer
 */
int16_t getMin_int16_t(uint16_t handle) {
    buffer_int16_t *theirBuffer = &bufferContainer_int16_t[handle];
    return theirBuffer->min;
}

/**
 * Function: getMax
 * Retrieves the largest value that has been added to the buffer.
 * @return int16_t maximum number that is stored within the buffer
 */
int16_t getMax_int16_t(uint16_t handle) {
    buffer_int16_t *theirBuffer = &bufferContainer_int16_t[handle];
    return theirBuffer->max;
}

typedef struct buffer_uint16_t {
	uint16_t *bufferPt;
	uint16_t maxSize;
	uint16_t *startPt;
	uint16_t *currPt;
	uint16_t *endPt;
	uint16_t currentSize;
	uint16_t min;
	uint16_t max;
	uint16_t resetMinMaxFlag;
	uint16_t resetAvgFlag;
} buffer_uint16_t;

uint16_t handleNumber_uint16_t = 0;
#ifdef __DATA_ARRAYS_COUNT__
	buffer_uint16_t bufferContainer_uint16_t[__DATA_ARRAYS_COUNT__];
	const uint8_t bufferContainerLength_uint16_t = __DATA_ARRAYS_COUNT__;
#else
	#warning need to define how many data arrays you are using 20 is default
	buffer_uint16_t bufferContainer_uint16_t[20];
#endif
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
uint16_t init_uint16_t(uint16_t *userBuffer, uint16_t size) {
                          /* MSP430 */
	buffer_uint16_t *myBuffer = &bufferContainer_uint16_t[handleNumber_uint16_t];
	myBuffer->bufferPt = userBuffer;
	myBuffer->maxSize = size;
	myBuffer->startPt = userBuffer;
	myBuffer->currPt = userBuffer;
	myBuffer->endPt = userBuffer;
	myBuffer->currentSize = 0;
	myBuffer->min = (uint16_t)0;
	myBuffer->max = (uint16_t)0;
	myBuffer->resetMinMaxFlag = 0;
	myBuffer->resetAvgFlag = 0;
	return handleNumber_uint16_t++;
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
void addData_uint16_t(uint16_t handle, uint16_t data) {
	buffer_uint16_t *theirBuffer = &bufferContainer_uint16_t[handle];
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
void resetAll_uint16_t(uint16_t handle) {
	/* Resets*/
	buffer_uint16_t *theirBuffer = &bufferContainer_uint16_t[handle];
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
void resetAvg_uint16_t(uint16_t handle) {
    buffer_uint16_t *theirBuffer = &bufferContainer_uint16_t[handle];
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
void resetMinMax_uint16_t(uint16_t handle) {
    buffer_uint16_t *theirBuffer = &bufferContainer_uint16_t[handle];
    theirBuffer->min = (uint16_t)0;
    theirBuffer->max = (uint16_t)0;
    theirBuffer->resetMinMaxFlag = 1;
    theirBuffer->resetAvgFlag = 0;
}

/**
 * Function: getSum
 * Returns the sum of all numbers stored in the array. If an overflow occurred during calculation,
 * returns zero.
 * @param handle the identifier for the buffer that will be manipulated.
 * @return uint16_t sum of the values within
 */
NEXT_uint16_t getSum_uint16_t(uint16_t handle) {
	NEXT_uint16_t sum = (uint16_t)0;
	buffer_uint16_t *theirBuffer = &bufferContainer_uint16_t[handle];
	uint16_t *curr = theirBuffer->startPt;
	while (curr != theirBuffer->endPt) {
		uint16_t number = *curr;
		sum += number;
		curr++;
   }

    /* handles overflow */

	return sum;
}

/**
 * Function: getAvg
 * Returns the average of value within the current buffer
 *
 * @return uint16_t average of values stored within the buffer.
 */
uint16_t getAvg_uint16_t(uint16_t handle) {
    buffer_uint16_t *theirBuffer = &bufferContainer_uint16_t[handle];
    if (theirBuffer->currentSize == 0) {
        return 0;
    }
    NEXT_uint16_t sum = getSum_uint16_t(handle);
    uint16_t avg = sum / theirBuffer->currentSize;
    return avg;
}

/**
 * Function: getMin
 * Returns the smallest value stored in the buffer
 *  the number that is associated with a buffer
 * @return uint16_t maximum number that is stored within the buffer
 */
uint16_t getMin_uint16_t(uint16_t handle) {
    buffer_uint16_t *theirBuffer = &bufferContainer_uint16_t[handle];
    return theirBuffer->min;
}

/**
 * Function: getMax
 * Retrieves the largest value that has been added to the buffer.
 * @return uint16_t maximum number that is stored within the buffer
 */
uint16_t getMax_uint16_t(uint16_t handle) {
    buffer_uint16_t *theirBuffer = &bufferContainer_uint16_t[handle];
    return theirBuffer->max;
}
typedef struct buffer_uint8_t {
	uint8_t *bufferPt;
	uint8_t maxSize;
	uint8_t *startPt;
	uint8_t *currPt;
	uint8_t *endPt;
	uint16_t currentSize;
	uint8_t min;
	uint8_t max;
	uint16_t resetMinMaxFlag;
	uint16_t resetAvgFlag;
} buffer_uint8_t;

uint16_t handleNumber_uint8_t = 0;
#ifdef __DATA_ARRAYS_COUNT__
	buffer_uint8_t bufferContainer_uint8_t[__DATA_ARRAYS_COUNT__];
	const uint8_t bufferContainerLength_uint8_t = __DATA_ARRAYS_COUNT__;
#else
	#warning need to define how many data arrays you are using 20 is default
	buffer_uint8_t bufferContainer_uint8_t[20];

#endif
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
uint16_t init_uint8_t(uint8_t *userBuffer, uint16_t size) {
                          /* MSP430 */
	buffer_uint8_t *myBuffer = &bufferContainer_uint8_t[handleNumber_uint8_t];
	myBuffer->bufferPt = userBuffer;
	myBuffer->maxSize = size;
	myBuffer->startPt = userBuffer;
	myBuffer->currPt = userBuffer;
	myBuffer->endPt = userBuffer;
	myBuffer->currentSize = 0;
	myBuffer->min = (uint8_t)0;
	myBuffer->max = (uint8_t)0;
	myBuffer->resetMinMaxFlag = 0;
	myBuffer->resetAvgFlag = 0;
	return handleNumber_uint8_t++;
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
void addData_uint8_t(uint16_t handle, uint8_t data) {
	buffer_uint8_t *theirBuffer = &bufferContainer_uint8_t[handle];
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
void resetAll_uint8_t(uint16_t handle) {
	/* Resets*/
	buffer_uint8_t *theirBuffer = &bufferContainer_uint8_t[handle];
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
void resetAvg_uint8_t(uint16_t handle) {
    buffer_uint8_t *theirBuffer = &bufferContainer_uint8_t[handle];
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
void resetMinMax_uint8_t(uint16_t handle) {
    buffer_uint8_t *theirBuffer = &bufferContainer_uint8_t[handle];
    theirBuffer->min = (uint8_t)0;
    theirBuffer->max = (uint8_t)0;
    theirBuffer->resetMinMaxFlag = 1;
    theirBuffer->resetAvgFlag = 0;
}

/**
 * Function: getSum
 * Returns the sum of all numbers stored in the array. If an overflow occurred during calculation,
 * returns zero.
 * @param handle the identifier for the buffer that will be manipulated.
 * @return uint8_t sum of the values within
 */
NEXT_uint8_t getSum_uint8_t(uint16_t handle) {
	NEXT_uint8_t sum = (uint8_t)0;
	buffer_uint8_t *theirBuffer = &bufferContainer_uint8_t[handle];
	uint8_t *curr = theirBuffer->startPt;
	while (curr != theirBuffer->endPt) {
		uint8_t number = *curr;
		sum += number;
		curr++;
   }

    /* handles overflow */

	return sum;
}

/**
 * Function: getAvg
 * Returns the average of value within the current buffer
 *
 * @return uint8_t average of values stored within the buffer.
 */
uint8_t getAvg_uint8_t(uint16_t handle) {
    buffer_uint8_t *theirBuffer = &bufferContainer_uint8_t[handle];
    if (theirBuffer->currentSize == 0) {
        return 0;
    }
    NEXT_uint8_t sum = getSum_uint8_t(handle);
    uint8_t avg = sum / theirBuffer->currentSize;
    return avg;
}

/**
 * Function: getMin
 * Returns the smallest value stored in the buffer
 *  the number that is associated with a buffer
 * @return uint8_t maximum number that is stored within the buffer
 */
uint8_t getMin_uint8_t(uint16_t handle) {
    buffer_uint8_t *theirBuffer = &bufferContainer_uint8_t[handle];
    return theirBuffer->min;
}

/**
 * Function: getMax
 * Retrieves the largest value that has been added to the buffer.
 * @return uint8_t maximum number that is stored within the buffer
 */
uint8_t getMax_uint8_t(uint16_t handle) {
    buffer_uint8_t *theirBuffer = &bufferContainer_uint8_t[handle];
    return theirBuffer->max;
}
