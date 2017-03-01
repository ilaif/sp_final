#include "SPConfig.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define CHAR_LEN 1024

typedef enum sp_kd_tree_split_method {
    RANDOM,
    MAX_SPREAD,
    INCREMENTAL
} SP_KD_TREE_SPLIT_METHOD;

// Global variable holding the logger
SPConfig config = NULL;

struct sp_config_t {
    char spImagesDirectory[CHAR_LEN];
    char spImagesPrefix[CHAR_LEN];
    char spImagesSuffix[CHAR_LEN];
    int spNumOfImages;
    int spPCADimension;
    char spPCAFilename[CHAR_LEN];
    int spNumOfFeatures;
    bool spExtractionMode;
    int spNumOfSimilarImages;
    SP_KD_TREE_SPLIT_METHOD spKDTreeSplitMethod;
    int spKNN;
    bool spMinimalGUI;
    int spLoggerLevel;
    char spLoggerFilename[CHAR_LEN];
};

SPConfig spConfigCreate(const char *filename, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (filename == NULL) { //Already exists
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NULL;
    }

    config = (SPConfig) malloc(sizeof(*config));
    if (config == NULL) { //Allocation failure
        *msg = SP_CONFIG_ALLOC_FAIL;
        return NULL;
    }

    FILE *f = fopen(filename, "r");
    if (config->input == NULL) { //Open failed
        free(config);
        config = NULL;
        *msg = SP_CONFIG_CANNOT_OPEN_FILE;
        return NULL;
    }


    return config;
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG *msg) {

}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG *msg) {

}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG *msg) {

}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG *msg) {

}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG *msg) {

}

SP_CONFIG_MSG spConfigGetImagePath(char *imagePath, const SPConfig config, int index) {

}

SP_CONFIG_MSG spConfigGetPCAPath(char *pcaPath, const SPConfig config) {

}

void spConfigDestroy(SPConfig config);
