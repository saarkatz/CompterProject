#include <stdbool.h>
#include <sys/time.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include "unit_test_util.h"

#include "../SPGlobals.h"
#include "../SPLogger.h"
#include "../SPConfig.h"
#include "../SPPoint.h"
#include "../SPBPriorityQueue.h"
#include "../SPKDArray.h"
#include "../SPKDTree.h"

/* Tests for kd-tree */
/*
UT - unit test
TCT - test create tree
*/

#define UT_LOGGER_FILENAME "logs/sp_kdtree_unit_test.log"
#define UT_LOGGER_LEVEL SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL

/* Test create-tree*/
#define TCT_REPEAT 30

#define TCT_MIN_X 1
#define TCT_MAX_X 31
#define TCT_MIN_Y 1
#define TCT_MAX_Y 51

#define TCT_VAL_MIN -20
#define TCT_VAL_MAX 80

#define TCT_USE_RANDOM_SPLIT_AFTER 0
#define TCT_USE_MAX_SPREAD_SPLIT_AFTER 10
#define TCT_USE_INCREMENTAL_SPLIT_AFTER 20

#define TCT_CONFIG_RANDOM_SPLIT "configs/sp_kdtree_test/random_split.config"
#define TCT_CONFIG_MAX_SPREAD_SPLIT "configs/sp_kdtree_test/max_spread_split.config"
#define TCT_CONFIG_INCREMENTAL_SPLIT "configs/sp_kdtree_test/incremental_split.config"

/* Signutures of helper function defined in this section */
typedef enum metrix_generator_option {
  G_RANDOM,
  G_LINE
} MG_OPTION;

/* Declerations of function in this section */
unsigned int getIntFracTime();
double rand_double(bool include_one);
double **generatDoubleMatrix(int x_start, int x_end, int y_start, int y_end,
  double val_min, double val_max, MG_OPTION option, int *x, int *y);
void shuffle(SPPoint **point_arr, size_t n);
void destroyDoubleMatrix(double **matrix, int x);

/**
Initializes the random number generator to seed
If seed is not NULL the the value within seed is checked. If it is 0 the a
random seed will be used and stored in seed, otherwise the value of seed will
be used. If seed is NULL a random number will be used and won't be stored in
seed.
*/
void set_random_generator(unsigned int *seed) {
  /* Set random generator */
  if (seed) {
    if (*seed) {
      srand(*seed);
    }
    else {
      *seed = getIntFracTime();
      srand(*seed);
    }
  }
  else {
    srand(getIntFracTime());
  }
}

/**
  Returns an unsigned fraction of the time in microsecond (loops around).
  Use to get seed for quick consecutive runs of srand()
*/
unsigned int getIntFracTime() {
  struct timeval timev;
  gettimeofday(&timev, NULL);
  return (unsigned int)timev.tv_usec;
}

/**
Generates a number between 0 and 1.
Use only after srand() has been called.
*/
double rand_double(bool include_one) {
  int val = rand();
  if (include_one || 0 == val) {
    return (double)val / (double)RAND_MAX;
  }
  else {
    return (double)(rand() - 1) / (double)RAND_MAX;
  }
}

