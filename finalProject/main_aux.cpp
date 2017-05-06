#include "main_aux.h"

typedef struct sp_counter {
  int counter;
  int index;
} SPCounter;

int saveFeatureToFile(char *filepath, int index, int nFeatures,
  SPPoint** featureList) {
  FILE* file;

  /* Not checking argument validity */

  file = fopen(filepath, F_WRITE);
  if (NULL == file) {
    LOG_E(MSG_CANT_OPEN_FILE, filepath);
    return 0;
  }

  /* Write the features to the file */
  fprintf(file, FEATURE_FILE_HEAD "\n", index, nFeatures,
    spPointGetDimension(featureList[0]));
  for (int i = 0; i < nFeatures; i++) {
    for (int j = 0; j < spPointGetDimension(featureList[i]); j++) {
      fprintf(file, FEATURE_FILE_LINE_VALUE " ",
        spPointGetAxisCoor(featureList[i], j));
    }
    fprintf(file, FEATURE_FILE_LINE_END);
  }

  fclose(file);
  return 1;
}

SPPoint** readFeaturesFile(const char *filepath, int *nFeatures) {
  SPPoint** returnv;
  int index;
  int dimention;
  double *data;
  FILE *file;

  /* Not checking argument validity */

  file = fopen(filepath, F_READ);
  if (NULL == file) {
    LOG_E(MSG_CANT_OPEN_FILE, filepath);
    return NULL;
  }
  do {
    fscanf(file, FEATURE_FILE_HEAD, &index, nFeatures, &dimention);
    data = (double*)malloc(sizeof(*data) * dimention);
    if (NULL == data) {
      LOG_E(MSG_MEMORY_ALLOC_FAILURE);
      returnv = NULL;
      break;
    }
    do {
      returnv = (SPPoint**)malloc(sizeof(*returnv) * (*nFeatures));
      if (NULL == returnv) {
        LOG_E(MSG_MEMORY_ALLOC_FAILURE);
        break;
      }
      /* Read data from file */
      for (int i = 0; i < *nFeatures && NULL != returnv; i++) {
        for (int j = 0; j < dimention; j++) {
          fscanf(file, FEATURE_FILE_LINE_VALUE, data + j);
        }

        returnv[i] = spPointCreate(data, dimention, index);
        if (NULL == returnv[i]) {
          LOG_E(MSG_MEMORY_ALLOC_FAILURE);
          spPointArrayDestroy(returnv, i);
          returnv = NULL;
          break;
        }
      }
    } while (0);
    free(data);
  } while (0);
  fclose(file);
  return returnv;
}

int getCommand(char *command){
  scanf("%s", command);
  if (0 == strcmp(CL_STOP_QUERYING, command)) {
    return 0;
  }
  return 1;
}

SPPoint **imagesListToFeatureList(SPConfig config, SPPoint*** imagesList,
  int* nFeatures, int *size) {
  SPPoint **featuresList;
  SP_CONFIG_MSG msg;
  *size = 0;

  /* Not checking argument validity */

  for (int i = 0; i < spConfigGetNumOfImages(config, &msg); i++) {
    size += nFeatures[i];
  }

  featuresList = (SPPoint**)malloc(sizeof(*featuresList) * (*size));
  if (NULL == featuresList) {
    LOG_E(MSG_MEMORY_ALLOC_FAILURE);
    return NULL;
  }
  /* ONLY the pointers are being copied */
  for (int i = 0, j = 0, k = 0; i < *size; i++, k++) {
    if (nFeatures[j] == k) {
      j++;
      k = 0;
    }

    featuresList[i] = imagesList[j][k];
  }
  return featuresList;
}

int spCounterCmp(const void* p1, const void* p2) {
  return (((SPCounter*)p1)->counter) - (((SPCounter*)p2)->counter);
}

/* Increment the count of all pictures that appear in bpq in matchArray */
void incCounters(SPConfig config, SPBPQueue* bpq, SPCounter* matchArray) {
  SP_CONFIG_MSG msg;
  BPQueueElement* peekElement = (BPQueueElement*)malloc(sizeof(*peekElement));
  for (int i = 0; i < spConfigGetKNN(config, &msg); i++) {
    spBPQueuePeek(bpq, peekElement);
    matchArray[peekElement->index].counter++;
    spBPQueueDequeue(bpq);
  }
  free(peekElement);
}

int *searchSimilarImages(SPConfig config, char *queryPath,
  SPKDTreeNode *kdTree) {
  SP_CONFIG_MSG msg;
  int *returnv;

  SPCounter *matchArray;
  SPPoint **queryImage;
  int nFeatures;

  SPBPQueue* bpq;

  /* Not checking arguments validity */

  sp::ImageProc *imageProc = new sp::ImageProc(config);

  queryImage = imageProc->getImageFeatures(queryPath,
    spConfigGetNumOfImages(config, &msg), &nFeatures);
  if (NULL == queryImage) {
    delete imageProc;
    return NULL;
  }
  delete imageProc;

  do {
    matchArray = (SPCounter*)malloc(spConfigGetNumOfImages(config, &msg) *
      sizeof(*matchArray));
    if (NULL == matchArray) {
      returnv = NULL;
      break;
    }
    do {
      bpq = spBPQueueCreate(spConfigGetKNN(config, &msg));
      if (NULL == bpq) {
        returnv = NULL;
        break;
      }
      do {
        /* for every feature of the query */
        /* run knn search and and increment the counter */

        for (int i = 0; i < nFeatures; i++) {
          k_nearest_search(kdTree, bpq, queryImage[i]);
          incCounters(config, bpq, matchArray);
          spBPQueueDestroy(bpq);
        }

        returnv = (int*)malloc(spConfigGetKNN(config, &msg) *
          sizeof(*returnv));
        if (NULL == returnv) {
          break;
        }

        /* Sort the matching array according to the count of each image */
        qsort(matchArray, spConfigGetNumSimilarImages(config, &msg),
          sizeof(SPCounter), spCounterCmp);

        /* Get the NumSimilarImages best images indices */
        for (int i = 0; i < spConfigGetKNN(config, &msg); i++) {
          returnv[i] = matchArray[i].index;
        }
      } while (0);
      spBPQueueDestroy(bpq);
    } while (0);
    free(matchArray);
  } while (0);
  spPointArrayDestroy(queryImage, nFeatures);
  return returnv;
}

void imagesListDestroy(SPPoint ***imagesList, int length, int *nFeatures) {
  if (imagesList) {
    if (!nFeatures) {
      LOG_E("Attempting to free imagesList without sending nFeatures "
        "array!\n");
      return;
    }
    for (int i = 0; i < length; i++) {
      spPointArrayDestroy(imagesList[i], nFeatures[i]);
    }
  }
}


