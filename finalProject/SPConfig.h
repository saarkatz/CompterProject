#ifndef SPCONFIG_H_
#define SPCONFIG_H_

#include <stdbool.h>
#include <stdio.h>
#include "SPLogger.h"

#define SP_DEFAULT_PCADIM 20
#define SP_DEFAULT_PCAFILENAME "pca.yml"
#define SP_DEFAULT_NUMOFFEATURS 100
#define SP_DEFAULT_EXTRACTMODE true
#define SP_DEFAULT_MINIMALGUI false
#define SP_DEFAULT_NUMSIMIMG 1
#define SP_DEFAULT_KNN 1
#define SP_DEFUALT_SPLITMETHOD MAX_SPREAD
#define SP_DEFUALT_LOGGERLEVEL 3
#define SP_DEFUALT_LOGGERFILENAME "stdout"


/**
 * A data-structure which is used for configuring the system.
 */

#define PCA_FILENAME "spPCAFilename"
#define SIM_NUM "spNumOfSimilarImages"
#define PCA_DIM "spPCADimension"
#define IMG_NUM "spNumOfImages"
#define NUM_FEAT "spNumOfFeatures"
#define MINIMAL_GUI "spMinimalGUI"
#define LOGGERLVL "spLoggerLevel"
#define LOGGER_FILENAME "spLoggerFilename"
#define KNN "spKNN"
#define SPLIT_METHOD "spKDTreeSplitMethod"
#define IMAG_SUFIX "spImagesSuffix"
#define IMG_PREFIX "spImagesPrefix"
#define EXTRACT_MODE "spExtractionMode"
#define IMG_DIR "spImagesDirectory"
#define RANDOM_STR "RANDOM"
#define MAX_SPREAD_STR "MAX_SPREAD"
#define INCREMENTAL_STR "INCREMENTAL"
/* System variables
Variable that MUST be initialized:
  spImagesDirectory - string, Constraint: the string contains no spaces
  spImagesPrefix - string, Constraint: the string contains no spaces
  spImagesSuffix - string, Constraint: string in { .jpg , .png , .bmp , .gif }
  spNumOfImages - int, Constraint: positive integer

Variables that have default value:
  spPCADimension - Constraint: positive integer in the range [10 , 28]
  spPCAFilename - string, Constraint: the string contains no spaces
  spNumOfFeatures - int, Constraint: positive integer
  spExtractionMode - bool, Constraint: value in { true , false}
  spNumOfSimilarImages - int, Constraint: value > 0
  spKDTreeSplitMethod - enum, 
    Constraint: value in {RANDOM, MAX_SPREAD, INCREMENTAL }
  spKNN - int, Constraint: value > 0
  spMinimalGUI - bool, Constraint: value in { true , false}
  spLoggerLevel - int, Constraint: value in { 1 , 2 , 3 , 4}
    1 - error level
    2 - warning level
    3 - info level
    4 - debug level
  spLoggerFilename - string, Constraint: the string contains no spaces

Default values of variables:
  spPCADimension: 20
  spPCAFilename: pca.yml
  spNumOfFeatures: 100
  spExtractionMode: true
  spNumOfSimilarImages: 1
  spKNN: 1
  spKDTreeSplitMethod: MAX_SPREAD
  spMinimalGUI: false
  spLoggerLevel: 3
  spLoggerFilename: stdout
*/

typedef enum sp_config_msg_t {
  SP_CONFIG_MISSING_DIR,
  SP_CONFIG_MISSING_PREFIX,
  SP_CONFIG_MISSING_SUFFIX,
  SP_CONFIG_MISSING_NUM_IMAGES,
  SP_CONFIG_CANNOT_OPEN_FILE,
  SP_CONFIG_ALLOC_FAIL,
  SP_CONFIG_INVALID_INTEGER,
  SP_CONFIG_INVALID_STRING,
  SP_CONFIG_INVALID_ARGUMENT,
  SP_CONFIG_INDEX_OUT_OF_RANGE,
  SP_CONFIG_SUCCESS
} SP_CONFIG_MSG;

typedef enum search_method_t {
  RANDOM, MAX_SPREAD,
  INCREMENTAL
}SPSearchMethod;

typedef struct sp_config_t* SPConfig;

void printInvalidLine(const char* filename,int linenumber);

void printConstranitNotMet(const char* filename,int linenumber);

void printParameterNotSet(const char* filename,int linenumbers,const char* path);

/**
 * Creates a new system configuration struct. The configuration struct
 * is initialized based on the configuration file given by 'filename'.
 * 
 * @param filename - the name of the configuration file
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct which
 * 		   contains all system configuration.
 * 
 * The resulting value stored in msg is as follow:
 * - SP_CONFIG_INVALID_ARGUMENT - if filename == NULL
 * - SP_CONFIG_CANNOT_OPEN_FILE - if the configuration file given by filename cannot be open
 * - SP_CONFIG_ALLOC_FAIL - if an allocation failure occurred
 * - SP_CONFIG_INVALID_INTEGER - if a line in the config file contains invalid integer
 * - SP_CONFIG_INVALID_STRING - if a line in the config file contains invalid string
 * - SP_CONFIG_MISSING_DIR - if spImagesDirectory is missing
 * - SP_CONFIG_MISSING_PREFIX - if spImagesPrefix is missing
 * - SP_CONFIG_MISSING_SUFFIX - if spImagesSuffix is missing 
 * - SP_CONFIG_MISSING_NUM_IMAGES - if spNumOfImages is missing
 * - SP_CONFIG_SUCCESS - in case of success
 *
 *
 */
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg);