/**
COMMENT NOT UPDATED - Added MG_OPTION enum

Helper function

Generates (allocates) a random matrix of doubles of random size within 
[x_start, x_end - 1]x[y_start, y_end - 1] up to maximum of RAND_MAX.
The values with in the matrix are in the rande [val_min, val_max]. 
If val_min is equal to val_max then all the values are set to val_min.

x and y are set to the dimention of the matrix.

The return value is the pointer to the matrix or NULL if the ranges are
invalid of if memory allocation failed.

Don't forget to free the matrix after use.
*/
double **generatDoubleMatrix(int x_start, int x_end, int y_start, int y_end,
  double val_min, double val_max, MG_OPTION option, int *x, int *y) {
  /* Declare variables */
  double **matrix;
  double min;
  double max;

  /* Validate arguments */
  if (1 > x_start || 1 > y_start ||
    x_start >= x_end || y_start >= y_end || val_min > val_max) {
    return NULL;
  }

  /* Generate random variables */
  *x = x_start + rand() % (x_end - x_start);
  *y = y_start + rand() % (y_end - y_start);
  /* Allocate matrix */
  matrix = (double**)malloc((*x) * sizeof(*matrix));
  if (NULL == matrix) {
    return NULL;
  }
  for (int i = 0; i < (*x); i++) {
    matrix[i] = (double*)malloc((*y) * sizeof(*matrix[i]));
    if (NULL == matrix[i]) {
      destroyDoubleMatrix(matrix, i);
      return NULL;
    }
  }

  if (G_LINE == option) {
    min = val_min + rand_double(true) * (val_max - val_min);
    max = val_min + rand_double(true) * (val_max - val_min);
  }
  else if (G_RANDOM == option) {
    min = val_min;
    max = val_max;
  }

  /* Initialize matrix */
  for (int i = 0; i < (*x); i++) {
    for (int j = 0; j < (*y); j++) {
      if (G_RANDOM == option) {
        matrix[i][j] = min;
        if (max > min)
          matrix[i][j] += rand_double(true) * (max - min);
      }
      else if (G_LINE == option) {
        matrix[i][j] = min + max * i;
      }
    }
  }

  return matrix;
}

/**
Arrange the N elements of ARRAY in random order.
*/
void shuffle(SPPoint **arr, size_t n)
{
  size_t i;
  size_t j;
  SPPoint *t;
  if (n > 1)
  {
    for (i = 0; i < n - 1; i++)
    {
      j = i + (size_t)(rand_double(false) * (n - i));
      t = arr[j];
      arr[j] = arr[i];
      arr[i] = t;
    }
  }
}

