#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataArray.h"

void printArray(int *theirArray, int size) {
	for (int i = 0; i < size; i++) {
		printf("Pos:%d is %d\n", i, theirArray[i]);
	}
}

int getSum(int *theirArray, int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum += theirArray[i];
	}
	return sum;
}

int getAvg(int *theirArray, int size) {
	int average = getSum(theirArray, size) / size;
	return average;
}

int getMax(int *theirArray, int size) {
	int max = theirArray[0];
	for (int i = 0; i < size; i++) {
		if (max < theirArray[i]) {
			max = theirArray[i];
		}
	}
	return max;
}

int getMin(int *theirArray, int size) {
	int min = theirArray[0];
	for (int i = 0; i < size; i++) {
		if (min > theirArray[i]) {
			min = theirArray[i];
		}
	}
	return min;
}
