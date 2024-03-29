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
    void rotateLeft(AVLNode<Key,Value>* g);
    void rotateRight(AVLNode<Key,Value>* g);
    void rotate(AVLNode<Key,Value>* g, AVLNode<Key,Value>* p);
    void insert_fix(AVLNode<Key,Value>* g, int8_t diff);
    void remove_fix(AVLNode<Key,Value>* g, int8_t diff);
};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* g){
    AVLNode<Key,Value>* gg, * p, * n;
    gg = g->getParent();
    p = g->getRight();
    n = p->getLeft();

    p->setParent(gg);
    p->setLeft(g);
    g->setRight(n);
    g->setParent(p);
    if(n != nullptr)
        n->setParent(g);

    if(gg != nullptr){
        if(gg->getLeft() == g)
            gg->setLeft(p);
        else
            gg->setRight(p);
    } else{
        BinarySearchTree<Key, Value>::root_ = p;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* g){
    AVLNode<Key,Value>* gg, * p, * n;
    gg = g->getParent();
    p = g->getLeft();
    n = p->getRight();

    p->setParent(gg);
    p->setRight(g);
    g->setLeft(n);
    g->setParent(p);
    if(n != nullptr)
        n->setParent(g);

    if(gg != nullptr)
    {
        if(gg->getLeft() == g)
            gg->setLeft(p);
        else
            gg->setRight(p);
    } else{
        BinarySearchTree<Key, Value>::root_ = p;
    }
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotate(AVLNode<Key,Value>* g, AVLNode<Key,Value>* p){
    if(p == g->getLeft()){
        if(p->getBalance() == -1){
            rotateRight(g);
            p->setBalance(0);
            g->setBalance(0);
        }
        else if(p->getBalance() == 0){
            rotateRight(g);
            p->setBalance(1);
            g->setBalance(-1);
        }
        else{
            AVLNode<Key,Value>* n = p->getRight();
            int8_t i = n->getBalance();
            rotateLeft(p);
            rotateRight(g);
            n->setBalance(0);
            g->setBalance(0);
            p->setBalance(0);
            if(i == -1)
                g->setBalance(1);
            else if(i == 1)
                p->setBalance(-1);
        }
    } else{
        if(p->getBalance() == 1){
            rotateLeft(g);
            p->setBalance(0);
            g->setBalance(0);
        }
        else if(p->getBalance() == 0){
            rotateLeft(g);
            p->setBalance(-1);
            g->setBalance(1);
        }
        else{
            AVLNode<Key,Value>* n = p->getLeft();
            int8_t i = n->getBalance();
            rotateRight(p);
            rotateLeft(g);
            n->setBalance(0);
            g->setBalance(0);
            p->setBalance(0);
            if(i == -1)
                p->setBalance(1);
            else if(i == 1)
                g->setBalance(-1);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key, Value>* g, int8_t diff){
    if(g == nullptr)
        return;
    g->setBalance(g->getBalance() + diff);
    if(g->getBalance() == 0)
        return;
    else if(g->getBalance() == diff)
        insert_fix(g->getParent(), g->getParent() == nullptr ? 0 : (g == g->getParent()->getLeft() ? -1 : 1));
    else
        rotate(g, diff < 0 ? g->getLeft() : g->getRight());
    
};
/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    Key key = new_item.first;
    Value val = new_item.second;
    if(BinarySearchTree<Key,Value>::empty()){
        BinarySearchTree<Key, Value>::root_ = new AVLNode<Key, Value>(key, val, nullptr);
        return;
    }

    AVLNode<Key, Value>* curr = dynamic_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
    AVLNode<Key, Value>* p;
    
    while(curr != nullptr){
        p = curr;
        if(key < curr->getKey())
            curr = curr->getLeft();
        else if(key > curr->getKey())
            curr = curr->getRight();
        else{
            curr->setValue(val);
            return;
        }
    }

    AVLNode<Key, Value>* n = new AVLNode<Key, Value>(key, val, p);
    if(key < p->getKey())
        p->setLeft(n);
    else
        p->setRight(n);

    
    if(p->getBalance() != 0)
        p->setBalance(0);
    else{
        p->setBalance(n == p->getLeft() ? -1 : 1);
        AVLNode<Key, Value>* g = p->getParent();
        int8_t diff = g == nullptr ? 0 : (p == g->getLeft() ? -1 : 1);
        insert_fix(g, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key, Value>* g, int8_t diff){
    if(g == nullptr)
        return;
    g->setBalance(g->getBalance() + diff);
    AVLNode<Key, Value>* gg = g->getParent();
    int8_t g_diff = g->getParent() == nullptr ? 0 : (g == g->getParent()->getLeft() ? 1 : -1);

    if(g->getBalance() == 0)
        remove_fix(gg, g_diff);
    else if(g->getBalance() == diff)
        return;
    else{
        AVLNode<Key, Value>* p = diff < 0 ? g->getLeft() : g->getRight();
        int8_t i = p->getBalance();
        rotate(g, p);
        if(i != 0)
            remove_fix(gg, g_diff);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* curr = dynamic_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
    while(curr != nullptr && !(curr->getKey() == key)){
        if(key < curr->getKey())
            curr = curr->getLeft();
        else
            curr = curr->getRight();
    }

    if(curr == nullptr)
        return;

    AVLNode<Key, Value>* p = curr->getParent();
    int8_t diff = 0;

    if(curr->getLeft() == nullptr){
        if(p == nullptr)
            BinarySearchTree<Key, Value>::root_ = curr->getRight();
        else if(curr == p->getLeft()){
            p->setLeft(curr->getRight());
            diff = 1;
        }
        else{
            p->setRight(curr->getRight());
            diff = -1;
        }

        if(curr->getRight() != nullptr)
            curr->getRight()->setParent(p);
    }
    else if(curr->getRight() == nullptr){
        if(p == nullptr)
            BinarySearchTree<Key, Value>::root_ = curr->getLeft();
        else if(curr == p->getLeft()){
            p->setLeft(curr->getLeft());
            diff = 1;
        }
        else{
            p->setRight(curr->getLeft());
            diff = -1;
        }

        if(curr->getLeft() != nullptr)
            curr->getLeft()->setParent(p);
    }
    else{
        AVLNode<Key, Value>* n = dynamic_cast<AVLNode<Key, Value>*>(this->predecessor(curr));
        nodeSwap(curr, n);
        p = curr->getParent();
        if(curr == p->getLeft()){
            p->setLeft(curr->getLeft());
            diff = 1;
        }
        else{
            p->setRight(curr->getLeft());
            diff = -1;
        }

        if(curr->getLeft() != nullptr)
            curr->getLeft()->setParent(p);
    }

    delete curr;
    remove_fix(p, diff);
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
