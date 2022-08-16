#include <stdlib.h>
#include <stdio.h>

void heap_sort(int arr[], int len_arr);

void heap_forming(int *arr, int len_arr, int start_element);

void swap_elements(int *arr, int first_elem, int second_elem);

int main(void) {
    int N = 0;
    if (scanf("%d", &N) != 1){
        printf("bad input");
    }
    int* arr = (int*)malloc(sizeof(int)*N);
    if (arr == NULL){
        printf("bad memory");
    } else{
        for (int i = 0; i < N; ++i) {
            if(scanf("%d", &arr[i]) != 1){
                printf("bad input");
            }
        }
        heap_sort(arr, N);
        for (int i = 0; i < N; ++i) {
            printf("%d ", arr[i]);
        }
    }
    free(arr);
    return EXIT_SUCCESS;
}

void heap_sort(int *arr, int len_arr) {
    for (int i = len_arr / 2 - 1; i >= 0; --i) {
        heap_forming(arr, len_arr, i);
    }
    for (int i = len_arr - 1; i >= 0; --i) {
        int first_index_arr = 0;
        swap_elements(arr, first_index_arr, i);
        heap_forming(arr, i, first_index_arr);
    }
}

void heap_forming(int *arr, int len_arr, int start_element) {
    int largest = start_element;
    int left_child = start_element * 2 + 1, right_child = start_element * 2 + 2;

    if (left_child < len_arr && arr[left_child] > arr[largest]) {
        largest = left_child;
    }
    if (right_child < len_arr && arr[right_child] > arr[largest]) {
        largest = right_child;
    }
    if (largest != start_element) {
        swap_elements(arr, largest, start_element);
        heap_forming(arr, len_arr, largest);
    }
}

void swap_elements(int *arr, int first_elem, int second_elem) {
    int buffer = arr[first_elem];
    arr[first_elem] = arr[second_elem];
    arr[second_elem] = buffer;
}
