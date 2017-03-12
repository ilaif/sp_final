#ifndef SPKDTREE_H_
#define SPKDTREE_H_

#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPConfig.h"
#include "SPBPriorityQueue.h"

/**
 * KdTree Node
 */
typedef struct sp_kd_tree_t {
    int d; // Split dimension
    double val; // Median value on split
    struct sp_kd_tree_t *left; // Left node
    struct sp_kd_tree_t *right; // Right node
    SPPoint *data; // Point (if a leaf)
} SPKDTree;

//TODO: Docs
SPKDTree *spKdTreeBuild(SPKDArray *kd_arr, const SPConfig conf);

//TODO:
void spKdTreeDestroy(SPKDTree *t);

//TODO:
void spKdTreeKNNSearch(SPKDTree *cur, SPBPQueue *bpq, SPPoint *p);

#endif /* SPKDTREE_H_ */
