
#define DEFAULT_FILE "spcbir.config"
//typedef enum create_features_enum{QUERY,DATABASE_IMAGE}SPFeatureCreateMode;
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
	int* numFeatureArray=(int*)malloc(config->spNumOfImages*sizeof(int));
	SPPoint*** featureArray;
	if(msg!=SP_CONFIG_SUCCESS){
		printConfigError(msg);
	}
	ImageProc* proc_util=NULL;
	if(config->spExtractionMode){
		featureArray= createFeatureFiles(config,proc_util,numFeatureArray);
	}
	else{
		featureArray= extractFeaturesFromFeatureFiles(config,proc_util,numFeatureArray);
	}
	int totalNumOfFeatures = sumFeatures(numFeatureArray);

	char* cmd;
	char* path;
	SPPoint** queryPointFeatures;
	SPKDArray* kdarr;
	SPKDTreeNode* tree;
	int* resultArray;
	int* nFeatures=(int*)malloc(sizeof(int));
	resultArray=(int*)malloc(config->spKNN*sizeof(int));


	//SPPoint** database = (SPPoint**)malloc(totalNumOfFeatures*sizeof(SPPoint*));
	SPPoint** database = downgradeStar(config,featureArray,numFeatureArray,totalNumOfFeatures);

	kdarr  = init(database,config->spNumOfImages);
	tree =	create_tree_main(config,kdarr);
	do{
		cmd = reciveCommand();
		if(requestingQuery(cmd)){
			path=getPath(cmd);
			queryPointFeatures = createPointFeaturesFromPath(config,path,proc_util,nFeatures);
			resultArray = findSimilarImages(config,tree,queryPoint,*nFeatures);
			showResults(config, resultArray);
		}
		else break;
	}while(1);
	//destroy stuff here

}
