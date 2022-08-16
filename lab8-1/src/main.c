#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "heap.h"
#include "listAdjacency.h"

void deleteNode(heapNode **arr, int **matrix, int numVertex, int len_arr, int *position);

int main(void) {
    FILE *file;
    if ((file = fopen("in.txt", "r")) == NULL) {
        printf("Bad input 1\n");
        return EXIT_SUCCESS;
    }
    int n, m;
    if (fscanf(file, "%d", &n) != 1) {
        printf("bad number of lines");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if ((n < 0) || (n > 5000)) {
        fclose(file);
        printf("bad number of vertices");
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
    if (m == 0 && n == 0) {
        printf("no spanning tree");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if (m == 0 && n == 1) {
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
    FILE *outfile;
    if ((outfile = fopen("out.txt", "w")) == NULL) {
        printf("Bad input 1\n");
        fclose(file);
        return EXIT_SUCCESS;
    }
    heapNode **arr = createArr(n);
    int positions[n];
    for (int j = 0; j < n; ++j) {
        positions[j] = j;
    }
    int numEdge = 0;
    for (int j = 0; j < n; ++j) {
        heapSort(arr, n - j, positions);
        heapNode *node = arr[0];
        int *neighbours = matrix[node->name];
        for (int k = 0; k < n; ++k) {
            if (neighbours[k] != -1) {
                int position = positions[k];
                if (arr[position]->weight > neighbours[k] ||
                    ((arr[position]->weight == neighbours[k]) && (arr[position]->parent == -1))) {
                    arr[position]->weight = neighbours[k];
                    arr[position]->parent = node->name;
                }
            }
        }
        if (arr[0]->parent != -1 && arr[0]->parent + 1 != arr[0]->name + 1) {
            fprintf(outfile, "%d %d\n", arr[0]->parent + 1, arr[0]->name + 1);
            ++numEdge;
        }
        deleteNode(arr, matrix, n, n - j, positions);
    }
    fclose(outfile);
    if (n - numEdge != 1) {
        if ((outfile = fopen("out.txt", "w")) == NULL) {
            printf("Bad input 1\n");
            deleteHeap(arr, n);
            deleteMatrix(matrix, n);
            fclose(file);
            return EXIT_SUCCESS;
        }
        fprintf(outfile, "no spanning tree");
        fclose(outfile);
    }
    deleteHeap(arr, n);
    deleteMatrix(matrix, n);
    fclose(file);
    return EXIT_SUCCESS;
}

void deleteNode(heapNode **arr, int **matrix, int numVertex, int len_arr, int *position) {
    int *neighbours = matrix[arr[0]->name];
    for (int i = 0; i < numVertex; ++i) {
        if(neighbours[i] != -1){
            matrix[arr[0]->name][i] = -1;
            matrix[i][arr[0]->name] = -1;
        }
    }
    swapElements(arr, 0, len_arr - 1, position);
}





