#include <stdio.h>
#include "RBTree.h"
#include <stdlib.h>


#define NO_ROOT NULL
#define EMPTY_TREE 0
#define EQUALS 0
#define INSERT_FAILED 0
#define INSERT_SUCCESS 1



Node * findSuccessor(Node * start);
Node * minNodeInSubTree(Node * head);
Node * createNode(void * data);
Node * potentialParent(RBTree *tree, Node * node);

void freeNodesInDepth(RBTree * t, Node * node);
void leftLeftCase(Node *, RBTree *);
void leftRightCase(Node * , RBTree *);
void rightLeftCase(Node *, RBTree *);
void rightRightCase(Node *, RBTree *);


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
    // initializing default fields in tree;
    newTree->compFunc = compFunc;
    newTree->freeFunc = freeFunc;
    newTree->root = NO_ROOT;
    newTree->size = EMPTY_TREE;
    return newTree;
}


/**
 * Finds a given node t's uncle
 * @param nef the node we are looking it's uncle
 * @return a pointer to the uncle node
 */
Node * findUncle(Node * nef)
{
    Node * grandparent = nef->parent->parent; //must have a grandparent because it's father is red!
    if(grandparent->right == nef->parent)
    {
        return grandparent->left;
    }
    else
    {
        return grandparent->right;
    }
}

/**
 * finds a node's color
 * @param node a node in the tree
 * @return return a node's color, if node is NULL. return's BLACK
 */
int findColor(Node * node)
{
    if(node == NULL)
    {
        return BLACK;
    }
    return node->color;
}

/**
 *
 * @param parent
 * @param uncle
 * @param grandpa
 */
void fixColors(Node * parent, Node * uncle, Node* grandpa)
{
    // uncle, parent and grandparent surely exist due to having father and uncle colored red
    parent->color = uncle->color = BLACK;
    grandpa->color = RED;
}

/**
 *
 * @param node
 */
void handleRotation(Node * node, RBTree * tree)
{
    Node * parent = node->parent;
    Node * grandparent = parent->parent;
    if(node == parent->left)
    {
        if(parent == grandparent->left)
        {
             leftLeftCase(node, tree);
        }
        else
        {
            rightLeftCase(node, tree);
        }
    }
    else
    {
        if(parent == grandparent->left)
        {
            leftRightCase(node, tree);
        }
        else
            rightRightCase(node, tree);
    }
}


/**
 *
 * @param tree
 * @param z
 */
void balanceTree(RBTree * tree, Node * z)
{
    if(tree == NULL || z == NULL) // check if input is valid
    {
        return;
    }
    if(tree->root == z) //root fix and change;
    {
        z->color = BLACK;
        return;
    }
    if(z->parent->color == RED) //Has a parent (checked if not above)
    {
        Node * uncle = findUncle(z);
        int uncleColor = findColor(uncle);
        if(uncleColor == RED)
        {
            Node * grandpa = uncle->parent;
            fixColors(z->parent, uncle, grandpa); // parent, uncle, grandparent
            balanceTree(tree, grandpa); // recursively fixing the grandparent
        }
        else
        {
            handleRotation(z, tree);
        }
    }
    tree->root->color = BLACK;
}

void swapChildToCorrectPos(Node * parent, Node * current, Node* new)
{
    if(parent->right == current)
    {
        parent->right = new;
    }
    else
    {
        parent->left = new;
    }
}

/**
 *
 * @param node
 */
void leftLeftCase(Node * node, RBTree * tree)
{
    Node * parent = node->parent;
    Node * grandparent = parent->parent;
    Node * upTree = grandparent->parent;
    if(upTree)
    {
        swapChildToCorrectPos(upTree, grandparent, parent);
    }
    parent->parent = upTree;
    grandparent->parent = parent;
    grandparent->left = parent->right;
    if(grandparent->left)
    {
        grandparent->left->parent = grandparent;
    }
    parent->right = grandparent;
    parent->color = BLACK;
    grandparent->color = RED;
    if(parent->parent == NULL)
    {
        tree->root = parent;
    }
}

void leftRightCase(Node * node, RBTree * tree)
{
    Node * parent = node->parent;
    Node * grandparent = parent->parent;
    parent->right = node->left;
    if(parent->right)
    {
        parent->right->parent = parent;
    }
    grandparent->left = node;
    node->parent = grandparent;
    parent->parent = node;
    node->left = parent;
    leftLeftCase(parent, tree);
}

void rightRightCase(Node * node, RBTree * tree)
{
    Node * parent = node->parent;
    Node * grandparent = parent->parent;
    Node * upTree = grandparent->parent;
    if(upTree)
    {
        swapChildToCorrectPos(upTree, grandparent, parent);
    }
    parent->parent = upTree;
    grandparent->parent = parent;
    grandparent->right = parent->left;
    if(grandparent->right)
    {
        grandparent->right->parent = grandparent;
    }
    parent->left = grandparent;
    parent->color = BLACK;
    grandparent->color = RED;
    if(parent->parent == NULL)
    {
        tree->root = parent;
    }
}

void rightLeftCase(Node * node, RBTree * tree)
{
    Node * parent = node->parent;
    Node * grandparent = parent->parent;
    parent->left = node->right;
    if(parent->left)
    {
        parent->left->parent = parent;
    }
    grandparent->right = node;
    node->parent = grandparent;
    parent->parent = node;
    node->right = parent;
    rightRightCase(parent, tree);
}


/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int addToRBTree(RBTree *tree, void *data)
{
    if(tree == NULL || data == NULL) // checking argument validity
    {
        return INSERT_FAILED;
    }
    if (containsRBTree(tree, data)) // Tree already contains data;
    {
        return INSERT_FAILED;
    }
    Node * z = createNode(data); // Creating the node to be inserted
    if(z == NULL) // checking if memory allocation worked
    {
        return INSERT_FAILED;
    }
    Node * pot = potentialParent(tree, z);
    z->parent = pot;
    if(z->parent == NULL)
    {
        tree->root = z;
    }
    else if(tree->compFunc(z->data, z->parent->data)<0)
    {
        z->parent->left = z;
    }
    else
    {
        z->parent->right = z;
    }
    balanceTree(tree, z);
    ++tree->size;
    return INSERT_SUCCESS;
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
    return node;
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
    int f = func(start->data, args);
    if (!f)
    {
        return 0;
    }
    Node * current = findSuccessor(start);
    while(current != NULL)
    {
        f = func(current->data, args);
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
    if(start == NULL) // checks base case
    {
        return NULL;
    }
    if(start->right != NULL) // if start has a right child
    {
        return minNodeInSubTree(start->right); //returns the minimum of start's right child
    }
    Node * parent = start->parent;
    if(parent != NULL)
    {
        if (parent->left == start) // if start is a left child of it's parent
        {
            return start->parent; // returns the parent
        }
        if (parent->right == start)
        {
            Node *successor = parent;
            while (successor != NULL && successor->right == start)
            {
                start = successor;
                successor = start->parent;
            }
            return successor; // can return a pointer because successor is not a local variable
        }
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

void freeRBTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return;
    }
    freeNodesInDepth(tree, tree->root);
    free(tree);
}

void freeNodesInDepth(RBTree * t, Node * node)
{
    if(node == NULL)
    {
        return;
    }
    freeNodesInDepth(t, node->left);
    freeNodesInDepth(t, node->right);
    if (node->data != NULL)
    {
        t->freeFunc(node->data); // freeing data of a node
        node->data = NULL;
    }
    free(node);
}