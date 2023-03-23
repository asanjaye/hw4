#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child);
    void rotateLeft(AVLNode<Key,Value>* curr);
    void rotateRight(AVLNode<Key,Value>* curr);
    void removeFix(AVLNode<Key,Value>* current, int8_t diff);



};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key,Value> *curr = static_cast<AVLNode<Key, Value>*>(this->root_);

    AVLNode<Key,Value> *newNode = new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr);
    // newNode->setLeft(nullptr);
    // newNode->setRight(nullptr);

    if((this->root_)==nullptr){
        (this->root_)  = newNode;
    }
    else{
        while(curr!=nullptr){
            if(newNode->getKey()==curr->getKey()){
                curr->setValue(new_item.second);
                break;
            }
            else if(newNode->getKey() < curr->getKey()){
                if(curr->getLeft()==nullptr){
                    curr->setLeft(newNode);
                    newNode->setParent(curr);
                    if(curr->getBalance()==1){
                        curr->setBalance(0);
                        break;
                    }
                    else if(curr->getBalance()==0){
                        curr->setBalance(-1);
                        insertFix(curr,curr->getLeft());
                        break;
                    }
                    
                }
                else{
                    curr = curr->getLeft();
                }
            }
            else if(newNode->getKey() > curr->getKey()){
                if(curr->getRight()==nullptr){
                    curr->setRight(newNode);
                    newNode->setParent(curr);
                    if(curr->getBalance()==-1){
                        curr->setBalance(0);
                        break;
                    }
                    else if(curr->getBalance()==0){
                        curr->setBalance(1);
                        insertFix(curr,curr->getRight());
                        break;
                    }
                    
                }
                else{
                    curr = curr->getRight();

                }
            }            
    }
    
    //curr should be parent of newNode, if not then fix it
    // else if(curr->getBalance()==-1 || curr->getBalance()==1){
    //     curr->setBalance(0);
    // }
    // else if(curr->getBalance()==0){
    //     if(curr->getLeft()==newNode){
    //         curr->setBalance(-1);
    //     }
    //     else if(curr->getRight()==newNode){
    //         curr->setBalance(1);
    //     }
    //     //call insertfix
    //     insertFix(curr,newNode);
    // }

    
}
}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

template<class Key, class Value>
void AVLTree<Key,Value>::rotateLeft(AVLNode<Key,Value>* curr)
{
    AVLNode<Key,Value>* child = curr->getRight();
    if(curr->getParent()==nullptr){
        (this->root_) = child;
    }
    else{
            AVLNode<Key,Value>* grandparent = curr->getParent();
    child->setParent(grandparent);
    if(grandparent->getLeft()==curr){
        grandparent->setLeft(child);
    }
    else if(grandparent->getRight()==curr){
        grandparent->setRight(child);
    }
    }

    child->setLeft(curr);
    curr->setParent(child);
    curr->setRight(nullptr);
}

