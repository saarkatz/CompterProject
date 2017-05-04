printConfigError(SP_CONFIG_MSG msg){

}
extractFeatures(){

}
int saveFeatureToFile(SPConfig config, SPPoint** point){

}

char* reciveCommand(){

}

int* findSimilarImages(SPConfig config,SPKDTreeNode* tree,SPPoint* queryPoint){


}
void showResults(SPConfig config, int* array){

}

SPPoint** createPointFeaturesFromPath(SPConfig config, char* path,ImageProc* proc_util){

}

SPPoint***  extractFeaturesFromFeatureFiles(SPConfig config,ImageProc* proc_util){

}

//method to check if the input we recived was a request for another query.
bool requestingQuery(char* cmd){

}

SPPoint***  createFeatureFiles(SPConfig config,ImageProc* proc_util){
	char tmp_path[BUFF_SIZE];
	SP_CONFIG_MSG msg;
	proc_util=ImageProc(config);
	SPPoint*** resultArray=(SPPoint***)malloc((config->spNumOfImages)*sizeof(SPPoint**));
	for(int i=0;i<config->spNumOfImages;i++){//for every picture in the images folder
		spConfigGetImagePath(tmp_path,config,i);
		resultArray[i]=createPointFeaturesFromPath(config,tmp_path,proc_util);
		saveFeatureToFile(config,resultArray[i]);
	}
	return resultArray;
}

char* getPath(char* cmd){
	
}