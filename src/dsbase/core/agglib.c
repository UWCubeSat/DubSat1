/*
 * agglib.c
 *
 *  Created on: 10 May 2017
 *      Author: Emory Eng on UW Satellite Team
 */

#include "agglib.h"

void aggVec_init(void* vector) {
	aggVec_reset(vector);
}

uint8_t aggVec_f_push(aggVec_f* vector, float val) {
	// Check to make sure we have enough space
	if (vector->avgSumCount == UINT16_MAX || vector->minCount == UINT16_MAX) return 1;

	// Update sum
	vector->sum += val;

	// Update min if val is less than the min, or if its the first val
	if (val < vector->min || vector->minCount == 0) vector->min = val;

	// Update max if the max is less than val, or if its the first val
	if (vector->max < val || vector->minCount == 0) vector->max = val;

    // Increment the counts
    vector->avgSumCount++;
    vector->minCount++;
    vector->maxCount++;


	// Return success:
	return 0;
}
uint8_t aggVec_i_push(aggVec_i* vector, int32_t val) {
	if (vector->avgSumCount == UINT16_MAX || vector->minCount == UINT16_MAX) return 1;
	vector->sum += val;
	if (val < vector->min || vector->minCount == 0) vector->min = val;
	if (vector->max < val || vector->minCount == 0) vector->max = val;
    vector->avgSumCount++;
    vector->minCount++;
    vector->maxCount++;
	return 0;
}

void aggVec_reset(void* vector) {
	aggVec_mm_reset(vector);
	aggVec_as_reset(vector);
}

void aggVec_min_reset(void* vector) {
  aggVec_i *vec = (aggVec_i *) vector;
	vec->min = 0;
	vec->minCount = 0;
}

void aggVec_max_reset(void* vector) {
  aggVec_i *vec = (aggVec_i *) vector;
	vec->max = 0;
	vec->maxCount = 0;
}

void aggVec_mm_reset(void* vector) {
  aggVec_min_reset(vector);
  aggVec_max_reset(vector);
}

void aggVec_as_reset(void* vector) {
  aggVec_i *vec = (aggVec_i *) vector;
	vec->sum = 0;
	vec->avgSumCount = 0;
}

float aggVec_f_min(aggVec_f* vector) {
	return vector->min;
}
int32_t aggVec_i_min(aggVec_i* vector) {
	return vector->min;
}

float aggVec_f_max(aggVec_f* vector) {
	return vector->max;
}
int32_t aggVec_i_max(aggVec_i* vector) {
	return vector->max;
}

float aggVec_f_min_count(aggVec_f* vector) {
	return vector->minCount;
}
int32_t aggVec_i_min_count(aggVec_i* vector) {
	return vector->minCount;
}

float aggVec_f_max_count(aggVec_f* vector) {
	return vector->maxCount;
}
int32_t aggVec_i_max_count(aggVec_i* vector) {
	return vector->maxCount;
}

float aggVec_f_as_count(aggVec_f* vector) {
	return vector->avgSumCount;
}
int32_t aggVec_i_as_count(aggVec_i* vector) {
	return vector->avgSumCount;
}

float aggVec_f_sum(aggVec_f* vector) {
	return vector->sum;
}
int32_t aggVec_i_sum(aggVec_i* vector) {
	return vector->sum;
}

float aggVec_f_avg_f(aggVec_f* vector){
	return vector->sum / vector->avgSumCount;
}
float aggVec_i_avg_f(aggVec_i* vector){
		return (float) (vector->sum) / vector->avgSumCount;
}
int32_t aggVec_i_avg_i(aggVec_i* vector){
			return vector->sum / vector->avgSumCount;
}
