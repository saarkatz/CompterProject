#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "SPConfig.h"
#define JPG ".jpg"
#define PNG ".png"
#define BMP ".bmp"
#define GIF ".gif"
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
int setPCAFilename(SPConfig config, SPVar* str);
int setPCADimension(SPConfig config, SPVar* str);
int setNumOfSimilarImages(SPConfig config, SPVar* str);
int setNumOfImages(SPConfig config, SPVar* str);
int setNumOfFeatures(SPConfig config, SPVar* str);
int setMinimalGUI(SPConfig config, SPVar* str);
int setLoggerLevel(SPConfig config, SPVar* str);
int setLoggerFilename(SPConfig config, SPVar* str);
int setKNN(SPConfig config, SPVar* str);
int setKDTreeSplitMethod(SPConfig config, SPVar* str);
int setImagesSuffix(SPConfig config, SPVar* str);
int setImagesPrefix(SPConfig config, SPVar* str);
int setImagesDirectory(SPConfig config, SPVar* str);
int setExtractionMode(SPConfig config, SPVar* str);


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
  	//char *strchr(const char *str, int c)

  return v;
}

void print_var_arr(SPVar* array,int size){
  for(int i=0;i<size;i++){
    printf("(%s),(%s)\n",array[i].before,array[i].after);
  }
}

