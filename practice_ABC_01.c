#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ARBORI BINARI DE CAUTARE (ABC)

typedef struct Masina Masina;
struct Masina
{
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};


// creare structura pentru un nod dintr-un arbore binar de cautare
typedef struct Nod Nod; // alias pentru struct Nod

struct Nod {
	Masina info;   // informa?ia din nod
	Nod* stanga;  // copil stânga: id-uri mai mici
	Nod* dreapta; // copil dreapta: id-uri mai mari
};


void afisareMasina(Masina masina)
{
	if (masina.id == -1) return; // dac? ma?ina este invalid?, nu o afi??m

	printf("Id: %d\n", masina.id);
	printf("Nr usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}


Masina citireMasinaFisier(FILE* f)
{
	Masina m;

	char buffer[256];
	char sep[3] = ",\n"; // virgula si new line(\n) + null terminator => sep[3]

	fgets(buffer, 255, f); // citim o linie din fi?ier

	m.id = atoi(strtok(buffer, sep));        // citim id-ul
	m.nrUsi = atoi(strtok(NULL, sep));       // citim num?rul de u?i
	m.pret = atof(strtok(NULL, sep));        // citim pre?ul

	char* aux;

	aux = strtok(NULL, sep); // citim modelul
	m.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.model, aux);

	aux = strtok(NULL, sep); // citim numele ?oferului
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.numeSofer, aux);

	m.serie = strtok(NULL, sep)[0]; // citim seria

	return m; // return?m ma?ina citit?
}


// adauga o noua masina pe care o primim ca parametru in arbore,
// astfel incat sa respecte principiile de arbore binar de cautare
// dupa o anumita cheie pe care o decideti - aici folosim ID
void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	// radacina este pointer la pointer, ca s? putem modifica arborele

	if (*radacina) {
		// dac? nodul curent exist?, compar?m id-urile

		if ((*radacina)->info.id < masinaNoua.id) {
			// id-ul nou este mai mare, deci mergem în dreapta
			adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
		}
		else if ((*radacina)->info.id > masinaNoua.id) {
			// id-ul nou este mai mic, deci mergem în stânga
			adaugaMasinaInArbore(&(*radacina)->stanga, masinaNoua);
		}
		// dac? id-urile sunt egale, nu inser?m nimic
	}
	else {
		// am g?sit o pozi?ie liber? ?i cre?m un nod nou

		Nod* nou = (Nod*)malloc(sizeof(Nod)); // aloc?m memorie pentru nod

		nou->info = masinaNoua; // salv?m ma?ina în nod

		nou->dreapta = NULL; // nodul nou nu are copil dreapta
		nou->stanga = NULL;  // nodul nou nu are copil stânga

		*radacina = nou; // leg?m nodul nou în arbore
	}
}


Nod* citireArboreDeMasiniDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r"); // deschidem fi?ierul pentru citire

	Nod* radacina = NULL; // arborele este gol la început

	if (!f) {
		// dac? fi?ierul nu s-a deschis, return?m arbore gol
		printf("Eroare deschidere fisier!\n");
		return NULL;
	}

	while (!feof(f))
	{
		// citim o ma?in? ?i o ad?ug?m în arbore
		adaugaMasinaInArbore(&radacina, citireMasinaFisier(f));
	}

	fclose(f); // închidem fi?ierul

	return radacina; // return?m r?d?cina arborelui construit
}


// =============== AFISARI ==================
// Preordine:   R?d?cin? -> Stânga -> Dreapta
// Inordine:    Stânga -> R?d?cin? -> Dreapta
// Postordine:  Stânga -> Dreapta -> R?d?cin?

void afisareMasiniDinArborePreordine(Nod* radacina)
{
	if (radacina) {
		// 1. afi??m informa?ia din nodul curent
		afisareMasina(radacina->info);

		// 2. parcurgem subarborele stâng
		afisareMasiniDinArborePreordine(radacina->stanga);

		// 3. parcurgem subarborele drept
		afisareMasiniDinArborePreordine(radacina->dreapta);
	}
}


void afisareMasiniDinArborePostordine(Nod* radacina)
{
	if (radacina)
	{
		// 1. parcurgem subarborele stâng
		afisareMasiniDinArborePostordine(radacina->stanga);

		// 2. parcurgem subarborele drept
		afisareMasiniDinArborePostordine(radacina->dreapta);

		// 3. afi??m r?d?cina
		afisareMasina(radacina->info);
	}
}


