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

//Validate input args and create spConfig. opens the default file in case of invalid path
//returns the config
/**
 *
 * @param argc
 * @param argv
 * @param msg
 * @return
 **/
SPConfig loadConfig(int argc, char *argv[], SP_CONFIG_MSG *msg);

/**
 *
 * @param conf
 * @return
 * */
//Take as a parameter the config and creates the logger with the config name and level
SP_LOGGER_MSG initLogger(SPConfig conf);

/**
 *
 * @param features
 * @param num_of_features
 * @param index
 * @param conf
 * @return bool saveImageFeaturesToFile(SPPoint **features, int num_of_features, int index, SPConfig conf);
 **/

/**
 *
 * @param index
 * @param conf
 * @param num_of_features
 * @return SPPoint **loadImageFeaturesFromFile(int index, SPConfig conf, int *num_of_features);

//extract all the features of the images which are in the images directory.
//we hold an EXTRACTION MODE bool, when it's true, , we first extract the features of each image and then store each
// of these features to a file which will be located in the images directory.
// if it's false that means NON-EXTRACTION MODE. in that case  the features
// the images are extracted from the features files that we generated in extraction mode */
SPPoint **extractImageFeatures(SPConfig conf, int num_images, int *total_num_features, ImageProc *imp);

/**
 *
 * @param x 
 * @param y
 * @return  */
int reversed_compare(const void *x, const void *y);

#endif /* MAIN_AUX_H_ */