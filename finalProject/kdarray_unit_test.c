#include <stdio.h>
#include <malloc.h>
#include "SPPoint.h"
#include "SPKDArray.h"

#define POINT_NUM 5
#define DIM 2
int printKDArray(SPKDArray* arr){

		for (int j = 0; j < arr->num_of_points; ++j){
			printf("%c\t", spPointGetIndex(arr->point_array[j]));
		}
		
	
	printf("\n");
		for(int i=0;i<spPointGetDimension(arr->point_array[0]);i++){
		for (int j = 0; j <arr->num_of_points ; ++j){
			printf("%d\t", arr->index_array[i][j].point_index);
		}
		printf("\n");
		}
	
	return 1;
}

int main(int argc, char const *argv[])
{
	
	double data[POINT_NUM][DIM] = {{1,2},
	{123,70},
	{2,7},
	{9,11},
	{3,4}};


	SPPoint** p_arr=(SPPoint**)malloc(POINT_NUM*sizeof(SPPoint**));
	for (int i = 0; i < POINT_NUM; ++i){
		p_arr[i]=spPointCreate(data[i],DIM,97+i);
	}
	SPKDArray* kdarr = init(p_arr,POINT_NUM);
	printKDArray(kdarr);
	SPKDArray** arr = split(kdarr,0);
	SPKDArray* left = arr[0];
	SPKDArray* right = arr[1];

	printf("\n");
	printKDArray(left);
	printf("\n");
	printKDArray(right);

	printf("hello\n");
	return 0;
}



