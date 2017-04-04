#include "../SPBPriorityQueue.h"
#include "unit_test_util.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static SPBPQueue* createQueue_WithValues_ForTest() {
	SP_BPQUEUE_MSG msg = SP_BPQUEUE_SUCCESS;

	SPBPQueue* queueForTest = spBPQueueCreate(5);
	ASSERT_TRUE(queueForTest != NULL);

	ASSERT_TRUE(spBPQueueEnqueue(queueForTest, 1, 2.0) == msg);
	ASSERT_TRUE(spBPQueueEnqueue(queueForTest, 2, 5.0) == msg);
	ASSERT_TRUE(spBPQueueEnqueue(queueForTest, 3, 3.0) == msg);
	ASSERT_TRUE(spBPQueueEnqueue(queueForTest, 4, 4.0) == msg);
	ASSERT_TRUE(spBPQueueEnqueue(queueForTest, 5, 1.0) == msg);
    
	return queueForTest;
}

static SPBPQueue* createQueueForTest(int maxSize) {
	SPBPQueue* queueForTest = spBPQueueCreate(maxSize);
	ASSERT_TRUE(queueForTest != NULL);
    
	return queueForTest;
}

static bool test_spBPQueueDestroy() {
	SPBPQueue* queueToDelete = createQueueForTest(1);
	spBPQueueDestroy(queueToDelete);
    
	return true;
}

static bool test_spBPQueueCreate() {
	SPBPQueue* testedQueue = createQueueForTest(1);
	ASSERT_TRUE(testedQueue != NULL);
	spBPQueueDestroy(testedQueue);
    
	return true;
}

static bool test_spBPQueueCopy() {
	int i;
	bool compareRes = true;


	SPBPQueue* source = createQueueForTest(1);
	SPBPQueue* destinationQueue = spBPQueueCopy(source);
	ASSERT_TRUE(destinationQueue != NULL);

	ASSERT_TRUE(spBPQueueSize(source) == spBPQueueSize(destinationQueue));
	ASSERT_TRUE(spBPQueueGetMaxSize(source) == spBPQueueGetMaxSize(destinationQueue));

    BPQueueElement* currentSourceElement = (BPQueueElement*) malloc(sizeof(BPQueueElement));
    BPQueueElement* currentDestElement = (BPQueueElement*) malloc(sizeof(BPQueueElement));
    
	for (i = 0; i < spBPQueueSize(source); i++) {
        spBPQueuePeek(source, currentSourceElement);
        spBPQueueDequeue(source);
        spBPQueuePeek(source, currentDestElement);
        spBPQueueDequeue(source);
        
        compareRes = (currentSourceElement->index == currentDestElement->index)
						&& (currentSourceElement->value == currentDestElement->value);
		if (compareRes) {
			break;
		}
	}

	ASSERT_TRUE(compareRes);

    free(currentSourceElement);
    free(currentDestElement);
	spBPQueueDestroy(source);
	spBPQueueDestroy(destinationQueue);

	return true;
}

static bool test_spBPQueueClear() {
	SPBPQueue* queueToClear = createQueueForTest(1);
	spBPQueueClear(queueToClear);

	ASSERT_TRUE(spBPQueueSize(queueToClear) == 0);
	ASSERT_TRUE(spBPQueueIsEmpty(queueToClear));

	spBPQueueDestroy(queueToClear);

	return true;
}

static bool test_spBPQueueSize() {
	SP_BPQUEUE_MSG msg = SP_BPQUEUE_SUCCESS;

	SPBPQueue* queue = createQueue_WithValues_ForTest();

	ASSERT_TRUE(spBPQueueSize(queue) == 5);
	ASSERT_TRUE(spBPQueueDequeue(queue) == msg);
	ASSERT_TRUE(spBPQueueSize(queue) == 4);
	ASSERT_TRUE(spBPQueueEnqueue(queue, 7, 7.0) == msg);
	ASSERT_TRUE(spBPQueueSize(queue) == 5);
	ASSERT_TRUE(spBPQueueEnqueue(queue, 10, 10.0) == SP_BPQUEUE_FULL);
	ASSERT_TRUE(spBPQueueSize(queue) == 5);
	spBPQueueClear(queue);
	ASSERT_TRUE(spBPQueueSize(queue) == 0);

	//release resource
	spBPQueueDestroy(queue);

	return true;
}

static bool test_spBPQueueGetMaxSize() {
	int maxSize;
	SPBPQueue* queue = createQueueForTest(1);

	maxSize = spBPQueueGetMaxSize(queue);
	ASSERT_TRUE(maxSize == 1);
	//release resource
	spBPQueueDestroy(queue);
	return true;
}

