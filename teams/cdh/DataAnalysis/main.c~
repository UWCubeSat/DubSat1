#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "dataArray.h"
// Is there a better way to do this?

int main() {
	uint16_t myArray[10];
	uint16_t array1 = init_uint16_t(myArray, 10);
	addData_uint16_t(array1, (uint16_t)5);
	addData_uint16_t(array1, (uint16_t)11);
	addData_uint16_t(array1, (uint16_t)544);
	addData_uint16_t(array1, (uint16_t)245);
	addData_uint16_t(array1, (uint16_t)123);
	addData_uint16_t(array1, (uint16_t)3);
	addData_uint16_t(array1, (uint16_t)324);
	addData_uint16_t(array1, (uint16_t)89);
	addData_uint16_t(array1, (uint16_t)53);
	addData_uint16_t(array1, (uint16_t)20);
	printf("Handle Number: %u\n",array1);
	printArray_uint16_t(array1);
	printAllStats_uint16_t(array1);

	/*
	 * Testing for reset
	 */
	addData_uint16_t(array1, (uint16_t)100);
	printArray_uint16_t(array1);
	printAllStats_uint16_t(array1);

	uint16_t secArray[10];
	uint16_t array2 = init_uint16_t(secArray, 10);
  //	addData(array2, 10);
	printArray_uint16_t(array2);
	printAllStats_uint16_t(array2);
  for(int i=0;i<10;i++){
    addData_uint16_t(array2, 7000);
  }
	printArray_uint16_t(array2);
	printAllStats_uint16_t(array2);
	/*
	int maxVal = getMax(myArray, 8);
	int minVal = getMin(myArray, 8);
	printf("Avg: %d, Max: %d, Min: %d\n", avg, maxVal, minVal);
	*/
	float myArray4[10];
	uint16_t array4 = init_float(myArray4, 10);
	addData_float(array4, (float)5.2);
	addData_float(array4, (float)6.3);
	addData_float(array4, (float)8.1);
	addData_float(array4, (float)-10.8);
	addData_float(array4, (float)100.2);
	addData_float(array4, (float)56.8);
	addData_float(array4, (float)91.4);
	addData_float(array4, (float)43.5);
	addData_float(array4, (float)3.2);
	addData_float(array4, (float)1.1);
	printf("Handle Number: %u\n",array4);
	printArray_float(array4);
	printAllStats_float(array4);

	return 0;
}
