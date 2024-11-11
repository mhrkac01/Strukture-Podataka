#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE 1024

struct el;
typedef struct el* position;

typedef struct el {

	int koef;
	int exp;

	position next;

}monom;

int MonomUPolinom(position head, position mono)
{
	position prev = head;
	position temp = NULL;

	// Koeficijent nula
	if (mono->koef == 0) {
		free(mono);
		return 0;
	}

	// While petlja pronalazi prethodnika
	while (prev->next != NULL && prev->next->exp > mono->exp) 
		prev = prev->next;

	// U koliko su koeficijent isti, oni se zbroje
	if (prev->next != NULL && mono->exp == prev->next->exp)
	{
		prev->next->koef += mono->koef;
		free(mono);
		//U koliko je rezultirajucu koeficijent 0
		if (prev->next->koef == 0)
		{
			temp = prev->next;
			prev->next = prev->next->next;
			free(temp);
		}
	}
		
	// Postavljanje novog monoma
	else {
		mono->next = prev->next;
		prev->next = mono;
	}

	return 0;
}

int StringUPolinom(position head, char* str)
{
	int i = 0, size = 0;
	position mono = NULL;

	while (strlen(str) > 0)
	{
		mono = (position)malloc(sizeof(monom));
		if (mono == NULL) return -1;

		if (sscanf(str, " %dx^%d %n", &mono->koef, &mono->exp, &size) != 2) return -1;

		MonomUPolinom(head, mono);
		str += size;
	}

	return 0;
}

//Ispisivanje monoma
int IspisiMonom(position mono)
{
	if (mono->koef > 0)
    	printf("%d", mono->koef);
	else
    	printf("- %d", -mono->koef);


	if (mono->exp == 1) printf("x ");
	else if (mono->exp != 0) printf("x^%d ", mono->exp);

	return 0;
}

//Ispisivanje polinoma
int IspisiPolinom(position curr)
{
	if (curr != NULL) {
		IspisiMonom(curr);
		curr = curr->next;
	}

	while (curr != NULL)
	{
		if (curr->koef > 0) printf("+ ");
		IspisiMonom(curr);
		curr = curr->next;
	}

	printf("\n");

	return 0;
}

int ZbrojiPolinome(position sum, position poly1, position poly2)
{
	position temp = NULL;

	while (poly1 != NULL)
	{
		temp = (position)malloc(sizeof(monom));
		if (temp == NULL) return -1;

		temp->koef = poly1->koef;
		temp->exp = poly1->exp;

		MonomUPolinom(sum, temp);
		poly1 = poly1->next;
	}

	while (poly2 != NULL)
	{
		temp = (position)malloc(sizeof(monom));
		if (temp == NULL) return -1;

		temp->koef = poly2->koef;
		temp->exp = poly2->exp;

		MonomUPolinom(sum, temp);
		poly2 = poly2->next;
	}

	return 0;
}

int ProduktPolinoma(position product, position poly1, position poly2)
{
	position temp = NULL;
	position poly2first = poly2;

	while (poly1 != NULL)
	{
		poly2 = poly2first;
		while (poly2 != NULL)
		{
			temp = (position)malloc(sizeof(monom));
			if (temp == NULL) return -1;

			temp->koef = poly1->koef * poly2->koef;
			temp->exp = poly1->exp + poly2->exp;

			MonomUPolinom(product, temp);
			poly2 = poly2->next;
		}
		poly1 = poly1->next;
	}

	return 0;
}

int main() {
	position polinom1;
	position polinom2;
	position sumaPol;
	position prodPol;

	polinom1 = (position)malloc(sizeof(monom));
	if (polinom1 == NULL)
		return -1;

	polinom2 = (position)malloc(sizeof(monom));
	if (polinom2 == NULL)
		return -1;
	
	sumaPol = (position)malloc(sizeof(monom));
	if (sumaPol == NULL)
		return -1;
	
	prodPol = (position)malloc(sizeof(monom));
	if (prodPol == NULL)
		return -1;


	polinom1->koef = 0;
	polinom1->exp = 0;
	polinom1->next = NULL;

	polinom2->koef = 0;
	polinom2->exp = 0;
	polinom2->next = NULL;

	sumaPol->koef = 0;
	sumaPol->exp = 0;
	sumaPol->next = NULL;

	prodPol->koef = 0;
	prodPol->exp = 0;
	prodPol->next = NULL;

	int i = 0, size = 0;
	char buffer[MAX_LINE] = { 0 };

	FILE* dat;
	dat = fopen("polinomi.txt", "r");

	if(dat==NULL){
		printf("Nije moguce otvoriti .txt datoteku");
		return -1;
	}

	// Cita prvu liniju
	fgets(buffer, MAX_LINE, dat);
	StringUPolinom(polinom1, buffer);

	// Cita drugu liniju
	fgets(buffer, MAX_LINE, dat);
	StringUPolinom(polinom2, buffer);

	printf("Polinom 1: ");
	IspisiPolinom(polinom1->next);
	printf("Polinom 2: ");
	IspisiPolinom(polinom2->next);

	ZbrojiPolinome(sumaPol, polinom1->next, polinom2->next);
	printf("Zbroj: ");
	IspisiPolinom(sumaPol->next);

	ProduktPolinoma(prodPol, polinom1->next, polinom2->next);
	printf("Produkt: ");
	IspisiPolinom(prodPol->next);

	return 0;
}