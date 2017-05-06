#include <cstring>

#include "main_aux.h"
#include "SPImageProc.h"


int main(int argc, char const *argv[]) {
  int returnv = EXIT_SUCCESS;
  SP_CONFIG_MSG config_msg;
  SPConfig config;

  SP_LOGGER_MSG logger_msg;

  sp::ImageProc *imageProc;

  SPPoint ***imagesList;
  int *nFeatures;
  
  SPPoint **featuresList;
  int size;

  SPKDArray *kdArray;
  SPKDTreeNode *kdTree;

  int *similarImagesIndecies;

  /* Buffers */
  char buffer[STRING_MAX_LENGTH];

  /* Read command line arguments */
  if (1 == argc) {
    config = spConfigCreate(DEFAULT_CONFIG, &config_msg);
    printf("config_msg: %d\n", config_msg);
    if (SP_CONFIG_CANNOT_OPEN_FILE == config_msg) {
      printf(CL_DEFAULT_CONFIG_NO_OPEN_MESSAGE, DEFAULT_CONFIG);
      return EXIT_FAILURE;
    }
  }
  else if (3 == argc && 0 == strcmp(CL_CONFIG_FLAG, argv[1])) {
    config = spConfigCreate(argv[2], &config_msg);
    if (SP_CONFIG_CANNOT_OPEN_FILE == config_msg) {
      printf(CL_CONFIG_NO_OPEN_MESSAGE, argv[1]);
      return EXIT_FAILURE;
    }
  }
  else {
    printf(CL_USAGE_MESSAGE);
    return EXIT_FAILURE;
  }
  do {
    /* Initialize imageProc */
    imageProc = new sp::ImageProc(config);

    /* Initialize the logger */
    config_msg = spConfigGetLoggerFilename(buffer, config);
    if (SP_CONFIG_SUCCESS != config_msg) {
      printf("Fatal: failed to retrive logger filename from config, "
        "aborting\n");
      returnv = EXIT_FAILURE;
      break;
    }
    logger_msg = spLoggerCreate(buffer,
      (SP_LOGGER_LEVEL)spConfigGetLoggerLevel(config, &config_msg));
    if (SP_LOGGER_SUCCESS != logger_msg || SP_CONFIG_SUCCESS != config_msg) {
      printf("Fatal: failed to create logger, aborting\n");
      returnv = EXIT_FAILURE;
      break;
    }
    do {
      /* Initialize array of numbers of features */
      nFeatures = (int*)malloc(sizeof(*nFeatures) *
        spConfigGetNumOfImages(config, &config_msg));
      if (NULL == nFeatures) {
        LOG_E(MSG_MEMORY_ALLOC_FAILURE);
        returnv = EXIT_FAILURE;
        break;
      }
      do {
        /* Initialize images list */
        imagesList = (SPPoint***)malloc(sizeof(*imagesList) *
          spConfigGetNumOfImages(config, &config_msg));
        if (NULL == imagesList) {
          LOG_E(MSG_MEMORY_ALLOC_FAILURE);
          returnv = EXIT_FAILURE;
          break;
        }
        do {
          if (spConfigIsExtractionMode(config, &config_msg)) {
            /* Iterate over the image and extract them into .feats files */
            for (int i = 0; i < spConfigGetNumOfImages(config, &config_msg);
              i++) {
              config_msg = spConfigGetImagePath(buffer, config, i);
              if (SP_CONFIG_SUCCESS != config_msg) {
                LOG_E(MSG_ERROR_GETTING_VALUE);
                returnv = EXIT_FAILURE;
                break;
              }
              imagesList[i] = imageProc->getImageFeatures(buffer, i,
                nFeatures + i);
              if (NULL == imagesList[i]) {
                LOG_E(MSG_CANT_OPEN_FILE, buffer);
                returnv = EXIT_FAILURE;
                break;
              }

              /* Create .feats file for the image */
              config_msg = spConfigGetFeatsFilePath(buffer, config, i);
              if (SP_CONFIG_SUCCESS != config_msg) {
                LOG_E(MSG_ERROR_GETTING_VALUE);
                returnv = EXIT_FAILURE;
                break;
              }

              if (!saveFeatureToFile(buffer, i, nFeatures[i], imagesList[i])) {
                LOG_E(MSG_FAILED_TO_SAVE_FILE, buffer);
                returnv = EXIT_FAILURE;
                break;
              }
            }

            /* If an error occured in the for loop, break */
            if (EXIT_FAILURE == returnv) break;
          }
          else {
            /* non-Extraction mode */
            /* Iterate over the freature files and extract them */
            for (int i = 0; i < spConfigGetNumOfImages(config, &config_msg);
              i++) {
              config_msg = spConfigGetFeatsFilePath(buffer, config, i);
              if (SP_CONFIG_SUCCESS != config_msg) {
                LOG_E(MSG_ERROR_GETTING_VALUE);
                returnv = EXIT_FAILURE;
                break;
              }

              imagesList[i] = readFeaturesFile(buffer, nFeatures + i);
              if (NULL == imagesList[i]) {
                LOG_E(MSG_MEMORY_ALLOC_FAILURE);
                //imagesListDestroy(imagesList, i, nFeatures);
                returnv = EXIT_FAILURE;
                break;
              }
            }
            if (EXIT_FAILURE == returnv) break;
          }

          /* Turn images list to a single features list */
          /* The points within the images list won't be copied. Only their
          pointers, */

          /* So DO NOT free the points pointed to by featuresList! */
          /* These points are going to be freed along with imagesList */

          featuresList = imagesListToFeatureList(config, imagesList, nFeatures,
            &size);      
          if (NULL == featuresList) {
            LOG_E(MSG_MEMORY_ALLOC_FAILURE);
            returnv = EXIT_FAILURE;
            break;
          }
          do {
            /* Initialize kdArray */
            kdArray = init(featuresList, size);
            if (NULL == kdArray) {
              LOG_E(MSG_FAILED_KDARRAY);
              returnv = EXIT_FAILURE;
              break;
            }
            do {
              /* Initialize kdTree */
              kdTree = create_tree_main(config, kdArray); // En-Capsulate reqursive call
              if (NULL == kdTree) {
                LOG_E(MSG_FAILED_KDTREE);
                returnv = EXIT_FAILURE;
                break;
              }
              do {
                /* Main interaction loop */
                printf("Please enter an image path:\n");
                while (getCommand(buffer)) {
                  /* Search for similar images */
               		//printf("kdTree%sNULL\n",(kdTree==NULL)?"==":"!=" );
                  similarImagesIndecies = searchSimilarImages(config, buffer,
                    kdTree);
                  if (NULL == similarImagesIndecies) {
                    printf(MSG_FAILED_TO_SEARCH_SIMIMG, buffer);
                    continue;
                  }
                  do {
                    /* Show results */
                    if (spConfigIsMinimalGui(config, &config_msg)) {
                      /* Minimal gui */
                      for (int i = 0;
                        i < spConfigGetNumSimilarImages(config, &config_msg);
                        i++) {
                        config_msg =
                          spConfigGetImagePath(buffer, config,
                            similarImagesIndecies[i]);
                        imageProc->showImage(buffer);
                      }
                    }
                    else {
                      /* Non-minimal gui */
                      printf(CL_BEST_FOR_IMG, buffer);
                      for (int i = 0;
                        i < spConfigGetNumSimilarImages(config, &config_msg);
                        i++) {
                        config_msg =
                          spConfigGetImagePath(buffer, config,
                            similarImagesIndecies[i]);
                        printf(CL_CANDIDATE_IMG, buffer);
                      }
                    }
                    printf(CL_EXIT_MSG);
                  } while (0);
                  free(similarImagesIndecies);
                }
              } while (0);
              spKDTreeDestroy(kdTree);
            } while (0);
            spKDArrayDestroy(kdArray);
          } while (0);
          free(featuresList);
        } while (0);
        //imagesListDestroy(imagesList,spConfigGetNumOfImages(config, &config_msg), nFeatures);
      } while (0);
      free(nFeatures);
    } while (0);
    spLoggerDestroy();

    /* Free imageProc */
    delete imageProc;
  } while (0);
  spConfigDestroy(config);
  
  return returnv;
}
