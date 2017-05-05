printConfigError(SP_CONFIG_MSG msg){

}
extractFeatures(){

}

int saveFeatureToFile(SPConfig config, SPPoint** point,char* path,int nFeatures){
	FILE* output_file =(FILE*)fopen(path,"w");
	int dim = spPointGetDimension(*point);
	fwrite(&nFeatures,sizeof(int),1,output_file);
	for(int i=0;i<nFeatures;i++){
		for(int j=0;j<dim;j++){
			fwrite(spPointGetAxisCoor(point[i],j),sizeof(double),1,output_file);
		}
	}
	return 0;
}


SPPoint** readPointFeaturesFromFile(SPConfig config,int index, char* path,int* nFeatures){
	FILE* input_file =(FILE*)fopen(path,"r");
	fread(nFeatures,sizeof(int),1,input_file);
	int dim = config->spPCADimension;
	double* data = (double*)malloc(dim*sizeof(double));
	SPPoint** pointFeatures = (SPPoint**)malloc(nFeatures*sizeof(SPPoint*));
	for(int i=0;i<nFeatures;i++){
		fread(data,sizeof(double),dim,output_file);
		pointFeatures[i]=spPointCreate(data,dim,index)
	}
	return pointFeatures;
}

char* reciveCommand(){

}
char* getPath(char* cmd){
	
}

//method to check if the input we recived was a request for another query.
bool requestingQuery(char* cmd){

}



void showResults(SPConfig config, int* array){
	if(config->spMinimalGUI){
		//TODO
	}
	else{
		//TODO
	}
}

typedef struct sp_counter{
	int counter;
	int index;
}SPCounter;

int* findSimilarImages(SPConfig config,SPKDTreeNode* tree,SPPoint** queryPoint,int nFeatures){

	SPCounter* matchCountArray = (SPCounter*)malloc(config->spNumOfImages*sizeof(SPCounter));
	//for every feature of the query
	//run knn search and for increment the counter 
	for(int i=0;i<nFeatures;i++){
		//k_nearest_search()///TODO
	}
	qsort(matchCountArray,config->spNumOfImages,sizeof(SPCounter),spCounterCmp);
	int* bestArray = (int*)malloc(config->spKNN*sizeof(int));
	for (int i = 0; i < config->spKNN; ++i){
		bestArray[i]=matchCountArray[i]->index;
	}
	return bestArray;
}
int spCounterCmp(const void* p1,const void* p2){
	return (((SPCounter*)p1)->counter)-(((SPCounter*)p1)->counter) ;
}




SPPoint** createPointFeaturesFromPath(SPConfig config,char* path,int index ,ImageProc* proc_util){
	return proc_util.getImageFeatures(config,index,path);
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

SPPoint***  extractFeaturesFromFeatureFiles(SPConfig config,ImageProc* proc_util){

}


