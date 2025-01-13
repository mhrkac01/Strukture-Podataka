#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define SCANF_ERROR -2

typedef struct _tree {
    int value;
    struct _tree* left;
    struct _tree* right;
} Tree;

Tree* createNode(int);
int add(Tree *, int);
Tree* search(Tree *, int);
Tree* deleteNode(Tree *, int);
Tree* findMinimum(Tree *);
void inorderPrint(Tree *);
void postorderPrint(Tree *);
void preorderPrint(Tree *);
void levelorderPrint(Tree *, int);
void freeTree(Tree *);

int main() {
    int numbers[] = {8, 3, 12, 5, 7, 15, 6, 4, 9, 13, 10};

    Tree* root = createNode(numbers[0]);
    if (root == NULL) return MALLOC_ERROR;
    
    for (int i = 1; i < (int)(sizeof(numbers) / sizeof(int)); i++) {
        if (add(root, numbers[i])) return MALLOC_ERROR;
    }
    
    inorderPrint(root);
    printf("\n");
    preorderPrint(root);
    printf("\n");
    postorderPrint(root);
    printf("\n");
    levelorderPrint(root, 0);
    printf("\n");
    
    Tree* result = search(root, 4);
    if (result) printf("Found value: %d\n", result->value);
    else printf("Value not found.\n");

    printf("\nDeleting value 12:\n");
    root = deleteNode(root, 12);
    levelorderPrint(root, 0);
    
    freeTree(root);
    
    return EXIT_SUCCESS;
}

Tree* createNode(int value) {
    Tree* newTree = NULL;
    newTree = (Tree*)malloc(sizeof(Tree));

    if (newTree == NULL) {
        printf("Memory allocation error!\n");
        return NULL;
    }

    newTree->value = value;
    newTree->left = NULL;
    newTree->right = NULL;
    
    return newTree;
}

int add(Tree* node, int value) {
    if (node == NULL) return MALLOC_ERROR;

    if (node->value > value) {
        if (node->left == NULL) {
            node->left = createNode(value);
            if (node->left == NULL) return MALLOC_ERROR;
        } else return add(node->left, value);
    } else if (node->value < value) {
        if (node->right == NULL) {
            node->right = createNode(value);
            if (node->right == NULL) return MALLOC_ERROR;
        } else return add(node->right, value);
    }
    
    return EXIT_SUCCESS;
}

Tree* search(Tree* node, int targetValue) {
    if (node == NULL || node->value == targetValue) return node;
    if (node->value > targetValue) return search(node->left, targetValue);
    else return search(node->right, targetValue);
}

Tree* deleteNode(Tree* node, int targetValue) {
    if (node == NULL) return NULL;

    if (node->value > targetValue) node->left = deleteNode(node->left, targetValue);
    else if (node->value < targetValue) node->right = deleteNode(node->right, targetValue);
    else {
        if (node->left == NULL) {
            Tree* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            Tree* temp = node->left;
            free(node);
            return temp;
        }
        
        Tree* temp = findMinimum(node->right);
        node->value = temp->value;
        node->right = deleteNode(node->right, temp->value);
    }
    
    return node;
}

Tree* findMinimum(Tree* node) {
    while (node->left != NULL) node = node->left;
    return node;
}

void inorderPrint(Tree* node) {
    if (node->left != NULL) inorderPrint(node->left);
    printf("%d ", node->value);
    if (node->right != NULL) inorderPrint(node->right);
}

void postorderPrint(Tree* node) {
    if (node->left != NULL) postorderPrint(node->left);
    if (node->right != NULL) postorderPrint(node->right);
    printf("%d ", node->value);
}

void preorderPrint(Tree* node) {
    printf("%d ", node->value);
    if (node->left != NULL) preorderPrint(node->left);
    if (node->right != NULL) preorderPrint(node->right);
}

void levelorderPrint(Tree* node, int currentLevel) {
    static int targetLevel = 0, foundElements = 0;
    
    if (targetLevel == currentLevel) {
        printf("%d ", node->value);
        foundElements = 1;
    } else {
        if (node->left != NULL) levelorderPrint(node->left, currentLevel + 1);
        if (node->right != NULL) levelorderPrint(node->right, currentLevel + 1);
    }
    
    if (!currentLevel && foundElements) {
        targetLevel++;
        foundElements = 0;
        levelorderPrint(node, currentLevel);
    }
    
    if (!currentLevel && !foundElements) {
        targetLevel = 0;
        foundElements = 0;
    }
}

void freeTree(Tree* node) {
    if (node != NULL) {
        freeTree(node->left);
        freeTree(node->right);
        free(node);
    }
}
