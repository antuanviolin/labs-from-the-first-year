#ifndef LAB8_1_HEAP_H
#define LAB8_1_HEAP_H

#include <stdlib.h>
#include <limits.h>

typedef struct {
    int weight;
    short name;
    short parent;
} heapNode;

void heapSort(heapNode **arr, int len_arr, int *position);

void heapForming(heapNode **arr, int len_arr, int start_element, int *position);

void swapElements(heapNode **arr, int first_elem, int second_elem, int *position);

int comparisonsElements(heapNode *n1, heapNode *n2);

heapNode **createArr(int numVertex);

void deleteHeap(heapNode **arr, int numVertex);

#endif //LAB8_1_HEAP_H
