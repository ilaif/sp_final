#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include <stdbool.h>

/**
 * SP Bounded Priority Queue summary
 *
 * Minimum priority queue of SPListElement elements.
 * Queue is prioritized by the element's value.
 * Queue is bounded by a maximum number of elements maxSize.
 *
 * The following functions are available:
 *
 *   spBPQueueCreate            - Allocates a new queue in the memory.
 *   spBPQueueCopy              - Allocates a copy of the given queue.
 *   spBPQueueDestroy           - Free all memory allocation associated with queue.
 *   spBPQueueClear		      	- Empties a given queue.
 *   spBPQueueSize              - A getter for the current queue's size.
 *   spBPQueueGetMaxSize        - A getter for the current queue's maxSize.
 *   spBPQueueEnqueue           - Inserts an element into the queue.
 *   spBPQueueDequeue           - Removes the minimum value element in the queue.
 *   spBPQueuePeek              - Allocates a new copy of the minimum value element in the queue.
 *   spBPQueuePeekLast          - Allocates a new copy of the maximum value element in the queue.
 *   spBPQueueMinValue          - Returns the value of the minimum element in the queue.
 *   spBPQueueMaxValue          - Returns the value of the maximum element in the queue.
 *	 spBPQueueIsEmpty           - Returns whether the queue is empty.
 *   spBPQueueIsFull            - Returns whether the queue is full.
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t SPBPQueue;

/** type used to define an element in the Bounded priority queue **/
typedef struct sp_bpq_element_t {
	int index;
	double value;
} BPQueueElement;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
    SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new queue in the memory.
 * Given maxSize will return a new queue of that bounded size.
 *
 * @return
 * NULL in case allocation failure ocurred OR maxSize < 1
 * Otherwise, the queue is returned
 */
SPBPQueue* spBPQueueCreate(int maxSize);

/**
 * Allocates a copy of the given queue.
 *
 * @param source - The source queue
 * @return
 * NULL in case memory allocation occurs
 * Others a copy of source is returned.
 */
SPBPQueue* spBPQueueCopy(SPBPQueue* source);

/**
 * Free all memory allocation associated with queue,
 * if queue is NULL nothing happens.
 */
void spBPQueueDestroy(SPBPQueue* source);

/**
 * Empties a given queue (dequeues all elements from the queue).
 */
void spBPQueueClear(SPBPQueue* source);

/**
 * A getter for the current queue's size (number of enqueued elements inside it).
 *
 * @param source - The source queue
 * @assert source != NULL
 * @return
 * The current size of the queue
 */
int spBPQueueSize(SPBPQueue* source);

/**
 * A getter for the current queue's maxSize.
 *
 * @param source - The source queue
 * @assert source != NULL
 * @return
 * maxSize of the queue
 */
int spBPQueueGetMaxSize(SPBPQueue* source);

/**
 * Enqueues a new element to source queue.
 *
 * @param source - target queue.
 * @param index - index to insert to the queue.
 * @param value - value to insert to the queue.
 * @return
 *	SP_BPQUEUE_FULL - If size == maxSize and value >= .
 *	SP_BPQUEUE_INVALID_ARGUMENT - If source is null.
 *	SP_BPQUEUE_SUCCESS - Otherwise.
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value);

/**
 * Dequeues the element with the lowest value if queue is not empty.
 *
 * @param source - target queue.
 * @return
 *	SP_BPQUEUE_INVALID_ARGUMENT - If source is NULL.
 *	SP_BPQUEUE_EMPTY - If queue is empty.
 *	SP_BPQUEUE_SUCCESS - Otherwise.
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source);

/**
 * Returns a copy of the element with the lowest value.
 *
 * @param source - target queue.
 * @param res - pointer to result element.
 * @return
 *	SP_BPQUEUE_INVALID_ARGUMENT - If source is NULL.
 *	SP_BPQUEUE_EMPTY - If queue is empty.
 *	SP_BPQUEUE_SUCCESS - Otherwise.
 */
SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res);

/**
 * Returns a copy of the element with the highest value.
 *
 * @param source - target queue.
 * @param res - pointer to result element.
 * @return
 *	SP_BPQUEUE_INVALID_ARGUMENT - If source is NULL.
 *	SP_BPQUEUE_EMPTY - If queue is empty.
 *	SP_BPQUEUE_SUCCESS - Otherwise.
 */
SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res);

/**
 * A getter for the current queue's element value such that it's value <= all other elements in the queue.
 * @param source - The source queue
 * @assert source != NULL
 * @return
 * NULL If queue is empty,
 * Otherwise, the mininmum value inside the queue.
 */
double spBPQueueMinValue(SPBPQueue* source);

/**
 * A getter for the current queue's element value such that it's value >= all other elements in the queue.
 * @param source - The source queue
 * @assert source != NULL
 * @return
 * NULL If queue is empty,
 * Otherwise, the maximum value inside the queue.
 */
double spBPQueueMaxValue(SPBPQueue* source);

/**
 * Returns whether the queue is empty
 *
 * @assert source != NULL
 * @return
 * true if the queue is empty (of size == 0),
 * Otherwise, false.
 */
bool spBPQueueIsEmpty(SPBPQueue* source);

/**
 * Returns whether the queue is full
 *
 * @assert source != NULL
 * @return
 * true if the queue is full (of size == maxSize),
 * Otherwise, false.
 */
bool spBPQueueIsFull(SPBPQueue* source);

#endif
