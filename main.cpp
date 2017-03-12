#include <iostream>

extern "C" {
#include "SPLogger.h"
#include "SPConfig.h"
#include "SPKDArray.h"
#include "SPKDTree.h"
}

#include "main_aux.h"
#include "SPImageProc.h"

#define ENTER_IMAGE_PATH "Please enter an image path:\n"
#define EXITING "Exiting...\n"
#define EXIT_CODE "<>"

using namespace sp;

//TODO: Docs + Move to aux
int reversed_compare(const void *x, const void *y) {
    const int *a = (const int *) x;
    const int *b = (const int *) y;
    if (a[0] == b[0])
        return b[1] - a[1];
    else
        return b[0] - a[0];
}

int main(int argc, char *argv[]) {

    int i, j, l, num_of_features, num_images, total_num_features, num_similar_images, knn, minimal_gui;
    bool is_extract;
    char inp[BUF_SIZE], img_path[BUF_SIZE];
    ImageProc *imp;
    int *images_features_num_of_features; // Keep array of num of features per image
    //int *image_knn_cou;
    SPPoint **features, **all_features, ***images_features;
    BPQueueElement *res;

    // Init config
    SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(msg));
    SPConfig conf = loadConfig(argc, argv, msg);
    if (*msg != SP_CONFIG_SUCCESS) return 1;

    // Init logger
    SP_LOGGER_MSG logger_msg = initLogger(conf);
    if (logger_msg != SP_LOGGER_SUCCESS) return 1;

    // TODO: TEST
    /*int n = 5;
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
    *//*SPKDArray *left = {}, *right = {};
    spKdArraySplit(arr, 0, &left, &right);*//*
    SPKDTree *t = spKdTreeBuild(arr, conf);
    free(p);
    free(data);
    spKdArrayDestroy(arr);
    spKdTreeDestroy(t);*/

    // Extract features
    is_extract = spConfigIsExtractionMode(conf, msg);
    num_images = spConfigGetNumOfImages(conf, msg);
    total_num_features = 0;
    images_features = (SPPoint ***) malloc(sizeof(**images_features) * num_images);
    images_features_num_of_features = (int *) malloc(sizeof(int) * num_images);
    if (*msg != SP_CONFIG_SUCCESS) return 1;
    imp = new ImageProc(conf);
    if (is_extract) {
        for (i = 0; i < num_images; i++) {
            spConfigGetImagePath(img_path, conf, i);
            features = imp->getImageFeatures(img_path, i, &num_of_features);
            saveImageFeaturesToFile(features, num_of_features, i, conf);
            total_num_features += num_of_features;
            images_features_num_of_features[i] = num_of_features;
            images_features[i] = features;
        }
    } else {
        for (i = 0; i < num_images; i++) {
            images_features[i] = loadImageFeaturesFromFile(i, conf, &num_of_features);
            images_features_num_of_features[i] = num_of_features;
            total_num_features += num_of_features;
        }
    }

    // Build KDTree from features of all images combined
    all_features = (SPPoint **) malloc(sizeof(SPPoint *) * total_num_features);
    l = 0;
    for (i = 0; i < num_images; i++) {
        for (j = 0; j < images_features_num_of_features[i]; j++) {
            all_features[l] = images_features[i][j];
            l++;
        }
    }

    SPKDArray *kd_arr = spKdArrayInit(all_features, total_num_features);
    if (kd_arr == NULL) return 1; // TODO: Better handling?
    SPKDTree *t = spKdTreeBuild(kd_arr, conf);
    if (t == NULL) return 1;
    num_similar_images = spConfigGetNumOfSimilarImages(conf, msg);
    if (*msg != SP_CONFIG_SUCCESS) return 1;
    minimal_gui = spConfigMinimalGui(conf, msg);
    if (*msg != SP_CONFIG_SUCCESS) return 1;
    knn = spConfigGetKNN(conf, msg);
    if (*msg != SP_CONFIG_SUCCESS) return -1;
    SPBPQueue *bpq = spBPQueueCreate(knn);
    //image_knn_cou = (int *) malloc(sizeof(int) * num_images);
    int image_knn_cou[num_images][2];
    res = (BPQueueElement *) malloc(sizeof(BPQueueElement));

    // TODO: Still not working...

    while (true) {

        printf(ENTER_IMAGE_PATH);
        scanf("%s", inp);

        if (strcmp(inp, EXIT_CODE) == 0) {
            printf(EXITING);
            break;
        }

        features = imp->getImageFeatures(inp, i, &num_of_features);
        if (features == NULL) {
            // TODO: What?
            continue;
        }
        for (i = 0; i < num_of_features; i++) {
            spKdTreeKNNSearch(t, bpq, features[i]);
            while (!spBPQueueIsEmpty(bpq)) {
                spBPQueuePeek(bpq, res);
                image_knn_cou[res->index][0]++; // Add image as knn
                spBPQueueDequeue(bpq);
            }
        }

        qsort(image_knn_cou, (size_t) num_images, sizeof(image_knn_cou[0]), reversed_compare);
        //qsort(image_knn_cou, (size_t) num_images, sizeof(int), reverse_compare);

        if (minimal_gui) {
            for (i = 0; i < num_similar_images; i++) {
                spConfigGetImagePath(img_path, conf, image_knn_cou[i][1]);
                imp->showImage(img_path);
            }
        } else {
            printf("Best candidates for - %s - are:\n", inp);
            for (i = 0; i < num_similar_images; i++) {
                spConfigGetImagePath(img_path, conf, image_knn_cou[i][1]);
                printf("%s\n", img_path);
            }
        }

        //Clear knn counts
        for (i = 0; i < num_images; i++) {
            image_knn_cou[i][0] = 0;
            image_knn_cou[i][1] = i;
        }
    }

    spKdArrayDestroy(kd_arr);
    spKdTreeDestroy(t);
    free(res);
    //free(image_knn_cou);
    free(images_features);
    free(images_features_num_of_features);
    clearConfig(conf, msg);
    clearLogger();

    return 0;
}
