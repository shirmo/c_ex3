#include <stdio.h>
#include "RBTree.h"
#include <stdlib.h>


#define NO_ROOT NULL
#define EMPTY_TREE 0
#define EQUALS 0


Node * findSuccessor(Node * start);
Node * minNodeInSubTree(Node * head);
Node * createNode(void * data);
Node * potentialParent(RBTree *tree, Node * node);

/**
 * constructs a new RBTree with the given CompareFunc.
 * @param compFunc a comparator that fits the data type of the tree
 * @param freeFunc a function that free's the fields we allocated during the tree build
 * @return a pointer to an instance of an RBTree
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    if (compFunc == NULL || freeFunc == NULL)
    {
        return NULL;
    }
    RBTree * newTree = (RBTree *) malloc(sizeof(RBTree));
    if (newTree == NULL) // in case were the allocation didn't go as expected
    {
        return NULL;
    }
    // initilizing default fields in tree;
    newTree->compFunc = compFunc;
    newTree->freeFunc = freeFunc;
    newTree->root = NO_ROOT;
    newTree->size = EMPTY_TREE;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int addToRBTree(RBTree *tree, void *data)
{
    if(tree == NULL || data == NULL)
    {
        return 0;
    }
    Node * newNode = createNode(data);
    if(newNode == NULL)
    {
        return 0;
    }
    if (containsRBTree(tree, newNode))
    {
        return 0;
    }
    // set new node's parent
    Node* parent = potentialParent(tree, newNode);
    newNode->parent = parent;
    if(tree->compFunc(parent->data, newNode -> data))
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }
    // using tree comparator // if comparator returns 0, then return 0 also because the tree already contains it
    // while loop from root to relevant position. //TODO
}

/**
 * finds the potential parent for a node we insert to the given tree
 * @param tree
 * @param node
 * @return the father to be set in the tree
 */
Node * potentialParent(RBTree *tree, Node * node)
{
    Node * current = tree -> root;
    Node * parent = NULL;
    int compare = 0;
    while (current != NULL)
    {
        parent = current;
        //value from compare func
        compare =  tree->compFunc(current->data, node -> data);

        // checks if current's data is grater/lower than node's data
        current = (compare > EQUALS)? current->left:current->right;
    }
    return parent;
}

Node * createNode(void * data)
{
    Node * node = (Node *) malloc(sizeof(Node));
    if (node == NULL || data == NULL)
    {
        return NULL;
    }
    node->data = data;
    node->color = RED;
    node->right = NULL;
    node->left = NULL;
    node->parent = NULL;
}

/**
 * check whether the tree contains this item or not.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int containsRBTree(RBTree *tree, void *data)
{
    if(tree == NULL || data == NULL)
    {
        return 1;
    }
    int compare;
    Node* current = tree->root;
    while (current != NULL)
    {
        compare = tree->compFunc(current->data, data); //calls comparator func
        if(compare > EQUALS) // checks if current's data is grater than data
        {
            current = current->left;
        }
        else if (compare < EQUALS) // checks if current's data is lower than data
        {
            current = current->right;
        }
        else // else they are equals witch means, data is in tree
        {
            return 1;
        }
    }
    return 0;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(RBTree *tree, forEachFunc func, void *args) // implement it in RBTree.c
{
    if(func == NULL)
    {
        return 0;
    }
    Node * start = minNodeInSubTree(tree->root);
    int f = func(start, args);
    if (!f)
    {
        return 0;
    }
    Node * current = findSuccessor(start);
    while(current != NULL)
    {
        f = func(current, args);
        if (!f)
        {
            return 0;
        }
        current = findSuccessor(current);
    }
    return 1;
}


/**
 * find a successor for a node
 * @param start the node to find it's successor
 * @return start's successor
 */
Node * findSuccessor(Node * start)
{
    if(start == NULL)
    {
        return NULL;
    }
    if(start->right != NULL) // if start has a right child
    {
        return minNodeInSubTree(start->right); //returns the minimum of start's right child
    }
    if (start->parent->left == start) // if start is a left child of it's parent
    {
        return start->parent; // returns the parent
    }
    if (start->parent->right == start)
    {
        Node * successor = start->parent;
        while(successor != NULL && successor->right == start)
        {
            start = successor;
            successor = start ->parent;
        }
        return successor; // can return a pointer because successor is not a local variable
    }
    return NULL;
}

/**
 * Finds min node of a sub tree
 * @param head the root of the sub tree
 * @return the min node of that sub tree
 */
Node * minNodeInSubTree(Node * head)
{
    Node *start = NULL;
    Node *current = head;
    while(current != NULL)
    {
        start = current;
        current = current->left;
    }
    return start;
}

// TODO
void freeRBTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return;
    }
}