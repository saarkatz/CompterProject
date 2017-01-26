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

typedef struct int_int_pair { int index; int count; } SPImageCounter;

void imageDirectoryPrompt();
void imagePrefixPrompt();
void numOfImagesPrompt();
void imageSuffixPrompt();
void numOfBinsPrompt();
void numOfFeaturesPrompt();
void queryImagePrompt();
void exitingMsg();
void printNearestImages(int* numImages);
void printError(char* msg);
void printKclosest(int* array,int kClosest,char* str);
int* spBestHistDistance(int kClosest, SPPoint** queryImageHistogram,
int numOfImages, SPPoint*** globalArray);
void freeHistogram(SPPoint** histogramArray);
void freeHistogramDatabase(SPPoint*** database, int databaseSize,int* sizeArray);
void freeFeatures(SPPoint** featuresArray,int arraySize);
void freeHistogramDatabase(SPPoint*** database, int databaseSize);
void freeHistogramDatabase(SPPoint*** database, int databaseSize);
/* qsort int comparison function */
int compare_count(const void *a, const void *b);

void terminateProgram(int numberOfImages, SPPoint*** globalArray,
  SPPoint*** localArray, int* featureSizes, SPImageCounter* totalMatches, 
  SPPoint** queryImageHistogram, SPPoint** queryImageFeatures, int numOfQueryFeatures);

#endif /* MAIN_AUX_H_ */
