
/**
 * Create a file where features are stored for future use
 * @param path          - a path to where file is created
 * @param features      - features to save
 * @param dim           - dimension of features
 * @param index         - the index of the image from which the features
 *                        where extracted
 * @param numOfFeatures - number of features
 * @return 1 on a successful call, otherwise 0
 */
int createFeaturesFile(char *path, SPPoint **features, int dim, int index,
                       int numOfFeatures);

/**
 * Read features from file return them and assign nFeatures
 * @param path      - path to file
 * @param nFeatures - number of features extracted from file
 * @return
 */
SPPoint **readFeaturesFile(const char *path, int *nFeatures);

/**
 * Extract features from config file images
 * @param config - Configuration file
 * @return 1 on a successful call, otherwise 0
 */
int extractFromImages(SPConfig config);

/**
 * Extract and return features from .feats files, assign total number
 * of features extracted to totalNumOfFeatures
 * @param config                - Configuration file
 * @param totalNumOfFeatures    - total number of features extracted
 * @return  features as SPPoints on a successful call, if error occurs
 *          return NULL pointer
 */
SPPoint **extractFromFile(SPConfig config, int *totalNumOfFeatures);

/**
 * Returns a KDTree constructed from features of config images
 * @param config - Configuration file
 * @return  KDTree on a successful call, if error occurs return
 *          NULL pointer
 */
KDTree *extractKDTree(SPConfig config);

/**
 *
 * @param config     - Configuration file
 * @param queryPath  - file path to query image
 * @param kdTree     - KDTree with features from all the images in
 *                     the configuration file
 * @return 1 on a successful call, otherwise 0
 */
int searchSimilarImages(SPConfig config, char *queryPath, KDTree *kdTree);