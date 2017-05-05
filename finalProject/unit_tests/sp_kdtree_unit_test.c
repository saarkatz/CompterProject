#include <stdbool.h>
#include <sys/time.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include "unit_test_util.h"

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
void destroySPPointArray(SPPoint **point_arr, int length);
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

  /* Validate arguments */
  if (1 > x_start || 1 > y_start ||
    x_start >= x_end || y_start >= y_end || val_min > val_max) {
    return NULL;
  }

  /* Generate random variables */
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
      if (G_RANDOM == option) {
        matrix[i][j] = val_min;
        if (val_max > val_min)
          matrix[i][j] += rand_double(true) * (val_max - val_min);
      }
      else if (G_LINE == option) {
        matrix[i][j] = val_min + val_max * i;
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
      j = i + (size_t)(rand_double(false) * (n - i)) + 1;
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
    set_random_generator(&seed);
    data = generatDoubleMatrix(TCT_MIN_X, TCT_MAX_X, TCT_MIN_Y, TCT_MAX_Y,
      TCT_VAL_MIN, TCT_VAL_MAX, G_RANDOM, &x, &y);
    if (NULL == data) {
      PRINT_E("Internal error, aborting!\n");
      returnv = false;
      break;
    }
    PRINT("Iteration %d, using seed %d\n", i, seed);
    do {
      /* Initialize point array */
      point_arr = (SPPoint**)malloc(x * sizeof(SPPoint*));
      if (NULL == point_arr) {
        PRINT_E("Failed to initialize point_arr.\n");
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
          PRINT_E("Failed to initialize SPPoint.\n");
          destroySPPointArray(point_arr, j);
          returnv = false;
          break;
        }
      }
      do {
        /* Shuffle the array before initializing the kdArray */
        shuffle(point_arr, x);

        /* Initialize kdArray */
        kdarr = init(point_arr, x);
        if (NULL == kdarr) {
          PRINT_E("Failed to initialize KDArray.\n");
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
            PRINT_E("Unable to load config file: %d\n", msg);
            returnv = false;
            break;
          }
          do {
            PRINT("Creating tree using split method %d\n",
              spConfigGetSplitMethod(config, &msg));


            kdtree = create_tree(config, kdarr, 0);
            ASSERT_FALSE(NULL == kdtree);

            spKDTreeDestroy(kdtree);
          } while (0);
          spConfigDestroy(config);
        } while (0);
        spKDArrayDestroy(kdarr);
      } while (0);
      destroySPPointArray(point_arr, x);
    } while (0);
    destroyDoubleMatrix(data, x);
  }

  return returnv;
}

/* Test k_nearest_search */
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

  PRINT("Repeating %d times:\n", TCT_REPEAT);
  for (int i = 0; i < TCT_REPEAT && true == returnv; i++) {
    /* Initializing data array */
    set_random_generator(&seed);
    data = generatDoubleMatrix(TCT_MAX_X - 1, TCT_MAX_X, TCT_MAX_Y - 1,
      TCT_MAX_Y, TCT_VAL_MIN, TCT_VAL_MAX, G_LINE, &x, &y);
    if (NULL == data) {
      PRINT_E("Internal error, aborting!\n");
      returnv = false;
      break;
    }
    PRINT("Iteration %d\n", i);
    do {
      /* Choose number of similar points */
      num_similay_images = (int)(rand_double(true) * x / 2);
      if (num_similay_images < 1) num_similay_images = 1;

      /* Choose query point and initialize point array */
      query_point_index = num_similay_images / 2 + 
        (int)(rand_double(false) * (x - num_similay_images / 2));
      if (query_point_index >= x) {
        PRINT_E("Chosen query point is out of range!\n");
        returnv = false;
        break;
      }

      point_arr = (SPPoint**)malloc(x * sizeof(SPPoint*));
      if (NULL == point_arr) {
        PRINT_E("Failed to initialize point_arr.\n");
        returnv = false;
        break;
      }
      for (int j = 0; j < x; j++) {
        point_arr[j] = spPointCreate(data[j], y, j);
        if (NULL == point_arr[j]) {
          PRINT_E("Failed to initialize SPPoint.\n");
          destroySPPointArray(point_arr, j);
          returnv = false;
          break;
        }
        /* Pick the chosen point */
        if (j == query_point_index) {
          query_point = point_arr[j];
        }
      }

      do {
//        PRINT("1\n");
        /* Suffle point array before initializing kdArray */
        shuffle(point_arr, x);
//        PRINT("1.5\n");
        /* Initialize kdArray */
        kdarr = init(point_arr, x);
        if (NULL == kdarr) {
          PRINT_E("Failed to initialize KDArray.\n");
          returnv = false;
          break;
        }
        do {
//          PRINT("2\n");
          if (msg || kdtree || config) {}
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
            PRINT_E("Unable to load config file: %d\n", msg);
            returnv = false;
            break;
          }
          do {
//            PRINT("3\n");
            /* Create KDTree */
            PRINT("Creating tree using split method %d\n",
              spConfigGetSplitMethod(config, &msg));

            kdtree = create_tree(NULL, kdarr, 0);
            if (NULL == kdtree) {
              PRINT_E("Failed to initialize KDTree.\n");
              returnv = false;
              break;
            }
            do {
//              PRINT("4\n");
              /* Create BPriorityQueue */
              priority_queue = spBPQueueCreate(num_similay_images);
              if (NULL == priority_queue) {
                PRINT_E("Failed to initialize priority queue!\n");
                returnv = false;
                break;
              }
              do {
//                PRINT("5\n");
                /* Create queue element */
                res = (BPQueueElement*)malloc(sizeof(*res));
                if (NULL == res) {
                  PRINT_E("Failed to initialize queue element!\n");
                  returnv = false;
                  break;
                }
                do {
//                  PRINT("6\n");
                  if (kdtree || query_point) {}
                  ///* Run k_nearest_search */
                  //k_nearest_search(kdtree, priority_queue, query_point);

                  //for (int i = 0; i < num_similay_images; i++) {
                  //  spBPQueuePeek(priority_queue, res);
                  //  /* This might not work due to rounding error */
                  //  if (0 == i % 2) {
                  //    ASSERT_TRUE_NO_EXIT(
                  //      res->index == spPointGetIndex(query_point) + i,
                  //      returnv);
                  //  }
                  //  else {
                  //    ASSERT_TRUE_NO_EXIT(
                  //      res->index == spPointGetIndex(query_point) - i,
                  //      returnv);
                  //  }

                  //  if (false == returnv) {
                  //    /* Stop on failure */
                  //    break;
                  //  }

                  //  spBPQueueDequeue(priority_queue);
                  //}
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
      destroySPPointArray(point_arr, x);
    } while (0);
    destroyDoubleMatrix(data, x);
  }

  return returnv;
}


int main() {
  /* Declare logger */
  SP_LOGGER_MSG msg = spLoggerCreate(UT_LOGGER_FILENAME, UT_LOGGER_LEVEL);
  if (SP_LOGGER_DEFINED != msg && SP_LOGGER_SUCCESS != msg) {
    PRINT_E("Unable to create logger (%d), aborting test suite.\n", msg);
    return -1;
  }

  RUN_TEST(testCreateTree);
  RUN_TEST(testKNearestSearch);

  spLoggerDestroy();
  return 0;
}
