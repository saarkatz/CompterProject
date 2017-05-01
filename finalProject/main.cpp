int main(){
	SP_CONFIG_MSG msg;
	SPConfig config = spConfigCreate(filename,&msg);
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