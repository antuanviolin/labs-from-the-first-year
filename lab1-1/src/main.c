#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void read_pattern(char pattern[], FILE *file);

void rabin_carp(char pattern[], FILE *file);

void checking_characters(char str[], char pattern[], int str_len, int pattern_len, int i);

int func_hash_pattern(char pattern[], int pattern_len);

void func_hash_str(char str[], int pattern_len, int i, int degree, int *str_hash, int *is_first);

#define MAX_PATTERN_LEN 18
#define MAX_BUFFER_LEN 512

int main(void) {
    FILE *file;
    char pattern[MAX_PATTERN_LEN];
    if ((file = fopen("in.txt", "r")) == NULL) {
        printf("bad input!");
        return EXIT_SUCCESS;
    }
    read_pattern(pattern, file);
    rabin_carp(pattern, file);
    fclose(file);
    return EXIT_SUCCESS;
}

void rabin_carp(char *pattern, FILE *file) {
    int pattern_len = (int) strlen(pattern);
    char str[MAX_BUFFER_LEN];
    int end_str = 0, str_len = pattern_len;
    int pattern_hash = func_hash_pattern(pattern, pattern_len), str_hash = 0;
    printf("%d ", pattern_hash);
    int degree = (int) (pow(3, pattern_len));
    while (end_str != 1) {
        int len_read = fread(&str[0], sizeof(char), MAX_BUFFER_LEN, file);
        int i = 0, is_first = 1;
        while (i <= len_read-pattern_len) {
            func_hash_str(str, pattern_len, i, degree, &str_hash, &is_first);
            if (str_hash == pattern_hash) {
                checking_characters(str, pattern, str_len, pattern_len, i);
            }
            str_len += 1;
            i += 1;
        }
        if (feof(file)) {
            end_str = 1;
        }
        fseek(file, -pattern_len + 1, SEEK_CUR);
    }
}

void checking_characters(char *str, char *pattern, int str_len, int pattern_len, int i) {
    for (int j = 0; j < pattern_len; ++j) {
        printf("%d ", str_len - pattern_len + j + 1);
        if (pattern[j] != str[j + i]) {
            break;
        }
    }
}

void read_pattern(char *pattern, FILE *file) {
    if (fgets(pattern, MAX_PATTERN_LEN, file) == NULL) {
        printf("bad input");
    }
    int len_with_transfer = (int) strlen(pattern);
    pattern[len_with_transfer - 1] = '\0';
}

int func_hash_pattern(char *pattern, int pattern_len) {
    int hash = 0;
    for (int i = 0; i < pattern_len; ++i) {
        hash += ((unsigned char) pattern[i] % 3) * (int) (pow(3, i));
    }
    return hash;
}

void func_hash_str(char *str, int pattern_len, int i, int degree, int *str_hash, int *is_first) {
    if (*is_first == 1) {
        for (int j = 0; j < pattern_len; ++j) {
            *str_hash += ((unsigned char) str[j] % 3) * (int) (pow(3, j));
        }
        *is_first = 0;
    } else {
        *str_hash -= (unsigned char) str[i - 1] % 3;
        *str_hash += ((unsigned char) str[i + pattern_len - 1] % 3) * degree;
        *str_hash /= 3;
    }
}
