#ifndef SPKDTREE_H_
#define SPKDTREE_H_
#include <stdlib.h>
#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPConfig.h"
#include "SPKDTree.h"
#include "SPBPriorityQueue.h"
typedef struct sp_kd_tree_node SPKDTreeNode;


SPKDTreeNode* create_tree(SPConfig config, SPKDArray* arr,int coor);

//internal function for create tree
int choose_max_spread(SPKDArray* kdarr);


//internal function for create tree
int choose_random();
double tree_median();

void k_nearest_search(SPKDTreeNode* kdnode ,SPBPQueue* bpq, SPPoint* p);


#endif /* SPKDTREE_H_ */
