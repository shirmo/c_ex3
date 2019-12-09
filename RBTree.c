#include <stdio.h>
#include "RBTree.h"
#include <stdlib.h>


#define NO_ROOT NULL
#define EMPTY_TREE 0
#define EQUALS 0
#define INSERT_SUCCESS 1
#define INSERT_FAILED 0
#define FIRST_STATE 1
#define SECOND_STATE 2
#define THIRD_STATE 3
#define FOURTH_STATE 4


Node * findSuccessor(Node * start);
Node * minNodeInSubTree(Node * head);
Node * createNode(void * data);
Node * potentialParent(RBTree *tree, Node * node);
int checkRotationState(RBTree * tree, Node * node);
void leftRotation(RBTree * tree, Node* z);
void rightRotation(RBTree * tree, Node * x);

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
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
//int addToRBTree(RBTree *tree, void *data)
//{
//    if(tree == NULL || data == NULL)
//    {
//        return INSERT_FAILED;
//    }
//    if (containsRBTree(tree, data)) // Tree already contains data;
//    {
//        return INSERT_FAILED;
//    }
//    Node * newNode = createNode(data);
//    if(newNode == NULL)
//    {
//        return INSERT_FAILED;
//    }
//    if(tree->root == NULL) // sets a root if one wasn't exists before
//    {
//        tree->root = newNode;
//        newNode->color = BLACK;
//        tree->size += 1;
//        return INSERT_SUCCESS;
//    }
//    // set new node's parent
//    Node* parent = potentialParent(tree, newNode);
//    newNode->parent = parent;
//    if(tree->compFunc(parent->data, newNode -> data))
//    {
//        parent->left = newNode;
//    }
//    else
//    {
//        parent->right = newNode;
//    }
//    if(parent->color == BLACK)  //no need to balance tree
//    {
//        return INSERT_SUCCESS;
//    }
//    int rotation = checkRotationState(tree, newNode);
//}
//
///**
// *
// * @param tree RBTee
// * @param node the node we inserted
// * @return the type of rotation we need to make
// */
//int checkRotationState(RBTree * tree, Node * node)
//{
//    if(tree == NULL || node == NULL)
//    {
//        return 0;
//    }
//    if(tree->root == node)
//    {
//        node -> color = BLACK;
//        return FIRST_STATE;
//    }
//    Node * parent = node->parent;
//    if (parent->color == BLACK)
//    {
//        return SECOND_STATE;
//    }
//    Node * grandpa = parent->parent;
//    Node * uncle = (grandpa->left == parent)?grandpa->right:grandpa->left; //set uncle
//    if(uncle->color == RED && parent->color == RED)
//    {
//        return THIRD_STATE;
//    }
//    if(uncle->color == BLACK && parent->color == RED)
//    {
//        return FOURTH_STATE;
//    }
//    return 0;
//}


void balanceTree(RBTree * tree, Node * z)
{
    if(tree == NULL || z == NULL)
    {
        return;
    }
    if(tree->root == z)
    {
        z->color = BLACK;
        return;
    }
    while (z->parent->color == RED)
    {
        Node * G = z->parent->parent;
        if(G == NULL)
        {// Just in case, but won't get here if z is a child of a root because a root is black! so must have grandparent
            return;
        }
        if (z->parent == G->left)
        {
            int yColor = RED;
            Node * y = G->right;
            if (y == NULL)
            {
                yColor = BLACK;
            }
            if(yColor == RED)
            {
               z->parent->color = BLACK;
                y->color = BLACK;
               G->color = RED;
               z = G;
            }
            else if(z == z->parent->right)
            {
                z = z->parent;
                leftRotation(tree, z);
            }
            z->parent->color = BLACK;
            G = z->parent->parent;
            if(G != NULL)
            {
                G->color = RED;
            }
            rightRotation(tree, G);
        }
        else
        {
            int yColor = RED;
            Node * y = G->left;
            if (y == NULL)
            {
                yColor = BLACK;
            }
            if(yColor == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                G->color = RED;
                z = G;
            }
            else if(z == z->parent->left)
            {
                z = z->parent;
                rightRotation(tree, z);
            }
            z->parent->color = BLACK;
            G = z->parent->parent;
            if(G != NULL)
            {
                G->color = RED;
            }
            leftRotation(tree, G);
        }
    }
    tree->root->color = BLACK;

}

