typedef struct sp_kd_tree{
	SPKDTreeNode* root;
}SPKDTree;

typedef struct sp_kd_tree_node{

	int dim;
	double val;
	SPKDTreeNode* left;
	SPKDTreeNode* right;
	SPPoint* data;
}SPKDTreeNode;


	

SPKDTreeNode* create_tree(SPKDArray* arr,int coor){
	int split_dim;
	SPKDTreeNode* tree_result;
	if(arr->num_of_points==1){
		tree_result->dim=-1;
		tree_result->val=-1;
		tree_result->left=NULL;
		tree_result->right=NULL;
		tree_result->data=arr->point_array[0];
	}
	SPKDTreeNode* tree_result;
	SPKDArray** split_result;
	switch(spKDTreeSplitMethod()){
	case MAX_SPREAD:
		split_dim=choose_max_spread();
		break;
	case RANDOM:
		split_dim=choose_random();
		break;
	case INCREMENTAL:
		split_dim=(coor+1)%arr->point_array[0]->dim;
		break;	
	}
	split_result=split(arr,split_dim);
	tree_result->dim=split_dim;
	tree_result->val=tree_median();//TODO
	tree_result->left=create_tree(split_result[0],split_dim);
	tree_result->right=create_tree(split_result[1],split_dim);
	tree_result->data=NULL;
	return tree_result;
}
//TODO
int choose_max_spread();
int choose_random();
double tree_median();
