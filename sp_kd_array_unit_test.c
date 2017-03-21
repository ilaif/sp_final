#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h"
#include "SPKDArray.h"
#include "SPPoint.h"

/**
 * Tests that building an array returns sorted indices per dimension correctly.
 * @return whether successful
 */
static bool testKdArrayOrder() {
    int d = 3, n = 3;
    SPPoint **arr = (SPPoint **) malloc(sizeof(SPPoint *) * n);

    double *d1 = (double *) malloc(sizeof(double) * d);
    d1[0] = 3, d1[1] = 2, d1[2] = 1;
    SPPoint *p1 = spPointCreate(d1, d, 0);
    arr[0] = p1;

    double *d2 = (double *) malloc(sizeof(double) * d);
    d2[0] = 0, d2[1] = 8, d2[2] = 4;
    SPPoint *p2 = spPointCreate(d2, d, 0);
    arr[1] = p2;

    double *d3 = (double *) malloc(sizeof(double) * d);
    d3[0] = -5, d3[1] = 0, d3[2] = -10;
    SPPoint *p3 = spPointCreate(d3, d, 0);
    arr[2] = p3;

    SPKDArray *a = spKdArrayInit(arr, n);

    // Correct d=1 order: -5, 0, 3 | Index: (2, 1, 0)
    ASSERT_TRUE(spKdArrayGetDataIndex(a, 0, 0) == 2);
    ASSERT_TRUE(spKdArrayGetDataIndex(a, 0, 1) == 1);
    ASSERT_TRUE(spKdArrayGetDataIndex(a, 0, 2) == 0);
    // Correct d=2 order: 0, 2, 8 | Index: (2, 0, 1)
    ASSERT_TRUE(spKdArrayGetDataIndex(a, 1, 0) == 2);
    ASSERT_TRUE(spKdArrayGetDataIndex(a, 1, 1) == 0);
    ASSERT_TRUE(spKdArrayGetDataIndex(a, 1, 2) == 1);
    // Correct d=3 order: -10, 1, 4 | Index: (2, 0, 1)
    ASSERT_TRUE(spKdArrayGetDataIndex(a, 2, 0) == 2);
    ASSERT_TRUE(spKdArrayGetDataIndex(a, 2, 1) == 0);
    ASSERT_TRUE(spKdArrayGetDataIndex(a, 2, 2) == 1);

    free(arr);
    free(d1);
    free(d2);
    free(d3);
    spKdArrayPointsDestroy(a);
    spKdArrayDestroy(a);

    return true;
}

/**
 * Tests that building an array and splitting it returns two split arrays ordered accordingly.
 * @return whether successful
 */
static bool testKdArraySplit() {
    int d = 3, n = 3;
    SPPoint **arr = (SPPoint **) malloc(sizeof(SPPoint *) * n);

    double *d1 = (double *) malloc(sizeof(double) * d);
    d1[0] = 3, d1[1] = 2, d1[2] = 1;
    SPPoint *p1 = spPointCreate(d1, d, 0);
    arr[0] = p1;

    double *d2 = (double *) malloc(sizeof(double) * d);
    d2[0] = 0, d2[1] = 8, d2[2] = 4;
    SPPoint *p2 = spPointCreate(d2, d, 0);
    arr[1] = p2;

    double *d3 = (double *) malloc(sizeof(double) * d);
    d3[0] = -5, d3[1] = 0, d3[2] = -10;
    SPPoint *p3 = spPointCreate(d3, d, 0);
    arr[2] = p3;

    SPKDArray *a = spKdArrayInit(arr, n);
    SPKDArray *l, *r;

    int split_dim = 1;
    spKdArraySplit(a, split_dim, &l, &r);

    // l - points: p3, p1 | order by dim: (1, 0), (1, 0), (1, 0)
    ASSERT_TRUE(spKdArrayDimension(l) == 3);
    ASSERT_TRUE(spKdArraySize(l) == 2);
    ASSERT_TRUE(spKdArrayGetDataIndex(l, 0, 0) == 1);
    ASSERT_TRUE(spKdArrayGetDataIndex(l, 0, 1) == 0);
    ASSERT_TRUE(spKdArrayGetDataIndex(l, 1, 0) == 1);
    ASSERT_TRUE(spKdArrayGetDataIndex(l, 1, 1) == 0);
    ASSERT_TRUE(spKdArrayGetDataIndex(l, 2, 0) == 1);
    ASSERT_TRUE(spKdArrayGetDataIndex(l, 2, 1) == 0);

    // r - points: p2 | order by dim: (0), (0), (0)
    ASSERT_TRUE(spKdArrayDimension(r) == 3);
    ASSERT_TRUE(spKdArraySize(r) == 1);
    ASSERT_TRUE(spKdArrayGetDataIndex(r, 0, 0) == 0);
    ASSERT_TRUE(spKdArrayGetDataIndex(r, 1, 0) == 0);
    ASSERT_TRUE(spKdArrayGetDataIndex(r, 2, 0) == 0);

    free(arr);
    free(d1);
    free(d2);
    free(d3);
    spKdArrayPointsDestroy(a);
    spKdArrayDestroy(a);
    spKdArrayDestroy(l);
    spKdArrayDestroy(r);

    return true;
}

int main() {
    RUN_TEST(testKdArrayOrder);
    RUN_TEST(testKdArraySplit);
    return 0;
}
