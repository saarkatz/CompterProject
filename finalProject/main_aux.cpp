printConfigError(SP_CONFIG_MSG msg){

}
extractFeatures(){

}
saveToDirectory(){

}
extractFromFile(){
	
}
initDatabase(){

}
reciveCommand(){

}
findSimilarImages(){

}
showResults(){

}



int createFeaturesFile(char *path, SPPoint **features, int dim, int index,
                       int numOfFeatures){
	
}

SPPoint **readFeaturesFile(const char *path, int *nFeatures){
	
}


int extractFromImages(SPConfig config){
	
}

SPPoint **extractFromFile(SPConfig config, int *totalNumOfFeatures){
	
}


KDTree *extractKDTree(SPConfig config){
	
}


int searchSimilarImages(SPConfig config, char *queryPath, KDTree *kdTree){
	
}