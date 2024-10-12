#define _CRT_SECURE_NO_WARNINGS
#define MAX 1024
#include<stdio.h>
#include<malloc.h>


typedef struct {
    char ime[20];
    char prezime[20];
    int br_bod;
    double rel_br_bod;
}stud;


int main() {

    int i;
    stud* studenti;
    FILE* dat;
    dat = fopen("lista.txt", "r");
    char buffer[MAX];
    int br_stud = 0;

    if (dat == NULL) {
        printf("Error: Nije moguce otvoriti lista.txt\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), dat) != NULL) {
        br_stud++;
    }
    rewind(dat);
    printf("Broj studenata: %d\n", br_stud);
    studenti = (stud*)malloc(br_stud * sizeof(stud));

    for (i = 0; i < br_stud; i++) {
        fscanf(dat, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].br_bod);
        studenti[i].rel_br_bod = ((double)studenti[i].br_bod / 50) * 100;
    }

    for (i = 0; i < br_stud; i++) {
        printf("%s %s %f\n", studenti[i].ime, studenti[i].prezime, studenti[i].rel_br_bod);
    }

    fclose(dat);
    free(studenti);
    return 0;
}