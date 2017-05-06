#include <cstring>

#include "SPImageProc.h"

extern "C" {
#include "SPGlobals.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "SPConfig.h"
#include "SPKDTree.h"
}

/**
 * Create a file where features are stored for future use
 * @param filepath      - a path to where file is created
 * @param index         - the index of the image from which the features
 * @param nFeatures     - number of features
 * @param features_list - features to save
 *
 * @return 1 on a successful call, otherwise 0
 */
int saveFeatureToFile(char *filepath, int index, int nFeatures,
  SPPoint** featureList);

/**
 * Read features from file return them and assign nFeatures
 * @param filepath  - path to file
 * @param nFeatures - number of features that were extracted from the file
 *
 * @return pointer to the newly created SPPoint array.
 */
SPPoint** readFeaturesFile(const char *filepath, int *nFeatures);

/**
 * Craetes a single list out of the list of lists 'imagesList'. The created
 * list contains pointers to the points WITHIN imagesList and therefor the
 * points SHOULD NOT be freed through the returned list.
 * Don't forget to DO free the list itself.
 * @param config     - the config object
 * @param imagesList - the list of lists to flatten
 * @param nFeatures  - an array of the sizes of the lists within imagesList
 * @param size       - a pointer to an integer in which the final size will be
 *                     returned.
 *
 * @return the pointer to the list of NULL if memory allocation failed
 */
SPPoint **imagesListToFeatureList(SPConfig config, SPPoint*** imagesList,
  int* nFeatures, int *size);

/**
 * Requests the user for a query path. Sets command to the path and returns 1
 * OR if the command recieved from the user is CL_STOP_QUERYING the returns 0
 * @param command - the recieved command will be stored in this variable. For
 *                  that reason command should be large enough to contain the
 *                  command
 * @return 1 or 0 if the command is CL_STOP_QUERYING (see SPGlobals.h)
 */
int getCommand(char *command);

/**
 * Searches the kdTree for similar images to the queryPath
 * @param config     - Configuration file
 * @param queryPath  - file path to query image
 * @param kdTree     - SPKDTreeNode with features from all the images in
 *                     the configuration file
 * @return a pointer the a list of similar images or NULL is searching failed.
 */
int *searchSimilarImages(SPConfig config, char *queryPath,
  SPKDTreeNode *kdTree, sp::ImageProc *imageProc);

/**
 * Frees all memory allocation associated with the images list, if the list is
 * NULL nothing happens
 */
void imagesListDestroy(SPPoint ***imagesList, int length, int *nFeatures);