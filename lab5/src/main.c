#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Tree {
    unsigned char data;
    char code[256];
    int freq;
    struct Tree *left;
    struct Tree *right;
} Tree;

typedef struct Byte {
    unsigned char data;
    char position;
    FILE *file;
} Byte;

void createTreeLeave(Tree **arrayNodePointer, int index, unsigned char data);

void FreeAllTree(Tree *root);

Tree *createTreeNode(unsigned char data, int freq, Tree *left, Tree *right);

int calcFreq(Tree **arrayPointer, unsigned char data);

void sortArray(Tree **arrayPointer, int len);

Tree *buildTree(Tree **arrayPointer, int len);

void makeEncodingTable(char **encodingTable, Tree *root, char *code, char *height, int level);

void writeTree(Tree *root, Byte *byte);

Byte *createByte(FILE *file);

void putByte(Byte *byte, unsigned char data);

void putBit(Byte *byte, unsigned char data);

void writeByte(Byte *byte);

void coding(FILE *inFile, FILE *outFile);

void decoding(FILE *inFile, FILE *outFile);

unsigned char getBit(Byte *byte);

unsigned char getByte(Byte *byte);

Tree *getTree(Byte *byte);

void readByte(Byte *byte);

void encodingChar(Byte *byte, Tree *root, FILE *outFile);

void freeNodes(Tree *root);


int main(void) {
    FILE *inFile = NULL;
    if ((inFile = fopen("in.txt", "r")) == NULL) {
        printf("Bad file!");
        return EXIT_SUCCESS;
    }
    FILE *outFile = NULL;
    if ((outFile = fopen("out.txt", "w")) == NULL) {
        printf("Bad file");
        fclose(inFile);
        return EXIT_SUCCESS;
    }
    char flag;
    if (!fread(&flag, sizeof(char), 1, inFile)) {
        printf("no flag in file");
    }
    if (flag == 'c') {
        coding(inFile, outFile);
    } else if (flag == 'd') {
        decoding(inFile, outFile);
    } else {
        printf("Bad input in file");
    }
    fclose(inFile);
    fclose(outFile);
    return EXIT_SUCCESS;
}

int calcFreq(Tree **arrayPointer, unsigned char data) {
    int len = 0;
    for (int i = 0; i < 256; ++i) {
        if (arrayPointer[i]->data == data && arrayPointer[i]->freq != 0) {
            arrayPointer[i]->freq += 1;
            break;
        } else if (arrayPointer[i]->freq == 0) {
            createTreeLeave(arrayPointer, i, data);
            len = 1;
            break;
        }
    }
    return len;
}

void createTreeLeave(Tree **arrayNodePointer, int index, unsigned char data) {
    Tree *tmp = arrayNodePointer[index];
    tmp->left = tmp->right = NULL;
    tmp->data = data;
    tmp->freq = 1;
    tmp->code[0] = '\0';
}

Tree *createTreeNode(unsigned char data, int freq, Tree *left, Tree *right) {
    Tree *tmp;
    tmp = (Tree *) calloc(1, sizeof(Tree));
    tmp->data = data;
    tmp->freq = freq;
    tmp->left = left;
    tmp->right = right;
    tmp->code[0] = '\0';
    return tmp;
}

void sortArray(Tree **arrayPointer, int len) {
    for (int i = 0; i < len; ++i) {
        for (int j = len - 1; j > i; --j) {
            if (arrayPointer[j - 1]->freq > arrayPointer[j]->freq) {
                Tree *tmp = arrayPointer[j - 1];
                arrayPointer[j - 1] = arrayPointer[j];
                arrayPointer[j] = tmp;
            }
        }
    }
}

