#include <stdlib.h>
#include <stdio.h>

void calculation(char expression[], int len_str);

int check_str(char expression[], int len_str);

int operation(char oper, int num1, int num2, int *is_div_zero);

void math_operation(int *buffer_numbers, char *buffer_operations, int *index_numbers, int *index_operations,
                    int *is_div_zero);

#define MAX_EXPRESSION_LEN 1000

int main(void) {
    FILE *file;
    char expression[MAX_EXPRESSION_LEN + 1];
    if ((file = fopen("in.txt", "r")) == NULL) {
        printf("bad input!");
        return EXIT_SUCCESS;
    }
    int len_str = fread(&expression[0], sizeof(char), MAX_EXPRESSION_LEN + 1, file);
    if (check_str(expression, len_str - 1) == 0) {
        printf("syntax error");
    } else {
        calculation(expression, len_str - 1);
    }
    fclose(file);
    return EXIT_SUCCESS;
}

int check_str(char *expression, int len_str) {
    int is_good_str = 1, brackets = 0;
    int is_open_bracket = 0, is_close_bracket = 0, is_binary_operation = 0;
    if (len_str < 1) {
        is_good_str = 0;
    }
    for (int i = 0; i < len_str; ++i) {
        char symbol = expression[i];
        if (symbol > '9' ||
            (symbol < '0' && symbol != '(' && symbol != ')' && symbol != '+' && symbol != '-' && symbol != '*' &&
             symbol != '/')) {
            is_good_str = 0;
            break;
        } else if (symbol == '(') {
            if (is_close_bracket == 1) {
                is_good_str = 0;
                break;
            } else {
                is_open_bracket = 1;
                is_binary_operation = 0;
                brackets += 1;
            }
        } else if (symbol == ')') {
            if (is_binary_operation == 1 || is_open_bracket == 1) {
                is_good_str = 0;
                break;
            } else {
                is_close_bracket = 1;
                brackets -= 1;
            }
        } else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/') {
            if (is_binary_operation == 1 || is_open_bracket == 1) {
                is_good_str = 0;
                break;
            } else {
                is_binary_operation = 1;
                is_close_bracket = 0;
            }
        } else {
            is_binary_operation = 0;
            is_open_bracket = 0;
            is_close_bracket = 0;
        }
    }
    if (brackets != 0 || is_binary_operation != 0 || is_open_bracket != 0) {
        is_good_str = 0;
    }
    return is_good_str;
}

void calculation(char *expression, int len_str) {
    int buffer_numbers[MAX_EXPRESSION_LEN];
    char buffer_operations[MAX_EXPRESSION_LEN];
    int index_numbers = 0, index_operations = 0;
    int is_div_zero = 0;
    int i = 0;
    while (i < len_str) {
        char symbol = expression[i];
        if (symbol >= '0' && symbol <= '9') {
            while (symbol >= '0' && symbol <= '9' && i < len_str){
                buffer_numbers[index_numbers] *= 10;
                buffer_numbers[index_numbers] += symbol - '0';
                if (++i < len_str){
                    symbol = expression[i];
                }
            }
            index_numbers += 1;
            buffer_numbers[index_numbers] = 0;
        } if (i < len_str){
            if (symbol == '(' || index_operations == 0) {
                buffer_operations[index_operations] = symbol;
                index_operations += 1;
            } else {
                if (symbol == '+' || symbol == '-') {
                    if (buffer_operations[index_operations - 1] != '(') {
                        math_operation(buffer_numbers, buffer_operations, &index_numbers, &index_operations,
                                       &is_div_zero);
                        if (is_div_zero == 1) {
                            break;
                        }
                    }
                    buffer_operations[index_operations] = symbol;
                    index_operations += 1;
                } else if (symbol == '*' || symbol == '/') {
                    if (buffer_operations[index_operations - 1] == '*' ||
                        buffer_operations[index_operations - 1] == '/') {
                        math_operation(buffer_numbers, buffer_operations, &index_numbers, &index_operations,
                                       &is_div_zero);
                        if (is_div_zero == 1) {
                            break;
                        }
                    }
                    buffer_operations[index_operations] = symbol;
                    index_operations += 1;
                } else if (symbol == ')') {
                    while (buffer_operations[index_operations - 1] != '(') {
                        math_operation(buffer_numbers, buffer_operations, &index_numbers, &index_operations,
                                       &is_div_zero);
                        if (is_div_zero == 1) {
                            break;
                        }
                    }
                    buffer_operations[index_operations] = 0;
                    index_operations -= 1;
                }
            }
        }
        ++i;
    }
    while (index_operations != 0) {
        math_operation(buffer_numbers, buffer_operations, &index_numbers, &index_operations, &is_div_zero);
        if (is_div_zero == 1) {
            break;
        }
    }
    if (is_div_zero == 1) {
        printf("division by zero");
    } else {
        printf("%d", buffer_numbers[0]);
    }
}

int operation(char oper, int num1, int num2, int *is_div_zero) {
    switch (oper) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 != 0) {
                return num1 / num2;
            } else {
                *is_div_zero = 1;
                return 0;
            }
        default:
            return num1;
    }
}

void math_operation(int *buffer_numbers, char *buffer_operations, int *index_numbers, int *index_operations,
                    int *is_div_zero) {
    buffer_numbers[*index_numbers - 2] = operation(buffer_operations[*index_operations - 1],
                                                   buffer_numbers[*index_numbers - 2],
                                                   buffer_numbers[*index_numbers - 1], is_div_zero);
    *index_numbers -= 1;
    buffer_numbers[*index_numbers] = 0;
    buffer_operations[*index_operations] = 0;
    *index_operations -= 1;
}
