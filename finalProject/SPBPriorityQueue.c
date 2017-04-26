#include <malloc.h>
#include <assert.h>

#include "SPBPriorityQueue.h"

struct sp_bp_queue_t {
  int capacity;
  int size;
  BPQueueElement** elements;
};

SPBPQueue* spBPQueueCreate(int maxSize) {
  SPBPQueue* queue;
  if (maxSize <= 0)
    return NULL;
  queue = (SPBPQueue*)malloc(sizeof(SPBPQueue));
  if (queue == NULL)
    return NULL;
  queue->capacity = maxSize;
  queue->size = 0;
  queue->elements = (BPQueueElement**)malloc(maxSize * sizeof(BPQueueElement*));
  if (queue->elements == NULL) {
    free(queue);
    return NULL;
  }
  for (int i = 0; i < maxSize; i++) {
    queue->elements[i] = (BPQueueElement*)malloc(sizeof(BPQueueElement));
    if (queue->elements[i] == NULL) {
      spBPQueueDestroy(queue);
      return NULL;
    }
  }
  return queue;
}

SPBPQueue* spBPQueueCopy(SPBPQueue* source) {
  assert(source != NULL);
  SPBPQueue* queue;
  queue = spBPQueueCreate(source->capacity);
  queue->size = source->size;
  if (queue == NULL)
    return NULL;
  for (int i = 0; i < source->size; i++) {
    queue->elements[i]->index = source->elements[i]->index;
    queue->elements[i]->value = source->elements[i]->value;
  }
  return queue;
}

void spBPQueueDestroy(SPBPQueue* queue) {
  if (queue != NULL) {
    for (int i = 0; i < queue->capacity; i++) {
      free(queue->elements[i]);
    }
    free(queue->elements);
    free(queue);
  }
}

void spBPQueueClear(SPBPQueue* queue) {
  assert(queue != NULL);
  queue->size = 0;
}

int spBPQueueSize(SPBPQueue* queue) {
  assert(queue != NULL);
  return queue->size;
}

int spBPQueueGetMaxSize(SPBPQueue* queue) {
  assert(queue != NULL);
  return queue->capacity;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* queue, int index, double value) {
  assert(queue != NULL);
  int i;
  if (index < 0 || value < 0.0) {
    return SP_BPQUEUE_INVALID_ARGUMENT;
  }
  // Find insertion place
  i = queue->size - 1;
  for (; i >= 0 && 
    (queue->elements[i]->value < value ||
    (queue->elements[i]->value == value &&
      queue->elements[i]->index < index));
    i--);
  // Insert
  if (queue->size == queue->capacity) {
    for (int j = 1; j <= i; j++) {
      queue->elements[j - 1]->index = queue->elements[j]->index;
      queue->elements[j - 1]->value = queue->elements[j]->value;
    }
    if (i > -1) {
      queue->elements[i]->index = index;
      queue->elements[i]->value = value;
    }
    return SP_BPQUEUE_FULL;
  }
  else {
    for (int j = queue->size; j > i + 1; j--) {
      queue->elements[j]->index = queue->elements[j - 1]->index;
      queue->elements[j]->value = queue->elements[j - 1]->value;
    }
    queue->elements[i + 1]->index = index;
    queue->elements[i + 1]->value = value;
    queue->size++;
    return SP_BPQUEUE_SUCCESS;
  }
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* queue) {
  assert(queue != NULL);
  if (spBPQueueIsEmpty(queue)) {
    return SP_BPQUEUE_EMPTY;
  }
  queue->size--;
  return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* queue, BPQueueElement* res) {
  assert(queue != NULL);
  assert(res != NULL);
  if (spBPQueueIsEmpty(queue)) {
    return SP_BPQUEUE_EMPTY;
  }
  res->index = queue->elements[queue->size - 1]->index;
  res->value = queue->elements[queue->size - 1]->value;
  return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* queue, BPQueueElement* res) {
  assert(queue != NULL);
  assert(res != NULL);
  if (spBPQueueIsEmpty(queue)) {
    return SP_BPQUEUE_EMPTY;
  }
  res->index = queue->elements[0]->index;
  res->value = queue->elements[0]->value;
  return SP_BPQUEUE_SUCCESS;
}

double spBPQueueMinValue(SPBPQueue* queue) {
  assert(queue != NULL);
  if (spBPQueueIsEmpty(queue)) {
    return -1.0;
  }
  return queue->elements[queue->size - 1]->value;
}

double spBPQueueMaxValue(SPBPQueue* queue) {
  assert(queue != NULL);
  if (spBPQueueIsEmpty(queue)) {
    return -1.0;
  }
  return queue->elements[0]->value;
}

bool spBPQueueIsEmpty(SPBPQueue* queue) {
  assert(queue != NULL);
  return queue->size == 0;
}

bool spBPQueueIsFull(SPBPQueue* queue) {
  assert(queue != NULL);
  return queue->size == queue->capacity;
}