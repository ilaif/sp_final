//
// Created by Noa Biton on 3/27/2017.
//

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "unit_test_util.h"
#include "../SPKDTree.h"


static SPKDArray *createArr() {
    //int d = 3, n = 8
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
    free(d1);
    free(d2);
    free(d3);
    free(arr);
    return a;
}

static SPKDArray *createBigArr() {
    int d = 3, n = 8;
    SPPoint **arr = (SPPoint **) malloc(sizeof(SPPoint *) * n);

    double *d1 = (double *) malloc(sizeof(double) * d);
    d1[0] = 3, d1[1] = 7, d1[2] = 1;
    SPPoint *p1 = spPointCreate(d1, d, 0);
    arr[0] = p1;

    double *d2 = (double *) malloc(sizeof(double) * d);
    d2[0] = 8, d2[1] = 1, d2[2] = 1;
    SPPoint *p2 = spPointCreate(d2, d, 0);
    arr[1] = p2;

    double *d3 = (double *) malloc(sizeof(double) * d);
    d3[0] = 6, d3[1] = 6, d3[2] = 2;
    SPPoint *p3 = spPointCreate(d3, d, 0);
    arr[2] = p3;

    double *d4 = (double *) malloc(sizeof(double) * d);
    d4[0] = 2, d4[1] = 6, d4[2] = 4;
    SPPoint *p4 = spPointCreate(d4, d, 0);
    arr[3] = p4;

    double *d5 = (double *) malloc(sizeof(double) * d);
    d5[0] = 2, d5[1] = 7, d5[2] = 1;
    SPPoint *p5 = spPointCreate(d5, d, 0);
    arr[4] = p5;

    double *d6 = (double *) malloc(sizeof(double) * d);
    d6[0] = 9, d6[1] = 8, d6[2] = 8;
    SPPoint *p6 = spPointCreate(d6, d, 0);
    arr[5] = p6;

    double *d7 = (double *) malloc(sizeof(double) * d);
    d7[0] = 5, d7[1] = 7, d7[2] = 1;
    SPPoint *p7 = spPointCreate(d7, d, 0);
    arr[6] = p7;

    double *d8 = (double *) malloc(sizeof(double) * d);
    d8[0] = 5, d8[1] = 6, d8[2] = 3;
    SPPoint *p8 = spPointCreate(d8, d, 0);
    arr[7] = p8;

    SPKDArray *a = spKdArrayInit(arr, n);

    free(d1);
    free(d2);
    free(d3);
    free(d4);
    free(d5);
    free(d6);
    free(d7);
    free(d8);
    free(arr);

    return a;
}

static bool testBuildTree() {

    SPKDArray *a = createArr();
    // SPKDTree *tree = *spKdTreeBuild(a, conf);
    SP_KD_TREE_SPLIT_METHOD method = MAX_SPREAD;
    SPKDTree *tree = buildTree(a, method, 3);

    ASSERT_TRUE(tree->d == 2);
    ASSERT_TRUE(tree->data == NULL);
    ASSERT_TRUE(tree->left->right->data != NULL);
    SPPoint *point = tree->left->right->data;

    ASSERT_TRUE(spPointGetAxisCoor(point, 0) == 3);
    ASSERT_TRUE(spPointGetAxisCoor(point, 1) == 2);
    ASSERT_TRUE(spPointGetAxisCoor(point, 2) == 1);

    spKdTreeDestroy(tree);

    SP_KD_TREE_SPLIT_METHOD method2 = INCREMENTAL;
    a = createArr();
    tree = buildTree(a, method2, 1);
    ASSERT_TRUE(tree->left->right->data != NULL);
    point = tree->left->right->data;

    ASSERT_TRUE(spPointGetAxisCoor(point, 0) == 3);
    ASSERT_TRUE(spPointGetAxisCoor(point, 1) == 2);
    ASSERT_TRUE(spPointGetAxisCoor(point, 2) == 1);

    spKdTreeDestroy(tree);
    return true;
}

static bool testFindHighestSpreadDimension() {
    SPKDArray *a = createArr();
    int highDim = findHighestSpreadDimension(a);
    ASSERT_TRUE(highDim == 2);
    spKdArrayPointsDestroy(a);
    spKdArrayDestroy(a);

    a = createBigArr();
    highDim = findHighestSpreadDimension(a);
    ASSERT_TRUE(highDim == 0);
    spKdArrayPointsDestroy(a);
    spKdArrayDestroy(a);

    return true;
}

static bool testIsLeaf() {
    SPKDTree *node = (SPKDTree *) malloc(sizeof(SPKDTree));
    assert(node != NULL);
    node->left = NULL;
    node->right = NULL;
    ASSERT_TRUE(isLeaf(node));
    SPKDTree *leftNode = (SPKDTree *) malloc(sizeof(SPKDTree));
    assert(leftNode != NULL);
    node->left = leftNode;
    ASSERT_FALSE(isLeaf(node));

    free(node);
    free(leftNode);

    return true;
}

static bool testSpKdTreeKNNSearch() {
    SPKDArray *b = createBigArr();
    SPBPQueue *bpq = spBPQueueCreate(3);
    SP_KD_TREE_SPLIT_METHOD method = MAX_SPREAD;
    double data_test[3] = {-1.0, 2.0, 4.0};
    int dim_test = 2;
    int index_test = 1;
    SPPoint *point = spPointCreate((double *) data_test, dim_test, index_test);
    SPKDTree *tree = buildTree(b, method, 1);
    spKdTreeKNNSearch(tree, bpq, point);
    ASSERT_TRUE(spBPQueueIsFull(bpq));
    //TODO: Finish this test
    BPQueueElement *currentSourceElement = (BPQueueElement *) malloc(sizeof(BPQueueElement));
    spBPQueuePeek(bpq, currentSourceElement);
    spBPQueueDequeue(bpq);
    ASSERT_TRUE(currentSourceElement->value == 25);
    spBPQueuePeek(bpq, currentSourceElement);
    spBPQueueDequeue(bpq);
    ASSERT_TRUE(currentSourceElement->value == 43);
    spBPQueuePeek(bpq, currentSourceElement);
    spBPQueueDequeue(bpq);
    ASSERT_TRUE(currentSourceElement->value == 53);
    spPointDestroy(point);
    return true;
}

int main() {
    RUN_TEST(testBuildTree);
    //TOOD:
    //RUN_TEST(testSpKdTreeKNNSearch);
    RUN_TEST(testFindHighestSpreadDimension);
    RUN_TEST(testIsLeaf);

    return 0;
}