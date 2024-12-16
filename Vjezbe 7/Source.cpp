#define MAX_LINE 1024
#define N 100
#define MALLOC_ERROR -1
#define EXIT_SUCCESS 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct _dir;
typedef struct _dir* positionDir;
typedef struct _dir {
    char name[N];
    positionDir nextDir;    // Sibling directory
    positionDir nextSubDir; // Subdirectory
} dir;

struct _stack;
typedef struct _stack* positionStack;
typedef struct _stack {
    positionDir dir;        // Directory pointer
    positionStack nextStack; // Next in stack
} stack;

void NameDir(positionDir);
int makeDir(positionDir);
positionDir findDir(positionDir, char*);
int changeDirStack(positionStack, positionDir);
void deleteDirStack(positionStack);
void listDir(positionDir);

int main() {
    positionDir headDir = (positionDir)malloc(sizeof(dir));
    if (headDir == NULL) {
        printf("Malloc error!\n");
        return MALLOC_ERROR;
    }
    strcpy(headDir->name, "root");
    headDir->nextDir = NULL;
    headDir->nextSubDir = NULL;

    positionStack headStack = (positionStack)malloc(sizeof(stack));
    if (headStack == NULL) {
        printf("Malloc error!\n");
        return MALLOC_ERROR;
    }
    headStack->dir = NULL;
    headStack->nextStack = NULL;

    positionDir currentDir = headDir; // Start at root directory
    bool loop = true;
    int selector;

    while (loop) {
        printf("\nCurrent Directory: %s\n", currentDir->name);
        printf("Commands:\n");
        printf("1. Create Subdirectory\n");
        printf("2. List Directories\n");
        printf("3. Enter Subdirectory\n");
        printf("4. Go Back to Parent Directory\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &selector);

        switch (selector) {
            case 1: // Create a directory
                makeDir(currentDir);
                break;

            case 2: // List subdirectories
                listDir(currentDir);
                break;

            case 3: { // Enter subdirectory
                char dirName[N];
                printf("Enter the name of the subdirectory: ");
                scanf("%s", dirName);

                positionDir targetDir = findDir(currentDir->nextSubDir, dirName);
                if (targetDir) {
                    changeDirStack(headStack, currentDir); // Push current directory to stack
                    currentDir = targetDir; // Move to subdirectory
                } else {
                    printf("Subdirectory not found!\n");
                }
                break;
            }

            case 4: // Go back to parent directory
                if (headStack->nextStack == NULL) {
                    printf("You are already in the root directory!\n");
                } else {
                    currentDir = headStack->nextStack->dir; // Move to parent directory
                    deleteDirStack(headStack); // Pop from stack
                }
                break;

            case 0: // Exit
                loop = false;
                break;

            default:
                printf("Invalid command!\n");
        }
    }

    return EXIT_SUCCESS;
}

// Function to name a directory
void NameDir(positionDir dir) {
    printf("Enter the name for your new directory: ");
    scanf("%s", dir->name);
}

// Create a subdirectory
int makeDir(positionDir head) {
    positionDir newDir = (positionDir)malloc(sizeof(dir));
    if (newDir == NULL) {
        printf("Malloc error\n");
        return MALLOC_ERROR;
    }

    newDir->nextDir = head->nextSubDir; // Add to subdirectory list
    newDir->nextSubDir = NULL;
    head->nextSubDir = newDir; // Link new subdirectory to parent
    NameDir(newDir);

    return EXIT_SUCCESS;
}

// Find a directory by name (sibling traversal)
positionDir findDir(positionDir headDir, char* dirName) {
    positionDir temp = headDir;

    while (temp != NULL) {
        if (strcmp(temp->name, dirName) == 0) {
            return temp;
        }
        temp = temp->nextDir;
    }

    return NULL; // Not found
}

// Push a directory to the stack
int changeDirStack(positionStack headStack, positionDir newDir) {
    positionStack newStack = (positionStack)malloc(sizeof(stack));
    if (newStack == NULL) {
        printf("Malloc error!\n");
        return MALLOC_ERROR;
    }

    newStack->dir = newDir;
    newStack->nextStack = headStack->nextStack;
    headStack->nextStack = newStack;

    return EXIT_SUCCESS;
}

// Pop a directory from the stack
void deleteDirStack(positionStack headStack) {
    if (headStack->nextStack == NULL) {
        return;
    }

    positionStack temp = headStack->nextStack;
    headStack->nextStack = temp->nextStack;
    free(temp);
}

// List all subdirectories
void listDir(positionDir headDir) {
    positionDir temp = headDir->nextSubDir;

    if (temp == NULL) {
        printf("No subdirectories found.\n");
        return;
    }

    printf("Subdirectories:\n");
    while (temp != NULL) {
        printf("- %s\n", temp->name);
        temp = temp->nextDir;
    }
}
