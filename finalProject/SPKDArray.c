#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdlib.h>
#include "SPPoint.h"
#include "SPKDArray.h"

void freeAll(SPKDArray** result, SPPoint** P1, SPPoint** P2, matrix_entry** M1, matrix_entry** M2) {
  if (result) {
    free(result[0]);
    free(result[1]);
  }
  free(result);
  free(P1);
  free(P2);
  free(M1);
  free(M2);
}

int compare_entry(const void* p1, const void* p2) {
  const matrix_entry *q1 = (matrix_entry*)p1, *q2 = (matrix_entry*)p2;
  double d1 = spPointGetAxisCoor(q1->point, q1->cor);
  double d2 = spPointGetAxisCoor(q2->point, q2->cor);
  if (d1 == d2) return 0;
  else if (d1 < d2) return -1;
  else return 1;
}

SPPoint** copyPointArray(SPPoint** arr, int size) {
  SPPoint** result = (SPPoint**)malloc(size * sizeof(*result));
  if (NULL == result) {
    return NULL;
  }
  for (int i = 0; i < size; i++) {
    result[i] = spPointCopy(arr[i]);
    if (NULL == result[i]) {
      spPointArrayDestroy(result, i);
      return NULL;
    }
  }
  return result;
}

void destroyMatrixEntryArray(matrix_entry** index_array, int dim) {
  if (index_array) {
    for (int i = 0; i < dim; i++) {
      free(index_array[i]);
    }
    free(index_array);
  }
}


SPKDArray** spKDArraySplit(SPKDArray* kdArr, int coor) {
  SPKDArray** result;
  int* X;

  int P1_size;
  int P2_size;
  SPPoint** P1;
  SPPoint** P2;

  int P1_cap;
  int P2_cap;
  int* map1;
  int* map2;

  matrix_entry** M1;
  matrix_entry** M2;

  int tmp;

  P1_size = kdArr->num_of_points - kdArr->num_of_points / 2;
  P2_size = kdArr->num_of_points / 2;

  P1_cap = 0;
  P2_cap = 0;

  /* Malloc all the components of the split */
  result = (SPKDArray**)malloc(2 * sizeof(*result));
  if (NULL == result) {
    return NULL;
  }
  result[0] = (SPKDArray*)malloc(sizeof(*result[0]));
  if (NULL == result[0]) {
    free(result);
    return NULL;
  }
  result[1] = (SPKDArray*)malloc(sizeof(*result[1]));
  if (NULL == result[1]) {
    free(result[0]);
    free(result);
    return NULL;
  }
  P1 = (SPPoint**)malloc(P1_size*(sizeof(*P1)));
  if (NULL == P1) {
    free(result[0]);
    free(result[1]);
    free(result);
    return NULL;
  }
  P2 = (SPPoint**)malloc(P2_size*(sizeof(*P2)));
  if (NULL == P2) {
    free(result[0]);
    free(result[1]);
    free(P1);
    free(result);
    return NULL;
  }
  M1 = (matrix_entry**)malloc(spPointGetDimension(kdArr->point_array[0]) *
      sizeof(*M1));
  if (NULL == M1) {
    free(result[0]);
    free(result[1]);
    free(P1);
    free(P2);
    free(result);
    return NULL;
  }
  M2 = (matrix_entry**)malloc(spPointGetDimension(kdArr->point_array[0]) *
      sizeof(*M2));
  if (NULL == M2) {
    free(result[0]);
    free(result[1]);
    free(P1);
    free(P2);
    free(M1);
    free(result);
    return NULL;
  }

  X = (int*)malloc(kdArr->num_of_points * sizeof(*X));
  if (NULL == X) {
    freeAll(result, P1, P2, M1, M2);
    return NULL;
  }
  do {
    map1 = (int*)malloc(kdArr->num_of_points * (sizeof(*map1)));
    if (NULL == map1) {
      freeAll(result, P1, P2, M1, M2);
      result = NULL;
      break;
    }
    do {
      map2 = (int*)malloc(kdArr->num_of_points * (sizeof(*map2)));
      if (NULL == map2) {
        freeAll(result, P1, P2, M1, M2);
        result = NULL;
        break;
      }
      do {
        for (int i = 0; i < kdArr->num_of_points; ++i) {
          X[i] = 1;
        }
        for (int i = 0; i < P1_size; ++i) {
          X[kdArr->index_array[coor][i].point_index] = 0;
        }

        for (int i = 0; i < kdArr->num_of_points && NULL != result; ++i) {
          if (!X[i]) {
            map1[i] = P1_cap;
            map2[i] = -1;
            P1[P1_cap] = spPointCopy(kdArr->point_array[i]);
            if (NULL == P1[P1_cap]) {
              freeAll(result, NULL, NULL, M1, M2);
              result = NULL;
              break;
            }
            P1_cap++;
          }
          else {
            map1[i] = -1;
            map2[i] = P2_cap;
            P2[P2_cap] = spPointCopy(kdArr->point_array[i]);
            if (NULL == P2[P2_cap]) {
              freeAll(result, NULL, NULL, M1, M2);
              result = NULL;
              break;
            }
            P2_cap++;
          }
        }
        if (NULL == result) {
          /* We exited the for loop due to nenory alloc fail */
          spPointArrayDestroy(P1, P1_cap);
          spPointArrayDestroy(P2, P2_cap);
          break;
        }

        P1_cap = 0;
        P2_cap = 0;
        for (int i = 0; i < spPointGetDimension(kdArr->point_array[0]) &&
          NULL != result; ++i) {
          M1[i] = (matrix_entry*)malloc((P1_size * sizeof(*M1[i])));
          if (NULL == M1[i]) {
            freeAll(result, NULL, NULL, NULL, NULL);
            spPointArrayDestroy(P1, P1_size);
            spPointArrayDestroy(P2, P2_size);
            result = NULL;
            break;
          }
          M2[i] = (matrix_entry*)malloc((P2_size * sizeof(*M2[i])));
          if (NULL == M2[i]) {
            free(M1[i]);
            freeAll(result, NULL, NULL, NULL, NULL);
            spPointArrayDestroy(P1, P1_size);
            spPointArrayDestroy(P2, P2_size);
            result = NULL;
            break;
          }
          for (int j = 0; j < kdArr->num_of_points; j++) {
            tmp = kdArr->index_array[i][j].point_index;
            if (!X[tmp]) {
              M1[i][P1_cap].point = P1[map1[tmp]];
              M1[i][P1_cap].cor = i;
              M1[i][P1_cap].point_index = map1[tmp];
              P1_cap++;
            }
            else {
              M2[i][P2_cap].point = P2[map2[tmp]];
              M2[i][P2_cap].cor = i;
              M2[i][P2_cap].point_index = map2[tmp];
              P2_cap++;
            }
          }
          P1_cap = 0;
          P2_cap = 0;
        }

        result[0]->point_array = P1;
        result[0]->index_array = M1;
        result[0]->num_of_points = P1_size;

        result[1]->point_array = P2;
        result[1]->index_array = M2;
        result[1]->num_of_points = P2_size;

      } while (0); /* map2 */
      free(map2);
    } while (0); /* map1 */
    free(map1);
  } while (0); /* X */
  free(X);

  return result;
}

