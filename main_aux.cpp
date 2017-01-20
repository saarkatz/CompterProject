#include <stdio.h>

#include "main_aux.h"

void imageDirectoryPrompt() {
	printf("Enter images directory path:\n");
}

void imagePrefixPrompt() {
	printf("Enter images prefix:\n");
}

void numOfImagesPrompt() {
	printf("Enter number of images:\n");
}

void imageSuffixPrompt(){
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

void terminateProgram(int numberOfFeatures, SPPoint*** globalArray,
  SPPoint*** localArray, int* featureSizes, char* filePath) {
  free(filePath);
  for (int i = 0; i < numberOfFeatures; i++) {
    if (globalArray != NULL) {
      for (int j = 0; j < HISTOGARM_SIZE; j++) {
        spPointDestroy(globalArray[i][j]);
      }
    }
    if (localArray != NULL && featureSizes != NULL) {
      for (int j = 0; j < featureSizes[i]; j++) {
        spPointDestroy(localArray[i][j]);
      }
    }
  }
  free(globalArray);
  free(localArray);
  free(featureSizes);
}

