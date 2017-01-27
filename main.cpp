#include <malloc.h>

#include "main_aux.h"
#include "sp_image_proc_util.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

extern "C" {
#include "SPPoint.h"
}

#define TERMINATE(Exit_code) terminateProgram(numOfImages, globalArray, \
                                localArray, featureSizes, totalMatches, \
                                queryImageHistogram, queryImageFeatures, \
                                numOfQueryFeatures); \
                             return Exit_code;

#define ERROR_AND_EXIT(Message) printError((char*)Message);TERMINATE(1);

#define CHECK_MEMORY_FAILURE(ARG) if (ARG == NULL) { \
                                    ERROR_AND_EXIT(MEMORY_FAILURE);}

#define MEMORY_FAILURE "memory failure"
#define INVALID_NUMBER_OF_IMAGES "invalid number of images\n"
#define INVALID_NUMBER_OF_BINS "invalid number of bins\n"
#define INVALID_NUMBER_OF_FEATURES "invalid number of features\n"
#define K_CLOSEST 5

int main() {
  char imageDirectory[STRING_SIZE], imagePrefix[STRING_SIZE],
    imageSuffix[STRING_SIZE], queryPath[STRING_SIZE];
  int numOfImages = 0, numberOfBins = 0, numberOfFeatures = 0;

  SPPoint*** globalArray = NULL;
  SPPoint*** localArray = NULL;
  int* featureSizes = NULL;

  char filePath[STRING_SIZE];

  SPPoint** queryImageHistogram = NULL;
  SPPoint** queryImageFeatures = NULL;
  int numOfQueryFeatures = 0;

  SPImageCounter* totalMatches = NULL;
  int* resultArray = NULL;

  imageDirectoryPrompt();
  scanf("%s", imageDirectory);

  imagePrefixPrompt();
  scanf("%s", imagePrefix);

  numOfImagesPrompt();
  scanf("%d", &numOfImages);
  if (numOfImages < 1) {
    ERROR_AND_EXIT(INVALID_NUMBER_OF_IMAGES);
  }

  imageSuffixPrompt();
  scanf("%s", imageSuffix);

  numOfBinsPrompt();
  scanf("%d", &numberOfBins);
  if (numberOfBins < 1 || numberOfBins > 255) {
    ERROR_AND_EXIT(INVALID_NUMBER_OF_BINS);
  }

  numOfFeaturesPrompt();
  scanf("%d", &numberOfFeatures);
  if (numberOfFeatures < 1) {
    ERROR_AND_EXIT(INVALID_NUMBER_OF_FEATURES);
  }

  //stage 2
  globalArray = (SPPoint***)malloc(numOfImages * sizeof(SPPoint**));
  CHECK_MEMORY_FAILURE(globalArray);

  localArray = (SPPoint***)malloc(numOfImages * sizeof(SPPoint**));
  CHECK_MEMORY_FAILURE(localArray);

  featureSizes = (int*)calloc(numOfImages, sizeof(int));
  CHECK_MEMORY_FAILURE(featureSizes);

  for (int i = 0; i < numOfImages; i++) {
    sprintf(filePath, "%s%s%d%s", imageDirectory, imagePrefix, i, imageSuffix);
    globalArray[i] = spGetRGBHist(filePath, i, numberOfBins);
    localArray[i] = spGetSiftDescriptors(filePath, i, numberOfFeatures, &featureSizes[i]); // This is the same as "features + i"
  }

  //---------------------------------
  queryImagePrompt();
  scanf("%s", queryPath);

  //stage 4
  totalMatches = (SPImageCounter*)malloc(numOfImages * sizeof(SPImageCounter));
  CHECK_MEMORY_FAILURE(totalMatches);

  while (queryPath[0] != '#') {
    for (int i = 0; i < numOfImages; i++) {
      totalMatches[i] = { i, 0 };
    }


    //initilize queryImageHistogram
    queryImageHistogram = spGetRGBHist(queryPath, 0, numberOfBins);
    CHECK_MEMORY_FAILURE(queryImageHistogram);

    //initilize queryImageFeatures
    queryImageFeatures = spGetSiftDescriptors(queryPath, 0, 
                                    numberOfFeatures, &numOfQueryFeatures);
    CHECK_MEMORY_FAILURE(queryImageFeatures);

    resultArray = spBestHistDistance(K_CLOSEST, queryImageHistogram,
                                      numOfImages, globalArray);
    CHECK_MEMORY_FAILURE(resultArray);

    printKclosest(resultArray, K_CLOSEST, (char*)"global");
    free(resultArray);

    //calculate closest local feature for each query feature
    for (int i = 0; i < numOfQueryFeatures; ++i) {
      resultArray = spBestSIFTL2SquaredDistance(K_CLOSEST, queryImageFeatures[i], localArray,
        numOfImages, featureSizes);
      CHECK_MEMORY_FAILURE(resultArray);

      //count the images with the closest features
      for (int j = 0; j < K_CLOSEST; ++j) {
        totalMatches[resultArray[j]].count++;
      }
      free(resultArray);
    }
    qsort(totalMatches, numOfImages, sizeof(SPImageCounter), compare_count);

    resultArray = (int*)malloc(K_CLOSEST * sizeof(int));
    CHECK_MEMORY_FAILURE(resultArray);

    for (int i = 0; i < K_CLOSEST; i++) {
      resultArray[i] = totalMatches[i].index;
    }

    printKclosest(resultArray, K_CLOSEST, (char*)"local");
    free(resultArray);

    freeHistogram(queryImageHistogram);
    freeFeatures(queryImageFeatures, numOfQueryFeatures);

    queryImagePrompt();
    scanf("%s", queryPath);
  }
  // Here queryImageHistogram and queryImageFeatures should be free so we
  // dont want to send invalid pointers to terminate or else we will get segfault
  queryImageHistogram = NULL;
  queryImageFeatures = NULL;

  exitingMsg();
  TERMINATE(0);
}
