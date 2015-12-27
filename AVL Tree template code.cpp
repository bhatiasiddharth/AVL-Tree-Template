/*	Author:	Siddharth Bhatia
*/



#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#define OGDF_type float	//change OGDF_type here



template <typename T>	//AVLNode structure
struct AVLNode
{
T element;
AVLNode* left;
AVLNode* right;
int height;
};



AVLNode<OGDF_type> *tree;	//all changes done to this tree. Also have an option of using another tree locally if required
AVLNode<OGDF_type> *ptr;




template <typename T>	
AVLNode<T> *lookup (const T &key)
{
	return lookupLocal(key,tree);						
}


template <typename T>
AVLNode<T> *lookupLocal (const T &key, AVLNode<T> *tree)	//if need to check in a local tree
{	


	if(tree==NULL) return NULL;

	if(key==tree->element)
		return tree;
	
	if(key<tree->element)
		return lookupLocal(key,tree->left);	
		
	else return lookupLocal(key,tree->right);	

}




template <typename T>
AVLNode<T> *findMin (AVLNode<T> *tree)				//returns node with minimum value
{
	if(tree==NULL) return NULL;
	while(tree->left!=NULL)
		tree=tree->left;
	return tree;
}



int max (int a, int b)
{
if (a>=b) return a;
else return b;
}



template <typename T>
int height (AVLNode<T> *P)							//returns height of a node P
{
if(P==NULL) return -1;
return P->height;
}



template <typename T>
AVLNode<T> *singleRotateWithLeft (AVLNode<T> *K) 	//if K has a left child
{
AVLNode<T> *K2;
K2=K->left;
K->left=K2->right;
K2->right=K;
K->height= max(height(K->right),height(K->left))+1;
K2->height=max(height(K2->left),K->height)+1;
return K2;
}



template <typename T>
AVLNode<T> *singleRotateWithRight (AVLNode<T> *K) 	//if K has a right child
{
AVLNode<T> *K2;
K2=K->right;
K->right=K2->left;
K2->left=K;
K->height= max(height(K->right),height(K->left))+1;
K2->height=max(height(K2->right),K->height)+1;
return K2;
}



template <typename T>
AVLNode<T> *doubleRotateWithLeft (AVLNode<T> *K)	//if K has a left child which has a right child
{
K->left= singleRotateWithRight(K->left);
return singleRotateWithLeft(K);
}



template <typename T>
AVLNode<T> *doubleRotateWithRight (AVLNode<T> *K)	//if K has a right child which has a left child
{
K->right= singleRotateWithLeft(K->right);
return singleRotateWithRight(K);
}



template <typename T>
int getBalance (AVLNode<T> *tree)					//used later to check if height is balanced
{
if (tree==NULL) return 0;
else
return (height(tree->left)-height(tree->right));
}





template <typename T>
AVLNode<T> *insert (const T& key)
{
	AVLNode<T> *ptr_tree;
	ptr=ptr_tree=NULL;
	ptr_tree=insertLocal(key,tree);
	return ptr==NULL?ptr_tree:ptr;					//inserts if key not present else returns node containing key

}


template <typename T>
AVLNode<T> *insertLocal (const T& key, AVLNode<T> *tree)
{
	if(tree==NULL)
	{
		tree=(AVLNode<T> *)malloc(sizeof(AVLNode<T>));
		if(tree==NULL) printf("\nError of space\n");
		else
		{
			tree->element=key;
			tree->left=NULL;
			tree->right=NULL;
			tree->height=0;
		}	
	}
	


	else if(key<tree->element)	//go left
	{
		tree->left=insertLocal(key,tree->left);
		if(height(tree->left)-height(tree->right)==2)	//another method to use unbalanced. can use getBalance also
		{
			if(key<tree->left->element)
				tree=singleRotateWithLeft(tree);
			else
				tree=doubleRotateWithLeft(tree);
		}
		
	}
	
	else if(key>tree->element)	//go right
	{
		tree->right=insertLocal(key,tree->right);
		if(height(tree->right)-height(tree->left)==2)	
		{
			if(key>tree->right->element)
				tree=singleRotateWithRight(tree);
			else
				tree=doubleRotateWithRight(tree);
		}
		
	}

	else		//for returning node if already exists
	{
		ptr= tree;
		return tree;
	}
	
	tree->height=max(height(tree->left),height(tree->right))+1;	//for updating height
	return tree;
	
}





template <typename T>
void  remove (const T& key)
{
	tree=removeLocal(key,tree);
}


template <typename T>
AVLNode<T>*  removeLocal (const T& key, AVLNode<T> *tree)
{
	if(tree==NULL) return NULL;
	
	if(key<tree->element)
		tree->left=removeLocal(key,tree->left);
		
		
	else if(key>tree->element)
		tree->right=removeLocal(key,tree->right);
		
		
	else
	{
		AVLNode<T> *temp=tree->left;
		if(tree->left==NULL || tree->right==NULL)
		
		{
			if(temp==NULL)
				temp=tree->right;
		
			if(temp==NULL)	//no child
			{
				temp=tree;
				tree=NULL;
			}
			
			else
			{
				tree->element=temp->element;
				tree->height=temp->height;
				tree->left=NULL;
				tree->right=NULL;
			}
		
			free (temp);
			
		}
	
	
		else		//case for deletion when two children
		{
			AVLNode<T> *temp=findMin(tree->right);
			tree->element=temp->element;
			tree->right= removeLocal(temp->element,tree->right);
			
			
		}
	
	}
	
	if(tree==NULL)
		return tree;
		
	tree->height=max(height(tree->left),height(tree->right))+1;
	
	if(getBalance(tree)>1)
	{
		if(getBalance(tree->left)>=0)
			return singleRotateWithLeft(tree);
		
		else
			return doubleRotateWithLeft(tree);
			
	}
	
	if(getBalance(tree)<-1)
	{
	
		if(getBalance(tree->right)<=0)
			return singleRotateWithRight(tree);
		else
			return doubleRotateWithRight(tree);
	
	}
	
	return tree;	

}





template <typename T>
void printTree (AVLNode<T> *tree)		//to print and see the AVL tree-preorder traversal
{
	if(tree==NULL) return;
	
	cout<<tree->element<<endl;
	printTree(tree->left);
	printTree(tree->right);
}







int main (int argc, char* argv[])
{

    tree = insert<OGDF_type>( 1 );
    tree = insert<OGDF_type>( 2 );
    tree = insert<OGDF_type>( 3 );
    tree = insert<OGDF_type>( 4 );
    tree = insert<OGDF_type>( 5 );
    tree = insert<OGDF_type>( 6 );
    
    printTree<OGDF_type>(tree);
	printf("\n");


    tree = lookup<OGDF_type>(2);		//retrns subtree rooted at 2
	printTree<OGDF_type>(tree);
	printf("\n");
    

    remove<OGDF_type>( 2 );				//removes node with key 2
    printTree<OGDF_type>(tree);
	printf("\n");
    

	tree = insert<OGDF_type>( 1 );		//since 1 already inserted, returns node with key 1
    printTree<OGDF_type>(tree);
	printf("\n");
    
}