#include "heap.h"

void heapSort(heapNode **arr, int len_arr, int *position) {
    for (int i = len_arr / 2 - 1; i >= 0; --i) {
        heapForming(arr, len_arr, i, position);
    }
    for (int i = len_arr - 1; i >= 0; --i) {
        int first_index_arr = 0;
        swapElements(arr, first_index_arr, i, position);
        heapForming(arr, i, first_index_arr, position);
    }
}

void heapForming(heapNode **arr, int len_arr, int start_element, int *position) {
    int largest = start_element;
    int left_child = start_element * 2 + 1, right_child = start_element * 2 + 2;

    if (left_child < len_arr && comparisonsElements(arr[left_child], arr[largest]) > 0) {
        largest = left_child;
    }
    if (right_child < len_arr && comparisonsElements(arr[right_child], arr[largest]) > 0) {
        largest = right_child;
    }
    if (largest != start_element) {
        swapElements(arr, largest, start_element, position);
        heapForming(arr, len_arr, largest, position);
    }
}

void swapElements(heapNode **arr, int first_elem, int second_elem, int *position) {
    position[arr[first_elem]->name] = second_elem;
    position[arr[second_elem]->name] = first_elem;
    heapNode *buffer = arr[first_elem];
    arr[first_elem] = arr[second_elem];
    arr[second_elem] = buffer;
}

int comparisonsElements(heapNode *n1, heapNode *n2) {
    return n1->weight - n2->weight;
}

heapNode **createArr(int numVertex){
    heapNode **newArr = malloc(numVertex*sizeof(heapNode));
    for(int i = 0; i < numVertex; ++i){
        heapNode *newNode = malloc(sizeof(heapNode));
        newNode->name = i;
        newNode->weight = INT_MAX;
        newNode->parent = -1;
        newArr[i] = newNode;
    }
    return newArr;
}

void deleteHeap(heapNode **arr, int numVertex){
    for (int i = 0; i < numVertex; ++i){
        free(arr[i]);
    }
    free(arr);
}


