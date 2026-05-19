#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// folosim fisierul masini.txt

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

typedef struct StructuraMasina Masina;


// citim o masina din fisier
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


// afisam o masina
void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}


// ================= STRUCTURI PENTRU GRAF =================

// NodP = nod din lista principala
// NodS = nod din lista secundara, adica lista de vecini

typedef struct NodPrincipal NodP;
typedef struct NodSecundar NodS;


// nod principal = o masina din graf
struct NodPrincipal {
	Masina info;  // masina salvata in nod
	NodP* next;  // urmatorul nod din lista principala
	NodS* vec;   // lista de vecini ai acestui nod
};


// nod secundar = un vecin al unui nod principal
struct NodSecundar {
	NodP* info;  // pointer catre nodul vecin din lista principala
	NodS* next;  // urmatorul vecin
};


// ================= INSERARE IN LISTA PRINCIPALA =================

void inserareListaPrincipala(NodP** graf, Masina masinaNoua) {
	// cream un nod principal nou
	NodP* nou = (NodP*)malloc(sizeof(NodP));

	nou->info = masinaNoua; // salvam masina
	nou->next = NULL;       // momentan nu are urmator
	nou->vec = NULL;        // momentan nu are vecini

	if (*graf) {
		// daca lista principala exista, inseram la final
		NodP* aux = *graf;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		// daca lista este goala, noul nod devine primul
		*graf = nou;
	}
}


// ================= INSERARE IN LISTA SECUNDARA =================

void inserareInListaSecundara(NodS** primS, NodP* vecin) {
	// cream un nod secundar nou
	NodS* nou = (NodS*)malloc(sizeof(NodS));

	nou->info = vecin; // salvam adresa nodului vecin
	nou->next = NULL;

	if (*primS) {
		// daca lista de vecini exista, inseram la final
		NodS* aux = *primS;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		// daca lista de vecini este goala, noul nod devine primul vecin
		*primS = nou;
	}
}


// ================= CAUTARE NOD DUPA ID =================

NodP* cautaNodDupaID(NodP* listaPrincipala, int id) {
	// parcurgem lista principala
	while (listaPrincipala) {
		// daca gasim masina cu id-ul cautat, returnam nodul
		if (listaPrincipala->info.id == id) {
			return listaPrincipala;
		}

		listaPrincipala = listaPrincipala->next;
	}

	// daca nu am gasit nimic, returnam NULL
	return NULL;
}


// ================= INSERARE MUCHIE =================

void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {
	if (listaPrincipala) {
		// cautam nodul de start
		NodP* prim = cautaNodDupaID(listaPrincipala, idStart);

		// cautam nodul de stop
		NodP* ult = cautaNodDupaID(listaPrincipala, idStop);

		if (prim && ult) {
			// adaugam ult ca vecin pentru prim
			inserareInListaSecundara(&(prim->vec), ult);

			// adaugam prim ca vecin pentru ult
			// pentru ca graful este neorientat
			inserareInListaSecundara(&(ult->vec), prim);
		}
	}
}


// ================= CITIRE NODURI DIN FISIER =================

NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");

	NodP* listaP = NULL;

	if (!f) {
		printf("Eroare la deschiderea fisierului de masini!\n");
		return NULL;
	}

	while (!feof(f)) {
		// citim masina
		Masina m = citireMasinaDinFisier(f);

		// o inseram in lista principala
		inserareListaPrincipala(&listaP, m);
	}

	fclose(f);

	return listaP;
}


// ================= CITIRE MUCHII DIN FISIER =================

void citireMuchiiDinFisier(const char* numeFisier, NodP* listaP) {
	FILE* f = fopen(numeFisier, "r");

	if (!f) {
		printf("Eroare la deschiderea fisierului de muchii!\n");
		return;
	}

	int idStart;
	int idStop;

	// citim perechi de forma: idStart idStop
	while (fscanf(f, "%d %d", &idStart, &idStop) == 2) {
		inserareMuchie(listaP, idStart, idStop);
	}

	fclose(f);
}


// ================= AFISARE GRAF =================

void afisareGraf(NodP* listaPrincipala) {
	while (listaPrincipala) {
		// afisam nodul principal
		printf("Masina cu ID %d are vecinii: ", listaPrincipala->info.id);

		// parcurgem lista de vecini
		NodS* aux = listaPrincipala->vec;

		while (aux) {
			printf("%d ", aux->info->info.id);
			aux = aux->next;
		}

		printf("\n");

		listaPrincipala = listaPrincipala->next;
	}
}


// ================= AFISARE COMPLETA =================

void afisareGrafComplet(NodP* listaPrincipala) {
	while (listaPrincipala) {
		printf("NOD PRINCIPAL:\n");
		afisareMasina(listaPrincipala->info);

		printf("Vecini:\n");

		NodS* aux = listaPrincipala->vec;

		while (aux) {
			afisareMasina(aux->info->info);
			aux = aux->next;
		}

		printf("=========================\n");

		listaPrincipala = listaPrincipala->next;
	}
}


// ================= DEZALOCARE GRAF =================

void dezalocareNoduriGraf(NodP** listaPrincipala) {
	while (*listaPrincipala) {
		// salvam nodul principal curent
		NodP* nodDeSters = *listaPrincipala;

		// dezalocam lista de vecini
		NodS* vecin = nodDeSters->vec;

		while (vecin) {
			NodS* vecinDeSters = vecin;
			vecin = vecin->next;

			free(vecinDeSters);
		}

		// eliberam campurile dinamice din masina
		free(nodDeSters->info.model);
		free(nodDeSters->info.numeSofer);

		// mutam lista principala la urmatorul nod
		*listaPrincipala = (*listaPrincipala)->next;

		// stergem nodul principal
		free(nodDeSters);
	}
}


// ================= MAIN =================

int main() {
	NodP* listaP = citireNoduriMasiniDinFisier("masini.txt");

	citireMuchiiDinFisier("muchii.txt", listaP);

	printf("Afisare graf simplu:\n");
	afisareGraf(listaP);

	printf("\nAfisare graf complet:\n");
	afisareGrafComplet(listaP);

	dezalocareNoduriGraf(&listaP);

	return 0;
}