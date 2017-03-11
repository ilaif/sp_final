#include <iostream>

extern "C" {
#include "SPLogger.h"
#include "SPConfig.h"
#include "SPKDArray.h"
#include "SPPoint.h"
#include "SPKDTree.h"
}

#include "main_aux.h"

#define ENTER_IMAGE_PATH "Please enter an image path:\n"
#define EXITING "Exiting...\n"
#define EXIT_CODE "<>"

int main(int argc, char *argv[]) {

    // Init config
    SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(msg));
    SPConfig conf = loadConfig(argc, argv, msg);
    if (*msg != SP_CONFIG_SUCCESS) {
        return 1;
    }

    // Init logger
    SP_LOGGER_MSG logger_msg = initLogger(conf);
    if (logger_msg != SP_LOGGER_SUCCESS) {
        return 1;
    }

    // TODO: TEST
    int n = 5;
    int d = 2;
    SPPoint **p = (SPPoint **) malloc(sizeof(*p) * n);
    double *data = (double *) malloc(sizeof(double) * d);
    data[0] = 1;
    data[1] = 2;
    p[0] = spPointCreate(data, d, 0);
    data[0] = 123;
    data[1] = 70;
    p[1] = spPointCreate(data, d, 0);
    data[0] = 2;
    data[1] = 7;
    p[2] = spPointCreate(data, d, 0);
    data[0] = 9;
    data[1] = 11;
    p[3] = spPointCreate(data, d, 0);
    data[0] = 3;
    data[1] = 4;
    p[4] = spPointCreate(data, d, 0);
    SPKDArray *arr = spKdArrayInit(p, n);
    int i, j;
    for (i = 0; i < d; i++) {
        for (j = 0; j < n; j++) {
            printf("%d, ", arr->data[i * n + j]);
        }
        printf("\n");
    }
    /*SPKDArray *left = {}, *right = {};
    spKdArraySplit(arr, 0, &left, &right);*/

    SPKDTree *t = spKdTreeBuild(arr, conf);

    free(p);
    free(data);
    spKdArrayDestroy(arr);
    spKdTreeDestroy(t);

    char inp[BUF_SIZE];
    while (true) {

        printf(ENTER_IMAGE_PATH);

        // Read input
        scanf("%s", inp);

        if (strcmp(inp, EXIT_CODE) == 0) {
            printf(EXITING);
            break;
        }
    }

    clearConfig(conf, msg);
    clearLogger();
    return 0;
}