Tree *buildTree(Tree **arrayPointer, int len) {
    Tree *tmp;
    if (len > 1) {
        while (len > 1) {
            tmp = createTreeNode(0, arrayPointer[0]->freq + arrayPointer[1]->freq, arrayPointer[0], arrayPointer[1]);
            for (int i = 2; i < len; ++i) {
                arrayPointer[i - 2] = arrayPointer[i];
            }
            len = len - 2;
            for (int i = 0; i < len; ++i) {
                if (i == 0 && arrayPointer[i]->freq >= tmp->freq) {
                    for (int j = len - 1; j >= 0; --j) {
                        arrayPointer[j + 1] = arrayPointer[j];
                    }
                    arrayPointer[0] = tmp;
                    break;
                } else if (i == len - 1 && arrayPointer[i]->freq <= tmp->freq) {
                    arrayPointer[len] = tmp;
                    break;
                } else if (arrayPointer[i]->freq >= tmp->freq) {
                    for (int j = len - 1; j >= i; --j) {
                        arrayPointer[j + 1] = arrayPointer[j];
                    }
                    arrayPointer[i] = tmp;
                    break;
                }
            }
            ++len;
        }
    } else if (len == 1) {
        tmp = createTreeNode(0, arrayPointer[0]->freq, arrayPointer[0], NULL);
    } else {
        tmp = createTreeNode(0, 0, NULL, NULL);
    }
    return tmp;
}


void makeEncodingTable(char **encodingTable, Tree *root, char *code, char *height, int level) {
    if (root->left == NULL && root->right == NULL) {
        strcpy(encodingTable[root->data], code);
        height[root->data] = level;
    } else {
        code[level] = '0';
        code[level + 1] = '\0';
        makeEncodingTable(encodingTable, root->left, code, height, level + 1);
        code[level] = '1';
        code[level + 1] = '\0';
        makeEncodingTable(encodingTable, root->right, code, height, level + 1);
    }
}


void writeTree(Tree *root, Byte *byte) {
    if (!root->right && !root->left) {
        putBit(byte, 1);
        putByte(byte, root->data);
    } else {
        putBit(byte, 0);
    }
    if (root->left) {
        writeTree(root->left, byte);
    }
    if (root->right) {
        writeTree(root->right, byte);
    }
}

Byte *createByte(FILE *file) {
    Byte *byte = (Byte *) calloc(1, sizeof(Byte));
    byte->position = 0;
    byte->data = 0;
    byte->file = file;
    return byte;
}

void putByte(Byte *byte, unsigned char data) {
    for (int i = 0; i < 8; ++i) {
        putBit(byte, (data >> (7 - i)) & 0x01);
    }
}

void putBit(Byte *byte, unsigned char data) {
    if (data == 1 || data == '1') {
        byte->data = byte->data | (1 << (7 - byte->position));
        byte->position += 1;
    } else if (data == 0 || data == '0') {
        byte->position += 1;
    } else {
        printf("Error putBit");
    }
    if (byte->position == 8) {
        writeByte(byte);
    }
}

void writeByte(Byte *byte) {
    fwrite(&byte->data, sizeof(char), 1, byte->file);
    byte->data = 0;
    byte->position = 0;
}

void freeNodes(Tree *root) {
    if (root->left) {
        freeNodes(root->left);
        freeNodes(root->right);
        free(root);
    }
}

