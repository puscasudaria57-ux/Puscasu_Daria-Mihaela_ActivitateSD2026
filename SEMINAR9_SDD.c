#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>


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
	Nod* stanga;
	Nod* dreapta;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	Masina m1;

	if (!fgets(buffer, 100, file)) {
		m1.id = -1;
		m1.model = NULL;
		m1.numeSofer = NULL;
		return m1;
	}

	char* aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	aux = strtok(NULL, sep);
	m1.serie = aux ? aux[0] : '-';

	return m1;
}

void afisareMasina(Masina masina) {
	if (masina.id == -1) return;

	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if (*radacina) {
		if ((*radacina)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
		}
		else if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&(*radacina)->stanga, masinaNoua);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		*radacina = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");

	if (!f) {
		printf("Eroare deschidere fisier!\n");
		return NULL;
	}

	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != -1) {
			adaugaMasinaInArbore(&radacina, m);
		}
	}

	fclose(f);
	return radacina;
}

void afisareMasiniDinArborePreordine(Nod* radacina) {
	if (radacina) {
		afisareMasina(radacina->info);
		afisareMasiniDinArborePreordine(radacina->stanga);
		afisareMasiniDinArborePreordine(radacina->dreapta);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeMasini(&(*radacina)->stanga);
		dezalocareArboreDeMasini(&(*radacina)->dreapta);

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);

		free(*radacina);
		*radacina = NULL;
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	if (radacina) {
		if (radacina->info.id == id) {
			Masina m;
			m.id = radacina->info.id;
			m.nrUsi = radacina->info.nrUsi;
			m.pret = radacina->info.pret;
			m.serie = radacina->info.serie;

			m.model = (char*)malloc(strlen(radacina->info.model) + 1);
			strcpy(m.model, radacina->info.model);

			m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
			strcpy(m.numeSofer, radacina->info.numeSofer);

			return m;
		}
		if (radacina->info.id < id)
			return getMasinaByID(radacina->dreapta, id);
		else
			return getMasinaByID(radacina->stanga, id);
	}

	Masina m;
	m.id = -1;
	m.model = NULL;
	m.numeSofer = NULL;
	return m;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		return 1 + determinaNumarNoduri(radacina->stanga) +
			determinaNumarNoduri(radacina->dreapta);
	}
	return 0;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina) {
		int st = calculeazaInaltimeArbore(radacina->stanga);
		int dr = calculeazaInaltimeArbore(radacina->dreapta);
		return 1 + (st > dr ? st : dr);
	}
	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina) {
		return radacina->info.pret +
			calculeazaPretTotal(radacina->stanga) +
			calculeazaPretTotal(radacina->dreapta);
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina) {
		float suma = calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer) +
			calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);

		if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
			suma += radacina->info.pret;

		return suma;
	}
	return 0;
}

int main() {
	Nod* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");

	afisareMasiniDinArborePreordine(radacina);

	printf("===============\n");

	Masina m = getMasinaByID(radacina, 7);
	afisareMasina(m);

	printf("===============\n");
	printf("Nr noduri: %d\n", determinaNumarNoduri(radacina));

	dezalocareArboreDeMasini(&radacina);

	return 0;
}