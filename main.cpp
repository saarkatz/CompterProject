#include <malloc.h>

#include "main_aux.h"
#include "sp_image_proc_util.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

extern "C" {
#include "SPPoint.h"

}

#define TERMINATE(Exit_code) terminateProgram(numberOfFeatures, globalArray, \
                              localArray, featureSizes, totalMatches, \
                              queryImageHistogram, queryImageFeatures, \
                              numOfQueryFeatures); \
                              return Exit_code;
#define ERROR_AND_EXIT(Message) printError((char*)Message);\
                          TERMINATE(1)
#define MEMORY_FAILURE "memory failure"
#define INVALID_NUMBER_OF_IMAGES "invalid number of images\n"
#define INVALID_NUMBER_OF_BINS "invalid number of bins\n"
#define INVALID_NUMBER_OF_FEATURES "invalid number of features\n"
#define K_CLOSEST 5

int main() {
  char imageDirectory[STRING_SIZE];
  char imagePrefix[STRING_SIZE];
  int numOfImages = 0;
  char imageSuffix[STRING_SIZE];
  int numberOfBins = 0;
  int numberOfFeatures = 0;
  char queryPath[STRING_SIZE];

  SPPoint*** globalArray = NULL;
  SPPoint*** localArray = NULL;
  int* featureSizes = NULL;

  char filePath[STRING_SIZE];

  SPPoint** queryImageHistogram = NULL;
  SPPoint** queryImageFeatures = NULL;
  int numOfQueryFeatures=0;
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

  globalArray = (SPPoint***)malloc(numOfImages * sizeof(SPPoint**));
  if (globalArray == NULL) {
    ERROR_AND_EXIT(MEMORY_FAILURE);
  }
  localArray = (SPPoint***)malloc(numOfImages * sizeof(SPPoint**));
  if (localArray == NULL) {
    ERROR_AND_EXIT(MEMORY_FAILURE);
  }
  featureSizes = (int*)calloc(numOfImages , sizeof(int));
  if (featureSizes == NULL) {
    ERROR_AND_EXIT(MEMORY_FAILURE);
  }


  for (int i = 0; i < numOfImages; i++) {
    //imageDirectory+imageSuffix+index+imageSuffix
    sprintf(filePath, "%s%s%d%s", imageDirectory, imagePrefix, i, imageSuffix);
    globalArray[i] = spGetRGBHist(filePath, i, numberOfBins);
    localArray[i] = spGetSiftDescriptors(filePath, i, numberOfFeatures, &featureSizes[i]); // This is the same as "features + i"
  }

  //---------------------------------
  queryImagePrompt();
  scanf("%s", queryPath);


  totalMatches = (SPImageCounter*)malloc(numOfImages * sizeof(SPImageCounter));
  if (totalMatches == NULL) {
    ERROR_AND_EXIT(MEMORY_FAILURE);
  }


  while (queryPath[0] != '#') {
   for (int i = 0; i < numOfImages; i++) {
      totalMatches[i] = { i, 0 };
    }


    //initilize queryImageHistogram
    queryImageHistogram = spGetRGBHist(queryPath, 0, numberOfBins);
    if (queryImageHistogram == NULL) {
      ERROR_AND_EXIT(MEMORY_FAILURE);
    }

    //initilize queryImageFeatures
    queryImageFeatures = spGetSiftDescriptors(queryPath,
      0, numberOfFeatures, &numOfQueryFeatures);
    if (queryImageFeatures == NULL) {
      ERROR_AND_EXIT(MEMORY_FAILURE);
    }

    resultArray = spBestHistDistance(K_CLOSEST, queryImageHistogram, numOfImages, globalArray);
    if (resultArray == NULL) {
      ERROR_AND_EXIT(MEMORY_FAILURE);
    }

    printKclosest(resultArray, K_CLOSEST, (char*)"global");
    free(resultArray);

    //calculate closest local feature for each query feature
    for (int i = 0; i < numOfQueryFeatures; ++i) {
      resultArray = spBestSIFTL2SquaredDistance(K_CLOSEST, queryImageFeatures[i], localArray,
        numOfImages, featureSizes);
      if (resultArray == NULL) {
        ERROR_AND_EXIT(MEMORY_FAILURE);
      }
      //count the images with the closest features
      for (int j = 0; j < K_CLOSEST; ++j) {
        totalMatches[resultArray[j]].count++;
      }
      free(resultArray);
    }
    qsort(totalMatches, numOfImages, sizeof(SPImageCounter), compare_count);
    resultArray = (int*)malloc(K_CLOSEST * sizeof(int));
    if (resultArray == NULL) {
      ERROR_AND_EXIT(MEMORY_FAILURE);
    }
    for (int i = 0; i < K_CLOSEST; i++) {
      resultArray[i] = totalMatches[i].index;
    }
    printKclosest(resultArray, K_CLOSEST, (char*)"local");
    free(resultArray);


    for (int i = 0; i < 3; ++i) {
            spPointDestroy(queryImageHistogram[i]);
          }
          free(queryImageHistogram);

          for (int i = 0; i < numOfQueryFeatures; ++i) {
            spPointDestroy(queryImageFeatures[i]);
          }
          free(queryImageFeatures);

    queryImagePrompt();
    scanf("%s", queryPath);
  }




  for (int i = 0; i < numOfImages; i++) {
	  for (int j = 0; j < HISTOGARM_SIZE; j++) {
        spPointDestroy(globalArray[i][j]);
        }
        free(globalArray[i]);
  }
  free(localArray);



  for (int i = 0; i < numOfImages; i++) {
      for (int j = 0; j < featureSizes[i]; j++) {
          spPointDestroy(localArray[i][j]);
      }
      free(localArray[i]);
  }
  free(globalArray);


  free(featureSizes);

      for (int i = 0; i < 3; ++i) {
        spPointDestroy(queryImageHistogram[i]);
      }
      free(queryImageHistogram);



      for (int i = 0; i < numOfQueryFeatures; ++i) {
        spPointDestroy(queryImageFeatures[i]);
      }
      free(queryImageFeatures);


    free(totalMatches);


  exitingMsg();
  TERMINATE(0);
}
