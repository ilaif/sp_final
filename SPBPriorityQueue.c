//
//  SPBPriorityQueue.c
//  Ex2
//
//  Created by Ilai Fallach on 24/12/2016.
//  Copyright © 2016 ilai. All rights reserved.
//

#include <stdlib.h>
#include "stdbool.h"
#include "string.h"
#include "assert.h"

#include "SPBPriorityQueue.h"

struct sp_bp_queue_t {
    int max_size;
    int size;
    BPQueueElement *arr;
};

SPBPQueue *spBPQueueCreate(int maxSize) {
    assert(maxSize > 0);

    SPBPQueue *queue = (SPBPQueue *) malloc(sizeof(SPBPQueue));

    if (queue == NULL) {
        return NULL;
    }

    queue->max_size = maxSize;
    queue->arr = (BPQueueElement *) malloc(sizeof(BPQueueElement) * maxSize);
    queue->size = 0;

    if (queue->arr == NULL) {
        return NULL;
    }

    return queue;
}

SPBPQueue *spBPQueueCopy(SPBPQueue *source) {
    assert(source != NULL);

    SPBPQueue *queue = spBPQueueCreate(source->max_size);

    if (queue == NULL) {
        return NULL;
    }

    memcpy(queue->arr, source->arr, sizeof(BPQueueElement) * queue->max_size);
    queue->size = spBPQueueSize(source);

    return queue;
}

void spBPQueueDestroy(SPBPQueue *source) {
    if (source == NULL) {
        return;
    }

    free(source->arr);
    free(source);
}

void spBPQueueClear(SPBPQueue *source) {
    assert(source != NULL);
    source->size = 0;
}

int spBPQueueSize(SPBPQueue *source) {
    assert(source != NULL);
    return source->size;
}

int spBPQueueGetMaxSize(SPBPQueue *source) {
    assert(source != NULL);
    return source->max_size;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue *source, int index, double value) {
    if (source == NULL || index < 0) {
        return SP_BPQUEUE_INVALID_ARGUMENT;
    }

    BPQueueElement e = {index, value};

    if (spBPQueueIsEmpty(source)) {
        source->arr[0] = e;
        source->size++;
        return SP_BPQUEUE_SUCCESS;
    }

    bool is_full = spBPQueueIsFull(source);

    // Queue is full and the new value is larger than the maximum
    if (is_full) {
        if (value > source->arr[0].value || (value == source->arr[0].value && index > source->arr[0].index)) {
            return SP_BPQUEUE_FULL;
        }
    }

    int cur_size = spBPQueueSize(source);
    if (is_full) {
        bool started_replace = false;
        BPQueueElement last_e = {index, value};
        BPQueueElement tmp_e;
        for (int i = cur_size - 1; i >= 0; i--) {
            if (started_replace) {
                tmp_e = source->arr[i];
                source->arr[i] = last_e;
                last_e = tmp_e;
            } else if (value <= source->arr[i].value) {
                started_replace = true;
                last_e = source->arr[i];
                source->arr[i] = e;
            }
        }
    } else {
        for (int i = cur_size - 1; i >= 0; i--) {
            source->arr[i + 1] = source->arr[i];
            if (value == source->arr[i].value) {
                if (index < source->arr[i].index) {
                    source->arr[i + 1] = e;
                } else {
                    source->arr[i] = e;
                }
                break;
            } else if (value < source->arr[i].value) {
                source->arr[i + 1] = e;
                break;
            }
        }
        if (value > source->arr[0].value) {
            source->arr[0] = e;
        }
        source->size++;
    }

    return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue *source) {
    if (source == NULL) {
        return SP_BPQUEUE_INVALID_ARGUMENT;
    }

    if (spBPQueueIsEmpty(source)) {
        return SP_BPQUEUE_EMPTY;
    }

    source->size--; // Just ignoring the value of the minimum value is like removing it.
    return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue *source, BPQueueElement *res) {
    if (source == NULL || res == NULL) {
        return SP_BPQUEUE_INVALID_ARGUMENT;
    }

    int cur_size = spBPQueueSize(source);
    if (spBPQueueIsEmpty(source)) {
        return SP_BPQUEUE_EMPTY;
    }

    res->index = source->arr[cur_size - 1].index;
    res->value = source->arr[cur_size - 1].value;
    return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue *source, BPQueueElement *res) {
    if (source == NULL || res == NULL) {
        return SP_BPQUEUE_INVALID_ARGUMENT;
    }

    if (spBPQueueIsEmpty(source)) {
        return SP_BPQUEUE_EMPTY;
    }

    res->index = source->arr[0].index;
    res->value = source->arr[0].value;
    return SP_BPQUEUE_SUCCESS;
}

double spBPQueueMinValue(SPBPQueue *source) {
    assert(source != NULL);
    assert(!spBPQueueIsEmpty(source));

    return source->arr[spBPQueueSize(source) - 1].value;
}

double spBPQueueMaxValue(SPBPQueue *source) {
    assert(source != NULL);
    assert(!spBPQueueIsEmpty(source));

    return source->arr[0].value;
}

bool spBPQueueIsEmpty(SPBPQueue *source) {
    assert(source != NULL);

    return spBPQueueSize(source) == 0;
}

bool spBPQueueIsFull(SPBPQueue *source) {
    assert(source != NULL);

    return spBPQueueSize(source) == spBPQueueGetMaxSize(source);
}
