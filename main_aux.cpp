#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "main_aux.h"

#define INVALID_COMMAND_LINE "Invalid command line : use -c <config_filename>\n"
#define DEFAULT_CONF_CANNOT_OPEN "The default configuration file spcbir.config couldn't be open\n"
#define CONF_CANNOT_OPEN "The configuration file %s couldn’t be open\n"
#define DEFAULT_CONFIG_PATH "./spcbir.config"

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

    conf = spConfigCreate((load_default_config) ? DEFAULT_CONFIG_PATH : argv[2], msg);
    if (*msg == SP_CONFIG_CANNOT_OPEN_FILE) { // If cannot open file, then open the default one.
        if (load_default_config) {
            printf(DEFAULT_CONF_CANNOT_OPEN);
        } else {
            printf(CONF_CANNOT_OPEN, argv[2]);
            conf = spConfigCreate(DEFAULT_CONFIG_PATH, msg);
            if (*msg == SP_CONFIG_CANNOT_OPEN_FILE) {
                printf(DEFAULT_CONF_CANNOT_OPEN);
            }
        }
    }

    return conf;
}

SP_LOGGER_MSG initLogger(SPConfig conf) {
    char filename[50];
    spConfigGetLoggerFilename(filename, conf);
    SP_LOGGER_LEVEL level;
    spConfigGetLoggerLevel(&level, conf);
    SP_LOGGER_MSG logger_msg = spLoggerCreate(filename, level);
    return logger_msg;
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
    if (fprintf(f, "%d\n", num_of_features) < 1) return false;
    for (i = 0; i < num_of_features; i++) {
        for (j = 0; j < d; j++) {
            if (fprintf(f, "%lf ", spPointGetAxisCoor(features[i], j)) < 1) return false;
        }
        if (fprintf(f, "\n") < 0) return false;
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
    if (features == NULL) return NULL;
    data = (double *) malloc(sizeof(double) * d);
    if (data == NULL) return NULL;
    for (i = 0; i < *num_of_features; i++) {
        for (j = 0; j < d; j++) {
            if (fscanf(f, "%lf ", &data[j]) < 1) return NULL;
        }
        features[i] = spPointCreate(data, d, index);
        if (features[i] == NULL) return NULL;
        if (fscanf(f, "\n") < 0) return NULL;
    }

    free(data);
    fclose(f);

    return features;
}

SPPoint **extractImageFeatures(SPConfig conf, int num_images, int *total_num_features, ImageProc *imp) {
    bool is_extract;
    int i, j, l, num_of_features;
    SPPoint ***images_features, **features, **all_features;
    int *images_features_num_of_features; // Keep array of num of features per image
    SP_CONFIG_MSG *msg = (SP_CONFIG_MSG *) malloc(sizeof(msg));
    if (msg == NULL) return NULL;
    char file_path[BUF_SIZE], log_msg[BUF_SIZE];

    is_extract = spConfigIsExtractionMode(conf, msg);
    if (*msg != SP_CONFIG_SUCCESS) return NULL;
    *total_num_features = 0;

    images_features = (SPPoint ***) malloc(sizeof(**images_features) * num_images);
    if (images_features == NULL) return NULL;
    images_features_num_of_features = (int *) malloc(sizeof(int) * num_images);
    if (images_features_num_of_features == NULL) return NULL;
    if (*msg != SP_CONFIG_SUCCESS) return NULL;

    //TODO: What happens if not is_extract and there are no features
    if (is_extract) {
        for (i = 0; i < num_images; i++) {
            *msg = spConfigGetImagePath(file_path, conf, i);
            if (*msg != SP_CONFIG_SUCCESS) return NULL;
            features = imp->getImageFeatures(file_path, i, &num_of_features);
            if (features == NULL) return NULL;
            if (!saveImageFeaturesToFile(features, num_of_features, i, conf)) return NULL;
            *total_num_features += num_of_features;
            images_features_num_of_features[i] = num_of_features;
            images_features[i] = features;
        }
    } else {
        for (i = 0; i < num_images; i++) {
            images_features[i] = loadImageFeaturesFromFile(i, conf, &num_of_features);
            if (images_features[i] == NULL) {
                *msg = spConfigGetFeaturePath(file_path, conf, i);
                if (*msg != SP_CONFIG_SUCCESS) return NULL;
                sprintf(log_msg, "Feature file %s not found.", file_path);
                spLoggerPrintError(log_msg, __FILE__, __func__, __LINE__);
                return NULL;
            }
            images_features_num_of_features[i] = num_of_features;
            *total_num_features += num_of_features;
        }
    }

    // Gather all features
    all_features = (SPPoint **) malloc(sizeof(SPPoint *) * (*total_num_features));
    if (all_features == NULL) return NULL;
    l = 0;
    for (i = 0; i < num_images; i++) {
        for (j = 0; j < images_features_num_of_features[i]; j++) {
            all_features[l] = images_features[i][j];
            l++;
        }
    }

    free(msg);
    free(images_features);
    free(images_features_num_of_features);

    return all_features;
}

int reversed_compare(const void *x, const void *y) {
    const int *a = (const int *) x;
    const int *b = (const int *) y;
    if (a[0] == b[0])
        return a[1] - b[1];
    else
        return b[0] - a[0];
}
