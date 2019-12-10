#include <stdio.h>
#include "Structs.h"
#include "RBTree.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define EQ 0
#define GRT 1 // a > b
#define SML -1 // a < b
#define ZERO 0

int compare(const double * v1, const double * v2, int len, int longer);
double normCaLc(Vector * v);


/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    return strcmp((char*) a, (char *) b);
}


/**
 * ForEach function that concatenates the given word to pConcatenated. pConcatenated is already allocated with
 * enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    char * ptr = strcat((char*) pConcatenated, (char*) word);

    if(ptr == NULL)
    {
        return 0; //TODO CHANGE TO DEFINE
    }
    char* ptr2 = strcat(ptr, "\n");
    if(ptr2 == NULL)
    {
        return 0;
    }
    return 1; //TODO CHANGE TO DEFINE
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    if(s != NULL) {
        char * str = (char *) s;
        free(str);
    }
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector * v1 = (Vector *) a;
    Vector * v2 = (Vector *) b;
    double * v1v = v1->vector;
    double * v2v = v2->vector;
    if(v1->len < v2->len)
    {
        return compare(v1v, v2v, v1->len, SML);
    }
    else if(v1->len > v2->len)
    {
        return compare(v1v, v2v, v2->len, GRT);
    }
    else
    {
        return compare(v1v, v2v, v2->len, EQ);
    }
}


int compare(const double * v1, const double * v2, int len, int longer)
{
    for(int i = 0; i < len; ++i)
    {
        if(v1[i] == v2[i])
        {
            continue;
        }
        else if(v1[i] > v2[i])
        {
            return GRT;
        }
        else {
            return SML;
        }
    }
    return longer; // if they are synced in all coordinates, return the longer value;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    if(pVector != NULL)
    {
        Vector * v = (Vector *) pVector;
        if(v->vector != NULL)
        {
            free(v->vector);
            v->vector = NULL;
        }
        free(v);
    }
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if(pVector == NULL || pMaxVector == NULL)
    {
        return 0; // todo definition fail
    }
    Vector * v = (Vector *) pVector;
    Vector * max = (Vector *) pMaxVector;
    double * pt;
    if(max->vector == NULL)
    {
        max->vector = (double *) malloc(sizeof(double)*(v->len));
        if(max->vector == NULL)
        {
            return 0; //TODO DEFINE
        }
        pt = memcpy(max->vector, v->vector, sizeof(double)*(v->len));
        if(pt == NULL)
        {
            return 0; //TODO DEFINE
        }
        max->len = v->len;
        return 1; // todo definition success
    }
    if(normCaLc(max) < normCaLc(v))
    {
        max->vector = (double *) realloc(max->vector, sizeof(double)*(v->len));
        if(max->vector == NULL)
        {
            return 0; //TODO DEFINE
        }
        max->len = v->len;
        pt = memcpy(max->vector, v->vector, sizeof(double)*(v->len));
        if(pt == NULL)
        {
            return 0; //TODO DEFINE
        }
        return 1; // todo definition success
    }
    return 1;
}

double normCaLc(Vector * v)
{
    double norm = 0;
    for (int i = 0; i < v->len; ++i)
    {
        norm += (double) (v->vector[i]*v->vector[i]);
    }
    norm = sqrt(norm);
    return norm;
}


/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    Vector * v = (Vector *) malloc(sizeof(Vector));
    if(v == NULL)
    {
        return NULL;
    }
    v->vector = NULL;
    v->len = ZERO;
    int a = forEachRBTree(tree, copyIfNormIsLarger, v);
    if(a)
    {
        return v;
    }
    freeVector(v);
    v = NULL;
    return v;

}