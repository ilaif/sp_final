//
//  sPPoint.c
//  Ex2
//
//  Created by Ilai Fallach on 23/12/2016.
//  Copyright © 2016 ilai. All rights reserved.
//

#include "assert.h"
#include <stdlib.h>

#include "SPPoint.h"

struct sp_point_t {
    int index;
    int dim;
    double* data;
};

SPPoint* spPointCreate(double* data, int dim, int index) {
    int i;
    
    if (data == NULL || dim <= 0 || index < 0) {
        return NULL;
    }
    
    SPPoint* point = (SPPoint*) malloc(sizeof(SPPoint));
    point->dim = dim;
    point->index = index;
    point->data = (double*) malloc(sizeof(double) * dim);
    
    if (point->data == NULL) {
        return NULL;
    }
    
    for (i = 0; i < dim; i++) {
        point->data[i] = data[i];
    }
    
    return point;
}

SPPoint* spPointCopy(SPPoint* source) {
    int i;
    assert(source != NULL);
    
    SPPoint* point = (SPPoint*) malloc(sizeof(SPPoint));
    
    if (point == NULL) {
        return NULL;
    }
    
    point->dim = source->dim;
    point->index = source->index;
    point->data = (double*) malloc(sizeof(double) * point->dim);
    
    if (point->data == NULL) {
        return NULL;
    }
    
    for (i = 0; i < point->dim; i++) {
        point->data[i] = source->data[i];
    }
    
    return point;
}

void spPointDestroy(SPPoint* point) {
    if (point != NULL) {
        free(point->data);
        free(point);
    }
}

int spPointGetDimension(SPPoint* point) {
    assert(point != NULL);
    
    return point->dim;
}

int spPointGetIndex(SPPoint* point) {
    assert(point != NULL);
    
    return point->index;
}

double spPointGetAxisCoor(SPPoint* point, int axis) {
    assert(point != NULL && axis < point->dim);
    
    return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint* p, SPPoint* q) {
    assert(p != NULL && q != NULL && p->dim == q->dim);
    
    double result = 0;
    int i;
    double dim_dist;
    
    for (i = 0; i < p->dim; i++) {
        dim_dist = (p->data[i] - q->data[i]);
        result += (dim_dist * dim_dist);
    }
    
    return result;
}
