#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "dataArray.h"

void main() {
	uint16_t myArray[10];
	int array1 = init(myArray, 10);
	addData(array1, (uint16_t)5);
	addData(array1, (uint16_t)11);
	addData(array1, (uint16_t)544);
	addData(array1, (uint16_t)245);
	addData(array1, (uint16_t)123);
	addData(array1, (uint16_t)3);
	addData(array1, (uint16_t)324);
	addData(array1, (uint16_t)89);
	addData(array1, (uint16_t)53);
	addData(array1, (uint16_t)1);
	addData(array1, (uint16_t)100);

	printArray(array1);
	uint16_t sum = getSum(array1);
	printf("Sum: %u\n", sum);
	uint16_t min = getMin(array1);
	uint16_t max = getMax(array1);
	printf("Min: %lu, Max: %lu\n", min, max);
	/*
	int maxVal = getMax(myArray, 8);
	int minVal = getMin(myArray, 8);
	printf("Avg: %d, Max: %d, Min: %d\n", avg, maxVal, minVal);
	*/
}