static bool test_spBPQueueEnqueue() {
	SPBPQueue* queue = createQueueForTest(3);

	SP_BPQUEUE_MSG msg = SP_BPQUEUE_SUCCESS;

	ASSERT_TRUE((spBPQueueEnqueue(queue,1,1.0)) == msg);
	// Check if the first element is the one we inserted
    BPQueueElement* currentSourceElement = (BPQueueElement*) malloc(sizeof(BPQueueElement));
    spBPQueuePeek(queue, currentSourceElement);
	ASSERT_TRUE(currentSourceElement->index == 1);
	ASSERT_TRUE(currentSourceElement->value == 1.0);

	spBPQueueEnqueue(queue, 2, 3.0);

	// After the next insertion, the array will be full
 	spBPQueueEnqueue(queue,4,2.0);
 	ASSERT_TRUE(spBPQueueIsFull(queue));

    //Checks if the array is sorted
    spBPQueueDequeue(queue);
    spBPQueuePeek(queue, currentSourceElement);
    ASSERT_TRUE(currentSourceElement->value == 2.0);
    spBPQueueDequeue(queue);

    //Test case of trying Enqueue an existing value with lower index
    spBPQueueEnqueue(queue,2,2.0);
 	//Checks if the element with the same value & lower index is in the current array, in place [1]
 	//The array should look like >> [{1,1.0},{2,2.0},{2,3.0}]
    spBPQueuePeek(queue, currentSourceElement);
 	ASSERT_TRUE(currentSourceElement->index == 2);

    free(currentSourceElement);
	spBPQueueDestroy(queue);

	return true;
}

static bool test_spBPQueueDequeue() {
	SP_BPQUEUE_MSG msg = SP_BPQUEUE_SUCCESS;
	SPBPQueue* queue = createQueue_WithValues_ForTest();
    BPQueueElement* elem_copy = (BPQueueElement*) malloc(sizeof(BPQueueElement));
	ASSERT_TRUE(spBPQueueDequeue(queue) == msg);
	
    //Check the lowest value after the deletion, should be 2.0 because we deleted 1.0
	spBPQueuePeek(queue, elem_copy);
	ASSERT_TRUE(elem_copy->value = 2.0);

    spBPQueueDestroy(queue);
    free(elem_copy);
    
    return true;
}



static bool test_spBPQueuePeek() {
	SPBPQueue* queue = createQueue_WithValues_ForTest();
    BPQueueElement* elem_copy = (BPQueueElement*) malloc(sizeof(BPQueueElement));
    // The copy should contain the lowest value - 0.1
	spBPQueuePeek(queue, elem_copy);
    ASSERT_TRUE(elem_copy->index == 5);
	ASSERT_TRUE(elem_copy->value == 1.0);
	//release resource
	spBPQueueDestroy(queue);
    free(elem_copy);
	return true;

}

static bool test_spBPQueuePeekLast() {
	SPBPQueue* queue = createQueue_WithValues_ForTest();
	BPQueueElement* elem_copy = (BPQueueElement*) malloc(sizeof(BPQueueElement));

	 //Test peek Last

	spBPQueuePeekLast(queue, elem_copy);
	// The copy should contain the highest value - 0.1

	ASSERT_TRUE(elem_copy->index == 2);
	ASSERT_TRUE(elem_copy->value == 5.0);

	SP_BPQUEUE_MSG msg = SP_BPQUEUE_SUCCESS;
	ASSERT_TRUE((spBPQueueEnqueue(queue,6,4.5))== msg);
	spBPQueuePeekLast(queue,elem_copy);
	ASSERT_TRUE(elem_copy->index == 6);
	ASSERT_TRUE(elem_copy->value == 4.5);
	
    spBPQueueDestroy(queue);
    free(elem_copy);

	return true;
}

static bool test_spBPQueueMinValue() {
	SPBPQueue* queue = createQueue_WithValues_ForTest();
	ASSERT_TRUE(spBPQueueMinValue(queue) == 1.0);

	//release resource
	spBPQueueDestroy(queue);

	return true;
}

static bool test_spBPQueueMaxValue() {
	SPBPQueue* queue = createQueue_WithValues_ForTest();
	ASSERT_TRUE(spBPQueueMaxValue(queue) == 5.0);

	//release resource
	spBPQueueDestroy(queue);

	return true;
}

static bool test_spBPQueueIsEmpty() {
	SPBPQueue* queue = createQueueForTest(5);
	spBPQueueClear(queue);
	ASSERT_TRUE(spBPQueueIsEmpty(queue));

	//release resource
	spBPQueueDestroy(queue);

	return true;
}

static bool test_spBPQueueIsFull() {
	SPBPQueue* queue = createQueue_WithValues_ForTest();

	ASSERT_TRUE(spBPQueueIsFull(queue));
	spBPQueueDequeue(queue);
	ASSERT_FALSE(spBPQueueIsFull(queue));

	//release resource
	spBPQueueDestroy(queue);

	return true;
}

int main() {
	RUN_TEST(test_spBPQueueCreate);
	RUN_TEST(test_spBPQueueCopy);
	RUN_TEST(test_spBPQueueDestroy);
	RUN_TEST(test_spBPQueueClear);
	RUN_TEST(test_spBPQueueSize);
	RUN_TEST(test_spBPQueueGetMaxSize);
	RUN_TEST(test_spBPQueueEnqueue);
	RUN_TEST(test_spBPQueueDequeue);
	RUN_TEST(test_spBPQueuePeek);
	RUN_TEST(test_spBPQueuePeekLast);
	RUN_TEST(test_spBPQueueMinValue);
	RUN_TEST(test_spBPQueueMaxValue);
	RUN_TEST(test_spBPQueueIsEmpty);
	RUN_TEST(test_spBPQueueIsFull);

	return 0;
}
