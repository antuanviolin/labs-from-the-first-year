#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void find_permutation(char sequence[], int *first_elem_perm, int *second_elem_perm);

void swap_elements(char sequence[], int first_elem_perm, int second_elem_perm);

void reveres_part(char sequence[], int start_part_revers);

int check_input(char sequence[]);

enum {
    PERMUTATION_SIZE = 10
};

int main(void) {
    char sequence[PERMUTATION_SIZE + 1];
    int N;
    int first_elem_perm = -1, second_elem_perm = -1;
    if ((scanf("%10s", sequence) == 1) && (getchar() == '\n') && (scanf("%d", &N) == 1) && check_input(sequence)) {
        for (int i = 0; i < N; ++i) {
            find_permutation(sequence, &first_elem_perm, &second_elem_perm);
            if (first_elem_perm != -1 && second_elem_perm != -1) {
                swap_elements(sequence, first_elem_perm, second_elem_perm);
                reveres_part(sequence, first_elem_perm);
                printf("%s\n", sequence);
            } else {
                break;
            }
        }
    } else {
        printf("bad input");
    }
    return 0;
}

void find_permutation(char *sequence, int *first_elem_perm, int *second_elem_perm) {
    int first_element = -1, second_element = -1;
    for (int i = 1; i < (int) strlen(sequence); ++i) {
        if (sequence[i] > sequence[i - 1]) {
            first_element = i - 1;
        }
    }
    if (first_element != -1) {
        for (int i = 1; i < (int) strlen(sequence); ++i) {
            if (sequence[i] > sequence[first_element]) {
                second_element = i;
            }
        }
    }
    *first_elem_perm = first_element;
    *second_elem_perm = second_element;
}

void swap_elements(char *sequence, int first_elem_perm, int second_elem_perm) {
    char buffer = sequence[first_elem_perm];
    sequence[first_elem_perm] = sequence[second_elem_perm];
    sequence[second_elem_perm] = buffer;
}

void reveres_part(char *sequence, int start_part_revers) {
    char copy_sequence[PERMUTATION_SIZE + 1];
    strcpy(copy_sequence, sequence);
    int len = (int) strlen(copy_sequence) - 1;
    for (int i = len; i > start_part_revers; --i) {
        sequence[i] = copy_sequence[len + start_part_revers + 1 - i];
    }
}

int check_input(char *sequence) {
    int is_good_input = 1;
    int number_of_digits[PERMUTATION_SIZE] = {0};
    for (int i = 0; i < (int) strlen(sequence); ++i) {
        char c = sequence[i];
        if ((c >= '0') && (c <= '9')) {
            number_of_digits[c - '0'] += 1;
        } else {
            is_good_input = 0;
        }
    }
    if (is_good_input == 1) {
        for (int i = 0; i < PERMUTATION_SIZE; ++i) {
            if (number_of_digits[i] > 1) {
                is_good_input = 0;
                break;
            }
        }
    }
    return is_good_input;
}

