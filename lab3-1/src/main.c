#include <stdlib.h>
#include <stdio.h>

void quick_sort(int arr[], int left_border, int right_border);

void swap_elements(int arr[], int first_elem, int second_elem);

int main(void) {
    int N = 0;
    if (scanf("%d", &N) != 1) {
        printf("bad input");
    }
    int *arr = (int *) malloc(sizeof(int) * N);
    if (arr == NULL) {
        printf("bad memory");
    } else {
        for (int i = 0; i < N; ++i) {
            if (scanf("%d", &arr[i]) != 1) {
                printf("bad input");
            }
        }
    }
    quick_sort(arr,0,N-1);
    for (int i = 0; i < N; ++i) {
        printf("%d ", arr[i]);
    }
    free(arr);
    return EXIT_SUCCESS;
}

void quick_sort(int *arr, int left_border, int right_border) {
    if (left_border < right_border) {
        int left_slider = left_border, right_slider = right_border;
        int middle = arr[(right_border + left_border) / 2];
        do {
            while (arr[left_slider] < middle) {
                ++left_slider;
            }
            while (arr[right_slider] > middle) {
                --right_slider;
            }
            if (left_slider <= right_slider) {
                swap_elements(arr, left_slider, right_slider);
                ++left_slider;
                --right_slider;
            }
        } while (left_slider <= right_slider);
        quick_sort(arr, left_border, right_slider);
        quick_sort(arr, left_slider, right_border);
    }
}

void swap_elements(int *arr, int first_elem, int second_elem) {
    int buffer = arr[first_elem];
    arr[first_elem] = arr[second_elem];
    arr[second_elem] = buffer;
}