template<class Key, class Value>
void AVLTree<Key,Value>::rotateRight(AVLNode<Key,Value>* curr)
{
    AVLNode<Key,Value>* child= curr->getLeft();
    if(curr->getParent()==nullptr){
        (this->root_) = child;
    }
    else{
            AVLNode<Key,Value>* grandparent = curr->getParent();
    child->setParent(grandparent);
    if(grandparent->getLeft()==curr){
        grandparent->setLeft(child);
    }
    else if(grandparent->getRight()==curr){
        grandparent->setRight(child);
    }
    }

    
    child->setRight(curr);
    curr->setParent(child);
    curr->setLeft(nullptr);
}
template<class Key, class Value>
void AVLTree<Key,Value>::insertFix(AVLNode<Key,Value>* current,AVLNode<Key,Value>* child){
   
    if(current==nullptr || current->getParent()==nullptr){
        return;
    }
    AVLNode<Key,Value>* parent = current->getParent();
    if(current == parent->getRight()){
        parent->updateBalance(1);
        if(parent->getBalance()==0){
            return;
        }
        else if(parent->getBalance()==1){
            insertFix(parent,current);
        }
        else{
            if(current->getRight()==child){
                rotateLeft(parent);
                parent->setBalance(0);
                current->setBalance(0);
            }
            else{
                rotateRight(current);
                rotateLeft(parent);
                if(child->getBalance()==-1){
                    current->setBalance(1);
                    parent->setBalance(0);
                    child->setBalance(0);
                }
                else if(child->getBalance()==0){
                    current->setBalance(0);
                    parent->setBalance(0);
                    child->setBalance(0);                    
                }
                else if(child->getBalance()==1){
                    current->setBalance(0);
                    parent->setBalance(-1);
                    child->setBalance(0);                    
                }
                
            }
        }
    }

    else{
        parent->updateBalance(-1);
        if(parent->getBalance()==0){
            return;
        }
        else if(parent->getBalance()==-1){
            insertFix(parent,current);
        }
        else{
            if(current->getLeft()==child){
                rotateRight(parent);
                parent->setBalance(0);
                current->setBalance(0);
            }
            else{
                rotateLeft(current);
                rotateRight(parent);
                if(child->getBalance()==-1){
                    current->setBalance(0);
                    parent->setBalance(1);
                    child->setBalance(0);
                }
                else if(child->getBalance()==0){
                    current->setBalance(0);
                    parent->setBalance(0);
                    child->setBalance(0);                    
                }
                else if(child->getBalance()==1){
                    current->setBalance(-1);
                    parent->setBalance(0);
                    child->setBalance(0);                    
                }
        
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    if(this->root_==nullptr){
        return;
    }    
    //predecessor(this->current_)
    AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if(target!=nullptr){
        if(target->getLeft()!=nullptr && target->getRight()!=nullptr){
            AVLNode<Key,Value>* curr = static_cast<AVLNode<Key,Value>*>(this->predecessor(target));
            nodeSwap(target, curr);
        }
        AVLNode<Key,Value>* parent = target->getParent();
        if(target->getLeft()!=nullptr && target->getRight()!=nullptr){
                if(target==this->root_){
                    delete target;
                    this->root_ = NULL;
                    removeFix(parent, 1);
                }

                if(target->getParent()->getLeft()==target){
                    target->getParent()->setLeft(nullptr);
                    delete target;
                    removeFix(parent,1);
                }
                else if(target->getParent()->getRight()==target){
                    target->getParent()->setRight(nullptr);
                    delete target;
                    removeFix(parent,-1);
                }                
        }
        else if(target->getLeft()!=nullptr && target->getRight()==nullptr){
            if(target==this->root_){
                target->getLeft()->setParent(nullptr);
                this->root_= target->getLeft();
                delete target;
                removeFix(parent,1);
            }
            else{
                target->getRight()->setParent(target->getParent());
                if(target->getParent()->getLeft()==target){
                    target->getParent()->setLeft(target->getLeft());
                    delete target;
                    removeFix(parent,1);
                }
            }
        }
        else if(target->getLeft()==nullptr && target->getRight()!=nullptr){
            if(target==this->root_){
                target->getRight()->setParent(nullptr);
                this->root_= target->getRight();
                delete target;
                removeFix(parent,0);
            }
            else{
                target->getLeft()->setParent(target->getParent());
                if(target->getParent()->getRight()==target){
                    target->getParent()->setRight(target->getRight());
                    delete target;
                    removeFix(parent,-1);
                }
            }
        }

}
}


template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key,Value>* current,int8_t diff){
    if(current->getParent()==nullptr){
			return;
		}
		AVLNode<Key,Value>* parent = current->getParent();
		int8_t newDiff;
		if(parent!=nullptr){
			if(current==current->getParent()->getLeft()){
				newDiff=1;
			}
			else if(current==current->getParent()->getRight()){
				newDiff=-1;
			}
		}
		if(current->getBalance()+diff==-2){
			if(current->getLeft()->getBalance()==-1){
				rotateRight(current);
				current->setBalance(0);
				current->getLeft()->setBalance(0);
				removeFix(parent,diff);
			}
			else if(current->getLeft()->getBalance()==0){
				rotateRight(current);
                current->setBalance(-1);
                current->getLeft()->setBalance(1);

			}
            else if(current->getLeft()->getBalance() == 1){
                rotateLeft(current->getLeft());
                rotateRight(current);
                if(current->getLeft()->getRight()->getBalance()==1){
                    current->setBalance(0);
                    current->getLeft()->setBalance(-1);
                    current->getLeft()->getRight()->setBalance(0);
                }
                else if(current->getLeft()->getRight()->getBalance() == 0){
                        current->setBalance(0);
                        current->getLeft()->setBalance(0);
                        current->getLeft()->getRight()->setBalance(0);
                        }                
                else if(current->getLeft()->getRight()->getBalance()== -1){
                    current->setBalance(1);
                    current->getLeft()->setBalance(0);
                    current->getLeft()->getRight()->setBalance(0);
                }
        
        removeFix(parent, newDiff);
		}
        }
        else if (parent->getBalance()+diff == 2){
            	if(current->getRight()->getBalance()==-1){
				rotateLeft(current);
				current->setBalance(0);
				current->getRight()->setBalance(0);
				removeFix(parent,diff);
			}
			else if(current->getRight()->getBalance()==0){
				rotateLeft(current);
                current->setBalance(-1);
                current->getRight()->setBalance(1);

			}
            else if(current->getRight()->getBalance() == 1){
                rotateRight(current->getRight());
                rotateLeft(current);
                if(current->getRight()->getLeft()->getBalance()==1){
                    current->setBalance(0);
                    current->getRight()->setBalance(-1);
                    current->getRight()->getLeft()->setBalance(0);
                }
                else if(current->getRight()->getLeft()->getBalance() == 0){
                        current->setBalance(0);
                        current->getRight()->setBalance(0);
                        current->getRight()->getLeft()->setBalance(0);
                        }                
                else if(current->getRight()->getLeft()->getBalance()== -1){
                    current->setBalance(1);
                    current->getRight()->setBalance(0);
                    current->getRight()->getLeft()->setBalance(0);
                }
        
        removeFix(parent, newDiff);
		}
         
}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
