#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _element;
typedef struct _element* position;
typedef struct _element {
    float br;
    position next;
} element;

void Push(position Head, float value) {
    position el = (position)malloc(sizeof(element));
    if (el == NULL) {
        printf("Nema dovoljno memorije!\n");
        return;
    }
    el->br = value;
    el->next = Head->next;
    Head->next = el;
}
//Head
float Pop(position Head) {
    if (Head->next == NULL) {
        printf("Stack underflow!\n");
        return 0;
    }
    position temp = Head->next;
    float value = temp->br;
    Head->next = temp->next;
    free(temp);
    return value;
}

void Calc(position Head, char op) {
    float b = Pop(Head);
    float a = Pop(Head);
    float result;

    switch (op) {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case '*':
        result = a * b;
        break;
    case '/':
        if (b != 0)
            result = a / b;
        else {
            printf("Error: Division by zero!\n");
            return;
        }
        break;
    default:
        printf("Invalid operator '%c'.\n", op);
        return;
    }
    Push(Head, result);
}

int Postfix(position Head, char* string) {
    int size = 0;
    float broj;
    char op;

    while (strlen(string) > 0) {
        if (sscanf(string, "%f%n", &broj, &size) == 1) {
            Push(Head, broj);
        }
        else if (sscanf(string, " %c%n", &op, &size) == 1 && (op == '+' || op == '-' || op == '*' || op == '/')) {
            Calc(Head, op);
        }
        else {
            printf("Invalid input or unsupported character.\n");
            return -1;
        }
        string += size;
    }
    return 0;
}

void Ispisi(position p) {
    if (p != NULL) {
        printf("Rezultat je: %f\n", p->br);
    }
    else {
        printf("Stack is empty.\n");
    }
}

int main() {
    char buffer[MAX_LINE];
    FILE* dat = fopen("postfix.txt", "r");
    if (dat == NULL) {
        printf("Nije moguce otvoriti datoteku!\n");
        return -1;
    }
    fgets(buffer, MAX_LINE, dat);
    fclose(dat);

    position head = (position)malloc(sizeof(element));
    if (head == NULL) {
        printf("Nema dovoljno memorije!\n");
        return -1;
    }
    head->br = 0;
    head->next = NULL;

    Postfix(head, buffer);
    Ispisi(head->next);

    
    position temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
