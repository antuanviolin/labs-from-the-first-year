#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void build_decoding_table(unsigned char *decoding_table, const unsigned char *encoding_table);

void to_base64(const unsigned char *data, int data_length, unsigned char *encoding_data,
               const unsigned char *encoding_table);

int from_base64(const unsigned char *data, unsigned char *decoding_data, const unsigned char *decoding_table);

void menu_to_base64(char **argv, const unsigned char *encoding_table);

void menu_from_base64(char **argv, const unsigned char *decoding_table);

FILE *open_file(const char *path, const char *type);

int main(int argc, char **argv) {
    unsigned char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                      'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                      'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                      'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                      'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                      'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                      'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                      '4', '5', '6', '7', '8', '9', '+', '/'};
    unsigned char decoding_table[256];
    build_decoding_table(decoding_table, encoding_table);


    FILE *output_file;
    if ((output_file = fopen("out.txt", "w")) == NULL) {
        printf("Bad file");
        return EXIT_SUCCESS;
    }

    if (argv[1][0] == '-') {
        switch (argv[1][1]) {
            case 'e':
                menu_to_base64(argv, encoding_table);
                break;
            case 'd':
                menu_from_base64(argv, decoding_table);
                break;
            default:
                printf("Bad command.");
        }
    } else {
        printf("Bad command");
    }
    return EXIT_SUCCESS;
}

void build_decoding_table(unsigned char *decoding_table, const unsigned char *encoding_table) {
    for (int i = 0; i < 256; ++i) {
        decoding_table[i] = 255;
    }
    for (int i = 0; i < 64; ++i) {
        decoding_table[encoding_table[i]] = i;
    }
}

void to_base64(const unsigned char *data, int data_length, unsigned char *encoding_data,
               const unsigned char *encoding_table) {
    encoding_data[0] = encoding_table[(data[0] >> 2) & 0x3F];
    encoding_data[1] = encoding_table[((data[0] << 4) & 0x30) | ((data[1] >> 4) & 0xF)];
    if (data_length == 1) {
        encoding_data[2] = '=';
        encoding_data[3] = '=';
        return;
    }
    encoding_data[2] = encoding_table[((data[1] << 2) & 0x3C) | ((data[2] >> 6) & 0x3)];
    if (data_length == 2) {
        encoding_data[3] = '=';
        return;
    }
    encoding_data[3] = encoding_table[data[2] & 0x3F];
}

int from_base64(const unsigned char *data, unsigned char *decoding_data, const unsigned char *decoding_table) {
    decoding_data[0] = ((decoding_table[data[0]] << 2) & 0xFC) | ((decoding_table[data[1]] >> 4) & 0x3);
    if (data[2] == '=') {
        decoding_data[1] = '\0';
        decoding_data[2] = '\0';
        return 1;
    }
    decoding_data[1] = ((decoding_table[data[1]] << 4) & 0xF0) | ((decoding_table[data[2]] >> 2) & 0xF);
    if (data[3] == '=') {
        decoding_data[2] = '\0';
        return 2;
    }
    decoding_data[2] = ((decoding_table[data[2]] << 6) & 0xFC) | (decoding_table[data[3]] & 0x3F);
    return 3;
}

void menu_to_base64(char **argv, const unsigned char *encoding_table) {
    if (!strcmp(argv[2], "-f")) {
        int length = strtol(argv[3], NULL, 10);
        FILE *input_file = open_file(argv[4], "r");
        FILE *output_file = open_file(argv[5], "w");
        unsigned char output_data[4] = "\0\0\0\0";
        int data_length, str_length = 0;
        while (!feof(input_file)) {
            unsigned char input_data[3] = "\0\0\0";
            data_length = (int) fread(input_data, sizeof(char), 3, input_file);
            if (data_length != 0) {
                to_base64(input_data, data_length, output_data, encoding_table);
                for (int i = 0; i < 4; ++i) {
                    fwrite(&output_data[i], sizeof(char), 1, output_file);
                    ++str_length;
                    if (str_length == length) {
                        fwrite("\r\n", sizeof(char), 1, output_file);
                        str_length = 0;
                    }
                }
            }
        }
    } else {
        FILE *input_file = open_file(argv[2], "r");
        FILE *output_file = open_file(argv[3], "w");
        unsigned char output_data[4] = "\0\0\0\0";
        int data_length;
        while (!feof(input_file)) {
            unsigned char input_data[3] = "\0\0\0";
            data_length = (int) fread(input_data, sizeof(char), 3, input_file);
            if (data_length != 0) {
                to_base64(input_data, data_length, output_data, encoding_table);
                fwrite(output_data, sizeof(char), 4, output_file);
            }
        }
    }
}

void menu_from_base64(char **argv, const unsigned char *decoding_table) {
    if (!strcmp(argv[2], "-i")) {
        FILE *input_file = open_file(argv[3], "r");
        FILE *output_file = open_file(argv[4], "w");
        unsigned char output_data[3] = "\0\0\0";
        int data_length;
        while (!feof(input_file)) {
            unsigned char input_data[4] = "\0\0\0\0";
            data_length = 0;
            for (int i = 0; i < 4; ++i) {
                data_length += (int) fread(&input_data[i], sizeof(char), 1, input_file);
                if (decoding_table[input_data[i]] == 255 && !feof(input_file)) {
                    i -= 1;
                }
            }
            if (data_length != 0) {
                int len_output = from_base64(input_data, output_data, decoding_table);
                for(int i = 0; i < len_output; ++i){
                    fwrite(&output_data[i], sizeof(char), 1, output_file);
                }
            }
        }
    } else {
        FILE *input_file = open_file(argv[2], "r");
        FILE *output_file = open_file(argv[3], "w");
        unsigned char output_data[3] = "\0\0\0";
        int data_length, index_data = 0, bad_input_flag = 0;
        while (!feof(input_file) && bad_input_flag == 0) {
            unsigned char input_data[4] = "\0\0\0\0";
            data_length = 0;
            for (int i = 0; i < 4; ++i) {
                data_length += (int) fread(&input_data[i], sizeof(char), 1, input_file);
                if (data_length == i + 1) {
                    index_data += 1;
                    if (decoding_table[input_data[i]] == 255 && input_data[i] != '=') {
                        printf("Bad input. Position %d.\n", index_data);
                        bad_input_flag = 1;
                    }
                }
                if (bad_input_flag == 1) {
                    break;
                }
            }
            if (data_length != 0 && bad_input_flag == 0) {
                int len_output = from_base64(input_data, output_data, decoding_table);
                for(int i = 0; i < len_output; ++i){
                    fwrite(&output_data[i], sizeof(char), 1, output_file);
                }
            }
        }
    }
}

FILE *open_file(const char *path, const char *type) {
    FILE *file;
    if ((file = fopen(path, type)) == NULL) {
        if (!strcmp(type, "r")) {
            printf("Bad input file");
        } else {
            printf("Bad output file");
        }
        return EXIT_SUCCESS;
    }
    return file;
}
