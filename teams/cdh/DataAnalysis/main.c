#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dataArray.h"

void main() {
	int myArray[] = { 1,3,-2,5,9,8,14,-12 };
	printArray(myArray, 8);
	int avg = getAvg(myArray, 8);
	int maxVal = getMax(myArray, 8);
	int minVal = getMin(myArray, 8);
	printf("Avg: %d, Max: %d, Min: %d\n", avg, maxVal, minVal);
}
