#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

struct article;
typedef struct article* ArtikalPozicija;
typedef struct article {
    char ime[20];
    int kolicina;
    int cijena;
    ArtikalPozicija next;
} Artikal;

struct racun;
typedef struct racun* RacunPozicija;
typedef struct racun {
    char datum[10];
    Artikal headArtikal;
    RacunPozicija next;
} Racun;

int dodajArtikal(ArtikalPozicija head, ArtikalPozicija article)
{
    ArtikalPozicija curr = head;

    while (curr->next != NULL && strcmp(curr->next->ime, article->ime) < 0)
        curr = curr->next;

    article->next = curr->next;
    curr->next = article;

    return 0;
}

int dodajRacun(RacunPozicija head, RacunPozicija receipt)
{
    RacunPozicija curr = head;

    while (curr->next != NULL && strcmp(curr->next->datum, receipt->datum) <= 0)
        curr = curr->next;

    receipt->next = curr->next;
    curr->next = receipt;

    return 0;
}

int citajRacun(char* imeDatoteke, RacunPozicija headRacun)
{
    char datum[20] = { 0 };
    char articleString[MAX_LINE] = { 0 };
    FILE* datPokazivac = NULL;
    RacunPozicija noviRacun = NULL;
    ArtikalPozicija noviArtikal = NULL;

    datPokazivac = fopen(imeDatoteke, "r");
    if (datPokazivac == NULL) {
        printf("Nije moguce otvoriti datoteku.\n");
        return -1;
    }

    noviRacun = (RacunPozicija)malloc(sizeof(Racun));
    if (noviRacun == NULL) {
        printf("Nema dovoljno memorije.\n");
        fclose(datPokazivac);
        return -1;
    }

    fgets(datum, sizeof(datum), datPokazivac);
    datum[strcspn(datum, "\n")] = 0; //Kada pronadje \n napise se \0

    strcpy(noviRacun->datum, datum);
    noviRacun->headArtikal.next = NULL;

    while (1) {
    noviArtikal = (ArtikalPozicija)malloc(sizeof(Artikal));
    if (noviArtikal == NULL) {
        printf("Nema dovoljno memorije.\n");
        fclose(datPokazivac);
        return -1;
    }

    if (fscanf(datPokazivac, "%[^,], %d, %d\n", noviArtikal->ime, &noviArtikal->kolicina, &noviArtikal->cijena) != 3) {
        free(noviArtikal); // Oslobadja memoriju ako citanje ne uspije
        break;
    }

    dodajArtikal(&noviRacun->headArtikal, noviArtikal);
}


    dodajRacun(headRacun, noviRacun);

    fclose(datPokazivac);

    return 0;
}

int IspisiArtikle(ArtikalPozicija curr)
{
    while (curr != NULL) {
        printf("\t%s, %d, %d\n", curr->ime, curr->kolicina, curr->cijena);
        curr = curr->next;
    }

    return 0;
}

int ispisiRacune(RacunPozicija curr)
{
    while (curr != NULL) {
        printf("Racun datuma %s:\n", curr->datum);
        IspisiArtikle(curr->headArtikal.next);
        curr = curr->next;
    }

    return 0;
}

int artikalUOdredjenomVremenu(RacunPozicija currRacun, char* artikal, char* pocetniDatum, char* zavrsniDatum, int* cijena, int* broj)
{
    ArtikalPozicija currArtikal = NULL;

    // Pomakne prvi racun na onaj s pocetnim datumom
    while (currRacun != NULL && strcmp(currRacun->datum, pocetniDatum) < 0)
        currRacun = currRacun->next;

    while (currRacun != NULL && strcmp(currRacun->datum, zavrsniDatum) <= 0) {
        currArtikal = currRacun->headArtikal.next;
        while (currArtikal != NULL) {
            if (strcmp(currArtikal->ime, artikal) == 0) {
                *cijena += currArtikal->kolicina * currArtikal->cijena;
                *broj += currArtikal->kolicina;
            }
            currArtikal = currArtikal->next;
        }
        currRacun = currRacun->next;
    }

    return 0;
}

int IzbrisiArtikle(ArtikalPozicija artikalHead)
{
    ArtikalPozicija temp = NULL;

    while (artikalHead->next != NULL) {
        temp = artikalHead->next;
        artikalHead->next = temp->next;
        free(temp);
    }

    return 0;
}

int izbrisiRacune(RacunPozicija racunHead)
{
    RacunPozicija temp = NULL;

    while (racunHead->next != NULL) {
        temp = racunHead->next;
        racunHead->next = temp->next;
        IzbrisiArtikle(&temp->headArtikal);
        free(temp);
    }

    return 0;
}

int main()
{
    int potroseno = 0, brojac = 0;
    char artikal[100] = { 0 };
    char pocetniDatum[20] = { 0 }, zavrsniDatum[20] = { 0 };
    char currRacun[MAX_LINE] = { 0 };
    FILE* datPokazivac = NULL;
    Racun headRacun = {.datum = {0}, .headArtikal = {.next = NULL}, .next = NULL};

    datPokazivac = fopen("racuni.txt", "r");
    if (datPokazivac == NULL) {
        printf("Nije moguce otvoriti datoteku.\n");
        return -1;
    }

    while (fgets(currRacun, MAX_LINE, datPokazivac) != NULL) {
        currRacun[strcspn(currRacun, "\n")] = 0; // Brise '\n'
        printf("%s\n", currRacun);
        citajRacun(currRacun, &headRacun);
    }

    ispisiRacune(headRacun.next);

    printf("\nUnesite ime artikla: ");
    if (scanf("%s", artikal) != 1) 
		return -1;
    printf("Unesite pocetni datum (YYYY-MM-DD): ");
    if (scanf("%s", pocetniDatum) != 1) 
		return -1;
    printf("Unesite zavrsni datum (YYYY-MM-DD): ");
    if (scanf("%s", zavrsniDatum) != 1) 
		return -1;

    artikalUOdredjenomVremenu(&headRacun, artikal, pocetniDatum, zavrsniDatum, &potroseno, &brojac);
    //printf("Income of %d %s articles: %d\n", brojac, artikal, potroseno);
	printf("Kupljeno %d %s artikala, ukupno potroseno %d $", brojac, artikal, potroseno);

    izbrisiRacune(&headRacun);
    fclose(datPokazivac);

    return 0;
}
