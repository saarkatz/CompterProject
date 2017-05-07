#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_

#include "SPPoint.h"

typedef struct mat_entry {
  int cor;
  int point_index;
  SPPoint* point;
}matrix_entry;

typedef struct sp_kd_array{
	matrix_entry** index_array;
	SPPoint** point_array;
	int num_of_points;
}SPKDArray;

/* Creates a new kdarray from a list of points */
SPKDArray* spKDArrayCreate(SPPoint** arr, int size);

/* Split kdArray */
SPKDArray** spKDArraySplit(SPKDArray* kdArr, int coor);

/* Frees all memory allocated for kdarray */
void spKDArrayDestroy(SPKDArray* kdarr);

void destroyMatrixEntryArray(matrix_entry** index_array,int dim);
SPPoint** copyPointArray(SPPoint** arr,int size);

#endif 