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

void printNearestImages(int*) {

}

void printError(char* msg) {
	printf("An error occurred - %s", msg);
	terminateProgram();
}
