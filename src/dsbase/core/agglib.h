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

typedef enum {
    TYPE_DOUBLE,
    TYPE_FLOAT,
    TYPE_INT,
} vector_type;

// DO NOT INITIALIZE THIS STRUCT.
typedef struct {
    uint16_t avgSumCount;
    uint16_t minCount;
    uint16_t maxCount;
    vector_type type;
} aggVec;

// INITIALIZE ONE OF THE 3 BELOW AND CAST.
typedef struct {
    aggVec meta;
    double min;
    double max;
    double sum;
} aggVec_d;

typedef struct {
    aggVec meta;
    float min;
    float max;
    float sum;
} aggVec_f;

typedef struct {
    aggVec meta;
    int32_t min;
    int32_t max;
    int32_t sum;
    int64_t squareSum;
    uint8_t varianceEnabled;
} aggVec_i;

/**
 * Initialize a new vector struct.
 * Takes a pointer to a vector struct as an argument.
 * Allocate aggVec_f or aggVec_i and pass in it's pointer to initialize.
 */
void aggVec_init_d(aggVec_d* vector);
void aggVec_init_f(aggVec_f* vector);
void aggVec_init_i(aggVec_i* vector);

void aggVec_init_i_Var(aggVec_i* vector); //initialize with variance enabled

/**
 * Push a value to a vector and updates the aggregates.
 * Takes a pointer to a vector struct and the value to add.
 * Return 0 if successful. 1 = Vector Length overflow.
 */
uint8_t aggVec_push_d(aggVec_d* vector, double val);
uint8_t aggVec_push_f(aggVec_f* vector, float val);
uint8_t aggVec_push_i(aggVec_i* vector, int32_t val);

// Or you can cast aggVec_d*, aggVec_f*, or aggVec_i* to aggVec*, and cast your &double to a void*
// I STRONGLY recommend you don't use this one.
uint8_t aggVec_push(aggVec* vector, void* val);
/**
 * Reset all aggregates and clear out the vector
 * Takes in a pointer the vector as an argument.
 */

void aggVec_reset(aggVec* vector);

/**
 * Reset min and max aggregates.
 * Takes in a pointer to the vector as an argument. Cast aggVec_d*, aggVec_f*, or aggVec_i* to aggVec*, it'll work.
 */
void aggVec_mm_reset(aggVec* vector);

 /**
 * Reset min aggregates.
 * Takes in a pointer to the vector as an argument. Cast aggVec_d*, aggVec_f*, or aggVec_i* to aggVec*, it'll work.
 */
void aggVec_min_reset(aggVec* vector);

/**
 * Reset max aggregates.
 * Takes in a pointer to the vector as an argument. Cast aggVec_d*, aggVec_f*, or aggVec_i* to aggVec*, it'll work.
 */
void aggVec_max_reset(aggVec* vector);

/**
 * Reset average and sum aggregates.
 * Takes in a pointer to the vector as an argument. Cast aggVec_d*, aggVec_f*, or aggVec_i* to aggVec*, it'll work.
 */
void aggVec_as_reset(aggVec* vector);

/**
 * Get the min of a vector.
 * Returns 0 if there's nothing in the array.
 */

double aggVec_min_d(aggVec_d* vector);
float aggVec_min_f(aggVec_f* vector);
int32_t aggVec_min_i(aggVec_i* vector);

// or you can cast to aggVec and do a pointer to the value you want to update.
// I STRONGLY recommend you don't use this one.
void aggVec_min(aggVec* vector, void* val);

/**
 * Get the max of a vector.
 * Returns 0 if there's nothing in the array.
 */

double aggVec_max_d(aggVec_d* vector);
float aggVec_max_f(aggVec_f* vector);
int32_t aggVec_max_i(aggVec_i* vector);

// or you can cast to aggVec and do a pointer to the value you want to update.
// I STRONGLY recommend you don't use this one.
void aggVec_max(aggVec* vector, void* val);

/**
 * Get the count of pushes since last min reset.
 */
uint16_t aggVec_min_count(aggVec* vector);


 /**
 * Get the count of pushes since last max reset.
 */
 uint16_t aggVec_max_count(aggVec* vector);

/**
 * Get the count of pushes since last avg/sum reset.
 */
uint16_t aggVec_as_count(aggVec* vector);

/**
 * Get the sum of values since last avg/sum reset.
 */

double aggVec_sum_d(aggVec_d* vector);
float aggVec_sum_f(aggVec_f* vector);
int32_t aggVec_sum_i(aggVec_i* vector);

// or you can cast to aggVec and do a pointer to the value you want to update.
// I STRONGLY recommend you don't use this one.
void aggVec_sum(aggVec* vector, void* val);

/**
 * Get the average of a vector. (As a float or an int for int arrays)
 *
 * READ AS: AggVector of (float/int): get average as (float/int)
 *
 * Returns DBL_MAX / FLT_MAX / INT_MAX if there's nothing in the array.
 */
double aggVec_avg_d(aggVec_d* vector);
float aggVec_avg_f(aggVec_f* vector);

// Get the average as a float
float aggVec_avg_i_f(aggVec_i* vector);

// Get the average as an int (C integer dvision)
int32_t aggVec_avg_i_i(aggVec_i* vector);

//get the variance as a float
float aggVec_var_i_f(aggVec_i* vector);


// or you can cast to aggVec and do a pointer to the value you want to update.
// I STRONGLY recommend you don't use this one.
void aggVec_avg(aggVec* vector, void* val);

#endif /* AGGLIB_H */
