#include <stdio.h>
#include "RBTree.h"

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

}
