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


/**
 * Build the tree by a specific method, adding the nodes recursively while each node contains
 1- Dim = The splitting dimension
 2- Val = The median value of the splitting dimension
 3- Left = Pointer to the left subtree
 4- Right = Pointer to the right subtree
 5- Data = Pointer to a point (only if the current node is a leaf) otherwise this field value is NULL
 *
 * @param kd_arr
 * @param conf configuration file
 * @return the KdTree */
SPKDTree *spKdTreeBuild(SPKDArray *kd_arr, const SPConfig conf);

/**
 * Free all memory allocation associated with spKdTree
 * @param t to free
 */
void spKdTreeDestroy(SPKDTree *t);

/**
 * Recursive k nearest neighbor search algorithm
 * @param cur
 * @param bpq
 * @param p
 */
void spKdTreeKNNSearch(SPKDTree *cur, SPBPQueue *bpq, SPPoint *p);

/**
 * Builds a SPKDTree from an SPKDArray recursively
 * @param SPKDArray kd_arr
 * @param SP_KD_TREE_SPLIT_METHOD method:
 * RANDOM - Picks a random dimension for every level of the tree
 * MAX_SPREAD - Picks the dimension with the highest spread
 * INCREMENTAL - Increments dimension in every level module maximum dimension
 * @param int split_dim current dimension to split by (In case of INCREMENTAL only)
 * @return KDTree
 */
SPKDTree *buildTree(SPKDArray *kd_arr, SP_KD_TREE_SPLIT_METHOD method, int split_dim);

/**
 * Helper function to check if a SPKDTree is a leaf by checking that it has not children
 * @param SPKDTree node
 * @return boolean value of Is leaf node
 */
bool isLeaf(SPKDTree *node);

/**
 * Finds dimension with maximum spread between maximum and minimum points
 * @param SPKDArray kd_arr to do the search on
 * @return dimension with maximum spread
 */
int findHighestSpreadDimension(SPKDArray *kd_arr);

#endif /* SPKDTREE_H_ */
