#include <stdbool.h>
#include <sys/time.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include "unit_test_util.h"

#include "../SPLogger.h"
#include "../SPConfig.h"
#include "../SPPoint.h"
#include "../SPKDArray.h"
#include "../SPKDTree.h"

#define PRINT(...) printf(__VA_ARGS__)
#define PRINT_E(...) do {\
printf("%s line %d: ", __FILE__, __LINE__); \
printf(__VA_ARGS__); \
} while (0)

/* Tests for kd-tree */
#define TCT_REPEAT 30

#define TCT_MIN_X 1
#define TCT_MAX_X 31
#define TCT_MIN_Y 1
#define TCT_MAX_Y 51

#define TCT_VAL_MIN -20
#define TCT_VAL_MAX 80

#define USE_RANDOM_SPLIT_AFTER 0
#define USE_MAX_SPREAD_SPLIT_AFTER 10
#define USE_INCREMENTAL_SPLIT_AFTER 20

#define CONFIG_RANDOM_SPLIT "configs/random_split.config"
#define CONFIG_MAX_SPREAD_SPLIT "configs/max_spread_split.config"
#define CONFIG_INCREMENTAL_SPLIT "configs/incremental_split.config"


#define LOGGER_FILENAME "sp_kdtree_unit_test.log"
#define LOGGER_LEVEL SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL

/* Signutures of helper function defined in this section */

unsigned int getIntFracTime();
double rand_double();
double **generatDoubleMatrix(int x_start, int x_end, int y_start, int y_end,
  double val_min, double val_max, int *x, int *y, unsigned int *seed);
void destroyDoubleMatrix(double **matrix, int x);
void destroySPPointArray(SPPoint **point_arr, int length);

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
double rand_double() {
  return (double)rand() / (double)RAND_MAX;
}

/**
Helper function

Generates (allocates) a random matrix of doubles of random size within 
[x_start, x_end - 1]x[y_start, y_end - 1] up to maximum of RAND_MAX.
The values with in the matrix are in the rande [val_min, val_max]. 
If val_min is equal to val_max then all the values are set to val_min.

x and y are set to the dimention of the matrix.

Use seed for reproducebility. If seed is not NULL the the value within seed is
checked. If it is 0 the a random seed will be used and stored in seed,
otherwise the value of seed will be used. If seed is NULL a random number will
be used and won't be stored in seed.

The return value is the pointer to the matrix or NULL if the ranges are
invalid of if memory allocation failed.

Don't forget to free the matrix after use.
*/
double **generatDoubleMatrix(int x_start, int x_end, int y_start, int y_end,
  double val_min, double val_max, int *x, int *y, unsigned int *seed) {
  /* Declare variables */
  double **matrix;

  /* Validate arguments */
  if (1 > x_start || 1 > y_start ||
    x_start >= x_end || y_start >= y_end || val_min > val_max) {
    return NULL;
  }

  /* Generate random variables */
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

  *x = x_start + rand() % (x_end - x_start);
  *y = y_start + rand() % (y_end - y_start);
  /* Allocate matrix */
  matrix = (double**)malloc((*x) * sizeof(double*));
  if (NULL == matrix) {
    return NULL;
  }
  for (int i = 0; i < (*x); i++) {
    matrix[i] = (double*)malloc((*y) * sizeof(double));
    if (NULL == matrix[i]) {
      destroyDoubleMatrix(matrix, i);
      return NULL;
    }
  }

  /* Initialize matrix */
  for (int i = 0; i < (*x); i++) {
    for (int j = 0; j < (*y); j++) {
      matrix[i][j] = val_min;
      if (val_max > val_min)
        matrix[i][j] += rand_double() * (val_max - val_min);
    }
  }

  return matrix;
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

/* Frees SPPoint array */
void destroySPPointArray(SPPoint **point_arr, int length) {
  if (point_arr) {
    for (int i = 0; i < length; i++) {
      spPointDestroy(point_arr[i]);
    }
    free(point_arr);
  }
}

/* Test create_tree */
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

  PRINT("Repeating %d times:\n", TCT_REPEAT);
  for (int i = 0; i < TCT_REPEAT && true == returnv; i++) {
    /* Initializing data array */
    seed = 0;
    data = generatDoubleMatrix(TCT_MIN_X, TCT_MAX_X, TCT_MIN_Y, TCT_MAX_Y,
      TCT_VAL_MIN, TCT_VAL_MAX, &x, &y, &seed);
    if (NULL == data) {
      PRINT_E("Internal error in test %s (%s, line %d), aborting!\n",
        __func__, __FILE__, __LINE__);
      break;
    }
    PRINT("Iteration %d, using seed %d\n", i, seed);
    do {
      /* Initialize point array */
      point_arr = (SPPoint**)malloc(x * sizeof(SPPoint*));
      if (NULL == point_arr) {
        PRINT_E("Failed to initialize point_arr.\n");
        break;
      }
      for (int j = 0; j < x; j++) {
        point_arr[j] = spPointCreate(data[j], y, j);
        if (NULL == point_arr[j]) {
          PRINT_E("Failed to initialize SPPoint.\n");
          destroySPPointArray(point_arr, j);
          break;
        }
      }
      do {
        /* Initialize kdArray */
        kdarr = init(point_arr, x);
        if (NULL == kdarr) {
          PRINT_E("Failed to initialize KDArray.\n");
          break;
        }
        do {
          /* Create KDTree */
          /* Choose split method */
          if (i > USE_INCREMENTAL_SPLIT_AFTER) {
            spConfigCreate(CONFIG_INCREMENTAL_SPLIT, &msg);
          }
          else if (i > USE_MAX_SPREAD_SPLIT_AFTER) {
            spConfigCreate(CONFIG_MAX_SPREAD_SPLIT, &msg);
          }
          else if (i > USE_RANDOM_SPLIT_AFTER) {
            spConfigCreate(CONFIG_RANDOM_SPLIT, &msg);
          }
          if (NULL == config) {
            PRINT_E("Unable to load config file: %d\n", msg);
            returnv = false;
            break;
          }

          PRINT("Creating tree using split method %d\n",
            getKDTreeSplitMethod(config));


          kdtree = create_tree(NULL, kdarr, 0);
          if (NULL == kdtree) {} // To prevent unused warning.


          PRINT("No method for destroying KDTree!\n");
        } while (0);
        spKDArrayDestroy();
      } while (0);
      destroySPPointArray(point_arr, x);
    } while (0);
    destroyDoubleMatrix(data, x);
  }

  spConfigDestroy(config);
  return returnv;
}

int main() {
  /* Declare logger */
  SP_LOGGER_MSG msg = spLoggerCreate(LOGGER_FILENAME, LOGGER_LEVEL);
  if (SP_LOGGER_DEFINED != msg && SP_LOGGER_SUCCESS != msg) {
    PRINT_E("Unable to create logger, aborting test suite.\n");
    return -1;
  }

  RUN_TEST(testCreateTree);

  spLoggerDestroy();
  return 0;
}
