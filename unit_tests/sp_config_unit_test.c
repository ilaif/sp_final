#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h"
#include "../SPConfig.h"

//Logger is not defined
static bool basicConfigTest() {
    SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(msg));
    SPConfig conf = spConfigCreate("C:/Users/Noa Biton/Desktop/pro/sp_final/basicConfigTest.cnf", msg);
    if (conf == NULL) {
        free(msg);
        return false;
    }
    ASSERT_FALSE(*msg == SP_CONFIG_MISSING_DIR);
    bool extractionMode = spConfigIsExtractionMode(conf, msg);
//  //  validateBooleanFunction(extractionMode,msg);
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (extractionMode) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    bool minGui = spConfigMinimalGui(conf,msg);
//    validateBooleanFunction(minGui,msg);

    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (minGui) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }

    int numOfImages =  spConfigGetNumOfImages(conf,msg);
    //validateIntegerFunction(numOfImages,msg);
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (numOfImages != -1) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    int numOfFeatures = spConfigGetNumOfFeatures(conf,msg);
////    validateIntegerFunction(numOfFeatures,msg);
//
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (numOfFeatures != -1) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    //SP_LOGGER_LEVEL *level = SP_LOGGER_ERROR_LEVEL;
    SP_LOGGER_LEVEL level;
    ASSERT_TRUE(spConfigGetLoggerLevel(&level,conf));
    int numOfSimilarImages = spConfigGetNumOfSimilarImages(conf,msg);
    //validateIntegerFunction(numOfSimilarImages,msg);
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (numOfSimilarImages != -1) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    int knn = spConfigGetKNN(conf,msg);
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (knn != -1) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    //validateIntegerFunction(knn,msg);

    int PCADim = spConfigGetPCADim(conf,msg);
    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
    if (PCADim != -1) {
        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
    }
    //validateIntegerFunction(PCADim,msg);
    spConfigDestroy(conf);
    free(msg);
    return true;
}

//bool validateBooleanFunction(bool value, SP_CONFIG_MSG *msg){
//    ASSERT_FALSE(*msg = SP_CONFIG_INVALID_ARGUMENT);
//    if (value) {
//        ASSERT_TRUE(*msg = SP_CONFIG_SUCCESS);
//    }
//
//}
//
//bool validateIntegerFunction(int value, SP_CONFIG_MSG *msg){
//    ASSERT_FALSE(*msg == SP_CONFIG_INVALID_ARGUMENT);
//    if (value != -1) {
//        ASSERT_TRUE(*msg == SP_CONFIG_SUCCESS);
//    }
//
//}


int main() {
    RUN_TEST(basicConfigTest);
    return 0;
}
