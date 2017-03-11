#ifndef SPKDTREE_H_
#define SPKDTREE_H_

#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPConfig.h"

//TODO: Docs
typedef struct sp_kd_tree_t {
    int d;
    double val;
    struct sp_kd_tree_t *left;
    struct sp_kd_tree_t *right;
    SPPoint *data;
} SPKDTree;

//TODO: Docs
SPKDTree *spKdTreeBuild(SPKDArray *kd_arr, const SPConfig conf);

//TODO:
void *spKdTreeDestroy(SPKDTree *t);

#endif /* SPKDTREE_H_ */
