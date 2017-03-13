#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#define BUF_SIZE 1024

#include "SPImageProc.h"

using namespace sp;

extern "C" {
#include "SPLogger.h"
#include "SPConfig.h"
#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPKDTree.h"
}

//TODO: Docs
SPConfig loadConfig(int argc, char *argv[], SP_CONFIG_MSG *msg);

//TODO: Docs
SP_LOGGER_MSG initLogger(SPConfig conf);

//TODO: Docs
bool saveImageFeaturesToFile(SPPoint **features, int num_of_features, int index, SPConfig conf);

//TODO: Docs
SPPoint **loadImageFeaturesFromFile(int index, SPConfig conf, int *num_of_features);

//TODO: Docs
SPPoint **extractImageFeatures(SPConfig conf, int num_images, int *total_num_features, ImageProc *imp);

//TODO: Docs
int reversed_compare(const void *x, const void *y);

#endif /* MAIN_AUX_H_ */