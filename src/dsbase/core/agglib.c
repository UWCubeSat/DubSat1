/*
 * agglib.c
 *
 *  Created on: 10 May 2017
 *      Author: Emory Eng on UW Satellite Team
 */

#include "agglib.h"

void aggVec_init_d(aggVec_d* vector) {
    vector->meta.type = TYPE_DOUBLE;
    aggVec_reset((aggVec *) vector);
}
void aggVec_init_f(aggVec_f* vector) {
    vector->meta.type = TYPE_FLOAT;
    aggVec_reset((aggVec *) vector);
}
void aggVec_init_i(aggVec_i* vector) {
    vector->meta.type = TYPE_INT;
    aggVec_reset((aggVec *) vector);
}

void aggVec_init_i_Var(aggVec_i* vector) {
    aggVec_init_i(vector);
    vector->varianceEnabled = 1;
}

uint8_t aggVec_push_d(aggVec_d* vector, double val){ return aggVec_push((aggVec *) vector, (void *) &val);}
uint8_t aggVec_push_f(aggVec_f* vector, float val){return aggVec_push((aggVec *) vector, (void *) &val);}
uint8_t aggVec_push_i(aggVec_i* vector, int32_t val){return aggVec_push((aggVec *) vector, (void *) &val);}

uint8_t aggVec_push(aggVec* vector, void* val) {
    if (vector->avgSumCount == UINT16_MAX) return 1;
    // Code for integer.
    if (vector->type == TYPE_INT) {
        int32_t add = *(int32_t *) val;
        aggVec_i *vector_i = (aggVec_i *) vector;
        vector_i->sum += add;
        if (add < vector_i->min || vector->minCount == 0) vector_i->min = add;
        if (vector_i->max < add || vector->maxCount == 0) vector_i->max = add;
        if (vector_i->varianceEnabled)
            vector_i->squareSum += add * add;
    }
    // Code for float
    else if (vector->type == TYPE_FLOAT) {
        float add = *(float *) val;
        aggVec_f *vector_f = (aggVec_f *) vector;
        vector_f->sum += add;
        if (add < vector_f->min || vector->minCount == 0) vector_f->min = add;
        if (vector_f->max < add || vector->maxCount == 0) vector_f->max = add;
    }
    // Code for double
    else if (vector->type == TYPE_DOUBLE) {
        double add = *(double *) val;
        aggVec_d *vector_d = (aggVec_d *) vector;
        vector_d->sum += add;
        if (add < vector_d->min || vector->minCount == 0) vector_d->min = add;
        if (vector_d->max < add || vector->maxCount == 0) vector_d->max = add;
    }
    vector->avgSumCount++;
    if(vector->minCount ^ UINT16_MAX)
        vector->minCount++;
    if(vector->maxCount ^ UINT16_MAX)
        vector->maxCount++;
    return 0;
}

void aggVec_reset(aggVec* vector) {
    aggVec_mm_reset(vector);
    aggVec_as_reset(vector);
}

void aggVec_mm_reset(aggVec* vector) {
  aggVec_min_reset(vector);
  aggVec_max_reset(vector);
}

void aggVec_min_reset(aggVec* vector) {
    if (vector->type == TYPE_DOUBLE){
        aggVec_d *vec = (aggVec_d *) vector;
        vec->min = 0;
        vec->meta.minCount = 0;
    }
    else {
        aggVec_i *vec = (aggVec_i *) vector;
        vec->min = 0;
        vec->meta.minCount = 0;
    }
}

void aggVec_max_reset(aggVec* vector) {
    if (vector->type == TYPE_DOUBLE){
        aggVec_d *vec = (aggVec_d *) vector;
        vec->max = 0;
        vec->meta.maxCount = 0;
    }
    else {
        aggVec_i *vec = (aggVec_i *) vector;
        vec->max = 0;
        vec->meta.maxCount = 0;
    }
}

void aggVec_as_reset(aggVec* vector) {
    if (vector->type == TYPE_DOUBLE){
        aggVec_d *vec = (aggVec_d *) vector;
        vec->sum = 0;
        vec->meta.avgSumCount = 0;
    }
    else {
        aggVec_i *vec = (aggVec_i *) vector;
        vec->sum = 0;
        vec->meta.avgSumCount = 0;
        if(vector->type == TYPE_INT)
            vec->squareSum = 0;
    }
}

