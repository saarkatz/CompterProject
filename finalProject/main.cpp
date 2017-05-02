
#define DEFAULT_FILE "spcbir.config"
/*spImagesDirectory
spExtractionMode
spImagesPrefix
spImagesSuffix
spKDTreeSplitMethod
spKNN
spLoggerFilename
spLoggerLevel
spMinimalGUI
spNumOfFeatures
spNumOfImages
spNumOfSimilarImages
spPCADimension
spPCAFilename*/
int main(int argc, char const *argv[]){
	SP_CONFIG_MSG msg;
	SPConfig config = spConfigCreate(filename,&msg);
	char* filename;
	if(argc==1){
		filename=DEFAULT_FILE;
	}
	else{
		if (argc>3){
			//TODO - handle the fact that we were given too much parameters
		}
	}
	if(msg!=SP_CONFIG_SUCCESS){
		printConfigError(msg);
	}
	if(config->spExtractionMode){
		extractFeatures();
		saveToDirectory();
	}
	else{
		extractFromFile();
	}
	initDatabase();
	do{
		if(reciveCommand()==QUERY){
			findSimilarImages();
			showResults();
		}
	}while();
}