SPKDArray* spKDArrayCreate(SPPoint** arr, int size) {
  SPPoint** p_arr;
  SPKDArray* result;
  matrix_entry** i_arr;

  p_arr = copyPointArray(arr, size);
  if (NULL == p_arr) {
    return NULL;
  }
  result = (SPKDArray*)malloc(sizeof(*result));
  if (NULL == result) {
    spPointArrayDestroy(p_arr, size);
    return NULL;
  }
  i_arr =
    (matrix_entry**)malloc(spPointGetDimension(arr[0]) * sizeof(*i_arr));
  if (NULL == i_arr) {
    spPointArrayDestroy(p_arr, size);
    free(result);
    return NULL;
  }

  for (int i = 0; i < spPointGetDimension(arr[0]); ++i) {
    i_arr[i] = (matrix_entry*)malloc((size) * sizeof(*i_arr[i]));
    if (NULL == i_arr[i]) {
      for (int _j = 0; _j < i; _j++) {
        free(i_arr[_j]);
      }
      spPointArrayDestroy(p_arr, size);
      free(result);
      free(i_arr);
      return NULL;
    }
    for (int j = 0; j < size; ++j) {
      i_arr[i][j].point = p_arr[j];
      i_arr[i][j].cor = i;
      i_arr[i][j].point_index = j;
    }
  }
  for (int i = 0; i < spPointGetDimension(arr[0]); i++) {
    qsort(i_arr[i], size, sizeof(*i_arr[i]), compare_entry);
  }
  result->index_array = i_arr;
  result->point_array = p_arr;
  result->num_of_points = size;

  return result;
}


void spKDArrayDestroy(SPKDArray* kdarr) {
  if (kdarr != NULL) {
    if (kdarr->num_of_points > 0) {
      destroyMatrixEntryArray(kdarr->index_array, spPointGetDimension(kdarr->point_array[0]));
      spPointArrayDestroy(kdarr->point_array, kdarr->num_of_points);
    }
  }
  free(kdarr);
}
