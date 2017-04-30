#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h"
#include "../SPConfig.h"

//Logger is not defined
static bool basicConfigTest() {
    SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(msg));
    SPConfig conf = spConfigCreate("./unit_tests/basicConfigTest.cnf", msg);
    if (conf == NULL) {
        printf("Error creating config: %d", *msg);
        free(msg);
        return false;
    }
    ASSERT_FALSE(*msg == SP_CONFIG_MISSING_DIR);
    bool extractionMode = spConfigIsExtractionMode(conf, msg);

    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (extractionMode) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    bool minGui = spConfigMinimalGui(conf, msg);

    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (minGui) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }

    int numOfImages = spConfigGetNumOfImages(conf, msg);
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (numOfImages != -1) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    int numOfFeatures = spConfigGetNumOfFeatures(conf, msg);
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (numOfFeatures != -1) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    SP_LOGGER_LEVEL level;
    ASSERT_TRUE(spConfigGetLoggerLevel(&level, conf));
    int numOfSimilarImages = spConfigGetNumOfSimilarImages(conf, msg);
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (numOfSimilarImages != -1) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    int knn = spConfigGetKNN(conf, msg);
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (knn != -1) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }

    int PCADim = spConfigGetPCADim(conf, msg);
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (PCADim != -1) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    spConfigDestroy(conf);
    free(msg);
    return true;
}

int main() {
    RUN_TEST(basicConfigTest);
    return 0;
}
