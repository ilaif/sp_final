#include <stdlib.h>

#include "SPKDTree.h"

//TODO: Add asserts where needed

// Private methods

//TODO: Docs
int findHighestSpreadDimension(SPKDArray *kd_arr) {
    int dims = spKdArrayDimension(kd_arr), n = spKdArraySize(kd_arr);
    SPPoint **points = spKdArrayGetPoints(kd_arr);
    double max_spread = 0, cur_spread = 0;
    int max_spread_dim = 0;

    for (int d = 0; d < dims; d++) {
        cur_spread = spPointGetAxisCoor(points[n - 1], d) - spPointGetAxisCoor(points[0], d);
        if (cur_spread >= max_spread) {
            max_spread = cur_spread;
            max_spread_dim = d;
        }
    }

    return max_spread_dim;
}

//TODO: Docs
SPKDTree *buildTree(SPKDArray *kd_arr, SP_KD_TREE_SPLIT_METHOD method, int split_dim) {

    SPKDTree *node = (SPKDTree *) malloc(sizeof(SPKDTree));
    node->left = NULL;
    node->right = NULL;

    // Recursive stop rule
    if (spKdArraySize(kd_arr) == 1) {
        node->d = 0;
        node->val = 0;
        node->data = spKdArrayGetPoints(kd_arr)[0];
        return node;
    }

    int dim;
    if (method == RANDOM) {
        dim = rand() % spKdArrayDimension(kd_arr);
    } else if (method == MAX_SPREAD) {
        dim = findHighestSpreadDimension(kd_arr);
    } else { // Incremental
        dim = split_dim;
    }

    node->d = dim;
    node->val = spKdArrayMedianByDim(kd_arr, dim);     //TODO: Test median!
    SPKDArray *left, *right;
    spKdArraySplit(kd_arr, node->d, &left, &right);
    node->left = buildTree(left, method, dim + 1);
    node->right = buildTree(right, method, dim + 1);
    node->data = NULL;
    return node;
}

// Public header methods

SPKDTree *spKdTreeBuild(SPKDArray *kd_arr, const SPConfig conf) {
    SP_KD_TREE_SPLIT_METHOD method;
    spConfigGetTreeSplitMethod(&method, conf);
    return buildTree(kd_arr, method, 1);
}

void *spKdTreeDestroy(SPKDTree *t) {
    if (t->left == NULL && t->right == NULL) {
        // just free memory
    } else if (t->left == NULL) {
        spKdTreeDestroy(t->right);
    } else if (t->right == NULL) {
        spKdTreeDestroy(t->left);
    } else {
        spKdTreeDestroy(t->right);
        spKdTreeDestroy(t->left);
    }
    free(t);
}