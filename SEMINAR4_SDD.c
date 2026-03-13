#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	while (cap != NULL)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) { //* e doar pt cazul in care lista e goala si cap se modifica
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	if (*cap == NULL) {
		*cap = nodNou;
	}
	else {
		Nod* aux = *cap; //aux pt a nu modifica pe cap care daca s-ar face NULL am pierde lista
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = nodNou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = *cap;
	*cap = nodNou;
}


Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		Nod* cap = NULL;
		while (!feof(file))
		{
			adaugaMasinaInLista(&cap, citireMasinaDinFisier(file));
		}
		fclose(file);
		return cap;
	}
	return NULL;
}

void dezalocareListaMasini(Nod** cap) {
	while (*cap != NULL)
	{
		Nod* p = *cap;
		(*cap) = p->next;
		if (p->info.numeSofer != NULL)
			free(p->info.numeSofer);
		if (p->info.model != NULL)
			free(p->info.model);
		free(p);
	}
}

float calculeazaPretMediu(Nod* cap) {
	float suma = 0;
	int contor = 0;
	while (cap != NULL)
	{
		suma = suma + cap->info.pret;
		contor++;
		cap = cap->next;
	}
	if (contor != 0)
		return suma / contor;
	return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	while (*cap && (*cap)->info.serie == serieCautata)
	{
		Nod* temp = *cap;
		(*cap) = temp->next;
		if (temp->info.numeSofer != NULL)
			free(temp->info.numeSofer);
		if (temp->info.model != NULL)
			free(temp->info.model);
		free(temp);
	}
	Nod* p = *cap;
	while (p != NULL)
	{
		while (p->next && p->next->info.serie != serieCautata)
		{
			p = p->next;
		}//trecem prin B-uri iar p ramane e=pe ultimul B
		if (p->next)
		{
			Nod* temp = p->next;
			p->next = temp->next;//refacem legatura
			if (temp->info.numeSofer != NULL)
			{
				free(temp->info.numeSofer);
			}
			if (temp->info.model != NULL)
			{
				free(temp->info.model);
			}
			free(temp);
		}
		else
		{
			p = p->next; //primeste null
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(/*lista masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main() {

	Nod* cap = NULL;
	cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);

	float medie = calculeazaPretMediu(cap);
	printf("\nPretul mediu este: %.2f\n", medie);

	stergeMasiniDinSeria(&cap, 'A');
	printf("\nDupa stergerea seria A:\n");
	afisareListaMasini(cap);
	dezalocareListaMasini(&cap);
	return 0;
}