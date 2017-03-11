#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include "SPConfig.h"
#include "SPLogger.h"

#define MAXBUF 1024
#define DELIM "="

// Defining text message:
#define INVALID_CONFIGURATION_LINE "Invalid configuration line"
#define INVALID_VALUE "Invalid value - constraint not met"

// Global variable holding the logger
SPConfig config = NULL;

struct sp_config_t {
    char spImagesDirectory[MAXBUF];
    char spImagesPrefix[MAXBUF];
    char spImagesSuffix[MAXBUF];
    int spNumOfImages;
    int spPCADimension;
    char spPCAFilename[MAXBUF];
    int spNumOfFeatures;
    bool spExtractionMode;
    int spNumOfSimilarImages;
    SP_KD_TREE_SPLIT_METHOD spKDTreeSplitMethod;
    int spKNN;
    bool spMinimalGUI;
    int spLoggerLevel;
    char spLoggerFilename[MAXBUF];
};

// Private Methods

//TODO: Doc
void removeSpaces(char *str) {
    if (str == NULL)
        return;
    char *i = str;
    char *j = str;
    while (*j != 0) {
        *i = *j++;
        if (*i != ' ' && *i != '\n')
            i++;
    }
    *i = 0;
}

//TODO: Doc
bool strToBool(char *str, SP_CONFIG_MSG *msg) {
    *msg = SP_CONFIG_SUCCESS;
    if (strcmp(str, "true") == 0) {
        return true;
    } else if (strcmp(str, "false") == 0) {
        return false;
    }
    *msg = SP_CONFIG_INVALID_STRING;
    return false;
}

//TODO: Doc
int strToInt(char *str, SP_CONFIG_MSG *msg) {
    *msg = SP_CONFIG_SUCCESS;
    char *ptr;
    int ret = (int) strtol(str, &ptr, 10);
    if (*ptr == '\0') {  // If parsed all the input then we got everything.
        return ret;
    } else {
        *msg = SP_CONFIG_INVALID_INTEGER;
        return -1;
    }
}

//TODO: Doc
void initDefaults() {
    config->spPCADimension = 20;
    strcpy(config->spPCAFilename, "pca.yml");
    config->spNumOfFeatures = 100;
    config->spExtractionMode = true;
    config->spMinimalGUI = false;
    config->spNumOfSimilarImages = 1;
    config->spKNN = 1;
    config->spKDTreeSplitMethod = MAX_SPREAD;
    config->spLoggerLevel = 3;
    strcpy(config->spLoggerFilename, "stdout");
}

