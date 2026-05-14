#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

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

//1. 
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara

typedef struct NodPrincipal NodP;
typedef struct NodSecundar NodS;

struct NodPrincipal {
	Masina info;
	NodP* next;
	NodS* vec;


};

struct NodSecundar {
	NodP* info;
	NodS* next;
};


//2.
//functii de inserare in liste
//si in principala si in secundara

void inserarelistaP(NodP** graf, Masina masinaNoua) {
	NodP* nou = (NodP*)malloc(sizeof(NodP));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->vec = NULL;
	if (*graf) {
		NodP* aux = *graf;

		while (aux->next) {
			aux = (aux)->next;
		}
		(aux)->next = nou;
	}
	else {
		*graf = nou;
	}

}


void inserareinlistaS(NodS** primS, NodP* vec) {
	NodS* nou = (NodS*)malloc(sizeof(NodS));
	nou->info = vec;
	nou->next = NULL;
	if (*primS) {
		NodS* aux = *primS;
		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		(*primS) = nou;
	}
}



//3.
//functie de cautarea in lista principala dupa ID
NodP* cautaNodDupaID(NodP* listaPrincipala, int id) {
	NodP* n = NULL;
	if (listaPrincipala) {
		while (listaPrincipala) {
			if (listaPrincipala->info.id == id) {
				return listaPrincipala;
			}
			listaPrincipala = listaPrincipala->next;
		}
	}
	return n;
}

//4.
//inserare muchie
void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {
	if (listaPrincipala) {
		NodP* prim = cautaNodDupaID(listaPrincipala, idStart);
		NodP* ult = cautaNodDupaID(listaPrincipala, idStop);

		if (prim && ult) {
			inserareinlistaS(&(prim->vec), ult);
			inserareinlistaS(&(ult->vec), prim);

		}
	}
}


NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	NodP* listaP = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		inserarelistaP(&listaP, m);

	}
	fclose(f);
	return listaP;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* listaP) {
	//functia primeste numele fisierului, il deschide si citeste 
	//toate id-urile de start si stop pentru fiecare muchie
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	NodP* listaS = NULL;
	while (!feof(f)) {
		int* idStart;
		int* idStop;
		fscanf(f, "%d %d", &idStart, &idStop);
		inserareMuchie(listaP, idStart, idStop);
	}
	fclose(f);



}

void dezalocareNoduriGraf(void* listaPrincipala) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste



}

int main() {

	NodP* listaP = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", listaP);

	return 0;
}