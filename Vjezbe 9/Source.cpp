#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct _cvor;
typedef struct _cvor* pozicija;
typedef struct _cvor {
    int broj;
    pozicija lijevo;
    pozicija desno;
} cvor;

pozicija stvori(pozicija, int);
int zamijeni(pozicija);
int izbrisiStablo(pozicija);
int ispisiUDatoteku(pozicija, int, FILE*);
void generirajNasumicneBrojeve(int*, int, int, int);

int main() {
    int brojevi[10];
    pozicija korijen = NULL;

    // Generiraj nasumicne brojeve u rasponu <10, 90>
    srand((unsigned)time(NULL));
    generirajNasumicneBrojeve(brojevi, 10, 10, 90);

    // Kreiraj stablo koristeci nasumicne brojeve
    korijen = stvori(korijen, brojevi[0]);
    for (int i = 1; i < 10; i++) {
        korijen = stvori(korijen, brojevi[i]);
    }

    // Otvori datoteku za ispis
    FILE* datoteka = fopen("ispis.txt", "w");
    if (datoteka == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    // Ispisi originalno stablo
    fprintf(datoteka, "Originalno stablo:\n");
    ispisiUDatoteku(korijen, 0, datoteka);

    // Zamijeni vrijednosti u stablu
    zamijeni(korijen);

    // Ispisi zamijenjeno stablo
    fprintf(datoteka, "\nZamijenjeno stablo:\n");
    ispisiUDatoteku(korijen, 0, datoteka);

    // Zatvori datoteku i oslobodi memoriju
    fclose(datoteka);
    izbrisiStablo(korijen);

    return 0;
}

pozicija stvori(pozicija korijen, int broj) {
    if (korijen == NULL) {
        korijen = (pozicija)malloc(sizeof(cvor));
        if (korijen == NULL)
            return NULL;
        korijen->broj = broj;
        korijen->lijevo = NULL;
        korijen->desno = NULL;
    }
    else if (broj >= korijen->broj) {
        korijen->desno = stvori(korijen->desno, broj);
    }
    else {
        korijen->lijevo = stvori(korijen->lijevo, broj);
    }

    return korijen;
}

int zamijeni(pozicija korijen) {
    if (korijen == NULL) return 0;
    int vrijednost = korijen->broj;
    korijen->broj = zamijeni(korijen->lijevo) + zamijeni(korijen->desno);
    return vrijednost + korijen->broj;
}

int ispisiUDatoteku(pozicija korijen, int razina, FILE* datotekaPokazivac) {
    int i = 0;

    if (korijen->lijevo != NULL) {
        ispisiUDatoteku(korijen->lijevo, razina + 1, datotekaPokazivac);
    }

    for (i = 0; i < razina; i++) {
        fprintf(datotekaPokazivac, "   ");
    }
    fprintf(datotekaPokazivac, "%d\n", korijen->broj);

    if (korijen->desno != NULL) {
        ispisiUDatoteku(korijen->desno, razina + 1, datotekaPokazivac);
    }

    return 0;
}

int izbrisiStablo(pozicija korijen) {
    if (korijen == NULL) return 0;

    izbrisiStablo(korijen->lijevo);
    izbrisiStablo(korijen->desno);
    free(korijen);

    return 0;
}

void generirajNasumicneBrojeve(int* brojevi, int velicina, int min, int max) {
    for (int i = 0; i < velicina; i++) {
        brojevi[i] = rand() % (max - min + 1) + min;
    }
}
