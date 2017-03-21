#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_

#include "SPPoint.h"


typedef struct sp_kd_array_t {
    int d;
    int n;
    int *data;
    SPPoint **points;
} SPKDArray;

/**Initializes the kd-array with the data given by arr**/
SPKDArray *spKdArrayInit(SPPoint **arr, int size);

/**Returns two kd-arrays (kdLeft, kdRight) such that
// the first ⌈𝒏/𝟐⌉  points with respect to the coordinate coor are in kdLeft ,
and the rest of the points are in kdRight.**/
void spKdArraySplit(SPKDArray *kd_arr, int coor, SPKDArray **left, SPKDArray **right);

/**returns the element in the middle of the spKdArray**/
double spKdArrayMedianByDim(SPKDArray *kd_arr, int dim);

/*return n member which refers to the size of the array*/
int spKdArraySize(SPKDArray *kd_arr);

/*return d member which refers to the dimension of the array*/
int spKdArrayDimension(SPKDArray *kd_arr);

/*returns the array of points*/
SPPoint **spKdArrayGetPoints(SPKDArray *kd_arr);

/*Deallocate spKdArray allocated memory*/
void spKdArrayDestroy(SPKDArray *kd_arr);

#endif /* SPKDARRAY_H_ */
