#ifndef SPKDTREE_H_
#define SPKDTREE_H_

#include <stdlib.h>

#include "SPPoint.h"
#include "SPConfig.h"
#include "SPBPriorityQueue.h"
#include "SPKDArray.h"

typedef struct sp_kd_tree_node SPKDTreeNode;

SPKDTreeNode* spKDTreeCreate(SPConfig config, SPKDArray* arr);
int spKDTreeGetSplitDim(SPConfig config, SPKDArray* arr);

int spKDTreeKNearestSearch(SPKDTreeNode* kdnode, SPBPQueue* bpq, SPPoint* p);

void spKDTreeDestroy(SPKDTreeNode *tree);

#endif /* SPKDTREE_H_ */