void coding(FILE *inFile, FILE *outFile) {
    Tree *arrayLeave = (Tree *) calloc(256, sizeof(Tree));
    Tree **arrayPointer = (Tree **) calloc(256, sizeof(Tree *));
    for (int i = 0; i < 256; ++i) {
        arrayPointer[i] = &arrayLeave[i];
    }
    unsigned char data;
    int len = 0;
    while (!feof(inFile)) {
        if (fread(&data, sizeof(char), 1, inFile)) {
            len += calcFreq(arrayPointer, data);
        }
    }
    sortArray(arrayPointer, len);
    Tree *root = buildTree(arrayPointer, len);
    char **encodingTable = (char **) calloc(256, sizeof(char *));
    for (int i = 0; i < 256; ++i) {
        encodingTable[i] = (char *) calloc(256, sizeof(char));
    }
    char height[256];
    if (len == 1) {
        encodingTable[root->left->data][0]= '0';
        encodingTable[root->left->data][1]= '\0';
        height[root->left->data] = 1;
    } else {
        char code[256] = {0};
        makeEncodingTable(encodingTable, root, code, height, 0);
    }
    unsigned char numLeave = len - 1;
    fwrite(&numLeave, sizeof(unsigned char), 1, outFile);
    Byte *byte = createByte(outFile);
    writeTree(root, byte);
    if (byte->position != 0) {
        writeByte(byte);
    }
    int fileLen = 0;
    fseek(inFile, 1, SEEK_SET);
    while (!feof(inFile)) {
        if (fread(&data, sizeof(char), 1, inFile)) {
            fileLen += height[data];
        }
    }
    for (int i = 0; i < (8 - (fileLen % 8)); ++i) {
        if (i == (8 - (fileLen % 8)) - 1) {
            putBit(byte, 1);
        } else {
            putBit(byte, 0);
        }
    }
    fseek(inFile, 1, SEEK_SET);
    while (!feof(inFile)) {
        if (fread(&data, sizeof(char), 1, inFile)) {
            for (int i = 0; i < (int) height[data]; ++i) {
                putBit(byte, (unsigned char) encodingTable[data][i]);
            }
        }
    }
    if (len <= 1) {
        free(root);
    } else {
        freeNodes(root);

    }
    free(arrayPointer);
    free(arrayLeave);
    free(byte);
    for (int i = 0; i < 256; ++i) {
        free(encodingTable[i]);
    }
    free(encodingTable);
}

void decoding(FILE *inFile, FILE *outFile) {
    char len;
    if (!fread(&len, sizeof(char), 1, inFile)) {
        printf("Bad file decoding!");
    }
    Byte *byte = createByte(inFile);
    readByte(byte);
    Tree *root = NULL;
    if (len != 0) {
        root = getTree(byte);
    } else {
        unsigned char bit1 = getBit(byte);
        unsigned char bit2 = getBit(byte);
        if (bit1 == 0 && bit2 == 1) {
            root = createTreeNode(0, 0, NULL, NULL);
            root->left = createTreeNode(getByte(byte), 0, NULL, NULL);
        }
    }
    readByte(byte);
    unsigned char bit = 0;
    while (bit != 1) {
        bit = getBit(byte);
    }
    while (!feof(inFile)) {
        encodingChar(byte, root, outFile);
    }
    if(len == 0){
        free(root->left);
        free(root);
    }
    else{
        FreeAllTree(root);
    }
    free(byte);
}

void FreeAllTree(Tree *root) {
    if (root->left) {
        FreeAllTree(root->left);
        FreeAllTree(root->right);
    }
    free(root);
}

void encodingChar(Byte *byte, Tree *root, FILE *outFile) {
    if (!root->left && !root->right) {
        fwrite(&root->data, sizeof(char), 1, outFile);
    } else {
        unsigned char bit;
        bit = getBit(byte);
        if (bit) {
            encodingChar(byte, root->right, outFile);
        } else {
            encodingChar(byte, root->left, outFile);
        }
    }
}


Tree *getTree(Byte *byte) {
    unsigned char bit;
    bit = getBit(byte);
    if (bit) {
        return createTreeNode(getByte(byte), 0, NULL, NULL);

    } else {
        Tree *root = createTreeNode(0, 0, NULL, NULL);
        root->left = getTree(byte);
        root->right = getTree(byte);
        return root;
    }
}


unsigned char getBit(Byte *byte) {
    unsigned char bit = (byte->data >> (7 - byte->position)) & 0x01;
    byte->position += 1;
    if (byte->position == 8) {
        readByte(byte);
    }
    return bit;
}

unsigned char getByte(Byte *byte) {
    unsigned char data = 0;
    for (int i = 0; i < 8; ++i) {
        data = data | getBit(byte) << (7 - i);
    }
    return data;
}

void readByte(Byte *byte) {
    if (fread(&byte->data, sizeof(char), 1, byte->file)) {
        byte->position = 0;
    }
}

