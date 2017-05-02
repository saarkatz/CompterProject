#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "SPConfig.h"
#define MAXBUF 1024 

typedef struct config_var 
{
   char before[256];
   char after[256];
}SPVar;



typedef enum search_method_t{RANDOM, MAX_SPREAD,
INCREMENTAL}SPSearchMethod;

struct sp_config_t{
char spImagesDirectory[256];
bool spExtractionMode;
char spImagesPrefix[256];
char spImagesSuffix[256];
SPSearchMethod spKDTreeSplitMethod;
int spKNN;
char spLoggerFilename[256];
int spLoggerLevel;
bool spMinimalGUI;
int spNumOfFeatures;
int spNumOfImages;
int spNumOfSimilarImages;
int spPCADimension;
char spPCAFilename[256];
};
SPSearchMethod getKDTreeSplitMethod(SPConfig config){
	return config->spKDTreeSplitMethod;
}
SPVar get_var(char* line){
  SPVar v;
  sscanf(line,"%[^= ] = %s",v.before,v.after);
  return v;
}

void print_var_arr(SPVar* array,int size){
  for(int i=0;i<size;i++){
    printf("(%s),(%s)\n",array[i].before,array[i].after);
  }
}
int first_nonwhitespace(char* s){
    size_t i = 0;
    while(s[i] == ' ' || s[i] == '\t'|| s[i] == '\n' || s[i] == '\r' || s[i] == '\f' || s[i] == '\v')
    {
        ++i;
    }
    return i;
}



SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	SPConfig config = (SPConfig)malloc(sizeof(struct sp_config_t));
	if(config==NULL){
		//TODO - handle
	}
	

	bool ImagesDirectoryDefined;
	bool ImagesPrefixDefined;
	bool ImagesSuffixDefined; 
	bool NumOfImagesDefined;
	
	//put in default paramaters, we change this later if in the config they are diffrent
	FILE *file = fopen (filename, "r");
	char* arg_arr[14] ={"spImagesDirectory",
		"spExtractionMode",
		"spImagesPrefix",
		"spImagesSuffix",
		"spKDTreeSplitMethod",
		"spKNN",
		"spLoggerFilename",
		"spLoggerLevel",
		"spMinimalGUI",
		"spNumOfFeatures",
		"spNumOfImages",
		"spNumOfSimilarImages",
		"spPCADimension",
		"spPCAFilename"};
  SPVar var_array[14];

 config->spPCADimension=20;
 config->spPCAFilename="pca.yml";
 config->spNumOfFeatures=100;
 config->spExtractionMode=true;;
 config->spMinimalGUI=false;
 config->spNumOfSimilarImages=1;
 config->spKNN=1;
 config->spKDTreeSplitMethod=MAX_SPREAD;
 config->spLoggerLevel= 3;
 config->spLoggerFilename="stdout";

  int var_num=0;
  if (file != NULL){ 
    char line[MAXBUF];
    while(fgets(line, sizeof(line), file) != NULL){
          if(line[first_nonwhitespace(line)]!='#') 
            var_array[var_num++]=get_var(line);
            } // End while
            fclose(file);
     }
    SPVar* tmp=(SPVar*)malloc(sizeof(SPVar));
    qsort(var_array,var_num,sizeof(SPVar),spCmpVar);
    for(int i=0;i<14;i++){
    	tmp->before=arg_arr[i];
    	tmp = bsearch(tmp,var_array,var_num,sizeof(SPVar),spCmpVar);
    	if(tmp!=NULL){
    		spCaseChoose(config,i,tmp);
    		ImagesDirectoryDefined|=(i=0);
    		ImagesPrefixDefined|=(i=2);
    		ImagesSuffixDefined|=(i=3);
    		NumOfImagesDefined|=(i=10);
    	}
    }
    if(!(ImagesDirectoryDefined&&
    	ImagesPrefixDefined&&
    	ImagesSuffixDefined&&
    	NumOfImagesDefined)){//one of the mandatory variable was not defined
    	printf("!!!!!!!!!!\n");//TODO!
    }
}

void spCaseChoose(SPConfig config,int i,SPVar* var){
	switch(i){
		case 0:
			setImagesDirectory(config,var);
			break;
		case 1:
			setExtractionMode(config,var);
			break;
		case 2:
			setImagesPrefix(config,var);
			break;
		case 3:
			setImagesSuffix(config,var);
			break;
		case 4:
			setKDTreeSplitMethod(config,var);
			break;
		case 5:
			setKNN(config,var);
			break;
		case 6:
			setLoggerFilename(config,var);
			break;
		case 7:
			setLoggerLevel(config,var);
			break;
		case 8:
			setNumOfFeatures(config,var);
			break;
		case 9:
			spNumOfFeatures(config,var);
			break;
		case 10:
			setNumOfImages(config,var);
			break;
		case 11:
			setNumOfSimilarImages(config,var);
			break;
		case 12:
			setPCADimension(config,var);
			break;
		case 13:
			setPCAFilename(config,var);
			break;
	}

}
/*
void spSetVar(SPConfig config,char* var,char* val){
	if(){

	}
}*/



int spCmpVar(const void *p, const void *q){
    return strcmp(((SPVar *)p)->before,((SPVar *)p)->after);
}


bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg){
	return config->spExtractionMode;
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
	return config->spMinimalGUI;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){
	return config->spNumOfImages;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
	return config->spNumOfFeatures;
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
	return config->spPCADimension;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,int index){
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){

}

void spConfigDestroy(SPConfig config){

}



void setPCAFilename(SPConfig config,SPVar* str){
    strcpy(config->spPCAFilename,str->after);
}
void setPCADimension(SPConfig config,SPVar* str){
	config->spPCADimension=atoi(str->after);
}
void setNumOfSimilarImages(SPConfig config,SPVar* str){
	config->spNumOfSimilarImages=atoi(str->after);
}
void setNumOfImages(SPConfig config,SPVar* str){
	config->spNumOfImages=atoi(str->after);
}
void setNumOfFeatures(SPConfig config,SPVar* str){
	config->spNumOfFeatures=atoi(str->after);
}
void setMinimalGUI(SPConfig config,SPVar* str){
	config->spMinimalGUI=(bool)strcmp("true",str->after);
}
void setLoggerLevel(SPConfig config,SPVar* str){
	config->spLoggerLevel=atoi(str->after);
}
void setLoggerFilename(SPConfig config,SPVar* str){
    strcpy(config->spLoggerFilename,str->after);
}
void setKNN(SPConfig config,SPVar* str){
	config->spKNN=atoi(str->after);
}
void setKDTreeSplitMethod(SPConfig config,SPVar* str){
	//config->spKDTreeSplitMethod=(str->after);//TODO switch case
	if(strcmp("RANDOM",str->after)){
		config->spKDTreeSplitMethod=RANDOM;
	}
	else if(strcmp("MAX_SPREAD",str->after)){
		config->spKDTreeSplitMethod=MAX_SPREAD;
	}
	else if(strcmp("INCREMENTAL",str->after)){
		config->spKDTreeSplitMethod=INCREMENTAL;
	}
	else{
		//TODO - return error
	}
}
void setImagesSuffix(SPConfig config,SPVar* str){
    strcpy(config->spImagesSuffix,str->after);
}
void setImagesPrefix(SPConfig config,SPVar* str){
    strcpy(config->spImagesPrefix,str->after);
}
void setImagesDirectory(SPConfig config,SPVar* str){
    strcpy(config->spImagesDirectory,str->after);
}
void setExtractionMode(SPConfig config,SPVar* str){
    strcpy(config->spExtractionMode,str->after);
}