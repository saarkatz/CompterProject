#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#include <malloc.h>

extern "C" {
#include "SPPoint.h"
}

#define STRING_SIZE 1024
#define HISTOGARM_SIZE 3

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

void terminateProgram(int numberOfFeatures, SPPoint*** globalArray,
  SPPoint*** localArray, int* featureSizes, char* filePath)

#endif /* MAIN_AUX_H_ */
