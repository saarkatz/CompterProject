#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#include <malloc.h>
#include <cstdlib>

#include "sp_image_proc_util.h"

extern "C" {
#include "SPPoint.h"

}

#define STRING_SIZE 1024
#define HISTOGARM_SIZE 3

// Prompts for the user.
void imageDirectoryPrompt();
void imagePrefixPrompt();
void numOfImagesPrompt();
void imageSuffixPrompt();
void numOfBinsPrompt();
void numOfFeaturesPrompt();
void queryImagePrompt();
void exitingMsg();

// Print an error message
void printError(char* msg);

// Print first K items of an array. 
void printKclosest(int* array,int kClosest,char* str);

// Returns an array of size kClosest containing the indexes of the closest 
// images according to their histogram distance from queryImageHistogram.
int* spBestHistDistance(int kClosest, SPPoint** queryImageHistogram,
int numOfImages, SPPoint*** globalArray);

// Returns an array of size kClosest containing the indexes of the closest 
// images according to the number of sifts that are closest to sift of the
// queryImageFeatures.
int* spBestImagesBySifts(int kClosest, SPPoint** queryImageFeatures,
  int numOfQueryFeatures, int numOfImages, SPPoint*** localArray,
  int* featureSizes);

// Methods to free-up memory.
void freeHistogram(SPPoint** histogramArray);
void freeFeatureDatabase(SPPoint*** database, int databaseSize,int* sizeArray);
void freeFeatures(SPPoint** featuresArray,int arraySize);
void freeHistogramDatabase(SPPoint*** database, int databaseSize);

/* qsort int comparison function */
int compare_count(const void *a, const void *b);

// Calls all the methods responsible to release memory.
void terminateProgram(int numberOfImages, SPPoint*** globalArray,
  SPPoint*** localArray, int* featureSizes, SPPoint** queryImageHistogram,
  SPPoint** queryImageFeatures, int numOfQueryFeatures);

#endif /* MAIN_AUX_H_ */
