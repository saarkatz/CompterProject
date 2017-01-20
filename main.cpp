#include <malloc.h>

#include "main_aux.h"
#include "sp_image_proc_util.h"

extern "C" {
#include <cstdio>
#include <cstring>

#include "SPPoint.h"
}

#define TERMINATE() terminateProgram(numberOfFeatures, globalArray, \
                                  localArray, featureSizes, filePath); \
                  return;
#define ERROR_AND_EXIT(Message) printError(Message);\
                              TERMINATE()
#define MEMORY_FAILURE "memory failure"
#define INVALID_NUMBER_OF_IMAGES "invalid number of images\n"
#define INVALID_NUMBER_OF_BINS "invalid number of bins\n"
#define INVALID_NUMBER_OF_FEATURES "invalid number of features\n"


int main() {
	char* imageDirectory;
	char* imagePrefix;
	int numOfImages = 0;
	char* imageSuffix;
	int numberOfBins = 0;
	int numberOfFeatures = 0;
	char* queryPath;

	SPPoint*** globalArray;
	SPPoint*** localArray;
	int* featureSizes;

	char* filePath;

	imageDirectoryPrompt();
	scanf("%s",imageDirectory);

	imagePrefixPrompt();
	scanf("%s",imagePrefix);


	numOfImagesPrompt();
	scanf("%d",&numOfImages);
	if(numOfImages < 1){
      ERROR_AND_EXIT(INVALID_NUMBER_OF_IMAGES);
	}


	imageSuffixPrompt();
	scanf("%s",imageSuffix);

	numOfBinsPrompt();
	scanf("%d",&numberOfBins);
	if(numberOfBins < 1 || numberOfBins > 255){
      ERROR_AND_EXIT(INVALID_NUMBER_OF_BINS);
	}

	numOfFeaturesPrompt();
	scanf("%d",&numberOfFeatures);
	if(numberOfFeatures < 1){
      ERROR_AND_EXIT(INVALID_NUMBER_OF_FEATURES);
	}

	//calculates the file path string size
	filePath = (char*)malloc(STRING_SIZE*sizeof(char));
	if (filePath == NULL) {
      ERROR_AND_EXIT(MEMORY_FAILURE);
    }

	globalArray=(SPPoint***)malloc(numOfImages*sizeof(SPPoint**));
	if (globalArray == NULL) {
      ERROR_AND_EXIT(MEMORY_FAILURE);
    }
	localArray=(SPPoint***)malloc(numOfImages*sizeof(SPPoint**));
	if (localArray == NULL) {
      ERROR_AND_EXIT(MEMORY_FAILURE);
    }
	featureSizes=(int*)malloc(numOfImages*sizeof(int));
	if (featureSizes == NULL) {
      ERROR_AND_EXIT(MEMORY_FAILURE);
	}


	for(int i=0;i<numOfImages;i++){
		//imageDirectory+imageSuffix+index+imageSuffix
		sprintf(filePath,"%s%s%d%s",imageDirectory,imagePrefix,i,imageSuffix);
		globalArray[i] = spGetRGBHist(		 filePath, i, numberOfBins); 
		localArray[i] = spGetSiftDescriptors(filePath, i, numberOfFeatures, &featureSizes[i]); // This is the same as "features + i"
	}

//---------------------------------
	queryImagePrompt();
	scanf("%s",queryPath);
	while(queryPath[0]=='#'){

      // Insert code here

      queryImagePrompt();
      scanf("%s", queryPath);
	}
    exitingMsg();
    TERMINATE();









}
