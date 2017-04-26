#include <malloc.h>
#include <assert.h>

#include "SPPoint.h"

struct sp_point_t {
  int index;
  int dim;
  double* data;
};

double* copyArray(size_t size, double* source) {
  double* result = (double*)malloc(size * sizeof(double));
  if (result == NULL)
    return NULL;
  for (size_t i = 0; i < size; i++) {
    result[i] = source[i];
  }
  return result;
}

SPPoint* spPointCreate(double* data, int dim, int index) {
  if (data == NULL || dim < 1 || index < 0)
    return NULL;
  SPPoint* point = (SPPoint*)malloc(sizeof(SPPoint));
  if (point == NULL) 
    return NULL;
  point->index = index;
  point->dim = dim;
  point->data = copyArray(dim ,data);
  if (point->data == NULL)
    return NULL;
  return point;
}

SPPoint* spPointCopy(SPPoint* source) {
  assert(source != NULL);
  SPPoint* point = (SPPoint*)malloc(sizeof(SPPoint));
  if (point == NULL)
    return NULL;
  point->index = source->index;
  point->dim = source->dim;
  point->data = copyArray(source->dim, source->data);
  if (point->data == NULL)
    return NULL;
  return point;
}

void spPointDestroy(SPPoint* point) {
  if (point != NULL) {
    free(point->data);
    free(point);
  }
}

int spPointGetDimension(SPPoint* point) {
  assert(point != NULL);
  return point->dim;
}

int spPointGetIndex(SPPoint* point) {
  assert(point != NULL);
  return point->index;
}

double spPointGetAxisCoor(SPPoint* point, int axis) {
  assert(point != NULL);
  assert(axis >= 0 && axis < point->dim);
  return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint* p, SPPoint* q) {
  assert(p != NULL);
  assert(q != NULL);
  assert(p->dim == q->dim);
  double distance = 0;
  for (int i = 0; i < p->dim; i++) {
    distance += (p->data[i] - q->data[i]) * (p->data[i] - q->data[i]);
  }
  return distance;
}