//TODO: Doc
int loadConfigFromFile(FILE *f, const char *filename, SP_CONFIG_MSG *msg) {
    // Load config
    char line[MAXBUF];
    int i = 1;

    while (fgets(line, sizeof(line), f) != NULL) {
        char *key;
        char *val;
        key = strtok(line, DELIM);
        removeSpaces(key);
        val = strtok(NULL, DELIM);
        removeSpaces(val);
        bool bool_val;
        int int_val;
        const char space = ' ';

        //val = strstr((char *) line, DELIM) + strlen(DELIM);
        //key = strstr((char *) line, DELIM);

        if (*key == '#') {  // If first char is # then it's a comment
            // Do nothing
        } else if (strcmp(key, "spExtractionMode") == 0) {
            bool_val = strToBool(val, msg);
            if (*msg != SP_CONFIG_SUCCESS) {
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            config->spExtractionMode = bool_val;
        } else if (strcmp(key, "spMinimalGUI") == 0) {
            bool_val = strToBool(val, msg);
            if (*msg != SP_CONFIG_SUCCESS) {
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            config->spMinimalGUI = bool_val;
        } else if (strcmp(key, "spImagesDirectory") == 0) {
            if (strchr(val, space) != NULL) {
                *msg = SP_CONFIG_INVALID_STRING;
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            }
            strcpy(config->spImagesDirectory, val);
        } else if (strcmp(key, "spImagesPrefix") == 0) {
            if (strchr(val, space) != NULL) {
                *msg = SP_CONFIG_INVALID_STRING;
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            strcpy(config->spImagesPrefix, val);
        } else if (strcmp(key, "spImagesSuffix") == 0) {
            bool valid_suffix = (strcmp(val, ".jpg") == 0) || (strcmp(val, ".png") == 0) ||
                                (strcmp(val, ".bmp") == 0) || (strcmp(val, ".gif") == 0);
            if (!valid_suffix) {
                *msg = SP_CONFIG_INVALID_STRING;
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            strcpy(config->spImagesSuffix, val);
        } else if (strcmp(key, "spPCAFilename") == 0) {
            if (strchr(val, space) != NULL) {
                *msg = SP_CONFIG_INVALID_STRING;
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            strcpy(config->spPCAFilename, val);
        } else if (strcmp(key, "spLoggerFilename") == 0) {
            if (strchr(val, space) != NULL) {
                *msg = SP_CONFIG_INVALID_STRING;
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            strcpy(config->spLoggerFilename, val);
        } else if (strcmp(key, "spKDTreeSplitMethod") == 0) {
            if (strcmp(val, "RANDOM") == 0) {
                config->spKDTreeSplitMethod = RANDOM;
            } else if (strcmp(val, "MAX_SPREAD") == 0) {
                config->spKDTreeSplitMethod = MAX_SPREAD;
            } else if (strcmp(val, "INCREMENTAL") == 0) {
                config->spKDTreeSplitMethod = INCREMENTAL;
            } else {
                *msg = SP_CONFIG_INVALID_STRING;
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            }
        } else if (strcmp(key, "spNumOfImages") == 0) {
            int_val = strToInt(val, msg);
            if (*msg != SP_CONFIG_SUCCESS || int_val < 1) {
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            config->spNumOfImages = int_val;
        } else if (strcmp(key, "spPCADimension") == 0) {
            int_val = strToInt(val, msg);
            if (*msg != SP_CONFIG_SUCCESS || int_val < 10 || int_val > 28) {
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            config->spPCADimension = int_val;
        } else if (strcmp(key, "spNumOfFeatures") == 0) {
            int_val = strToInt(val, msg);
            if (*msg != SP_CONFIG_SUCCESS || int_val < 1) {
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            config->spNumOfFeatures = int_val;
        } else if (strcmp(key, "spNumOfSimilarImages") == 0) {
            int_val = strToInt(val, msg);
            if (*msg != SP_CONFIG_SUCCESS || int_val < 1) {
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            config->spNumOfSimilarImages = int_val;
        } else if (strcmp(key, "spKNN") == 0) {
            int_val = strToInt(val, msg);
            if (*msg != SP_CONFIG_SUCCESS || int_val < 1) {
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            config->spKNN = int_val;
        } else if (strcmp(key, "spLoggerLevel") == 0) {
            int_val = strToInt(val, msg);
            if (*msg != SP_CONFIG_SUCCESS || int_val < 1 || int_val > 4) {
                spRegularMessage(INVALID_VALUE, filename, i);
                return i;
            };
            config->spLoggerLevel = int_val;
        } else {
            *msg = SP_CONFIG_INVALID_ARGUMENT;
            spRegularMessage(INVALID_CONFIGURATION_LINE, filename, i);
            return i;
        }

        i++;
    }

    return i;
}

void validateConfig(const char *filename, int num_lines, SP_CONFIG_MSG *msg) {
    if (strlen(config->spImagesDirectory) == 0) {
        *msg = SP_CONFIG_MISSING_DIR;
        spRegularMessage("Parameter spImagesDirectory is not set", filename, num_lines);
        return;
    }
    if (strlen(config->spImagesPrefix) == 0) {
        *msg = SP_CONFIG_MISSING_PREFIX;
        spRegularMessage("Parameter spImagesPrefix is not set", filename, num_lines);
        return;
    }
    if (strlen(config->spImagesSuffix) == 0) {
        *msg = SP_CONFIG_MISSING_SUFFIX;
        spRegularMessage("Parameter spImagesSuffix is not set", filename, num_lines);
        return;
    }
    if (config->spNumOfImages == 0) {
        *msg = SP_CONFIG_MISSING_NUM_IMAGES;
        spRegularMessage("Parameter spNumOfImages is not set", filename, num_lines);
        return;
    }

    *msg = SP_CONFIG_SUCCESS;
}

// Public Methods

SPConfig spConfigCreate(const char *filename, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    *msg = SP_CONFIG_SUCCESS;

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
    if (f == NULL) { //Open failed
        free(config);
        config = NULL;
        *msg = SP_CONFIG_CANNOT_OPEN_FILE;
        return NULL;
    }

    initDefaults();

    int num_lines = loadConfigFromFile(f, filename, msg);
    fclose(f);
    if (*msg != SP_CONFIG_SUCCESS)
        return NULL;

    validateConfig(filename, num_lines, msg);
    if (*msg != SP_CONFIG_SUCCESS)
        return NULL;

    return config;
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return false;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spExtractionMode;
    }
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return false;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spMinimalGUI;
    }
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spNumOfImages;
    }
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spNumOfFeatures;
    }
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spPCADimension;
    }
}

SP_CONFIG_MSG spConfigGetImagePath(char *imagePath, const SPConfig config, int index) {
    if (config == NULL || imagePath == NULL)
        return SP_CONFIG_INVALID_ARGUMENT;
    if (index >= config->spNumOfImages)
        return SP_CONFIG_INVALID_ARGUMENT;

    char str_index[10];

    strcpy(imagePath, config->spImagesDirectory);
    strcat(imagePath, config->spImagesPrefix);
    sprintf(str_index, "%d", index);
    strcat(imagePath, str_index);
    strcat(imagePath, config->spImagesSuffix);

    return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetPCAPath(char *pcaPath, const SPConfig config) {
    if (config == NULL)
        return SP_CONFIG_INVALID_ARGUMENT;

    strcpy(pcaPath, config->spImagesDirectory);
    strcat(pcaPath, config->spPCAFilename);

    return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetLoggerLevel(SP_LOGGER_LEVEL *level, const SPConfig config) {
    if (config == NULL)
        return SP_CONFIG_INVALID_ARGUMENT;
    switch (config->spLoggerLevel) {
        case 1:
            *level = SP_LOGGER_ERROR_LEVEL;
        case 2:
            *level = SP_LOGGER_WARNING_ERROR_LEVEL;
        case 3:
            *level = SP_LOGGER_INFO_WARNING_ERROR_LEVEL;
        case 4:
        default:
            *level = SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL;
    }
    return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetLoggerFilename(char *filename, const SPConfig config) {
    if (config == NULL)
        return SP_CONFIG_INVALID_ARGUMENT;
    strcpy(filename, config->spLoggerFilename);
    return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetTreeSplitMethod(SP_KD_TREE_SPLIT_METHOD *method, const SPConfig config) {
    if (config == NULL)
        return SP_CONFIG_INVALID_ARGUMENT;
    *method = config->spKDTreeSplitMethod;
    return SP_CONFIG_SUCCESS;
}

void spConfigDestroy(SPConfig config) {
    if (!config) {
        return;
    }
    free(config);
    config = NULL;
}
