#include "matrixAdjacency.h"

void addEdge(int **matrix, int n1, int n2, int weight) {
    matrix[n1][n2] = weight;
    matrix[n2][n1] = weight;
}

int **createMatrix(int numVertex) {
    int **newMatrix = malloc(numVertex * sizeof(int *));
    for (int i = 0; i < numVertex; ++i) {
        newMatrix[i] = malloc(numVertex * sizeof(int));
        for (int j = 0; j < numVertex; ++j) {
            newMatrix[i][j] = -1;
        }
    }
    return newMatrix;
}

void deleteMatrix(int **matrix,int numVertex){
    for (int i = 0; i < numVertex; ++i){
        free(matrix[i]);
    }
    free(matrix);
}

