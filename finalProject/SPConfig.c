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

/* Declare the set functions */
void setPCAFilename(SPConfig config, SPVar* str);
void setPCADimension(SPConfig config, SPVar* str);
void setNumOfSimilarImages(SPConfig config, SPVar* str);
void setNumOfImages(SPConfig config, SPVar* str);
void setNumOfFeatures(SPConfig config, SPVar* str);
void setMinimalGUI(SPConfig config, SPVar* str);
void setLoggerLevel(SPConfig config, SPVar* str);
void setLoggerFilename(SPConfig config, SPVar* str);
void setKNN(SPConfig config, SPVar* str);
void setKDTreeSplitMethod(SPConfig config, SPVar* str);
void setImagesSuffix(SPConfig config, SPVar* str);
void setImagesPrefix(SPConfig config, SPVar* str);
void setImagesDirectory(SPConfig config, SPVar* str);
void setExtractionMode(SPConfig config, SPVar* str);


SPSearchMethod spConfigGetSplitMethod(SPConfig config, SP_CONFIG_MSG *msg) {
  if (NULL == config) {
    *msg = SP_CONFIG_INVALID_ARGUMENT;
    return -1;
  }
  *msg = SP_CONFIG_SUCCESS;
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

void spCaseChoose(SPConfig config, int i, SPVar* var) {
  switch (i) {
  case 0:
    setImagesDirectory(config, var);
    break;
  case 1:
    setExtractionMode(config, var);
    break;
  case 2:
    setImagesPrefix(config, var);
    break;
  case 3:
    setImagesSuffix(config, var);
    break;
  case 4:
    setKDTreeSplitMethod(config, var);
    break;
  case 5:
    setKNN(config, var);
    break;
  case 6:
    setLoggerFilename(config, var);
    break;
  case 7:
    setLoggerLevel(config, var);
    break;
  case 8:
    setMinimalGUI(config, var);
    break;
  case 9:
    setNumOfFeatures(config, var);
    break;
  case 10:
    setNumOfImages(config, var);
    break;
  case 11:
    setNumOfSimilarImages(config, var);
    break;
  case 12:
    setPCADimension(config, var);
    break;
  case 13:
    setPCAFilename(config, var);
    break;
  }
}

int spCmpVar(const void *p, const void *q) {
  return strcmp(((SPVar *)p)->before, ((SPVar *)q)->before);
}

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
  /* define all the variables */
  SPConfig config;

  bool ImagesDirectoryDefined=0;
  bool ImagesPrefixDefined=0;
  bool ImagesSuffixDefined=0;
  bool NumOfImagesDefined=0;

  char* arg_arr[14] = { "spImagesDirectory",
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
    "spPCAFilename" };

  SPVar var_array[14];

  int var_num = 0;

  char line[MAXBUF];

  SPVar* tmp; // TODO - tmp is a bad name

  if (NULL == filename) {
    *msg = SP_CONFIG_INVALID_ARGUMENT;
    return NULL;
  }
  
  FILE *file = fopen(filename, "r");
  if (NULL == file) {
    *msg = SP_CONFIG_CANNOT_OPEN_FILE;
    return NULL;
  }
  do {
    *msg = SP_CONFIG_SUCCESS;

    config = (SPConfig)malloc(sizeof(struct sp_config_t));
    if (NULL == config) {
      *msg = SP_CONFIG_ALLOC_FAIL;
      break;
    }
    do {
      //put in default paramaters, we change this later if in the config they are diffrent
      config->spPCADimension = SP_DEFAULT_PCADIM;
      strcpy(config->spPCAFilename, SP_DEFAULT_PCAFILENAME);
      config->spNumOfFeatures = SP_DEFAULT_NUMOFFEATURS;
      config->spExtractionMode = SP_DEFAULT_EXTRACTMODE;
      config->spMinimalGUI = SP_DEFAULT_MINIMALGUI;
      config->spNumOfSimilarImages = SP_DEFAULT_NUMSIMIMG;
      config->spKNN = SP_DEFAULT_KNN;
      config->spKDTreeSplitMethod = SP_DEFUALT_SPLITMETHOD;
      config->spLoggerLevel = SP_DEFUALT_LOGGERLEVEL;
      strcpy(config->spLoggerFilename, SP_DEFUALT_LOGGERFILENAME);

      while (fgets(line, sizeof(line), file) != NULL) {
        if (line[first_nonwhitespace(line)] != '#')
          var_array[var_num++] = get_var(line);
      }

      /* TODO - This entire part is broken and needs to be remade */
      qsort(var_array, var_num, sizeof(SPVar), spCmpVar);
      for (int i = 0; i < 14; i++) {
        strcpy(tmp->before, arg_arr[i]);
        tmp = (SPVar*)bsearch(tmp, var_array, var_num, sizeof(SPVar), spCmpVar);
        if (tmp != NULL) {
          spCaseChoose(config, i, tmp);
          ImagesDirectoryDefined |= (i == 0);
          ImagesPrefixDefined |= (i == 2);
          ImagesSuffixDefined |= (i == 3);
          NumOfImagesDefined |= (i == 10);
        }
      }

      /* Check that all the mandatory variables are defined */
      if (!ImagesDirectoryDefined) {
        *msg = SP_CONFIG_MISSING_DIR;
        break;
      }
      if (!ImagesPrefixDefined) {
        *msg = SP_CONFIG_MISSING_PREFIX;
        break;
      }
      if (!ImagesSuffixDefined) {
        *msg = SP_CONFIG_MISSING_SUFFIX;
        break;
      }
      if (!NumOfImagesDefined) {
        *msg = SP_CONFIG_MISSING_NUM_IMAGES;
        break;
      }
    } while (0);
    /* If we didn't succeed in creating the config, free it's memory */
    if (SP_CONFIG_SUCCESS != *msg) {
      spConfigDestroy(config);
      config = NULL;
    }
  } while (0);
  fclose(file);
  return config;
}

