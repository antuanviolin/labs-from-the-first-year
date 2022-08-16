#include <stdlib.h>
#include <stdio.h>

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef struct Tree {
    int data;
    unsigned char height;
    struct Tree *left;
    struct Tree *right;
} Tree;

Tree *getFreeNode(int value, Tree *arrayNode, int index);

Tree *balance(Tree *root);

Tree *insert(Tree *root, Tree *arrayNode, int value, int index);

unsigned char height(Tree *root);

int bfactor(Tree *root);

void fixheight(Tree *root);

Tree *rotateright(Tree *root);

Tree *rotateleft(Tree *root);

int main(void) {
    Tree *arrayNode = NULL;
    Tree *root = NULL;
    FILE *file;
    if ((file = fopen("in.txt", "r")) == NULL) {
        printf("Bad file");
        return EXIT_SUCCESS;
    }
    int data, vertices;
    if (fscanf(file, "%d", &vertices) == 0) {
        printf("Empty file");
        fclose(file);
        free(arrayNode);
        return EXIT_SUCCESS;
    }
    arrayNode = (Tree *) malloc(sizeof(Tree) * vertices);
    if (arrayNode == NULL) {
        printf("Memory error when creating a Tree");
        fclose(file);
        free(arrayNode);
        return EXIT_SUCCESS;
    }
    for (int i = 0; i < vertices; ++i) {
        if (fscanf(file, "%d", &data) == 0) {
            printf("Empty file");
            fclose(file);
            free(arrayNode);
            return EXIT_SUCCESS;
        }
        root = insert(root,arrayNode, data, i);
    }
    fclose(file);
    printf("%d", height(root));
    free(arrayNode);
    return EXIT_SUCCESS;
}

Tree *getFreeNode(int value, Tree *arrayNode, int index) {
    Tree *tmp = &arrayNode[index];
    tmp->left = tmp->right = NULL;
    tmp->data = value;
    tmp->height = 1;
    return tmp;
}

Tree *insert(Tree *root, Tree *arrayNode, int value, int index) {
    if (root == NULL) {
        root = getFreeNode(value, arrayNode, index);
        return root;
    }
    if (value < root->data)
        root->left = insert(root->left, arrayNode, value, index);
    else
        root->right = insert(root->right, arrayNode, value, index);
    return balance(root);
}

unsigned char height(Tree *root) {
    return root ? root->height : 0;
}

int bfactor(Tree *root) {
    return height(root->right) - height(root->left);
}

void fixheight(Tree *root) {
    unsigned char hl = height(root->left);
    unsigned char hr = height(root->right);
    root->height = MAX(hl, hr) + 1;
}

Tree *rotateright(Tree *root) {
    Tree *newroot = root->left;
    root->left = newroot->right;
    newroot->right = root;
    fixheight(root);
    fixheight(newroot);
    return newroot;
}

Tree *rotateleft(Tree *root) {
    Tree *newroot = root->right;
    root->right = newroot->left;
    newroot->left = root;
    fixheight(root);
    fixheight(newroot);
    return newroot;
}

Tree *balance(Tree *root) {
    fixheight(root);
    if (bfactor(root) == 2) {
        if (bfactor(root->right) < 0)
            root->right = rotateright(root->right);
        return rotateleft(root);
    }
    if (bfactor(root) == -2) {
        if (bfactor(root->left) > 0)
            root->left = rotateleft(root->left);
        return rotateright(root);
    }
    return root;
}

