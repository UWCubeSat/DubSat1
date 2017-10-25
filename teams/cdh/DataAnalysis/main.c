#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dataArray.h"

void main() {
	dataArray *x = initArray();
	/* -----test-----*/
	int value1 = 1;
	int value2 = 6;
	addValue(x, value1);
	addValue(x, value2);
	addValue(x, value2);
	addValue(x, value2);
	addValue(x, value2);
	addValue(x, value2);
	addValue(x, value2);
	value2 = 4;
	addValue(x, value2);
	addValue(x, value2);
	addValue(x, value2);
	value2 = 3;
	addValue(x, value2);
	addValue(x, value2);
	addValue(x, value2);
	printArray(x);
	int min = getMin(x);
	int max = getMax(x);
	int avg = getAvg(x);
	printf("Min: %d, Max: %d, Average: %d\n", min, max, avg);

}