/*
 * Returns true if spExtractionMode = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg);

/*
 * Returns true if spMinimalGUI = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spMinimalGUI = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
bool spConfigIsMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg);

/*
 * Returns the number of images set in the configuration file, i.e the value
 * of spNumOfImages.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg);

/*
 * Returns the number of features to be extracted. i.e the value
 * of spNumOfFeatures.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * Returns the dimension of the PCA. i.e the value of spPCADimension.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * Given an index 'index' the function stores in imagePath the full path of the
 * ith image.
 *
 * For example:
 * Given that the value of:
 *  spImagesDirectory = "./images/"
 *  spImagesPrefix = "img"
 *  spImagesSuffix = ".png"
 *  spNumOfImages = 17
 *  index = 10
 *
 * The functions stores "./images/img10.png" to the address given by imagePath.
 * Thus the address given by imagePath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @param index - the index of the image.
 *
 * @return
 * - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 * - SP_CONFIG_INDEX_OUT_OF_RANGE - if index >= spNumOfImages
 * - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index);

/**
* Given an index 'index' the function stores in featsFilePath the full
* path of the ith images .feats file.
*
* For example:
* Given that the value of:
*  spImagesDirectory = "./images/"
*  spImagesPrefix = "img"
*  spNumOfImages = 17
*  index = 10
*
* The functions stores "./images/img10.feats" to the address given by
* featsFilePath. Thus the address given by featsFilePath must contain enough
* space to store the resulting string.
*
* @param featsFilePath - an address to store the result in, it must contain
*         enough space.
* @param config - the configuration structure
* @param index - the index of the image.
*
* @return
* - SP_CONFIG_INVALID_ARGUMENT - if featsFilePath == NULL or config == NULL
* - SP_CONFIG_INDEX_OUT_OF_RANGE - if index >= spNumOfImages or index < 0
* - SP_CONFIG_SUCCESS - in case of success
*/
SP_CONFIG_MSG spConfigGetFeatsFilePath(char* featsFilePath,
        const SPConfig config, int index);

/**
 * The function stores in pcaPath the full path of the pca file.
 * For example given the values of:
 *  spImagesDirectory = "./images/"
 *  spPcaFilename = "pca.yml"
 *
 * The functions stores "./images/pca.yml" to the address given by pcaPath.
 * Thus the address given by pcaPath must contain enough space to
 * store the resulting string.
 *
 * @param pcaPath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @return
 *  - SP_CONFIG_INVALID_ARGUMENT - if pcaPath == NULL or config == NULL
 *  - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config);

/**
* Returns the number of images presented given a query image. i.e the value of
* spNumOfSimilarImages.
*
* @param config - the configuration structure
* @assert msg != NULL
* @param msg - pointer in which the msg returned by the function is stored
* @return positive integer in success, negative integer otherwise.
*
* - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
* - SP_CONFIG_SUCCESS - in case of success
*/
int spConfigGetNumSimilarImages(const SPConfig config, SP_CONFIG_MSG* msg);

/**
* Returns the split method of the kdtree. i.e. the value of spKDTreeSplitMethod
*
* @param config - the configuration structure
* @assert msg != NULL
* @param msg - pointer in which the msg returned by the function is stored
* @return SPSearchMethod if success, negative integer otherwise.
*
* - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
* - SP_CONFIG_SUCCESS - in case of success
*/
SPSearchMethod spConfigGetSplitMethod(SPConfig config,
  SP_CONFIG_MSG *msg);

/**
* Returns the positive integer which is used in the k nearest neighbor search
* algorithm. i.e. the value of spKNN
*
* @param config - the configuration structure
* @assert msg != NULL
* @param msg - pointer in which the msg returned by the function is stored
* @return positive integer in success, negative integer otherwise.
*
* - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
* - SP_CONFIG_SUCCESS - in case of success
*/
int spConfigGetKNN(SPConfig config, SP_CONFIG_MSG* msg);

/**
* Returns the logger level. i.e the value of spLoggerLevel
*
* @param config - the configuration structure
* @assert msg != NULL
* @param msg - pointer in which the msg returned by the function is stored
* @return positive integer in the range [1, 4] in success, negative integer
*         otherwise.
*
* - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
* - SP_CONFIG_SUCCESS - in case of success
*/
int spConfigGetLoggerLevel(SPConfig config, SP_CONFIG_MSG* msg);

/**
* The function stores in loggerFilename the name of logger file.
*
* @param loggerFilename - an address to store the result in, it must contain enough space.
* @param config - the configuration structure
* @return
*  - SP_CONFIG_INVALID_ARGUMENT - if loggerFilename == NULL or config == NULL
*  - SP_CONFIG_SUCCESS - in case of success
*/
SP_CONFIG_MSG spConfigGetLoggerFilename(char* loggerFilename, const SPConfig config);


/**
 * Frees all memory resources associate with config. 
 * If config == NULL nothig is done.
 */
void spConfigDestroy(SPConfig config);


#endif /* SPCONFIG_H_ */
