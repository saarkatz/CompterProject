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
	tree_result->val=spPointGetAxisCoor(arr->point_array[arr->index_array[split_dim][arr->num_of_points/2].point_index],split_dim);
	tree_result->left=create_tree(split_result[0],split_dim);
	tree_result->right=create_tree(split_result[1],split_dim);
	tree_result->data=NULL;
	return tree_result;
}
//TODO
int choose_max_spread(SPKDArray* kdarr){
	int size = kdarr->num_of_points;
	SPPoint** p_arr =kdarr->point_array;
	matrix_enter** m_arr =kdarr->index_array;
	int max_dim=0;
	double max_val=0;
	double d1;
	double d2;

	for(int i=spPointGetDimension(p_arr[0])-1;i>-1;i--){
			d1 = spPointGetAxisCoor(p_arr[m_arr[i][size-1].point_index],i);
			d2 = spPointGetAxisCoor(p_arr[m_arr[i][0].point_index],i);
			tmp_val=d1-d2;
			if(max_val<tmp_val){
				max_val=tmp_val;
				max_dim=i;
			}
	}
	return max_dim;
}
int choose_random();
double tree_median();

void k_nearest_search(KDTreeNode kdnode , SPBPQueue bpq, SPPoint p){
		if(kdnode==NULL){
			return;
		}
		if(isLeaf(kdnode)){	
		/* Add the current point to the BPQ. Note that this is a no-op if the
			 point is not as good as the points we've seen so far.*/
			/*
				enqueue (index(current), distance(curr,p)) into bpq*/
				return;
		}
		double eps=(kdnode->val)-spPointGetAxisCoor(p,kdnode->dim);
		if(0<=eps){
			//recursively search the left subtree
			k_nearest_search(kdnode->left,bpq,p);

		}
		else{
			k_nearest_search(kdnode->right,bpq,p);
			//recursively search the right subtree
		}
		if(!spBPQueueIsFull(bpq)||eps<spBPQueueMaxValue(bpq)){
			//recursively search the other subtree on the next axis
			//???
		}

	}