/**
 * Handles Third type rotation (parent and uncle are red)
 * @param tree RBTee
 * @param node the node we inserted
 * @return void
 */
void handleThirdRotation(RBTree * tree, Node * node)
{
    Node * parent = node->parent;
    Node * grandpa = parent->parent;
    Node * uncle = (grandpa->left == parent)?grandpa->right:grandpa->left; //set uncle
    uncle -> color = parent -> color = BLACK; // Set parent and uncle's color to Black
    grandpa -> color = RED; // set grandpa's color to RED
    balanceTree(tree, node);
}

/**
 * Handles Forth type rotation (parent and uncle are red)
 * @param tree RBTee
 * @param node the node we inserted
 * @return void
 */
//void handleForthRotation(Node * node)
//{
//    Node * parent = node->parent;
//    Node * grandpa = parent->parent;
//    // checks whether we need to make two rotations or only one
//    int rightLeft = (parent->right == node && grandpa->left == parent)?1:0;
//    int leftRight = (parent->left == node && grandpa->right == parent)?1:0;
//    if(rightLeft)
//    {
//        leftRotation(grandpa, parent, node);
//        parent = node; // set new parent
//    }
//    if(leftRight)
//    {
//        rightRotation(grandpa, parent, node);
//        parent = node; // set new parent
//    }
//    int rightRight = (parent->right == node && grandpa->right == parent)?1:0;
//    int leftLeft = (parent->left == node && grandpa->left == parent)?1:0;
//    if(rightRight)
//    {
//        leftRotation(grandpa->parent, grandpa, parent);
//    }
//    if(leftLeft)
//    {
//        rightRotation(grandpa->parent, grandpa, node);
//    }
//    parent->color = BLACK; // Set nodes to correct color according to algorithm
//    grandpa->color = RED;
//}

void leftRotation(RBTree * tree, Node* x)
{
    if (tree == NULL || x == NULL)
    {
        printf("problem with input to function leftRotation");
        return;
    }
    Node * y = x->right; // set y
    if( y == NULL)
    {
        printf("problem with input to function leftRotation - X HAS TO HAVE A RIGHT CHILD");
        return;
    }
    x->right = y->left;
    if(y->left != NULL)
    {
        y->left->parent = x;
    }
    y->parent = x->parent; // set's x's parent to be y's parent in order to make the shift
    if(x->parent == NULL)
    {
        tree->root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    y->left = x; // put x as a left child of y (now x is a child of y after y was x's)
    x->parent = y;
}

void rightRotation(RBTree * tree, Node * x)
{
        if (tree == NULL || x == NULL)
        {
            printf("problem with input to function rightRotation");
            return;
        }
        Node * y = x->left; // set y
        if( y == NULL)
        {
            printf("problem with input to function rightRotation - X HAS TO HAVE A LEFT CHILD");
            return;
        }
        x->left = y->right;
        if(y->right != NULL)
        {
            y->right->parent = x;
        }
        y->parent = x->parent; // set's x's parent to be y's parent in order to make the shift
        if(x->parent == NULL)
        {
            tree->root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
        y->right = x; // put x as a right child of y (now x is a child of y after y was x's)
        x->parent = y;
    }


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
    Node * y = NULL;
    Node * x = tree->root;
    while(x != NULL)
    {
        y = x;
        if (data < x->data)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    z->parent = y;
    if(y == NULL)
    {
        tree->root = z;
    }
    else if (z->data < y->data)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    balanceTree(tree, z);
}



/**
 * finds the potential parent for a node we insert to the given tree
 * @param tree
 * @param node
 * @return the father to be set in the tree
 */
//Node * potentialParent(RBTree *tree, Node * node)
//{
//    Node * current = tree -> root;
//    Node * parent = NULL;
//    int compare = 0;
//    while (current != NULL)
//    {
//        parent = current;
//        //value from compare func
//        compare =  tree->compFunc(current->data, node -> data);
//
//        // checks if current's data is grater/lower than node's data
//        current = (compare > EQUALS)? current->left:current->right;
//    }
//    return parent;
//}

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