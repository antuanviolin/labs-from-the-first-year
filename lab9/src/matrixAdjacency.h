#ifndef LAB9_MATRIXADJACENCY_H
#define LAB9_MATRIXADJACENCY_H

#include <stdlib.h>

void addEdge(int **matrix, int n1, int n2, int weight);

int **createMatrix(int numVertex);

void deleteMatrix(int **matrix,int numVertex);

#endif //LAB9_MATRIXADJACENCY_H
