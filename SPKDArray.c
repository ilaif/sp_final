#include "assert.h"
#include <stdlib.h>

#include "SPKDArray.h"

// Private methods

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

    if (n % 2 == 0) {
        // if there is an even number of elements, return mean of the two elements in the middle
        return ((kd_arr->data[median_i] + kd_arr->data[median_i - 1]) / 2.0);
    } else {
        // else return the element in the middle
        return kd_arr->data[median_i];
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

    if (arr == NULL) {
        //TODO: Exception?
        return NULL;
    }

    if (size == 0) {
        //TODO: Exception?
        return NULL;
    }

    SPKDArray *kd = (SPKDArray *) malloc(sizeof(SPKDArray));

    if (kd == NULL) {
        //TODO: Allocation failed
        return NULL;
    }

    int d = spPointGetDimension(arr[0]);
    int i, j;
    //TODO: Do we need to check if the other points are also of dim d?
    kd->n = size;
    kd->d = d;
    kd->data = (int *) malloc(sizeof(int) * kd->d * kd->n);
    kd->points = (SPPoint **) malloc(sizeof(*kd->points) * kd->n);
    for (i = 0; i < kd->n; i++) {
        kd->points[i] = spPointCopy(arr[i]);
    }

    if (kd->data == NULL) {
        //TODO: Allocation failed
        return NULL;
    }

    int dim_arr[kd->n][2]; // TODO: should we malloc this?
    for (i = 0; i < kd->d; i++) {

        for (j = 0; j < kd->n; j++) {
            dim_arr[j][0] = (int) spPointGetAxisCoor(arr[j], i);
            dim_arr[j][1] = j;
        }

        qsort(dim_arr, (size_t) kd->n, sizeof(dim_arr[0]), sort);

        for (j = 0; j < kd->n; j++) {
            kd->data[i * kd->n + j] = dim_arr[j][1];
        }
    }

    return kd;
}

void spKdArraySplit(SPKDArray *kd_arr, int coor, SPKDArray **left, SPKDArray **right) {
    int n = kd_arr->n, d = kd_arr->d;
    int x[n]; // TODO: should we malloc this?
    int half_n = (n + 2 - 1) / 2;
    int i, j;

    // Create the partition
    for (i = 0; i < n; i++) {
        x[kd_arr->data[coor * kd_arr->n + i]] = (i >= half_n) ? 1 : 0;
    }

    // Create left and right point arrays and mappings
    SPPoint **p1 = (SPPoint **) malloc(sizeof(*p1) * half_n);
    SPPoint **p2 = (SPPoint **) malloc(sizeof(*p2) * (n - half_n));
    int p1_j = 0, p2_j = 0;
    int map_1[n], map_2[n]; // TODO: Should malloc?
    for (i = 0; i < n; i++) {
        if (x[i] == 0) {
            p1[p1_j] = spPointCopy(kd_arr->points[i]);
            map_1[i] = p1_j;
            map_2[i] = -1;
            p1_j++;
        } else {
            p2[p2_j] = spPointCopy(kd_arr->points[i]);
            map_2[i] = p2_j;
            map_1[i] = -1;
            p2_j++;
        }
    }

    // Create left and right kd arrays
    *left = (SPKDArray *) malloc(sizeof(SPKDArray));
    *right = (SPKDArray *) malloc(sizeof(SPKDArray));
    (*left)->d = d;
    (*left)->n = half_n;
    (*left)->points = p1;
    (*left)->data = (int *) malloc(sizeof(int) * (*left)->d * (*left)->n);
    (*right)->d = d;
    (*right)->n = n - half_n;
    (*right)->points = p2;
    (*right)->data = (int *) malloc(sizeof(int) * (*right)->d * (*right)->n);

    for (i = 0; i < d; i++) {
        p1_j = 0, p2_j = 0;
        for (j = 0; j < kd_arr->n; j++) {

            // Map the original indices to the split indices
            if (map_1[kd_arr->data[i * kd_arr->n + j]] != -1) {
                (*left)->data[i * (*left)->n + p1_j] = map_1[kd_arr->data[i * kd_arr->n + j]];
                p1_j++;
            } else {
                (*right)->data[i * (*right)->n + p2_j] = map_2[kd_arr->data[i * kd_arr->n + j]];
                p2_j++;
            }
        }
    }

    /*int dim = 0;

    printf("\n---- X PARTITION\n");
    for (i = 0; i < n; i++) {
        printf(" %d , ", x[i]);
    }
    printf("\n---- ORIG KD ARRAY\n");
    for (i = 0; i < n; i++) {
        printf(" %d , ", kdArr->data[dim * kdArr->n + i]);
    }
    printf("\n---- LEFT POINTS \n");
    for (i = 0; i < half_n; i++) {
        printf(" %lf , ", spPointGetAxisCoor((*left)->points[i], dim));
    }
    printf("\n---- RIGHT POINTS \n");
    for (i = 0; i < n - half_n; i++) {
        printf(" %lf , ", spPointGetAxisCoor((*right)->points[i], dim));
    }
    printf("\n---- LEFT MAP\n");
    for (i = 0; i < n; i++) {
        printf("%d, ", map_1[i]);
    }
    printf("\n---- RIGHT MAP\n");
    for (i = 0; i < n; i++) {
        printf("%d, ", map_2[i]);
    }
    printf("\n---- LEFT INDICES\n");
    for (i = 0; i < half_n; i++) {
        printf("%d, ", (*left)->data[dim * (*left)->n + i]);
    }
    printf("\n---- RIGHT INDICES\n");
    for (i = 0; i < n - half_n; i++) {
        printf("%d, ", (*right)->data[dim * (*right)->n + i]);
    }
    printf("\n----\n");*/
}