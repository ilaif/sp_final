#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_

#include "SPPoint.h"

//TODO: Docs
typedef struct sp_kd_array_t {
    int d;
    int n;
    int *data;
    SPPoint **points;
} SPKDArray;

//TODO: Docs
SPKDArray *spKdArrayInit(SPPoint **arr, int size);

//TODO: Docs
void spKdArraySplit(SPKDArray *kd_arr, int coor, SPKDArray **left, SPKDArray **right);

//TODO: Docs
double spKdArrayMedianByDim(SPKDArray *kd_arr, int dim);

//TODO: Docs
int spKdArraySize(SPKDArray *kd_arr);

//TODO: Docs
int spKdArrayDimension(SPKDArray *kd_arr);

//TODO: Docs
SPPoint **spKdArrayGetPoints(SPKDArray *kd_arr);

//TODO: Docs
void spKdArrayDestroy(SPKDArray *kd_arr);

#endif /* SPKDARRAY_H_ */
