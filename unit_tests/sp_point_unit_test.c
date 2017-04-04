#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>


static SPPoint* CreateSspointForTest() {
    double data_test[2] = {1.0,2.0};
    int dim_test = 2;
    int index_test = 1;
    SPPoint* point = spPointCreate((double *) data_test, dim_test, index_test);
    return point;
}

static bool test_pointCreate() {
    int dataLength = rand() % 100;
    int index = 1;
    double num;
    double* data = (double*) malloc(sizeof(double) * dataLength);
    int i;
    for (i = 0; i < dataLength; i++) {
        num = rand() % 100;
        data[i] = num;
    }
    SPPoint* p = spPointCreate(data, dataLength, index);
    ASSERT_TRUE(spPointGetIndex(p) == index);
    ASSERT_TRUE(spPointGetDimension(p) == dataLength);
    int j;
    for (j = 0; j < spPointGetDimension(p); j++) {
        ASSERT_TRUE(spPointGetAxisCoor(p, j) == data[j]);
    }
    free(data);
    spPointDestroy(p);
    return true;
}

bool pointBasicCopyTest() {
    SPPoint* p = CreateSspointForTest();
    SPPoint* point_copy = spPointCopy(p);
    ASSERT_TRUE(spPointGetIndex(p) == spPointGetIndex(point_copy));
    ASSERT_TRUE(spPointGetDimension(p) == spPointGetDimension(point_copy));
    for (int i = 0; i < spPointGetDimension(p); i++) {
        ASSERT_TRUE(spPointGetAxisCoor(p, i) == spPointGetAxisCoor(point_copy, i));
    }
    spPointDestroy(p);
    spPointDestroy(point_copy);
    return true;
}

bool pointBasicL2Distance() {
    double data1[2] = { 1.0, 1.0 };
    double data2[2] = { 1.0, 0.0 };
    int dim1 = 2;
    int dim2 = 2;
    int index1 = 1;
    int index2 = 1;
    SPPoint* p = spPointCreate(data1, dim1, index1);
    SPPoint* q = spPointCreate(data2, dim2, index2);
    ASSERT_TRUE(spPointL2SquaredDistance(p,p) == 0.0);
    ASSERT_TRUE(spPointL2SquaredDistance(q,q) == 0.0);
    ASSERT_FALSE(spPointL2SquaredDistance(p,q) == 0.0);
    spPointDestroy(p);
    spPointDestroy(q);
    return true;
}

static bool test_pointL2DistanceRes() {
    double data[2] = { 1.5, 1.2 };
    int dim = 2;
    int index = 1;
    SPPoint* p = spPointCreate((double *) data, dim, index);
    ASSERT_TRUE(spPointL2SquaredDistance(p, p) == 0.0);
    spPointDestroy(p);
    return true;
}

static bool test_GetCoor() {
    SPPoint* p = CreateSspointForTest();
    ASSERT_TRUE(spPointGetAxisCoor(p, 0) == 1.0);
    ASSERT_TRUE(spPointGetAxisCoor(p, 1) == 2.0);
    spPointDestroy(p);
    return true;
}

static bool test_GetDimension() {
    SPPoint* p = CreateSspointForTest();
    ASSERT_TRUE(spPointGetDimension(p) == 2);
    spPointDestroy(p);
    return true;
}

static bool test_GetIndex() {
    SPPoint* p = CreateSspointForTest();
    ASSERT_TRUE(spPointGetIndex(p) == 1);
    
    spPointDestroy(p);
    return true;
}

int main() {
    RUN_TEST(test_pointCreate);
    RUN_TEST(pointBasicCopyTest);
    RUN_TEST(pointBasicL2Distance);
    RUN_TEST(test_GetIndex);
    RUN_TEST(test_GetDimension);
    RUN_TEST(test_GetCoor);
    RUN_TEST(test_pointL2DistanceRes);
    return 0;
}
