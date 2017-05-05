printConfigError(SP_CONFIG_MSG msg){

}

int saveFeatureToFile(SPConfig config, SPPoint** point,char* path,int nFeatures){
	char* featPath = createFeatPath(config,point,path);
	FILE* output_file =(FILE*)fopen(featPath,"w");
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

SPPoint** downgradeStar(SPConfig config,SPPoint*** array,int* numFeatureArray,int size){
	SPPoint** database = (SPPoint**)malloc(totalNumOfFeatures*sizeof(SPPoint*));
	//TODO
	return database;
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
	SPBPQueue* bpq;

	for(int i=0;i<nFeatures;i++){
		bpq = spBPQueueCreate(config->spKNN);
		k_nearest_search(tree,bpq,queryPoint[i]);
		incCounters(bpq,matchCountArray);
		spBPQueueDestroy(bpq);
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

void incCounters(SPConfig config,SPBPQueue* bpq,SPCounter* matchCountArray){
	BPQueueElement* peekElement = (BPQueueElement*)malloc(sizeof(BPQueueElement));
  	for (int i = 0; i < config->spKNN; i++) {
    spBPQueuePeek(bpq, peekElement);
    matchCountArray[peekElement->index].counter++;
    spBPQueueDequeue(bpq);
    //printf("rank %d:  , image index: %d,distace: %hf\n",i+1,peekElement->index,peekElement->value);
  }
	free(peekElement);
}


SPPoint** createPointFeaturesFromPath(SPConfig config,char* path,int index ,ImageProc* proc_util){
	return proc_util.getImageFeatures(config,index,path);
}



SPPoint***  createFeatureFiles(SPConfig config,ImageProc* proc_util,int* nFeaturesArray){
	char tmp_path[BUFF_SIZE];
	SP_CONFIG_MSG msg;
	proc_util=ImageProc(config);
	SPPoint*** resultArray=(SPPoint***)malloc((config->spNumOfImages)*sizeof(SPPoint**));
	for(int i=0;i<config->spNumOfImages;i++){//for every picture in the images folder
		spConfigGetImagePath(tmp_path,config,i);
		resultArray[i]=createPointFeaturesFromPath(config,tmp_path,proc_util,nFeaturesArray[i]);
		saveFeatureToFile(config,resultArray[i]);
	}
	return resultArray;
}

SPPoint***  extractFeaturesFromFeatureFiles(SPConfig config,
		ImageProc* proc_util,int* nFeaturesArray){
	SPPoint*** database = (SPPoint***)malloc(config->spNumOfImages*sizeof(SPPoint**));
	char tmp_path[BUFF_SIZE];
	int* nFeatures = (int*)malloc(sizeof(int));
	for (int i = 0; i < config->spNumOfImages; ++i){		
	spConfigGetImagePath(tmp_path,config,i);
	database[i]= readPointFeaturesFromFile(config,tmp_path,nFeaturesArray[i]);
	}
	return database;
	//more stuff here

}


