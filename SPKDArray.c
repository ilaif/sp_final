#include "assert.h"
#include <stdlib.h>

#include "SPKDArray.h"

// Private methods

/**
 * Comparator function for and item which is a 2-d array that holds value in [0] and an index in [1].
 * @param int[2] x
 * @param int[2] y
 * @return
 */
int sort(const void *x, const void *y) {
    const int *a = x;
    const int *b = y;
    if (a[0] == b[0])
        return a[1] - b[1];
    else
        return a[0] - b[0];
}

// Public header methods

double spKdArrayMedianByDim(SPKDArray *kd_arr, int dim) {
    assert(kd_arr != NULL);
    int n = kd_arr->n;
    int median_i = dim * n + n / 2;
    double p1, p2;

    if (n % 2 == 0) {
        // if there is an even number of elements, return mean of the two elements in the middle
        p1 = spPointGetAxisCoor(kd_arr->points[kd_arr->data[median_i]], dim);
        p2 = spPointGetAxisCoor(kd_arr->points[kd_arr->data[median_i - 1]], dim);
        return ((p1 + p2) / 2.0);
    } else {
        // else return the element in the middle
        p1 = spPointGetAxisCoor(kd_arr->points[kd_arr->data[median_i]], dim);
        return p1;
    }
}

int spKdArraySize(SPKDArray *kd_arr) {
    assert(kd_arr != NULL);
    return kd_arr->n;
}

int spKdArrayDimension(SPKDArray *kd_arr) {
    assert(kd_arr != NULL);
    return kd_arr->d;
}

SPPoint **spKdArrayGetPoints(SPKDArray *kd_arr) {
    assert(kd_arr != NULL);
    return kd_arr->points;
}

void spKdArrayDestroy(SPKDArray *kd_arr) {
    if (kd_arr == NULL) {
        return;
    }
    free(kd_arr->data);
    free(kd_arr->points); // TODO: Correct?
    free(kd_arr);
}

SPKDArray *spKdArrayInit(SPPoint **arr, int size) {
    if (arr == NULL || size == 0) return NULL;
    SPKDArray *kd = (SPKDArray *) malloc(sizeof(SPKDArray));
    if (kd == NULL) return NULL;

    int dim = spPointGetDimension(arr[0]);
    int d, j;
    kd->n = size;
    kd->d = dim;
    kd->data = (int *) malloc(sizeof(int) * kd->d * kd->n);
    if (kd->data == NULL) return NULL;
    kd->points = (SPPoint **) malloc(sizeof(*kd->points) * kd->n);
    if (kd->points == NULL) return NULL;
    for (d = 0; d < kd->n; d++) {
        kd->points[d] = spPointCopy(arr[d]);
    }

    int *dim_arr = (int *) malloc(sizeof(int) * kd->n * 2);

    // For every dimension
    for (d = 0; d < kd->d; d++) {

        // For every dimension point
        for (j = 0; j < kd->n; j++) {
            dim_arr[j * 2 + 0] = (int) spPointGetAxisCoor(arr[j], d);
            dim_arr[j * 2 + 1] = j;
        }

        // Sort the points over the current dimension d
        qsort(dim_arr, (size_t) kd->n, sizeof(dim_arr[0]), sort);

        // Assign the sorted indices of the points over that dimension
        for (j = 0; j < kd->n; j++) {
            kd->data[d * kd->n + j] = dim_arr[j * 2 + 1];
        }
    }

    free(dim_arr);
    return kd;
}

void spKdArraySplit(SPKDArray *kd_arr, int coor, SPKDArray **left, SPKDArray **right) {
    int n = kd_arr->n, dim = kd_arr->d;
    int X[n];
    int half_n = (n + 2 - 1) / 2;
    int d, j;

    // Create the partition
    for (d = 0; d < n; d++) {
        X[kd_arr->data[coor * kd_arr->n + d]] = (d >= half_n) ? 1 : 0;
    }

    // Create left and right point arrays and mappings
    SPPoint **p1 = (SPPoint **) malloc(sizeof(*p1) * half_n);
    assert(p1 != NULL);
    SPPoint **p2 = (SPPoint **) malloc(sizeof(*p2) * (n - half_n));
    assert(p2 != NULL);
    int p1_j = 0, p2_j = 0;
    int map_1[n], map_2[n];
    for (d = 0; d < n; d++) {
        if (X[d] == 0) {
            p1[p1_j] = spPointCopy(kd_arr->points[d]);
            map_1[d] = p1_j;
            map_2[d] = -1;
            p1_j++;
        } else {
            p2[p2_j] = spPointCopy(kd_arr->points[d]);
            map_2[d] = p2_j;
            map_1[d] = -1;
            p2_j++;
        }
    }

    // Create left and right kd arrays
    *left = (SPKDArray *) malloc(sizeof(SPKDArray));
    assert(left != NULL);
    *right = (SPKDArray *) malloc(sizeof(SPKDArray));
    assert(right != NULL);
    (*left)->d = dim;
    (*left)->n = half_n;
    (*left)->points = p1;
    (*left)->data = (int *) malloc(sizeof(int) * (*left)->d * (*left)->n);
    assert((*left)->data);
    (*right)->d = dim;
    (*right)->n = n - half_n;
    (*right)->points = p2;
    (*right)->data = (int *) malloc(sizeof(int) * (*right)->d * (*right)->n);
    assert((*right)->data);

    for (d = 0; d < dim; d++) {
        p1_j = 0, p2_j = 0;
        for (j = 0; j < kd_arr->n; j++) {

            // Map the original indices to the split indices
            if (map_1[kd_arr->data[d * kd_arr->n + j]] != -1) {
                (*left)->data[d * (*left)->n + p1_j] = map_1[kd_arr->data[d * kd_arr->n + j]];
                p1_j++;
            } else {
                (*right)->data[d * (*right)->n + p2_j] = map_2[kd_arr->data[d * kd_arr->n + j]];
                p2_j++;
            }
        }
    }
}
