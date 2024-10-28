#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct _lista;
typedef struct _lista* position;
typedef struct _lista {
	int god;
	char ime[100];
	char prezime[100];

	position next;

}lista;
//Memorijska lokacija strukture
void Unesi(position p) {
	printf("Unesite godine: ");
	scanf("%d", &p->god);
	printf("Unesite ime: ");
	scanf("%s", p->ime);
	printf("Unesite prezime: ");
	scanf("%s", p->prezime);
	printf("\n");
}

//Ova funkcija trazi memorijsku lokaciju head elementa
int UmetniNaPocetak(position p) {
	position q = NULL;
	q = (position)malloc(sizeof(lista));
	if (q == NULL)
		return -1;
	Unesi(q);
	q->next = p->next;
	p->next = q;
	return 0;
}
//Funkcija kao argument prima head->next
int Ispisi(position p) {
	if (p == NULL) {
		printf("Nema elemenata, lista je prazna.\n");
		return -1;
	}
	else {
		while (p != NULL) {
			printf("%s\n%s\n%d\n", p->ime, p->prezime, p->god);
			p = p->next;
			printf("\n\n");
		}
		return 0;
	}
}

//Adresa head elementa
position PronadiPosljednji(position p) {
	while (p->next != NULL)
		p = p->next;
	return p;
}

//Adresa head elementa
void UmetniNaKraj(position p) {
	position last = NULL;
	last = PronadiPosljednji(p);
	UmetniNaPocetak(last);

}

//Funkcija prima head->next
position Pronadi(position p, char* prezime) {
	while (p != NULL && strcmp(p->prezime, prezime) != NULL) {
		p = p->next;
	}
	if (p == NULL)
		printf("Takav element ne postoji\n");
	return p;
}

//Funkcija prima head->next i prezime kojemu trazimo prethodinka
position PronadjiPrethodnika(position p, char* prezime) {
	position prev = NULL;
	while (p != NULL && strcmp(p->prezime, prezime) != 0) {
		prev = p;
		p = p->next;
	}
	return prev;
}

//U funkciju se salje memorijska lokacija head elementa i prezime
int IzbrisiElement(position p, char* prezime) {
	position q, n;

	if (p->next == NULL) {
		printf("Vezana lista je prazna.\n");
		return -1;
	}


	q = PronadjiPrethodnika(p->next, prezime);
	n = Pronadi(p->next, prezime);
	if (q == NULL) {

		p->next = n->next;
	}
	else {
		q->next = n->next;
	}

	free(n);
	return 0;
}

int main() {
	char string[100];
	char string1[100];
	position q;
	position head = NULL;
	head = (position)malloc(sizeof(lista));
	if (head == NULL)
		return -1;
	head->next = NULL;
	UmetniNaPocetak(head);
	UmetniNaKraj(head);
	Ispisi(head->next);
	printf("Koji element zelite izbrisati(unesite prezime): ");
	scanf("%s", string);
	IzbrisiElement(head, string);
	Ispisi(head->next);
	printf("Memorijsku lokaciju kojeg elementa zelite pronaci(unesite prezime): ");
	scanf("%s", string1);
	q=Pronadi(head->next, string1);
	printf("Memorijska lokacija elementa %s je: %p.", string1, q);
	return 0;
}