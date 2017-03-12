#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "main_aux.h"
#include "SPPoint.h"

#define INVALID_COMMAND_LINE "Invalid command line : use -c <config_filename>\n"
#define DEFAULT_CONF_CANNOT_OPEN "The default configuration file spcbir.config couldn’t be open\n"
#define CONF_CANNOT_OPEN "The configuration file %s couldn’t be open\n"

SPConfig loadConfig(int argc, char *argv[], SP_CONFIG_MSG *msg) {
    SPConfig conf = NULL;
    bool load_default_config = false;

    if (argc < 3) {
        load_default_config = true;
    } else {
        if (strcmp(argv[1], "-c") != 0) {
            printf(INVALID_COMMAND_LINE);
            load_default_config = true;
        }
    }

    conf = spConfigCreate((load_default_config) ? "./spcbir.config" : argv[2], msg);
    if (*msg == SP_CONFIG_CANNOT_OPEN_FILE) { // If cannot open file, then open the default one.
        if (load_default_config) {
            printf(DEFAULT_CONF_CANNOT_OPEN);
        } else {
            printf(CONF_CANNOT_OPEN, argv[2]);
            conf = spConfigCreate("./spcbir.config", msg);
            if (*msg == SP_CONFIG_CANNOT_OPEN_FILE) {
                printf(DEFAULT_CONF_CANNOT_OPEN);
            }
        }
    }

    return conf;
}

void clearConfig(SPConfig conf, SP_CONFIG_MSG *msg) {
    free(conf);
    free(msg);
}

SP_LOGGER_MSG initLogger(SPConfig conf) {
    char filename[50];
    spConfigGetLoggerFilename(filename, conf);
    SP_LOGGER_LEVEL level;
    spConfigGetLoggerLevel(&level, conf);
    SP_LOGGER_MSG logger_msg = spLoggerCreate(filename, level);
    return logger_msg;
}

void clearLogger() {
    spLoggerDestroy();
}

bool saveImageFeaturesToFile(SPPoint **features, int num_of_features, int index, SPConfig conf) {
    char featurePath[BUF_SIZE];
    int i, j, d;
    FILE *f;
    SP_CONFIG_MSG msg = spConfigGetFeaturePath(featurePath, conf, index);
    if (msg != SP_CONFIG_SUCCESS) return false;
    d = spConfigGetPCADim(conf, &msg);
    if (msg != SP_CONFIG_SUCCESS) return false;

    f = fopen(featurePath, "w");
    if (f == NULL) return false;
    fprintf(f, "%d\n", num_of_features);
    for (i = 0; i < num_of_features; i++) {
        for (j = 0; j < d; j++) {
            fprintf(f, "%lf ", spPointGetAxisCoor(features[i], j));
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return true;
}

SPPoint **loadImageFeaturesFromFile(int index, SPConfig conf, int *num_of_features) {
    char featurePath[BUF_SIZE];
    int i, j, d;
    double *data;
    FILE *f;
    SP_CONFIG_MSG msg = spConfigGetFeaturePath(featurePath, conf, index);
    if (msg != SP_CONFIG_SUCCESS) return NULL;
    d = spConfigGetPCADim(conf, &msg);
    if (msg != SP_CONFIG_SUCCESS) return NULL;

    f = fopen(featurePath, "r");
    if (f == NULL) return NULL;
    fscanf(f, "%d\n", num_of_features);
    SPPoint **features = (SPPoint **) malloc(sizeof(*features) * (*num_of_features));
    data = (double *) malloc(sizeof(double) * d);
    if (data == NULL) return NULL;
    for (i = 0; i < *num_of_features; i++) {
        for (j = 0; j < d; j++) {
            fscanf(f, "%lf ", &data[j]);
        }
        features[i] = spPointCreate(data, d, index);
        if (features[i] == NULL) return NULL;
        fscanf(f, "\n");
    }

    free(data);
    fclose(f);

    return features;
}