int first_nonwhitespace(char* s){
    size_t i = 0;
    while(s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\f' || s[i] == '\v')
    {
        ++i;
    }
    return i;
}

int spCaseChoose(SPConfig config, SPVar* var) {
	int result;
  if(strcmp(var->before,"spImagesDirectory")==0){
    result = setImagesDirectory(config, var);
  	if(result==0)result =  0;
  	else result = -1;	
  	  	if(strchr(var->after,' ')!=NULL) result=-1;

  }
  else if(strcmp(var->before,"spExtractionMode")==0){
    result = setExtractionMode(config, var);
  	if(result==0)result =  1;
  	else result = -1;
  	  if(strchr(var->after,' ')!=NULL) result=-1;

  }
  else if(strcmp(var->before,"spImagesPrefix")==0){
    result = setImagesPrefix(config, var);
  	if(result==0)result =  2;
  	else result = -1;
  	 if(strchr(var->after,' ')!=NULL) result=-1;

  }
  else if(strcmp(var->before,"spImagesSuffix")==0){
    result = setImagesSuffix(config, var);
  	if(result==0)result =  3;
  	else result = -1;
  	if(strchr(var->after,' ')!=NULL) result=-1;
	
  }
  else if(strcmp(var->before,"spKDTreeSplitMethod")==0){
    result = setKDTreeSplitMethod(config, var);
  	if(result==0)result =  4;
  	else result = -1;	
  	if(strchr(var->after,' ')!=NULL) result=-1;

  }
  else if(strcmp(var->before,"spKNN")==0){
    result = setKNN(config, var);
  	if(result==0)result =  5;
  	else result = -2;
  	if(strchr(var->after,' ')!=NULL) result=-2;
	
  }
  else if(strcmp(var->before,"spLoggerFilename")==0){
    result = setLoggerFilename(config, var);
  	if(result==0)result =  6;
  	else result = -1;	
  	  	if(strchr(var->after,' ')!=NULL) result=-1;

  }
  else if(strcmp(var->before,"spLoggerLevel")==0){
    result = setLoggerLevel(config, var);
  	if(result==0)result =  7;
  	else result = -2;
  	  	if(strchr(var->after,' ')!=NULL) result=-2;
	
  }
  else if(strcmp(var->before,"spMinimalGUI")==0){
    result = setMinimalGUI(config, var);
  	if(result==0)result =  8;
  	else result = -1;	
  	 if(strchr(var->after,' ')!=NULL) result=-1;

  }
  else if(strcmp(var->before,"spNumOfFeatures")==0){
    result = setNumOfFeatures(config, var);
  	if(result==0)result =  9;
  	else result = -2;
  	if(strchr(var->after,' ')!=NULL) result=-2;
  	///////////////////////////////////////////////////////////////////////////
  }
  else if(strcmp(var->before,"spNumOfImages")==0){
    result = setNumOfImages(config, var);
  	if(result==0)result =  10;
  		else result = -2;
  	if(strchr(var->after,' ')!=NULL) result=-2;
  	///////////////////////////////////////////////////////////////////////////

  }
  else if(strcmp(var->before,"spNumOfSimilarImages")==0){
    result = setNumOfSimilarImages(config, var);
  	if(result==0)result =  11;
  		else result = -2;
  	 if(strchr(var->after,' ')!=NULL) result=-2;

  }
  else if(strcmp(var->before,"spPCADimension")==0){
    result = setPCADimension(config, var);
  	if(result==0)result =  12;
  		else result = -2;
  	if(strchr(var->after,' ')!=NULL) result=-2;

  }
  else if(strcmp(var->before,"spPCAFilename")==0){
    result = setPCAFilename(config, var);
  	if(result==0)result =  13;
  	else result = -1;
    if(strchr(var->after,' ')!=NULL) result=-1;

  }
	else{
		//TODO
		result =  -1;
	}
	return result;
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
SPVar* v;
/*  char* arg_arr[14] = { "spImagesDirectory",
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
    "spPCAFilename" };*/

  SPVar var_array[14];

  int linenumber=0;
  int ret;
  int var_num = 0;

  char line[MAXBUF];
  char str_trap[MAXBUF];
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
        str_trap[0]='\0';
        char first_no_white = line[first_nonwhitespace(line)];
        if ('\n' != first_no_white && '#' != first_no_white && var_num < 14){
        	v = &(var_array[var_num]);
       		sscanf(line,"%[^= ] = %s %s",v->before,v->after,str_trap);
            if(str_trap[0]!='\0'){
       			ret = spCaseChoose(config,v);
       			if(ret==12||ret==11||ret==10||ret==9||ret==7||ret==5){
        			*msg =SP_CONFIG_INVALID_INTEGER;
       			}
       			 if(ret==-1||ret==13||ret==8||ret==6||ret==4||ret==3||ret==2||ret==1||ret==0){
        			*msg =SP_CONFIG_INVALID_STRING;
            }
              printInvalidLine(filename,linenumber);
        			break;
       		}
       		else if(strchr(v->before,' ')!=NULL){
       			*msg=SP_CONFIG_INVALID_STRING;
       			break;
       		}
            else var_num++;
       		 //char *strchr(const char *str, int c)
        }
        linenumber++;
      }
      if(*msg != SP_CONFIG_SUCCESS)break;

      /* TODO - This entire part is broken and needs to be remade */
      for (int i = 0; i < var_num; i++) {
          ret = spCaseChoose(config, &var_array[i]);
          //	 printf("var_array[i]: %s,ret: %d\n",var_array[i].after,ret );
          ImagesDirectoryDefined = ImagesDirectoryDefined||(ret == 0);
          ImagesPrefixDefined = ImagesPrefixDefined||(ret == 2);
          ImagesSuffixDefined = ImagesSuffixDefined||(ret == 3);
          NumOfImagesDefined = NumOfImagesDefined||(ret == 10);
          if(ret<0){
          	if(ret==-1){
          		*msg =SP_CONFIG_INVALID_STRING;
          	}
          	else{
          		*msg =SP_CONFIG_INVALID_INTEGER;
          	}
            printConstranitNotMet(filename,linenumber);
          	break;
          }
      }

      /* Check that all the mandatory variables are defined */
      if (!ImagesDirectoryDefined) {
        *msg = SP_CONFIG_MISSING_DIR;
        printParameterNotSet(filename,linenumber,"spImagesDirectory");
        break;
      }
      if (!ImagesPrefixDefined) {
        *msg = SP_CONFIG_MISSING_PREFIX;
        printParameterNotSet(filename,linenumber,"spImagesPrefix");
        break;
      }
      if (!ImagesSuffixDefined) {
        *msg = SP_CONFIG_MISSING_SUFFIX;
        printParameterNotSet(filename,linenumber,"spImagesSuffix");
        break;
      }
      if (!NumOfImagesDefined) {
        *msg = SP_CONFIG_MISSING_NUM_IMAGES;
        printParameterNotSet(filename,linenumber,"spNumOfImages");
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
  //print_config(config);//DEBUG
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
  if (!imagePath || !config) {  
  	return SP_CONFIG_INVALID_ARGUMENT;
	}
	if(index>=config->spNumOfImages||index<0){
  	return SP_CONFIG_INDEX_OUT_OF_RANGE;

	}

  //printf("spConfigGetPCAPath is called but not implemented!\n");
  sprintf(imagePath,"%s%s%d%s",config->spImagesDirectory,config->spImagesPrefix,index,config->spImagesSuffix);
  //printf("in spConfigGetPCAPath: pcaPath =%s\n",pcaPath );
  return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){
  if (!pcaPath || !config) {  
  	return SP_CONFIG_INVALID_ARGUMENT;
	}

  //printf("spConfigGetPCAPath is called but not implemented!\n");
  sprintf(pcaPath,"%s%s",config->spImagesDirectory,config->spPCAFilename);
  //printf("in spConfigGetPCAPath: pcaPath =%s\n",pcaPath );
  return SP_CONFIG_SUCCESS;
}

int spConfigGetNumSimilarImages(const SPConfig config, SP_CONFIG_MSG* msg) {
  //printf("spConfigGetNumSimilarImages is called but not implemented!\n");
  if (config) {
	*msg = SP_CONFIG_SUCCESS;
  	return config->spNumOfSimilarImages;
  }
  *msg = SP_CONFIG_ALLOC_FAIL;
  return -1;
}

int spConfigGetKNN(SPConfig config, SP_CONFIG_MSG* msg) {
  //printf("spConfigGetKNN is called but not implemented!\n");
  if (config) {
	*msg = SP_CONFIG_SUCCESS;

  	return config->spKNN;
  }
  *msg = SP_CONFIG_ALLOC_FAIL;
  return -1;
}

int spConfigGetLoggerLevel(SPConfig config, SP_CONFIG_MSG* msg) {
  //printf("spConfigGetLoggerLevel is called but not implemented!\n");
  if (config) {
  	*msg = SP_CONFIG_SUCCESS;
  	return config->spLoggerLevel;
  }
  *msg = SP_CONFIG_ALLOC_FAIL;
  return -1;
}

SP_CONFIG_MSG spConfigGetLoggerFilename(char* loggerFilename,
  const SPConfig config) {
  //printf("spConfigGetLoggerFilename is called but not implemented!\n");
  if (loggerFilename || config) {
  	  sprintf(loggerFilename,"%s",config->spLoggerFilename);
  	  return SP_CONFIG_SUCCESS;
  }
  return SP_CONFIG_ALLOC_FAIL;
}


void spConfigDestroy(SPConfig config) {
	if(config!=NULL){
		
  free(config);
	}
  
}



int setPCAFilename(SPConfig config,SPVar* str){
    strcpy(config->spPCAFilename,str->after);
    return 0;
}
int setPCADimension(SPConfig config,SPVar* str){
	config->spPCADimension=atoi(str->after);
	if(config->spPCADimension>128||config->spPCADimension<10){
		return 1;
	}
	else return 0;
}
int setNumOfSimilarImages(SPConfig config,SPVar* str){
	config->spNumOfSimilarImages=atoi(str->after);
	if(config->spNumOfSimilarImages<=0){
		return 1;
	}else return 0;
}
int setNumOfImages(SPConfig config,SPVar* str){
	config->spNumOfImages=atoi(str->after);
	if(config->spNumOfImages<=0){
		return 1;
	}else return 0;
}
int setNumOfFeatures(SPConfig config,SPVar* str){
	config->spNumOfFeatures=atoi(str->after);
	if(config->spNumOfFeatures<=0){
		return 1;
	}else return 0;
}
int setMinimalGUI(SPConfig config,SPVar* str){
	if(strcmp("true",str->after)!=0&&strcmp("false",str->after)!=0){
		return 1;
	}
	else {
	config->spMinimalGUI=!((bool)strcmp("true",str->after));
	return 0;	
	}
}
int setLoggerLevel(SPConfig config,SPVar* str){
	config->spLoggerLevel=atoi(str->after);
	if(config->spLoggerLevel<1||config->spLoggerLevel>4){
		return 1;
	} else return 0;
}
int setLoggerFilename(SPConfig config,SPVar* str){
    strcpy(config->spLoggerFilename,str->after);
    return 0;
}
int setKNN(SPConfig config,SPVar* str){
	config->spKNN=atoi(str->after);
	if(config->spKNN==0){
		return 1;
	} else return 0;
}
int setKDTreeSplitMethod(SPConfig config,SPVar* str){
	//config->spKDTreeSplitMethod=(str->after);//TODO switch case
	if(strcmp("RANDOM",str->after)==0){
		config->spKDTreeSplitMethod=RANDOM;
		return 0;
	}
	else if(strcmp("MAX_SPREAD",str->after)==0){
		config->spKDTreeSplitMethod=MAX_SPREAD;
		return 0;
	}
	else if(strcmp("INCREMENTAL",str->after)==0){
		config->spKDTreeSplitMethod=INCREMENTAL;
		return 0;
	}
	else{
		return 1;
	}
}
int setImagesSuffix(SPConfig config,SPVar* str){
    if(strcmp(JPG,str->after)==0||strcmp(PNG,str->after)==0||strcmp(BMP,str->after)==0||strcmp(GIF,str->after)==0){
    strcpy(config->spImagesSuffix,str->after);
    return 0;
  }
  else{
    return 1;
  }
}
int setImagesPrefix(SPConfig config,SPVar* str){
    strcpy(config->spImagesPrefix,str->after);
     return 0;
}
int setImagesDirectory(SPConfig config,SPVar* str){
    strcpy(config->spImagesDirectory,str->after);
     return 0;
}
int setExtractionMode(SPConfig config,SPVar* str){
	if(strcmp("true",str->after)!=0&&strcmp("false",str->after)!=0){
		return 1;
	}
	else {
	config->spExtractionMode=!((bool)strcmp("true",str->after));
	return 0;	
	}
}

/*int print_config(SPConfig config){
	printf("spImagesDirectory: %s, spExtractionMode: %d, spImagesPrefix: %s, spImagesSuffix: %s\n",config->spImagesDirectory,config->spExtractionMode,config->spImagesPrefix,config->spImagesSuffix);
	printf( "spKDTreeSplitMethod: %d, spKNN: %d, spLoggerFilename: %s, spLoggerLevel: %d\n",config->spKDTreeSplitMethod,config->spKNN,config->spLoggerFilename,config->spLoggerLevel);
	printf(  "spMinimalGUI: %d, spNumOfFeatures: %d, spNumOfImages: %d\n",config->spMinimalGUI,config->spNumOfFeatures,config->spNumOfImages);
	printf( "spNumOfSimilarImages: %d, spPCADimension: %d, spPCAFilename: %s\n",config->spNumOfSimilarImages,config->spPCADimension,config->spPCAFilename);
}*/


void printInvalidLine(const char* filename,int linenumber){
//1 - invalid line meaning not a comment or a parameter 
printf("File: %s\nLine: %d\nMessage: Invalid configuration line\n",filename,linenumber);
}

void printConstranitNotMet(const char* filename,int linenumber){
//2 - any of the constraints on the system parameters are not met
printf("File: %s\nLine: %d\nMessage: Invalid value - constraint not met\n",filename,linenumber);
}

void printParameterNotSet(const char* filename,int linenumbers,const char* parm){
//3 - one of the default value is not set
printf("File: %s\nLine: %d\nMessage: Parameter %s not set\n",filename,linenumbers,parm);
}

