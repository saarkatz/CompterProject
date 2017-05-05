#include <stdlib.h>
#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPKDTree.h"
#include "SPBPriorityQueue.h"
#include "SPConfig.h"

///typedef struct sp_kd_tree_node SPKDTreeNode;

struct sp_kd_tree_node{
	int dim;
	double val;
	SPKDTreeNode* left;
	SPKDTreeNode* right;
	SPPoint* data;
};
 
int choose_random(int n) {
  return rand()%n;
}

//TODO
int choose_max_spread(SPKDArray* kdarr) {
  int size = kdarr->num_of_points;
  SPPoint** p_arr = kdarr->point_array;
  matrix_entry** m_arr = kdarr->index_array;
  int max_dim = 0;
  double max_val = 0;
  double d1;
  double d2;

  for (int i = spPointGetDimension(p_arr[0]) - 1; i>-1; i--) {
    d1 = spPointGetAxisCoor(p_arr[m_arr[i][size - 1].point_index], i);
    d2 = spPointGetAxisCoor(p_arr[m_arr[i][0].point_index], i);
    int tmp_val = d1 - d2;
    if (max_val<tmp_val) {
      max_val = tmp_val;
      max_dim = i;
    }
  }
  return max_dim;
}

SPKDTreeNode* create_tree_main(SPConfig config, SPKDArray* arr) {

  return create_tree(config,arr,spGetSplitDim(config,arr));
}

int spGetSplitDim(SPConfig config, SPKDArray* arr){
 // int split_dim;
  SP_CONFIG_MSG msg;
  switch (spConfigGetSplitMethod(config, &msg)) {
  case MAX_SPREAD:
    return choose_max_spread(arr);
  case RANDOM:
    return choose_random(spPointGetDimension(arr->point_array[0]));
  case INCREMENTAL:
    return 0;
  default:
    return -1;
  }

}

//coor is the current coordinate
SPKDTreeNode* create_tree(SPConfig config, SPKDArray* arr, int coor) {
  SP_CONFIG_MSG msg;
  int split_dim;
  SPKDTreeNode* tree_result = (SPKDTreeNode*)malloc(sizeof(SPKDTreeNode));
  if (arr->num_of_points == 1) {
    tree_result->dim = -1;
    tree_result->val = -1;
    tree_result->left = NULL;
    tree_result->right = NULL;
    tree_result->data = arr->point_array[0];
    return tree_result;
  }
  SPKDArray** split_result;
  //printf("spConfigGetSplitMethod(config, &msg): %d\n", spConfigGetSplitMethod(config, &msg));
  switch (spConfigGetSplitMethod(config, &msg)) {
  case MAX_SPREAD:
    split_dim = choose_max_spread(arr);
    break;
  case RANDOM:
    split_dim = choose_random(spPointGetDimension(arr->point_array[0]));
    break;
  case INCREMENTAL:
    split_dim = ((coor + 1) % (spPointGetDimension(arr->point_array[0])));
    break;
  default:
    // TODO - Handle this better
    printf("Error occured while creating the tree.\n");
    free(tree_result);
    return NULL;
  }

  split_result = split(arr, split_dim);
  tree_result->dim = split_dim;
  tree_result->val = spPointGetAxisCoor(arr->point_array[arr->index_array[split_dim][arr->num_of_points / 2].point_index], split_dim);
  tree_result->left = create_tree(config, split_result[0], split_dim);
  tree_result->right = create_tree(config, split_result[1], split_dim);
  tree_result->data = NULL;
  return tree_result;
}

double tree_median();

bool isLeaf(SPKDTreeNode* kdnode){
  return kdnode->data==NULL;
}

void k_nearest_search(SPKDTreeNode* kdnode, SPBPQueue* bpq, SPPoint* query_point) {
  if (kdnode == NULL) {
    return;
  }
  if (isLeaf(kdnode)) {
    double dist = spPointL2SquaredDistance(kdnode->data, query_point);
    spBPQueueEnqueue(bpq, spPointGetIndex(query_point), dist);
    /* Add the current point to the BPQ. Note that this is a no-op if the
         point is not as good as the points we've seen so far.*/

    return;
  }
  double eps = (kdnode->val) - spPointGetAxisCoor(query_point, kdnode->dim);
  SPKDTreeNode* primary;
  SPKDTreeNode* secondary;

  if (0 <= eps) {
    primary = kdnode->left;
    secondary = kdnode->right;
  }
  else {
    primary = kdnode->right;
    secondary = kdnode->left;
  }
  k_nearest_search(primary, bpq, query_point);
  if (!spBPQueueIsFull(bpq) || eps < spBPQueueMaxValue(bpq)) {
    //recursively search the other subtree on the next axis
    k_nearest_search(secondary, bpq, query_point);
  }
  return;
}

void spKDTreeDestroy(SPKDTreeNode *tree) {
  if (NULL == tree) {
    return;
  }
  spKDTreeDestroy(tree->left);
  spKDTreeDestroy(tree->right);
  free(tree);
}


