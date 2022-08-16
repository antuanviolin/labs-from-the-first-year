#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int index_point(const char number[]);

int check_input(char number[], int base1, int base2);

int fromc_toi(char c);

double conv_to10(char number[], int base);

void conv_from10(double number, int base);

char fromi_toc(int figure);

int main(void) {
    int base1, base2;
    char number[14];
    if (scanf("%2d %2d", &base1, &base2) == 2 && scanf("%13s", number) == 1) {
        if (!check_input(number, base1, base2)) {
            printf("bad input");
        } else {
            conv_from10(conv_to10(number, base1), base2);
        }
    } else {
        printf("bad input");
    }
    return EXIT_SUCCESS;
}

//Функция, которая определяет индекс точки, если точки нет,то ее индекс = индекс последнего элемента+1
int index_point(const char *number) {
    int i = 0, point = -1, number_point = 0;
    while (number[i] != '\0') {
        if (number[i] == '.') {
            number_point += 1;
            point = i;
        }
        ++i;
    }
    int end_str = 13;
    if (point == 0 || number_point > 1 || ((number_point != 0 && point < end_str && number[point + 1] == '\0'))) {
        point = -1;
    } else if (number_point == 0) {
        point = i;
    }
    return point;
}

//Функция проверяет правильность введенных данных
int check_input(char *number, int base1, int base2) {
    int is_good_input = 1;
    int min_base = 2, max_base = 16;
    if ((base1 < min_base || base1 > max_base) || (base2 < min_base || base2 > max_base)) {
        is_good_input = 0;
    } else if (index_point(number) == -1) {
        is_good_input = 0;
    } else {
        int i = 0;
        while (number[i] != 0) {
            char symbol = number[i];
            int is_base1_digit = fromc_toi(symbol) >= base1;
            int is_hex_digit = ((symbol < '0') || ('9' < symbol && symbol < 'A') || ('F' < symbol && symbol < 'a') ||
                                (symbol > 'f'));
            if ((symbol != '.') && (is_base1_digit || is_hex_digit)) {
                is_good_input = 0;
            }
            ++i;
        }
    }

    return is_good_input;
}

//Функция конвертации символа в число
int fromc_toi(char c) {
    int figure = -1;
    if ((c >= '0') && (c <= '9')) { // Цифры от 0 до 9
        figure = c - '0';
    } else if ((c >= 'a') && (c <= 'f')) { // Цифры от a до f
        figure = c - 'a' + 10;
    } else if ((c >= 'A') && (c <= 'F')) { // Цифры от A до F
        figure = c - 'A' + 10;
    }
    return figure;
}

//Функция конвертации числа в символ(в его assii код)
char fromi_toc(int figure) {
    char c = 0;
    if ((figure >= 0) && (figure <= 9)) {
        c = figure + '0';
    } else if ((figure >= 10) && (c <= 15)) {
        c = (figure - 10) + 'a';
    }
    return c;
}

//Функция, которая конвертирует из заданной системы(base1) в 10-ую.
double conv_to10(char *number, int base) {
    double sum = 0;
    int point = index_point(number), deg = point - 1;
    for (int i = 0; number[i] != '\0'; ++i) {
        if (i != point) {
            int figure = fromc_toi(number[i]);
            sum += figure * pow(base, deg);
            --deg;
        }
    }
    return sum;
}

//Функция, которая конвертирует из 10-ой системы в заданную(base2).
void conv_from10(double number, int base) {
    long int integer_part = (long int) number;
    double fractional_part = number - (float) integer_part;
    if (integer_part == 0) {
        printf("0");
    } else {
        int i = 0;
        char answer[100];
        while (integer_part > 0) {
            answer[i] = fromi_toc(integer_part % base);
            integer_part = integer_part / base;
            ++i;
        }
        for (int j = i - 1; j >= 0; --j) {
            printf("%c", answer[j]);
        }
    }
    if (fractional_part > 0) {
        printf(".");
        int i = 0, max_number_after_point = 12;
        while (i != max_number_after_point && fractional_part != 0.0) {
            double c = fractional_part * (double) base;
            printf("%c", fromi_toc((int) c));
            fractional_part = c - (double) (int) c;
            ++i;
        }
    }
}
