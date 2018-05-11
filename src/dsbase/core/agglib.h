 /*
  * agglib.h
  *
  *  Created on: 10 May 2017
  *      Author: Emory Eng on UW Satellite Team
  */

 #ifndef AGGLIB_H
 #define AGGLIB_H

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdint.h>
 #include <float.h>

 /**
  * Vector Struct
  */
 typedef struct {
 	float min;
 	float max;
 	float sum;
 	uint16_t avgSumCount;
 	uint16_t minMaxCount;
 } aggVec_f;

 typedef struct {
 	int32_t min;
 	int32_t max;
 	int32_t sum;
 	uint16_t avgSumCount;
 	uint16_t minMaxCount;
 } aggVec_i;

 /**
  * Initialize a new vector struct.
  * Takes a pointer to a vector struct as an argument.
  * Use AGGVEC_F or AGGVEC_I for a float vector or int vector respectively
  */
 void aggVec_init(void* vector);

 /**
  * Push a value to a vector and updates the aggregates.
  * Takes a pointer to a vector struct and the value to add.
  * Return 0 if successful. 1 = Vector Length overflow.
  */
 uint8_t aggVec_f_push(aggVec_f* vector, float val);
 uint8_t aggVec_i_push(aggVec_i* vector, int32_t val);

 /**
  * Reset all aggregates and clear out the vector
  * Takes in a pointer the vector as an argument.
  */
 void aggVec_reset(void* vector);

 /**
  * Reset min and max aggregates.
  * Takes in a pointer to the vector as an argument.
  */
 void aggVec_mm_reset(void* vector);

 /**
  * Reset average and sum aggregates.
  * Takes in a pointer to the vector as an argument.
  */
 void aggVec_as_reset(void* vector);

 /**
  * Get the min of a vector.
  * Returns 0 if there's nothing in the array.
  */
 float aggVec_f_min(aggVec_f* vector);
 int32_t aggVec_i_min(aggVec_i* vector);

 /**
  * Get the max of a vector.
  * Returns 0 if there's nothing in the array.
  */
 float aggVec_f_max(aggVec_f* vector);
 int32_t aggVec_i_max(aggVec_i* vector);

 /**
  * Get the count of pushes since last min/max reset.
  */
 float aggVec_f_mm_count(aggVec_f* vector);
 int32_t aggVec_i_mm_count(aggVec_i* vector);

 /**
  * Get the count of pushes since last avg/sum reset.
  */
 float aggVec_f_as_count(aggVec_f* vector);
 int32_t aggVec_i_as_count(aggVec_i* vector);

 /**
  * Get the sum of values since last avg/sum reset.
  */
 float aggVec_f_sum(aggVec_f* vector);
 int32_t aggVec_i_sum(aggVec_i* vector);


 /**
  * Get the average of a vector. (As a float or an int for int arrays)
  *
  * READ AS: AggVector of (float/int): get average as (float/int)
  *
  * Returns FLT_MAX / INT_MAX if there's nothing in the array.
  */
 float aggVec_f_avg_f(aggVec_f* vector);
 float aggVec_i_avg_f(aggVec_i* vector);
 int32_t aggVec_i_avg_i(aggVec_i* vector);

 #endif /* AGGLIB_H */
