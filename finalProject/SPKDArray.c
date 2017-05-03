#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdlib.h>
#include "SPPoint.h"
#include "SPKDArray.h"


//
SPKDArray* init(SPPoint** arr, int size) {
	SPPoint** p_arr = copyPointArray(arr,size);//free this in DESTROY
	SPKDArray* result=(SPKDArray*)malloc(sizeof(SPKDArray*));//return this
	matrix_entry** i_arr= 
	(matrix_entry**)malloc(spPointGetDimension(arr[0])*sizeof(matrix_entry*));//free this in DESTROY
	for (int i = 0; i < spPointGetDimension(arr[0]); ++i){
		i_arr[i]=(matrix_entry*)malloc(((size)*sizeof(matrix_entry)));//free this in DESTORY
		for (int j = 0; j < size; ++j){
			i_arr[i][j].point=p_arr[j];
			i_arr[i][j].cor=i;
			i_arr[i][j].point_index=j;
		}
	}
	for(int i=0;i<spPointGetDimension(arr[0]);i++){
		qsort(i_arr[i],size,sizeof(matrix_entry),compare_entry);
	}
	result->index_array=i_arr;
	result->point_array=p_arr;
	result->num_of_points=size;
	return result;
}

SPKDArray** split(SPKDArray* kdArr, int coor){
	SPKDArray* left=(SPKDArray*)malloc(sizeof(SPKDArray*));;
	SPKDArray* right=(SPKDArray*)malloc(sizeof(SPKDArray*));;
	SPKDArray** result=(SPKDArray**)malloc(2*sizeof(SPKDArray*));
	int* X = (int*)malloc(kdArr->num_of_points*sizeof(int));//free this before exiting
	int P1_size = kdArr->num_of_points-kdArr->num_of_points/2;
	int P2_size = kdArr->num_of_points/2;
	//TODO -check malloc
	for (int i = 0; i < kdArr->num_of_points; ++i){
		X[i]=1;
	}
	for (int i = 0; i < P1_size; ++i){
		X[kdArr->index_array[coor][i].point_index]=0;///
	}

	
	int P1_cap=0;
	int P2_cap=0;
	SPPoint** P1=(SPPoint**)malloc(P1_size*(sizeof(SPPoint*)));
	SPPoint** P2=(SPPoint**)malloc(P2_size*(sizeof(SPPoint*)));
	for (int i = 0; i < kdArr->num_of_points; ++i){
		if(!X[i]){
			P1[P1_cap++]=kdArr->point_array[i];
		}
		else{
			P2[P2_cap++]=kdArr->point_array[i];
		}
	}
	int* map1=(int*)malloc(kdArr->num_of_points*(sizeof(int)));
	P1_cap=0;
	for(int i=0;i<kdArr->num_of_points;i++){
		if(kdArr->point_array[i]==P1[P1_cap]){//is pointer cmp ok?	
			map1[i]=P1_cap++;
		}
		else{
			map1[i]=-1;
		}
	}
	P2_cap=0;
	int* map2=(int*)malloc(kdArr->num_of_points*(sizeof(int)));
	for(int i=0;i<kdArr->num_of_points;i++){
		if(kdArr->point_array[i]==P2[P2_cap]){
			map2[i]=P2_cap++;
		}
		else{
			map2[i]=-1;
		}
	}
	matrix_entry** M1 =(matrix_entry**) malloc(spPointGetDimension(kdArr->point_array[0])*sizeof(matrix_entry*));
	matrix_entry** M2 =(matrix_entry**) malloc(spPointGetDimension(kdArr->point_array[0])*sizeof(matrix_entry*));
	int tmp;
	P1_cap=0;
	P2_cap=0;
		for (int i = 0; i < spPointGetDimension(kdArr->point_array[0]); ++i){
			M1[i]=(matrix_entry*)malloc((P1_size*sizeof(matrix_entry)));
			M2[i]=(matrix_entry*)malloc((P2_size*sizeof(matrix_entry)));
			for (int j = 0; j < kdArr->num_of_points; j++){
				tmp=kdArr->index_array[i][j].point_index;
				if(!X[tmp]){

					M1[i][P1_cap].point=kdArr->point_array[tmp];
					M1[i][P1_cap].cor=i;
					M1[i][P1_cap].point_index=map1[tmp];
					P1_cap++;
				}
				else{
					M2[i][P2_cap].point=kdArr->point_array[tmp];
					M2[i][P2_cap].cor=i;
					M2[i][P2_cap].point_index=map2[tmp];
					P2_cap++;
				}			
			}
			P1_cap=0;
			P2_cap=0;
		}
	left->point_array=P1;
	left->index_array=M1;
	left->num_of_points=P1_size;

	right->point_array=P2;
	right->index_array=M2;
	right->num_of_points=P2_size;
	result[0]=left;	
	result[1]=right;

	return result;
}

void spKDArrayDestroy(SPKDArray* kdarr) { 
	if(!kdarr){
	destroyMatrixEntryArray(kdarr->index_array,spPointGetDimension(kdarr->point_array[0]));
	destroyPointArray(kdarr->point_array,kdarr->num_of_points);
 	 //printf("spKDArrayDestroy is called but not implemented!\n");
	free(kdarr);		
	}
}
	
int compare_entry(const void* p1,const void* p2){
	const matrix_entry *q1=(matrix_entry*)p1,*q2=(matrix_entry*)p2;
	double d1 = spPointGetAxisCoor(q1->point,q1->cor);
	double d2 = spPointGetAxisCoor(q2->point,q2->cor);
	if(d1==d2)return 0;
	else if(d1<d2)return -1;
	else return 1;
}
SPPoint** copyPointArray(SPPoint** arr,int size){
	SPPoint** result = (SPPoint**)malloc(size*sizeof(SPPoint*));
	for(int i=0;i<size;i++){
		result[i]=spPointCopy(arr[i]);
	}
	return result;
}
void destroyMatrixEntryArray(matrix_entry** index_array,int dim){
	for(int i=0;i<dim;i++){
		free(index_array[i]);
	}
	free(index_array);
}
void destroyPointArray(SPPoint** point_array,int num){
	for(int i=0;i<num;i++){
		spPointDestroy(point_array[i]);
	}
	free(point_array);
}