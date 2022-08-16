#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_PATTERN_LEN 18
#define MAX_BUFFER_LEN 16
#define MAX_STOP_TABLE_LEN 256

void boyer_moore(char pattern[], FILE *file);

void shifting_str(char str[], int shift_len, int pattern_len);

void finder(char str[], char pattern[], int stop_table[], int pattern_len, int str_len, int *shift);

void filling_table(int stop_table[], char pattern[], int pattern_len);

void read_pattern(char pattern[], FILE *file);

int main() {
    FILE *file;
    char pattern[MAX_PATTERN_LEN];
    if ((file = fopen("in.txt", "r")) == NULL) {
        printf("bad input!");
    }
    read_pattern(pattern, file);
    boyer_moore(pattern, file);
    fclose(file);
    return EXIT_SUCCESS;
}

void boyer_moore(char *pattern, FILE *file) {
    int pattern_len = (int) strlen(pattern);
    char str[MAX_BUFFER_LEN];
    int stop_table[MAX_STOP_TABLE_LEN] = {0};
    filling_table(stop_table, pattern, pattern_len);
    int shift = 0, end_str = 0, str_len = pattern_len;
    while (end_str != 1) {
        shifting_str(str, shift, pattern_len);
        if (fread(&str[shift], sizeof(char), pattern_len - shift, file) != (unsigned long) pattern_len - shift) {
            end_str = 1;
        }
        if (feof(file)) {
            end_str = 1;
        }
        if (end_str != 1) {
            finder(str, pattern, stop_table, pattern_len, str_len, &shift);
        }
        str_len += pattern_len - shift;
    }
}

void shifting_str(char *str, int shift_len, int pattern_len) {
    for (int i = 0; i < shift_len; ++i) {
        str[i] = str[i + pattern_len - shift_len];
    }
}

void finder(char *str, char *pattern, int *stop_table, int pattern_len, int str_len, int *shift) {
    for (int i = pattern_len - 1; i >= 0; --i) {
        printf("%d ", str_len);
        --str_len;
        if (pattern[i] != str[i]) {
            if (i != pattern_len - 1) {
                *shift = stop_table[(unsigned char) pattern[pattern_len - 1]];
            } else {
                *shift = stop_table[(unsigned char) str[i]];
            }
            break;
        }
    }
}

void filling_table(int *stop_table, char *pattern, int pattern_len) {
    for (int i = 0; i < pattern_len - 1; ++i) {
        stop_table[(unsigned char) pattern[i]] = i + 1;
    }
}

void read_pattern(char *pattern, FILE *file) {
    if (fgets(pattern, MAX_PATTERN_LEN, file) == NULL) {
        printf("bad input?");
    }
    int len_with_transfer = (int) strlen(pattern);
    pattern[len_with_transfer - 1] = '\0';
}
