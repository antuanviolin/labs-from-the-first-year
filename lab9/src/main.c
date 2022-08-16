#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "heap.h"
#include "matrixAdjacency.h"

void deleteNode(heapNode **arr, int **matrix, int numVertex, int len_arr, int *position);

int main(void) {
    FILE *file;
    if ((file = fopen("in.txt", "r")) == NULL) {
        printf("Bad input 1\n");
        return EXIT_SUCCESS;
    }
    int n, m, s, f;
    if (fscanf(file, "%d", &n) != 1) {
        printf("bad number of lines");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if ((n < 0) || (n > 5000)) {
        printf("bad number of vertices");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if (fscanf(file, "%d", &s) != 1) {
        printf("bad number of lines");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if ((s < 1) || (s > n)) {
        printf("bad vertex");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if (fscanf(file, "%d", &f) != 1) {
        printf("bad number of lines");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if ((f < 1) || (f > n)) {
        printf("bad vertex");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if (fscanf(file, "%d", &m) != 1) {
        printf("bad number of lines");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if ((m < 0) || (m > n * (n - 1) / 2)) {
        printf("bad number of edges");
        fclose(file);
        return EXIT_SUCCESS;
    }
    int **matrix = createMatrix(n);
    int n1, n2;
    long weight;
    int i = 0;
    while (fscanf(file, "%d %d %ld", &n1, &n2, &weight) == 3) {
        if (n1 < 1 || n1 > n || n2 < 1 || n2 > n) {
            printf("bad vertex");
            deleteMatrix(matrix, n);
            fclose(file);
            return EXIT_SUCCESS;
        }
        if (weight < 0 || weight > INT_MAX) {
            printf("bad length");
            deleteMatrix(matrix, n);
            fclose(file);
            return EXIT_SUCCESS;
        }
        addEdge(matrix, n1 - 1, n2 - 1, weight);
        ++i;
    }
    if (i != m) {
        printf("bad number of lines");
        deleteMatrix(matrix, n);
        fclose(file);
        return EXIT_SUCCESS;
    }
    heapNode **arr = createArr(n);
    int positions[n];
    for (int j = 0; j < n; ++j) {
        positions[j] = j;
    }
    arr[positions[s - 1]]->weight = 0;
    arr[positions[s - 1]]->parent = s - 1;
    int isOverflow = 0;
    for (int j = 0; j < n; ++j) {
        heapSort(arr, n - j, positions);
        heapNode *node = arr[0];
        if (node->parent == -1) break;
        int *neighbours = matrix[node->name];
        int positionNode = positions[node->name];
        for (int k = 0; k < n; ++k) {
            if (neighbours[k] != -1) {
                int position = positions[k];
                long newWeight = 0;
                if (arr[positionNode]->weight == -1) {
                    newWeight = (long) INT_MAX + 1;
                } else {
                    newWeight = (long)neighbours[k] + (long)arr[positionNode]->weight;
                }
                if (newWeight > INT_MAX){
                    isOverflow += 1;
                }
                if (arr[position]->weight > newWeight ||
                    ((arr[position]->weight == neighbours[k]) && (arr[position]->parent == -1))) {
                    if (newWeight > INT_MAX) {
                        arr[position]->weight = -1;
                    } else {
                        arr[position]->weight = newWeight;
                    }
                    arr[position]->parent = node->name;
                }
            }
        }
        deleteNode(arr, matrix, n, n - j, positions);
    }
    for (int j = 0; j < n; ++j) {
        heapNode *node = arr[positions[j]];
        if (node->weight == -1) {
            printf("INT_MAX+");
        } else if (node->parent == -1) {
            printf("oo");
        } else {
            printf("%d", node->weight);
        }
        printf(" ");
    }
    printf("\n");
    if (arr[positions[f - 1]]->parent == -1) {
        printf("no path");
    } else if (isOverflow >= 2 && arr[positions[f - 1]]->weight == -1) {
        printf("overflow");
    } else {
        heapNode *node = arr[positions[f - 1]];
        while (node->name != s - 1) {
            printf("%d ", node->name + 1);
            node = arr[positions[node->parent]];
        }
        printf("%d", node->name + 1);
    }
    deleteHeap(arr, n);
    deleteMatrix(matrix, n);
    fclose(file);
    return EXIT_SUCCESS;
}

void deleteNode(heapNode **arr, int **matrix, int numVertex, int len_arr, int *position) {
    int *neighbours = matrix[arr[0]->name];
    for (int i = 0; i < numVertex; ++i) {
        if (neighbours[i] != -1) {
            matrix[arr[0]->name][i] = -1;
            matrix[i][arr[0]->name] = -1;
        }
    }
    swapElements(arr, 0, len_arr - 1, position);
}