/* Frees the double matrix */
void destroyDoubleMatrix(double **matrix, int x) {
  if (matrix) {
    for (int i = 0; i < x; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
}

/* Test spKDTreeCreate */
bool testCreateTree() {
  /* Declare variables */
  bool returnv = true;
  double **data;
  unsigned int seed;
  int x;
  int y;
  SPPoint **point_arr;
  SPKDArray *kdarr;
  SPKDTreeNode *kdtree;

  /* Declear config */
  SP_CONFIG_MSG msg;
  SPConfig config;

  LOG_I("Repeating %d times:", TCT_REPEAT);
  for (int i = 0; i < TCT_REPEAT && true == returnv; i++) {
    /* Initializing data array */
    seed = 0;
    set_random_generator(&seed);
    data = generatDoubleMatrix(TCT_MIN_X, TCT_MAX_X, TCT_MIN_Y, TCT_MAX_Y,
      TCT_VAL_MIN, TCT_VAL_MAX, G_RANDOM, &x, &y);
    if (NULL == data) {
      LOG_E("Internal error, aborting!");
      returnv = false;
      break;
    }
    LOG_I("Iteration %d, using seed %d", i, seed);
    do {
      /* Initialize point array */
      point_arr = (SPPoint**)malloc(x * sizeof(*point_arr));
      if (NULL == point_arr) {
        LOG_E("Failed to initialize point_arr.");
        returnv = false;
        break;
      }
      for (int j = 0; j < x; j++) {
        if (j < x / 2) {
          point_arr[j] = spPointCreate(data[j], y, 0);
        }
        else {
          point_arr[j] = spPointCreate(data[j], y, j);
        }
        if (NULL == point_arr[j]) {
          LOG_E("Failed to initialize SPPoint.");
          spPointArrayDestroy(point_arr, j);
          returnv = false;
          break;
        }
      }
      if (false == returnv) break;

      do {
        /* Shuffle the array before initializing the kdArray */
        shuffle(point_arr, x);

        /* Initialize kdArray */
        LOG_I("Initialize kdArray");
        kdarr = spKDArrayCreate(point_arr, x);
        if (NULL == kdarr) {
          LOG_E("Failed to initialize KDArray.");
          returnv = false;
          break;
        }
        do {
          /* Create KDTree */
          /* Choose split method */
          if (i >= TCT_USE_INCREMENTAL_SPLIT_AFTER) {
            config = spConfigCreate(TCT_CONFIG_INCREMENTAL_SPLIT, &msg);
          }
          else if (i >= TCT_USE_MAX_SPREAD_SPLIT_AFTER) {
            config = spConfigCreate(TCT_CONFIG_MAX_SPREAD_SPLIT, &msg);
          }
          else if (i >= TCT_USE_RANDOM_SPLIT_AFTER) {
            config = spConfigCreate(TCT_CONFIG_RANDOM_SPLIT, &msg);
          }
          else {
            config = spConfigCreate(TCT_CONFIG_INCREMENTAL_SPLIT, &msg);
          }

          if (NULL == config) {
            LOG_E("Unable to load config file: %d", msg);
            returnv = false;
            break;
          }
          do {
            LOG_I("Creating tree using split method %d",
              spConfigGetSplitMethod(config, &msg));

            kdtree = spKDTreeCreate(config, kdarr);
            ASSERT_FALSE(NULL == kdtree);

            spKDTreeDestroy(kdtree);
          } while (0);
          spConfigDestroy(config);
        } while (0);
        spKDArrayDestroy(kdarr);
      } while (0);
      spPointArrayDestroy(point_arr, x);
    } while (0);
    destroyDoubleMatrix(data, x);
  }

  return returnv;
}

/* Test spKDTreeKNearestSearch */
bool testKNearestSearch() {
  /* Declare variables */
  bool returnv = true;
  unsigned int seed;
  double **data;
  int x;
  int y;
  SPPoint **point_arr;
  SPKDArray *kdarr;
  SPKDTreeNode *kdtree;

  /* Declear config */
  SP_CONFIG_MSG msg;
  SPConfig config;

  /* Declear priority queue and query point */
  SPBPQueue *priority_queue;
  BPQueueElement *res;
  int num_similay_images;
  SPPoint *query_point;
  int query_point_index;

  LOG_I("Repeating %d times:", TCT_REPEAT);
  for (int i = 0; i < TCT_REPEAT && true == returnv; i++) {
    /* Initializing data array */
    seed = 0;
    set_random_generator(&seed);
    data = generatDoubleMatrix(TCT_MIN_X, TCT_MAX_X, TCT_MIN_Y,
      TCT_MAX_Y, TCT_VAL_MIN, TCT_VAL_MAX, G_LINE, &x, &y);
    if (NULL == data) {
      LOG_E("Internal error, aborting!");
      returnv = false;
      break;
    }
    LOG_I("Iteration %d, using seed: %d", i, seed);
    do {
      /* Choose number of similar points */
      num_similay_images = (int)(rand_double(true) * x / 2);
      if (num_similay_images < 1) num_similay_images = 1;

      /* Choose query point and initialize point array */
      query_point_index = num_similay_images / 2 + 
        (int)(rand_double(false) * (x - num_similay_images));
      if (query_point_index >= x) {
        LOG_E("Chosen query point is out of range!");
        returnv = false;
        break;
      }

      point_arr = (SPPoint**)malloc(x * sizeof(*point_arr));
      if (NULL == point_arr) {
        LOG_E("Failed to initialize point_arr.");
        returnv = false;
        break;
      }
      for (int j = 0; j < x; j++) {
        point_arr[j] = spPointCreate(data[j], y, j);
        if (NULL == point_arr[j]) {
          LOG_E("Failed to initialize SPPoint.");
          spPointArrayDestroy(point_arr, j);
          returnv = false;
          break;
        }
        /* Pick the chosen point */
        if (j == query_point_index) {
          query_point = point_arr[j];
        }
      }

      if (false == returnv) break;

      do {
        /* Suffle point array before initializing kdArray */
        shuffle(point_arr, x);

        /* Initialize kdArray */
        kdarr = spKDArrayCreate(point_arr, x);
        if (NULL == kdarr) {
          LOG_E("Failed to initialize KDArray.");
          returnv = false;
          break;
        }
        do {
          /* Choose split method */
          if (i >= TCT_USE_INCREMENTAL_SPLIT_AFTER) {
            config = spConfigCreate(TCT_CONFIG_INCREMENTAL_SPLIT, &msg);
          }
          else if (i >= TCT_USE_MAX_SPREAD_SPLIT_AFTER) {
            config = spConfigCreate(TCT_CONFIG_MAX_SPREAD_SPLIT, &msg);
          }
          else if (i >= TCT_USE_RANDOM_SPLIT_AFTER) {
            config = spConfigCreate(TCT_CONFIG_RANDOM_SPLIT, &msg);
          }
          else {
            config = spConfigCreate(TCT_CONFIG_INCREMENTAL_SPLIT, &msg);
          }

          if (NULL == config) {
            LOG_E("Unable to load config file: %d", msg);
            returnv = false;
            break;
          }
          do {
            /* Create KDTree */
            LOG_I("Creating tree using split method %d",
              spConfigGetSplitMethod(config, &msg));

            kdtree = spKDTreeCreate(config, kdarr);
            if (NULL == kdtree) {
              LOG_E("Failed to initialize KDTree.");
              returnv = false;
              break;
            }
            do {
              /* Create BPriorityQueue */
              priority_queue = spBPQueueCreate(num_similay_images);
              if (NULL == priority_queue) {
                LOG_E("Failed to initialize priority queue!");
                returnv = false;
                break;
              }
              do {
                /* Create queue element */
                res = (BPQueueElement*)malloc(sizeof(*res));
                if (NULL == res) {
                  LOG_E("Failed to initialize queue element!");
                  returnv = false;
                  break;
                }
                do {
                  /* Run spKDTreeKNearestSearch */
                  spKDTreeKNearestSearch(kdtree, priority_queue, query_point);

                  /* Check that the correct indices are returned */
                  /* Due to rounding error this is a tiny bit limited */
                  for (int k = 0; k < num_similay_images; k++) {
                    spBPQueuePeek(priority_queue, res);
                    ASSERT_TRUE_NO_EXIT(
                      res->index == 
                      spPointGetIndex(query_point) + (k + 1) / 2 ||
                      res->index == spPointGetIndex(query_point) - (k + 1) / 2,
                      returnv);
                    if (false == returnv) {
                      /* Stop on failure */
                      LOG_I("k: %d\nquery index: %d\nres->index: %d", k, spPointGetIndex(query_point), res->index);
                      break;
                    }

                    spBPQueueDequeue(priority_queue);
                  }
                } while (0);
                free(res);
              } while (0);
              spBPQueueDestroy(priority_queue);
            } while (0);
            spKDTreeDestroy(kdtree);
          } while (0);
          spConfigDestroy(config);
        } while (0);
        spKDArrayDestroy(kdarr);
      } while (0);
      spPointArrayDestroy(point_arr, x);
    } while (0);
    destroyDoubleMatrix(data, x);
  }

  return returnv;
}


int main() {
  /* Declare logger */
  SP_LOGGER_MSG msg = spLoggerCreate(UT_LOGGER_FILENAME, UT_LOGGER_LEVEL);
  if (SP_LOGGER_DEFINED != msg && SP_LOGGER_SUCCESS != msg) {
    PRINT_E("Unable to create logger (%d), aborting test suite.", msg);
    return -1;
  }

  RUN_TEST(testCreateTree);
  RUN_TEST(testKNearestSearch);

  spLoggerDestroy();
  return 0;
}