double aggVec_min_d(aggVec_d* vector){ double out; aggVec_min((aggVec *) vector, &out); return out; }
float aggVec_min_f(aggVec_f* vector){ float out; aggVec_min((aggVec *) vector, &out); return out; }
int32_t aggVec_min_i(aggVec_i* vector){ int32_t out; aggVec_min((aggVec *) vector, &out); return out; }

void aggVec_min(aggVec* vector, void* val) {
    if (vector->type == TYPE_DOUBLE){
        *(uint64_t *)(val) = *((uint64_t *)(vector + 1) + 0);
    }
    else {
        *(uint32_t *)(val) = *((uint32_t *)(vector + 1) + 0);
    }
}

double aggVec_max_d(aggVec_d* vector){ double out; aggVec_max((aggVec *) vector, &out); return out; }
float aggVec_max_f(aggVec_f* vector){ float out; aggVec_max((aggVec *) vector, &out); return out; }
int32_t aggVec_max_i(aggVec_i* vector){ int32_t out; aggVec_max((aggVec *) vector, &out); return out; }

void aggVec_max(aggVec* vector, void* val) {
    if (vector->type == TYPE_DOUBLE){
        *(uint64_t *)(val) = *((uint64_t *)(vector + 1) + 1);
    }
    else {
        *(uint32_t *)(val) = *((uint32_t *)(vector + 1) + 1);
    }
}

uint16_t aggVec_min_count(aggVec* vector) { return vector->minCount; }
uint16_t aggVec_max_count(aggVec* vector) { return vector->maxCount; }
uint16_t aggVec_as_count(aggVec* vector) { return vector->avgSumCount; }

double aggVec_sum_d(aggVec_d* vector){ double out; aggVec_sum((aggVec *) vector, &out); return out; }
float aggVec_sum_f(aggVec_f* vector){ float out; aggVec_sum((aggVec *) vector, &out); return out; }
int32_t aggVec_sum_i(aggVec_i* vector){ int32_t out; aggVec_sum((aggVec *) vector, &out); return out; }

void aggVec_sum(aggVec* vector, void* val) {
    if (vector->type == TYPE_DOUBLE){
        *(uint64_t *)(val) = *((uint64_t *)(vector + 1) + 2);
    }
    else {
        *(uint32_t *)(val) = *((uint32_t *)(vector + 1) + 2);
    }
}

double aggVec_avg_d(aggVec_d* vector){ double out; aggVec_avg((aggVec *) vector, &out); return out; }
float aggVec_avg_f(aggVec_f* vector){ float out; aggVec_avg((aggVec *) vector, &out); return out; }
float aggVec_avg_i_f(aggVec_i* vector){ float out; aggVec_avg((aggVec *) vector, &out); return out; }

void aggVec_avg(aggVec* vector, void* val) {
    if (vector->type == TYPE_DOUBLE){
        if (!vector->avgSumCount) {
            *(double *)(val) = DBL_MAX;
            return;
        }
        *(double *)(val) = *((double *)(vector + 1) + 2) / vector->avgSumCount;
    }
    else if (vector->type == TYPE_FLOAT) {
        if (!vector->avgSumCount) {
            *(float *)(val) = FLT_MAX;
            return;
        }
        *(float *)(val) = *((float *)(vector + 1) + 2) / vector->avgSumCount;
    }
    else if (vector->type == TYPE_INT) {
        if (!vector->avgSumCount) {
            *(float *)(val) = FLT_MAX;
            return;
        }
        *(float *)(val) = ((float) *((int32_t *)(vector + 1) + 2)) / vector->avgSumCount;
    }
}

int32_t aggVec_avg_i_i(aggVec_i* vector) {
    if (!vector->meta.avgSumCount) return INT32_MAX;
    return vector->sum / vector->meta.avgSumCount;
}

float aggVec_var_i_f(aggVec_i* vector)
{
    if(vector->varianceEnabled && vector->meta.avgSumCount)
        return (float)(((double)vector->squareSum - ((double)vector->sum * vector->sum) / vector->meta.avgSumCount) / (vector->meta.avgSumCount - 1)); //calculate variance
    else
        return FLT_MAX;
}
