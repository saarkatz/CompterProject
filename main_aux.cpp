#include <stdio.h>

#include "main_aux.h"

typedef struct hist_distance { int index; double distance; } SPHist;

void imageDirectoryPrompt() {
  printf("Enter images directory path:\n");
}

void imagePrefixPrompt() {
  printf("Enter images prefix:\n");
}

void numOfImagesPrompt() {
  printf("Enter number of images:\n");
}

void imageSuffixPrompt() {
  printf("Enter images suffix:\n");
}

void numOfBinsPrompt() {
  printf("Enter number of bins:\n");
}

void numOfFeaturesPrompt() {
  printf("Enter number of features:\n");
}

void queryImagePrompt() {
  printf("Enter a query image or # to terminate:\n");
}

void exitingMsg() {
  printf("Exiting...\n");
}

void printNearestImages(int* numImages) {

}

void printError(char* msg) {
  printf("An error occurred - %s", msg);
}

int compare_count(const void *a, const void *b)
{
  const SPImageCounter *ia = (const SPImageCounter *)a; // casting pointer types 
  const SPImageCounter *ib = (const SPImageCounter *)b;
  return ib->count - ia->count;
  /* integer comparison: returns negative if a > b
  and positive if b > a */
}

int compareHistDistance(const void *vp, const void *vq) {
  const SPHist *p = (SPHist*)vp;
  const SPHist *q = (SPHist*)vq;
  if(q->distance==p->distance)return 0;
  if(q->distance>p->distance)return -1;
  if(q->distance<p->distance)return 1;
}

int* spBestHistDistance(int kClosest, SPPoint** queryImageHistogram,
  int numOfImages, SPPoint*** globalArray) {
  //holds the kClosest indexs with respect to global features
  int* result = (int*)malloc(kClosest * sizeof(int));
  if (result == NULL) {
    return NULL;
  }

  //initilize  histDistanceArray
  SPHist* histDistanceArray = (SPHist*)malloc(numOfImages * sizeof(SPHist));
  if (histDistanceArray == NULL) {
    return NULL;
  }

  //calculate the distance from the query feature
  //with respect to RGB histogram for each image 
  for (int i = 0; i < numOfImages; i++) {
    histDistanceArray[i] = { i,spRGBHistL2Distance(queryImageHistogram,globalArray[i]) };
  }

  //sort with respect to the distance from the query image
  qsort(histDistanceArray, numOfImages, sizeof(SPHist), compareHistDistance);

  //create the result array with the kClosest images
  for (int i = 0; i < kClosest; ++i) {
    result[i] = histDistanceArray[i].index;
  }
  free(histDistanceArray);
  return result;
}

void terminateProgram(int numberOfFeatures, SPPoint*** globalArray,
  SPPoint*** localArray, int* featureSizes, SPImageCounter* totalMatches, 
  SPPoint** queryImageHistogram, SPPoint** queryImageFeatures, int numOfQueryFeatures) {
  for (int i = 0; i < numberOfFeatures; i++) {
    if (globalArray != NULL) {
      for (int j = 0; j < HISTOGARM_SIZE; j++) {
        if (globalArray[i] != NULL) {
          spPointDestroy(globalArray[i][j]);
        }
      }
      free(globalArray[i]);
    }
    if (localArray != NULL && featureSizes != NULL) {
      for (int j = 0; j < featureSizes[i]; j++) {
        if (localArray[i] != NULL) {
          spPointDestroy(localArray[i][j]);
        }
      }
      free(localArray[i]);
    }
  }
  free(globalArray);
  free(localArray);
  free(featureSizes);

  if (queryImageHistogram != NULL) {
    for (int i = 0; i < 3; ++i) {
      spPointDestroy(queryImageHistogram[i]);
    }
    free(queryImageHistogram);
  }

  if (queryImageFeatures != NULL) {
    for (int i = 0; i < numOfQueryFeatures; ++i) {
      spPointDestroy(queryImageFeatures[i]);
    }
    free(queryImageFeatures);
  }

  free(totalMatches);
}

void printKclosest(int* array, int kClosest, char* str) {
  printf("Nearest images using %s descriptors:\n", str);
  for (int i = 0; i < kClosest; ++i) {
    printf("%d", array[i]);
    if (i < kClosest - 1) {
      printf(", ");
    }
  }
  printf("\n");
}
