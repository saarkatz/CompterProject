
extern "C"{
#include "stdio.h"
}
#include "main_aux.h"
int main(){
	char* imageDirectory;
	char* imagePrefix;
	int numOfImages;
	char* imageSuffix;
	int numberOfBins;
	int numberOfFeatures;
	char* queryPath;

	imageDirectoryPrompt();
	scanf("%s",imageDirectory);

	imagePrefixPrompt();
	scanf("%s",imagePrefix);

	numOfImagesPrompt();
	scanf("%d",&numOfImages);


	imageSuffixPrompt();
	scanf("%s",imageSuffix);

	numOfBinsPrompt();
	scanf("%d",&numberOfBins);

	numOfFeaturesPrompt();
	scanf("%d",&numberOfFeatures);

	queryImagePrompt();
	scanf("%s",queryPath);



	int stringSize = strlen(imageDirectory)+strlen(imageSuffix)+5;
	char* filePath = (char*)malloc(stringSize*sizeof(char));

	int i=0;
	sprintf(filePath,"%s%s%d%s",imageDirectory,imagePrefix,i,imageSuffix);





	//imageDirectory+imageSuffix+index+imageSuffix


}