void afisareMasiniDinArboreInordine(Nod* radacina)
{
	if (radacina)
	{
		// 1. parcurgem subarborele stâng
		afisareMasiniDinArboreInordine(radacina->stanga);

		// 2. afi??m r?d?cina
		afisareMasina(radacina->info);

		// 3. parcurgem subarborele drept
		afisareMasiniDinArboreInordine(radacina->dreapta);
	}
}


void dezalocareArboreDeMasini(Nod** radacina)
{
	if (*radacina) { // dac? nodul exist?

		// dezaloc?m mai întâi subarborele stâng
		dezalocareArboreDeMasini(&(*radacina)->stanga);

		// dezaloc?m apoi subarborele drept
		dezalocareArboreDeMasini(&(*radacina)->dreapta);

		// eliber?m câmpurile alocate dinamic din Masina
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);

		// eliber?m nodul curent
		free(*radacina);

		// set?m pointerul la NULL dup? dezalocare
		*radacina = NULL;
	}
}


Masina getMasinaByID(Nod* radacina, int id) {
	if (radacina) { // dac? nodul curent exist?

		if (radacina->info.id == id) {
			// am g?sit ma?ina c?utat?

			Masina m;

			// copiem câmpurile simple
			m.id = radacina->info.id;
			m.nrUsi = radacina->info.nrUsi;
			m.pret = radacina->info.pret;
			m.serie = radacina->info.serie;

			// deep copy pentru model
			m.model = (char*)malloc(strlen(radacina->info.model) + 1);
			strcpy(m.model, radacina->info.model);

			// deep copy pentru numele ?oferului
			m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
			strcpy(m.numeSofer, radacina->info.numeSofer);

			return m; // return?m copia ma?inii
		}

		if (radacina->info.id < id)
			// id-ul c?utat este mai mare, mergem în dreapta
			return getMasinaByID(radacina->dreapta, id);
		else
			// id-ul c?utat este mai mic, mergem în stânga
			return getMasinaByID(radacina->stanga, id);
	}

	// dac? nu am g?sit ma?ina, return?m o ma?in? invalid?
	Masina m;
	m.id = -1;
	m.model = NULL;
	m.numeSofer = NULL;

	return m;
}


int determinaNumarNoduri(Nod* radacina)
{
	if (radacina)
	{
		// num?r?m nodul curent + nodurile din dreapta + nodurile din stânga
		return 1 + determinaNumarNoduri(radacina->dreapta)
			+ determinaNumarNoduri(radacina->stanga);
	}

	return 0; // dac? r?d?cina este NULL, nu exist? noduri
}


int calculeazaInaltimeArbori(Nod* radacina)
{
	if (radacina)
	{
		// calcul?m în?l?imea subarborelui stâng
		int st = calculeazaInaltimeArbori(radacina->stanga);

		// calcul?m în?l?imea subarborelui drept
		int dr = calculeazaInaltimeArbori(radacina->dreapta);

		// în?l?imea = 1 + maximul dintre stânga ?i dreapta
		return 1 + (st > dr ? st : dr);
	}

	return 0; // dac? nodul este NULL, în?l?imea este 0
}


float calculeazaPretTotal(Nod* radacina)
{
	if (radacina)
	{
		// pre?ul total = pre? curent + pre?uri din dreapta + pre?uri din stânga
		return radacina->info.pret
			+ calculeazaPretTotal(radacina->dreapta)
			+ calculeazaPretTotal(radacina->stanga);
	}

	return 0; // dac? nodul este NULL, suma este 0
}


float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina) {
		// calcul?m suma pre?urilor din subarborele stâng ?i drept
		float suma = calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer) +
			calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);

		// verific?m dac? ma?ina din nodul curent apar?ine ?oferului c?utat
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
			suma += radacina->info.pret; // ad?ug?m pre?ul ma?inii curente

		return suma; // return?m suma total? g?sit?
	}

	return 0; // dac? nodul este NULL, suma este 0
}


int main()
{
	Nod* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");

	afisareMasiniDinArborePreordine(radacina);

	int nrNoduri = determinaNumarNoduri(radacina);
	printf("Numarul de noduri ale arborelui este: %d\n", nrNoduri);

	Masina m = getMasinaByID(radacina, 7);
	afisareMasina(m);

	// pentru c? getMasinaByID face malloc, eliber?m copia returnat?
	free(m.model);
	free(m.numeSofer);

	// dezaloc?m arborele
	dezalocareArboreDeMasini(&radacina);

	return 0;
}