/*
void spSetVar(SPConfig config,char* var,char* val){
	if(){

	}
}*/

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg) {
  if (NULL == config) {
    *msg = SP_CONFIG_INVALID_ARGUMENT;
    return false;
  }
  *msg = SP_CONFIG_SUCCESS;
  return config->spExtractionMode;
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
  if (NULL == config) {
    *msg = SP_CONFIG_INVALID_ARGUMENT;
    return false;
  }
  *msg = SP_CONFIG_SUCCESS;
  return config->spMinimalGUI;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){
  if (NULL == config) {
    *msg = SP_CONFIG_INVALID_ARGUMENT;
    return -1;
  }
  *msg = SP_CONFIG_SUCCESS;
  return config->spNumOfImages;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
  if (NULL == config) {
    *msg = SP_CONFIG_INVALID_ARGUMENT;
    return -1;
  }
  *msg = SP_CONFIG_SUCCESS;
  return config->spNumOfFeatures;
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
  if (NULL == config) {
    *msg = SP_CONFIG_INVALID_ARGUMENT;
    return -1;
  }
  *msg = SP_CONFIG_SUCCESS;
  return config->spPCADimension;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,int index){
  printf("spConfigGetImagePath is called but not implemented!\n");
  if (imagePath || config || index) {}
  return SP_CONFIG_ALLOC_FAIL;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){
  printf("spConfigGetPCAPath is called but not implemented!\n");
  if (pcaPath || config) {}
  return SP_CONFIG_ALLOC_FAIL;
}

int spConfigGetNumSimilarImages(const SPConfig config, SP_CONFIG_MSG* msg) {
  printf("spConfigGetNumSimilarImages is called but not implemented!\n");
  if (config) {}
  *msg = SP_CONFIG_ALLOC_FAIL;
  return -1;
}

int spConfigGetKNN(SPConfig config, SP_CONFIG_MSG* msg) {
  printf("spConfigGetKNN is called but not implemented!\n");
  if (config) {}
  *msg = SP_CONFIG_ALLOC_FAIL;
  return -1;
}

int spConfigGetLoggerLevel(SPConfig config, SP_CONFIG_MSG* msg) {
  printf("spConfigGetLoggerLevel is called but not implemented!\n");
  if (config) {}
  *msg = SP_CONFIG_ALLOC_FAIL;
  return -1;
}

SP_CONFIG_MSG spConfigGetLoggerFilename(char* loggerFilename,
  const SPConfig config) {
  printf("spConfigGetLoggerFilename is called but not implemented!\n");
  if (loggerFilename || config) {}
  return SP_CONFIG_ALLOC_FAIL;
}


void spConfigDestroy(SPConfig config) {
  free(config);
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
  	config->spExtractionMode=(bool)strcmp("true",str->after);

}

void print_config(SPConfig config){
	printf("spImagesDirectory: %s, spExtractionMode: %d, spImagesPrefix: %s, spImagesSuffix: %s\n",config->spImagesDirectory,config->spExtractionMode,config->spImagesPrefix,config->spImagesSuffix);
	printf( "spKDTreeSplitMethod: %d, spKNN: %d, spLoggerFilename: %s, spLoggerLevel: %d\n",config->spKDTreeSplitMethod,config->spKNN,config->spLoggerFilename,config->spLoggerLevel);
	printf(  "spMinimalGUI: %d, spNumOfFeatures: %d, spNumOfImages: %d\n",config->spMinimalGUI,config->spNumOfFeatures,config->spNumOfImages);
	printf( "spNumOfSimilarImages: %d, spPCADimension: %d, spPCAFilename: %s\n",config->spNumOfSimilarImages,config->spPCADimension,config->spPCAFilename);
}