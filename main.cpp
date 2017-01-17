
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "main_aux.h"

int main() {
	char* imageDirectory;
	char* imagePrefix;
	int numOfImages;
	char* imageSuffix;
	int numberOfBins;
	int numberOfFeatures;
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
		printError("invalid number of images\n");
	}


	imageSuffixPrompt();
	scanf("%s",imageSuffix);

	numOfBinsPrompt();
	scanf("%d",&numberOfBins);
	if(numberOfBins < 1 || numberOfBins > 255){
		printError("invalid number of bins\n");
	}

	numOfFeaturesPrompt();
	scanf("%d",&numberOfFeatures);
	if(numberOfFeatures < 1){
		printError("invalid number of features\n");
	}

	//calculates the file path string size
	int stringSize = strlen(imageDirectory)+strlen(imageSuffix)+5;
	filePath = (char*)malloc(stringSize*sizeof(char));
	if (filePath == NULL) {
		printError(MEMORY_FAILURE);
	}

	globalArray=(SPPoint***)malloc(numOfImages*sizeof(SPPoint**));
	if (globalArray == NULL) {
		printError(MEMORY_FAILURE);
	}
	localArray=(SPPoint***)malloc(numOfImages*sizeof(SPPoint**));
	if (localArray == NULL) {
		printError(MEMORY_FAILURE);
	}
	featureSizes=(int*)malloc(numOfImages*sizeof(int));
	if (featureSizes == NULL) {
		printError(MEMORY_FAILURE);
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
	if(queryPath[0]=='#'){
		exitingMsg();
		terminateProgram();
	}









}
