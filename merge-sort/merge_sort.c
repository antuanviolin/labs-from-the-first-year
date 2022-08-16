#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quick_sort.h"

FILE *open_file(const char name[], char type);

void sort_small_piece(FILE *file_in, FILE *file_buffer, int buffer[]);

void merge_sort(FILE *file_in1, FILE *file_in2, FILE *file_out);

void change_name(char ch1, char ch2, char name[]);

void sort(int number_of_files);

#define MAX_BUFFER_LEN 10
#define NAME_LEN 4000

int main() {
    FILE *file_in = open_file("in.txt", 'r');
    int buffer[MAX_BUFFER_LEN];
    int i = 1;
    char name[NAME_LEN];
    while (!feof(file_in)) {
        change_name(1, i, name);
        FILE *file_buffer = open_file(name, 'w');
        sort_small_piece(file_in, file_buffer, buffer);
        fclose(file_buffer);
        ++i;
    }
    fclose(file_in);
    sort(i - 1);
    return EXIT_SUCCESS;
}

void sort(int number_of_files) {
    char name[NAME_LEN];
    int iteration = 2;
    while (number_of_files > 1) {
        for (int j = 1; j <= number_of_files / 2; ++j) {
            //сливаем два отсортированных файла в один отсортированный файл
            change_name(iteration - 1, j, name);
            FILE *file_in1 = open_file(name, 'r');
            change_name(iteration - 1, j + number_of_files / 2, name);
            FILE *file_in2 = open_file(name, 'r');
            change_name(iteration, j, name);
            FILE *file_out = open_file(name, 'w');
            merge_sort(file_in1, file_in2, file_out);
            change_name(iteration - 1, j, name);
            remove(name);
            change_name(iteration - 1, j + number_of_files / 2, name);
            remove(name);
            fclose(file_out);
        }
        if (number_of_files % 2 != 0) {
            char name1[12], name2[12];
            change_name(iteration - 1, number_of_files, name);
            strcpy(name1, name);
            change_name(iteration, number_of_files / 2 + 1, name);
            strcpy(name2, name);
            rename(name1, name2);
        }
        ++iteration;
        number_of_files = number_of_files / 2 + number_of_files % 2;
    }
    change_name(iteration - 1, 1, name);
    rename(name, "out.txt");
}

void change_name(char ch1, char ch2, char name[]) {
    char iteration[NAME_LEN/2];
    char number[NAME_LEN/2];
    sprintf(iteration, "%d", ch1);
    sprintf(number, "%d", ch2);
    snprintf(name, sizeof(char) * NAME_LEN, "%s_%s.txt", iteration, number);
}

void merge_sort(FILE *file_in1, FILE *file_in2, FILE *file_out) {
    int d1 = 0, d2 = 0;
    fscanf(file_in1, "%d", &d1);
    fscanf(file_in2, "%d", &d2);
    while (!feof(file_in1) && !feof(file_in2)) {
        if (d1 < d2) {
            fprintf(file_out, "%d ", d1);
            fscanf(file_in1, "%d", &d1);
        } else {
            fprintf(file_out, "%d ", d2);
            fscanf(file_in2, "%d", &d2);
        }
    }
    while (!feof(file_in1)) {
        fprintf(file_out, "%d ", d1);
        fscanf(file_in1, "%d", &d1);
    }
    while (!feof(file_in2)) {
        fprintf(file_out, "%d ", d2);
        fscanf(file_in2, "%d", &d2);
    }
}

void sort_small_piece(FILE *file_in, FILE *file_buffer, int buffer[]) {
    int i = 0;
    while (i < MAX_BUFFER_LEN && !feof(file_in)) {
        fscanf(file_in, "%d", &buffer[i]);
        i += 1;
    }
    quick_sort(buffer, 0, i - 1);
    for (int j = 0; j < i; ++j) {
        fprintf(file_buffer, "%d ", buffer[j]);
    }
}

FILE *open_file(const char name[], char type) {
    FILE *file;
    if ((file = fopen(name, &type)) == NULL) {
        printf("bad input!");
    } else {
        return file;
    }
}
