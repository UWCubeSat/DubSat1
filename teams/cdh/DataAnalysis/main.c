#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "dataArray.h"
// Is there a better way to do this?

int main() {
	TYPE myArray[10];
	uint16_t array1 = init(myArray, 10);
	addData(array1, (uint16_t)5);
	addData(array1, (uint16_t)11);
	addData(array1, (uint16_t)544);
	addData(array1, (uint16_t)245);
	addData(array1, (uint16_t)123);
	addData(array1, (uint16_t)3);
	addData(array1, (uint16_t)324);
	addData(array1, (uint16_t)89);
	addData(array1, (uint16_t)53);
	addData(array1, (uint16_t)20);
	printf("Handle Number: %u\n",array1);
	printArray(array1);
	printAllStats(array1);

	/*
	 * Testing for reset
	 */
	addData(array1, (uint16_t)100);
	printArray(array1);
	printAllStats(array1);

	uint16_t secArray[10];
	uint16_t array2 = init(secArray, 10);
	addData(array2, 10);
	printArray(array2);
	printAllStats(array2);
	/*
	int maxVal = getMax(myArray, 8);
	int minVal = getMin(myArray, 8);
	printf("Avg: %d, Max: %d, Min: %d\n", avg, maxVal, minVal);
	*/
	return 0;
}
