#include "SPPoint.h"

typedef struct mat_entry{
	int cor;
	int point_index;
	SPPoint* point;
}matrix_entry;

typedef struct sp_kd_array{
	matrix_entry** index_array;
	SPPoint** point_array;
	int num_of_points;
}SPKDArray;

//
SPKDArray* init(SPPoint** arr, int size);
SPKDArray** split(SPKDArray* kdArr, int coor);
void destroyArray();

SPPoint** copyPointArray(SPPoint** arr,int size);

int compare_entry(const void* p1,const void* p2);

