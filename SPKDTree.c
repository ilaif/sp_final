#include <stdlib.h>
#include <assert.h>

#include "SPKDTree.h"

//TODO: Add asserts where needed

// Private methods

/**
 * Finds dimension with maximum spread between maximum and minimum points
 * @param SPKDArray kd_arr to find in
 * @return dimension with maximum spread
 */
int findHighestSpreadDimension(SPKDArray *kd_arr) {
    int dim = spKdArrayDimension(kd_arr), n = spKdArraySize(kd_arr);
    SPPoint **points = spKdArrayGetPoints(kd_arr);
    double max_spread = -1, cur_spread = 0, lowest_coor, highest_coor;
    int max_spread_dim = 0;

    for (int d = 0; d < dim; d++) {
        lowest_coor = spPointGetAxisCoor(points[kd_arr->data[0]], d);
        highest_coor = spPointGetAxisCoor(points[kd_arr->data[n - 1]], d);
        cur_spread = highest_coor - lowest_coor;
        if (cur_spread > max_spread) {
            max_spread = cur_spread;
            max_spread_dim = d;
        }
    }

    return max_spread_dim;
}

/**
 * Helper function to check if a SPKDTree is a leaf by checking that it has not children
 * @param SPKDTree node
 * @return
 */
bool isLeaf(SPKDTree *node) {
    return (node->left == NULL && node->right == NULL);
}

/**
 * Builds a SPKDTree from an SPKDArray recursively
 * @param SPKDArray kd_arr
 * @param SP_KD_TREE_SPLIT_METHOD method:
 * RANDOM - Picks a random dimension for every level of the tree
 * MAX_SPREAD - Picks the dimension with the highest spread
 * INCREMENTAL - Increments dimension in every level module maximum dimension
 * @param int split_dim current dimension to split by (In case of INCREMENTAL only)
 * @return
 */
SPKDTree *buildTree(SPKDArray *kd_arr, SP_KD_TREE_SPLIT_METHOD method, int split_dim) {
    int dim;

    SPKDTree *node = (SPKDTree *) malloc(sizeof(SPKDTree));
    assert(node != NULL);
    node->left = NULL;
    node->right = NULL;

    // Recursive stop rule
    if (spKdArraySize(kd_arr) == 1) {
        node->d = 0;
        node->val = 0;
        node->data = spPointCopy(spKdArrayGetPoints(kd_arr)[0]);
        return node;
    }

    if (method == RANDOM) {
        dim = rand() % spKdArrayDimension(kd_arr);
    } else if (method == MAX_SPREAD) {
        dim = findHighestSpreadDimension(kd_arr);
    } else { // Incremental
        dim = split_dim;
    }

    node->d = dim;
    node->val = spKdArrayMedianByDim(kd_arr, dim);
    SPKDArray *left, *right;
    spKdArraySplit(kd_arr, node->d, &left, &right);
    node->left = buildTree(left, method, (dim + 1) % spKdArrayDimension(kd_arr));
    node->right = buildTree(right, method, (dim + 1) % spKdArrayDimension(kd_arr));
    node->data = NULL;
    return node;
}

// Public header methods

SPKDTree *spKdTreeBuild(SPKDArray *kd_arr, const SPConfig conf) {
    SP_KD_TREE_SPLIT_METHOD method;
    spConfigGetTreeSplitMethod(&method, conf);
    return buildTree(kd_arr, method, 1);
}

void spKdTreeDestroy(SPKDTree *t) {
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

void spKdTreeKNNSearch(SPKDTree *cur, SPBPQueue *bpq, SPPoint *p) {
    double dx, dx2;

    if (cur == NULL) {
        return;
    }

    /* Add the current point to the BPQ. Note that this is a no-op if the
	* point is not as good as the points we've seen so far.*/
    if (isLeaf(cur)) {
        spBPQueueEnqueue(bpq, spPointGetIndex(cur->data), spPointL2SquaredDistance(cur->data, p));
        return;
    }

    dx = cur->val - spPointGetAxisCoor(p, cur->d);
    dx2 = dx * dx;

    /* Recursively search the half of the tree that contains the test point. */
    spKdTreeKNNSearch(dx > 0 ? cur->left : cur->right, bpq, p);

    /* If the queue is not full OR the candidate hypersphere crosses this splitting plane, look on the
	* other side of the plane by examining the other subtree*/
    if (!spBPQueueIsFull(bpq) || dx2 < spBPQueueMaxValue(bpq)) {
        spKdTreeKNNSearch(dx > 0 ? cur->right : cur->left, bpq, p);
    }
}