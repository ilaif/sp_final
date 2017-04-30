#include <iostream>
#include <cstring>
#include <cstdlib>

extern "C" {
#include "SPLogger.h"
#include "SPConfig.h"
#include "SPKDArray.h"
#include "SPKDTree.h"
}

#include "main_aux.h"

#define ENTER_IMAGE_PATH "Please enter an image path:\n"
#define EXITING "Exiting...\n"
#define EXIT_CODE "<>"
#define BEST_CANDIDATES "Best candidates for - %s - are:\n"
#define CANNOT_LOAD_IMAGE "Cannot load image, please verify the path exists to an image and try again.\n"

using namespace sp;

int main(int argc, char *argv[]) {

    int i, num_images, total_num_features, num_similar_images, knn, minimal_gui, num_of_features = 0;
    char inp[BUF_SIZE], img_path[BUF_SIZE];
    ImageProc *imp;
    SPPoint **features = NULL, **all_features;
    BPQueueElement *res;

    // Init config
    SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(msg));
    SPConfig conf = loadConfig(argc, argv, msg);
    if (*msg != SP_CONFIG_SUCCESS) return 1;

    // Init logger
    SP_LOGGER_MSG logger_msg = initLogger(conf);
    if (logger_msg != SP_LOGGER_SUCCESS) return 1;

    // Extract features
    imp = new ImageProc(conf);
    num_images = spConfigGetNumOfImages(conf, msg);
    if (*msg != SP_CONFIG_SUCCESS) return 1;
    all_features = extractImageFeatures(conf, num_images, &total_num_features, imp);
    if (all_features == NULL) return 1;

    // Load needed config vars
    knn = spConfigGetKNN(conf, msg);
    if (*msg != SP_CONFIG_SUCCESS) return 1;
    SPBPQueue *bpq = spBPQueueCreate(knn);
    if (bpq == NULL) return 1;
    num_similar_images = spConfigGetNumOfSimilarImages(conf, msg);
    if (*msg != SP_CONFIG_SUCCESS) return 1;
    minimal_gui = spConfigMinimalGui(conf, msg);
    if (*msg != SP_CONFIG_SUCCESS) return 1;
    res = (BPQueueElement *) malloc(sizeof(BPQueueElement));
    if (res == NULL) return 1;

    // Build KDTree from features of all images combined
    SPKDArray *kd_arr = spKdArrayInit(all_features, total_num_features);
    if (kd_arr == NULL) return 1;
    SPKDTree *t = spKdTreeBuild(kd_arr, conf);
    if (t == NULL) return 1;

    // Initialize per image knn counts
    int *image_knn_cou = (int *) malloc(sizeof(int) * num_images * 2);
    for (i = 0; i < num_images; i++) {
        image_knn_cou[i * 2 + 0] = 0;
        image_knn_cou[i * 2 + 1] = i;
    }

    // Continue until user breaks loop
    while (true) {

        printf(ENTER_IMAGE_PATH);
        scanf("%s", inp);

        if (strcmp(inp, EXIT_CODE) == 0) {
            printf(EXITING);
            break;
        }

        // Load query image features
        features = imp->getImageFeatures(inp, i, &num_of_features);
        if (features == NULL) { // If not found for some reason, continue
            printf(CANNOT_LOAD_IMAGE);
            continue;
        }

        // Iterate over query image features and search each of them in the KDTree
        for (i = 0; i < num_of_features; i++) {
            spKdTreeKNNSearch(t, bpq, features[i]);
            while (!spBPQueueIsEmpty(bpq)) {
                spBPQueuePeek(bpq, res);
                image_knn_cou[res->index * 2 + 0]++; // Add image as knn
                spBPQueueDequeue(bpq);
            }
        }

        // Sort the image's knn scores in reversed order so the highest hits are first
        qsort(image_knn_cou, (size_t) num_images, sizeof(int) * 2, reversed_compare);

        if (minimal_gui) {
            for (i = 0; i < num_similar_images; i++) {
                if (spConfigGetImagePath(img_path, conf, image_knn_cou[i * 2 + 1]) != SP_CONFIG_SUCCESS) return 1;
                imp->showImage(img_path);
            }
        } else {
            printf(BEST_CANDIDATES, inp);
            for (i = 0; i < num_similar_images; i++) {
                if (spConfigGetImagePath(img_path, conf, image_knn_cou[i * 2 + 1]) != SP_CONFIG_SUCCESS) return 1;
                printf("%s\n", img_path);
            }
        }

        // Clean all variables for the next loop

        for (i = 0; i < num_images; i++) {
            image_knn_cou[i * 2 + 0] = 0;
            image_knn_cou[i * 2 + 1] = i;
        }

        for (i = 0; i < num_of_features; i++) {
            spPointDestroy(features[i]);
        }
        free(features);
    }

    spKdTreeDestroy(t);
    spBPQueueDestroy(bpq);
    free(res);
    free(all_features);
    free(image_knn_cou);
    free(conf);
    free(msg);
    spLoggerDestroy();

    return 0;
}
