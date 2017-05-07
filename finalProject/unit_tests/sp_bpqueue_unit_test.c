#include "../SPBPriorityQueue.h"
#include "unit_test_util.h"
#include <malloc.h>
#include <stdbool.h>

// Test Enqueue/Dequeue
bool queueEnqueueDequeueTest() {
  double values[5] = { 3.0, 2.0, 1.0, 4.0, 0.5 };
  int capacity = 3;
  SPBPQueue* q1 = spBPQueueCreate(capacity);
  ASSERT_TRUE(spBPQueueSize(q1) == 0);
  for (int i = 0; i < 5; i++) {
    if (i == 3) {
      ASSERT_TRUE(spBPQueueMaxValue(q1) == 3.0);
      ASSERT_TRUE(spBPQueueEnqueue(q1, i, values[i]) == SP_BPQUEUE_FULL);
      ASSERT_TRUE(spBPQueueMaxValue(q1) == 3.0);
    }
    else if (i == 4) {
      ASSERT_TRUE(spBPQueueMaxValue(q1) == 3.0);
      ASSERT_TRUE(spBPQueueEnqueue(q1, i, values[i]) == SP_BPQUEUE_FULL);
      ASSERT_TRUE(spBPQueueMaxValue(q1) == 2.0);
    }
    else {
      ASSERT_TRUE(spBPQueueEnqueue(q1, i, values[i]) == SP_BPQUEUE_SUCCESS);
      ASSERT_TRUE(spBPQueueSize(q1) == i + 1);
    }
  }
  ASSERT_TRUE(spBPQueueIsFull(q1));
  ASSERT_TRUE(spBPQueueMinValue(q1) == 0.5);
  ASSERT_TRUE(spBPQueueDequeue(q1) == SP_BPQUEUE_SUCCESS);
  ASSERT_TRUE(spBPQueueMinValue(q1) == 1.0);
  ASSERT_TRUE(spBPQueueDequeue(q1) == SP_BPQUEUE_SUCCESS);
  ASSERT_TRUE(spBPQueueMinValue(q1) == 2.0);
  ASSERT_TRUE(spBPQueueDequeue(q1) == SP_BPQUEUE_SUCCESS);
  ASSERT_TRUE(spBPQueueMinValue(q1) == -1.0);
  ASSERT_TRUE(spBPQueueDequeue(q1) == SP_BPQUEUE_EMPTY);
  spBPQueueDestroy(q1);
  return true;
}

//Test copy
bool queueCopyTest() {
  double values[4] = { 3.0, 2.0, 1.0, 4.0 };
  int capacity = 3;
  SPBPQueue* q1 = spBPQueueCreate(capacity);
  SPBPQueue* q2;
  for (int i = 0; i < 4; i++) {
    spBPQueueEnqueue(q1, i, values[i]);
  }
  q2 = spBPQueueCopy(q1);
  ASSERT_TRUE(spBPQueueIsFull(q2));
  ASSERT_TRUE(spBPQueueGetMaxSize(q1) == spBPQueueGetMaxSize(q2));
  ASSERT_TRUE(spBPQueueSize(q1) == spBPQueueSize(q2));
  for (int i = 0; i < 3; i++) {
    ASSERT_TRUE(spBPQueueMinValue(q1) == spBPQueueMinValue(q2));
  }
  spBPQueueDestroy(q1);
  spBPQueueDestroy(q2);
  return true;
}

// Test clear
bool queueClearTest() {
  double values[5] = { 3.0, 2.0, 1.0, 4.0, 0.5 };
  int capacity = 5;
  SPBPQueue* q1 = spBPQueueCreate(capacity);
  for (int i = 0; i < 5; i++) {
    spBPQueueEnqueue(q1, i, values[i]);
  }
  spBPQueueClear(q1);
  ASSERT_TRUE(spBPQueueIsEmpty(q1));
  ASSERT_TRUE(spBPQueueSize(q1) == 0);
  ASSERT_TRUE(spBPQueueDequeue(q1) == SP_BPQUEUE_EMPTY);
  ASSERT_TRUE(spBPQueueMinValue(q1) == -1.0);
  ASSERT_TRUE(spBPQueueMaxValue(q1) == -1.0);
  ASSERT_TRUE(spBPQueueGetMaxSize(q1) == 5);
  spBPQueueDestroy(q1);
  return true;
}

// Test peek/last peek
bool queuePeekLastPeekTest() {
  double values[5] = { 3.0, 2.0, 1.0, 4.0, 0.5 };
  int capacity = 5;
  SPBPQueue* q1 = spBPQueueCreate(capacity);
  BPQueueElement* res = (BPQueueElement*)malloc(sizeof(BPQueueElement));
  ASSERT_TRUE(spBPQueuePeek(q1, res) == SP_BPQUEUE_EMPTY);
  for (int i = 0; i < 5; i++) {
    spBPQueueEnqueue(q1, i, values[i]);
  }
  ASSERT_TRUE(spBPQueuePeek(q1, res) == SP_BPQUEUE_SUCCESS);
  ASSERT_TRUE(res->value == 0.5);
  ASSERT_TRUE(res->index == 4);
  ASSERT_TRUE(spBPQueuePeekLast(q1, res) == SP_BPQUEUE_SUCCESS);
  ASSERT_TRUE(res->value == 4.0);
  ASSERT_TRUE(res->index == 3);
  spBPQueueDequeue(q1);
  ASSERT_TRUE(spBPQueuePeek(q1, res) == SP_BPQUEUE_SUCCESS);
  ASSERT_TRUE(res->value == 1.0);
  ASSERT_TRUE(res->index == 2);
  free(res);
  spBPQueueDestroy(q1);
  return true;
}

int main() {
  RUN_TEST(queueEnqueueDequeueTest);
  RUN_TEST(queueCopyTest);
  RUN_TEST(queueClearTest);
  RUN_TEST(queuePeekLastPeekTest);
  return 0;
}