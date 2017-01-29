#include <stdio.h>

#include "main_aux.h"

typedef struct hist_distance { int index; double distance; } SPHist;
typedef struct int_int_pair { int index; int count; } SPImageCounter;

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
  else if(q->distance>p->distance)return -1;
  else return 1;
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

int* spBestImagesBySifts(int kClosest, SPPoint** queryImageFeatures,
  int numOfQueryFeatures, int numOfImages, SPPoint*** localArray,
  int* featureSizes) {
  int* resultArray = NULL;
  SPImageCounter* totalMatches = (SPImageCounter*)malloc(numOfImages*sizeof(SPImageCounter));
  if (totalMatches == NULL) {
    return NULL;
  }

  for (int i = 0; i < numOfImages; i++) {
    totalMatches[i] = { i, 0 };
  }

  //calculate closest local feature for each query feature
  for (int i = 0; i < numOfQueryFeatures; ++i) {
    resultArray = spBestSIFTL2SquaredDistance(kClosest, queryImageFeatures[i],
      localArray, numOfImages, featureSizes);
    if (resultArray == NULL) {
      free(totalMatches);
      return NULL;
    }

    //count the images with the closest features
    for (int j = 0; j < kClosest; ++j) {
      totalMatches[resultArray[j]].count++;
    }

    free(resultArray);
  }
  qsort(totalMatches, numOfImages, sizeof(SPImageCounter), compare_count);

  resultArray = (int*)malloc(kClosest * sizeof(int));
  if (resultArray == NULL) {
    free(totalMatches);
    return NULL;
  }

  // Get the k first items from totalMatches.
  for (int i = 0; i < kClosest; i++) {
    resultArray[i] = totalMatches[i].index;
  }
  
  free(totalMatches);
  return resultArray;
}

void terminateProgram(int numOfImages, SPPoint*** globalArray,
  SPPoint*** localArray, int* featureSizes, SPPoint** queryImageHistogram,
  SPPoint** queryImageFeatures, int numOfQueryFeatures) {
  
  freeHistogramDatabase(globalArray,numOfImages);
  freeFeatureDatabase(localArray,numOfImages,featureSizes);
  freeHistogram(queryImageHistogram);
  freeFeatures(queryImageFeatures,numOfQueryFeatures);
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
 

void freeHistogram(SPPoint** histogramArray){
  if(histogramArray!=NULL){
    for (int i = 0; i < 3; ++i) {
        if(histogramArray[i]!=NULL)
          spPointDestroy(histogramArray[i]);
        }
        free(histogramArray);
  }
}
void freeFeatureDatabase(SPPoint*** database, int databaseSize,int* sizeArray){
  if(database!=NULL){
    for (int i = 0; i < databaseSize; i++) {
      freeFeatures(database[i],sizeArray[i]);
     }
      free(database);
  }
  if(sizeArray!=NULL) free(sizeArray);
}

void freeFeatures(SPPoint** featuresArray,int arraySize){
    if(featuresArray!=NULL){
      for (int i = 0; i < arraySize; ++i) {
        if(featuresArray[i]!=NULL)
          spPointDestroy(featuresArray[i]);
          }
      free(featuresArray);
    }
        
 }

 void freeHistogramDatabase(SPPoint*** database, int databaseSize){
    if(database!=NULL){
     for (int i = 0; i < databaseSize; ++i){
      if(database[i]!=NULL)
        freeHistogram(database[i]);
     }
      free(database);
    }
  }