#include <stdlib.h>
#include <stdio.h>

#define WHITE 0
#define BLACK 1
#define GREY 2

typedef struct Turn {
    int *data;
    int counter_turn;
} Turn;

char **createMatrix(int size);

char *createColorArray(int size);

void topSort(char **matrix, char *arrayColor, int n);

void topSortIn(char **matrix, char *arrayColor, int n, int i, Turn *turn, char *cycle);

void freeMatrix(char **matrix, int size);

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
    if ((n < 0) || (n > 2000)) {
        fclose(file);
        printf("bad number of vertices");
        return EXIT_SUCCESS;
    }
    if (fscanf(file, "%d", &m) != 1) {
        printf("bad number of lines");
        fclose(file);
        return EXIT_SUCCESS;
    }
    if ((m < 0) || (m > n * n / 2)) {
        printf("bad number of edges");
        fclose(file);
        return EXIT_SUCCESS;
    }
    int buffer[2];
    char **matrix = createMatrix(n);
    while (fscanf(file, "%d%d", &buffer[0], &buffer[1]) == 2) {
        if (buffer[0] > n || buffer[0] <= 0 || buffer[1] > n || buffer[1] <= 0) {
            freeMatrix(matrix, n);
            printf("bad vertex");
            fclose(file);
            return EXIT_SUCCESS;
        }
        --buffer[0];
        --buffer[1];
        matrix[buffer[0]][buffer[1] / 8] = (char) (matrix[buffer[0]][buffer[1] / 8] | (1 << (buffer[1] % 8)));
        --m;
    }
    if (m != 0) {
        freeMatrix(matrix, n);
        fclose(file);
        printf("bad number of lines");
        return EXIT_SUCCESS;
    }
    char *arrayColor = createColorArray(n);
    topSort(matrix, arrayColor, n);
    free(arrayColor);
    freeMatrix(matrix, n);
    fclose(file);
    return EXIT_SUCCESS;
}

char **createMatrix(int size) {
    char **matrix = (char **) calloc(size, sizeof(char *));
    for (int i = 0; i < size; ++i) {
        matrix[i] = (char *) calloc(size / 8 + 1, sizeof(char));
    }
    return matrix;
}

char *createColorArray(int size) {
    char *array = (char *) calloc(size, sizeof(char));
    for (int i = 0; i < size; ++i) {
        array[i] = WHITE;
    }
    return array;
}

void topSort(char **matrix, char *arrayColor, int n) {
    Turn *turn = (Turn *) calloc(1 ,sizeof(Turn));
    turn->counter_turn = 0;
    turn->data = (int *) calloc(n, sizeof(int));
    char isCycle = 0;
    for (int i = 0; i < n; ++i) {
        if (arrayColor[i] == WHITE) {
            topSortIn(matrix, arrayColor, n, i, turn, &isCycle);
        }
    }
    if (!isCycle) {
        for (int i = n - 1; i > 0; --i) {
            printf("%d ", turn->data[i]);
        }
        printf("%d", turn->data[0]);
    }
    free(turn->data);
    free(turn);
}

void topSortIn(char **matrix, char *arrayColor, int n, int i, Turn *turn, char *cycle) {
    if (*cycle)
        return;
    arrayColor[i] = GREY;
    for (int j = 0; j < n; ++j) {
        if ((matrix[i][j / 8] & (1 << (j % 8))) == 0) {
            continue;
        }
        if (arrayColor[j] == GREY) {
            *cycle = 1;
            printf("impossible to sort");
        }
        if (arrayColor[j] == WHITE) {
            topSortIn(matrix, arrayColor, n, j, turn, cycle);
        }
    }
    turn->data[turn->counter_turn++] = i + 1;
    arrayColor[i] = BLACK;
}

void freeMatrix(char **matrix, int size) {
    for (int i = size - 1; i > -1; --i) {
        free(matrix[i]);
    }
    free(matrix);
}
