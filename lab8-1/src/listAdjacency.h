#ifndef LAB8_1_LISTADJACENCY_H
#define LAB8_1_LISTADJACENCY_H

#include <stdlib.h>

void addEdge(int **matrix, int n1, int n2, int weight);

int **createMatrix(int numVertex);

void deleteMatrix(int **matrix,int numVertex);

#endif //LAB8_1_LISTADJACENCY_H
