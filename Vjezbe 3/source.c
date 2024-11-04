#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _lista;
typedef struct _lista* position;
typedef struct _lista {
    int god;
    char ime[100];
    char prezime[100];
    position next;
} lista;

//Memorijska lokacija strukture
void Unesi(position p) {
    printf("Unesite godine, ime i prezime:\n");
    scanf("%d", &p->god);
    scanf("%s", p->ime);
    scanf("%s", p->prezime);
}

//Adresa head elementa
position PronadiPosljednji(position p) {
    while (p->next != NULL)
        p = p->next;
    return p;
}

// Adresa head elementa
int UmetniNaPocetak(position p) {
    position q = (position)malloc(sizeof(lista));
    if (q == NULL)
        return -1;
    Unesi(q);
    q->next = p->next;
    p->next = q;
    return 0;
}

// Function to insert an element at the end
void UmetniNaKraj(position p, int god, char* ime, char* prezime) {
    position last = PronadiPosljednji(p);
    position q = (position)malloc(sizeof(lista));
    if (q == NULL) {
        printf("Memory allocation error.\n");
        return;
    }
    q->god = god;
    strcpy(q->ime, ime);
    strcpy(q->prezime, prezime);
    q->next = NULL;
    last->next = q;
}

// Funkcija prima head->next
int Ispisi(position p) {
    if (p == NULL) {
        printf("Nema elemenata, lista je prazna.\n");
        return -1;
    }
    else {
        while (p != NULL) {
            printf("%s %s %d\n", p->ime, p->prezime, p->god);
            p = p->next;
        }
        return 0;
    }
}

// Head->next i ime liste
void IspisiUDatoteku(position p, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error, nije moguce otvoriti datoteku.\n");
        return;
    }

    while (p != NULL) {
        fprintf(file, "%s %s %d\n", p->ime, p->prezime, p->god);
        p = p->next;
    }

    fclose(file);
    printf("Lista upisana u datoteku.\n");
}

// Head i ime liste
void UcitajIzDatoteke(position p, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error, nije moguce otvoriti datoteku.\n");
        return;
    }

    int god;
    char ime[100];
    char prezime[100];

    while (fscanf(file, "%s %s %d", ime, prezime, &god) == 3) {
        UmetniNaKraj(p, god, ime, prezime);
    }

    fclose(file);
    printf("Lista ucitana iz datoteke.\n");
}

int main() {
    position head = NULL;
    head = (position)malloc(sizeof(lista));
    if (head == NULL)
        return -1;
    head->next = NULL;

    UcitajIzDatoteke(head, "lista.txt");

    printf("Trenutna lista:\n");
    Ispisi(head->next);

    UmetniNaPocetak(head);

    IspisiUDatoteku(head->next, "lista.txt");

    Ispisi(head->next);

    return 0;
}
