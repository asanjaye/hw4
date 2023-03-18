#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    if(root==nullptr){
        return 1;
    }
    
    // Add your code below
	
    int leftPath = findHeight(root->left);
    int rightPath = findHeight(root->right);
    if(root->left==nullptr && root->right!=nullptr){
        leftPath+= 1;
    }
    if(root->left!=nullptr && root->right==nullptr){
        rightPath+= 1;
    }
    
	

    return (leftPath==rightPath);
}

int findHeight(Node* root){
    
    if(root==nullptr){
			return 0;
		}
        
		int path = findHeight(root->left) + findHeight(root->right) +1;
        if(root->left !=nullptr && root->right !=nullptr){
            path-=1;
        }
        
		return path;
}
