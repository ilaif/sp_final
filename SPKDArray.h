#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_

#include "SPPoint.h"


typedef struct sp_kd_array_t {
    int d;
    int n;
    int *data;
    SPPoint **points;
} SPKDArray;

/**
 * Initializes the kd-array with the data given by arr
 * @param arr to initialize
 * @param size to allocate
 * @return initialized kd-array
 */
SPKDArray *spKdArrayInit(SPPoint **arr, int size);

/**
 * Returns two kd-arrays (kdLeft, kdRight) such that
 * the first ⌈𝒏/𝟐⌉  points with respect to the coordinate coor are in kdLeft ,
 * and the rest of the points are in kdRight
 * @param kd_arr
 * @param coor to split the points by
 * @param left
 * @param right
 */

 void spKdArraySplit(SPKDArray *kd_arr, int coor, SPKDArray **left, SPKDArray **right);


/**
 *
 * @param kd_arr
 * @param dim
 * @return the element in the middle of the spKdArray
 */
double spKdArrayMedianByDim(SPKDArray *kd_arr, int dim);

/**
 *
 * @param kd_arr
 * @return  n member which refers to the size of the array
 */
int spKdArraySize(SPKDArray *kd_arr);
/**
 *return d member which refers to the dimension of the array
 * @param kd_arr
 * @return d member which refers to the dimension of the array
 */
int spKdArrayDimension(SPKDArray *kd_arr);

/**
 * returns the array of points
 * @param kd_arr
 * @return the array of points
 */
SPPoint **spKdArrayGetPoints(SPKDArray *kd_arr);

/**
 *
 * @param kd_arr
 * @param d
 * @param n
 * @return the index of the point ordered by dim
 */
int spKdArrayGetDataIndex(SPKDArray *kd_arr, int d, int n);

/**
 * Deallocate spKdArray allocated memory
 * @param kd_arr
 */
void spKdArrayDestroy(SPKDArray *kd_arr);

/**
 * Memory deallocation
 * @param kd_arr
 */
void spKdArrayPointsDestroy(SPKDArray *kd_arr);

#endif /* SPKDARRAY_H_ */
