#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina
{
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina)
{
	//afiseaza toate atributele unei masini

	printf("ID: %d\n", masina.id);
	printf("Nr usi: %d\n", masina.nrUsi);
	printf("Pret: %5.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Seria: %c\n\n", masina.serie);
}

//functie care afiseaza vectorul de Masini
void afisareVectorMasini(Masina* vector, int nrElemente)
{
	if (vector == NULL || nrElemente == 0)
	{
		printf("Vectorul este gol!\n");
		return;
	}

	for (int i = 0; i < nrElemente; i++)
	{
		afisareMasina(vector[i]);//vector[0] este prima masina din vectorul de masini
	}
}

//Realizați o funcție care adaugă o mașină într-un vector dinamic, redimensionând vectorul.
void adaugaMasinaInVector(Masina** vector, int* nrElemente, Masina masinaNoua)
{
	//voi construi un vector temporar pentru care ii voi aloca dinamic nrElemente+1 (+1 pt noua masina pe care o adaugam)
	//temp este creat pe stiva functiei si va fi sters la iesirea din functie
	Masina* temp = (Masina*)malloc(sizeof(Masina) * ((*nrElemente) + 1)); //nrElemente trebuie dereferntiat! pt ca este un pointer
	//si retine o adresa de memorie, dar noi vrem sa folosim valoarea la care se afla adresa aceea de memorie
	//GRIJA LA: (*nrElemente) + 1, mai intai dereferntiezi si apoi adaugi 1 (+1) de aceea (*nrElemente) este intre ()

	//voi copia toate elementele din vectorul nostru in temp
	for (int i = 0; i < (*nrElemente); i++)
	{
		temp[i] = (*vector)[i];
	}
	temp[*nrElemente] = masinaNoua; //pe pozitia nrElemente adaug masina 
	free(*vector);//sterg tot ce se afla in vector pt ca el va pointa spre noul vector cu elementul nou adaugat
	(*vector) = temp;//vectorul va pointa spre temp
	//creste si numarul de elemente acum pt ca am adaugat o masina
	(*nrElemente)++;

}


void dezalocare(Masina** vector, int* nrElemente)
{
	for (int i = 0; i < (*nrElemente); i++)
	{
		if ((*vector)[i].model != NULL)
		{
			free((*vector)[i].model);
		}

		if ((*vector)[i].numeSofer != NULL)
		{
			free((*vector)[i].numeSofer);
		}

		free(*vector);
		*vector = NULL;
		*nrElemente = 0;
	}
}

//Masina citireMasinaFisier(FILE* file) {}

//Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {}


Masina citireMasinaFisier(FILE* f)
{
	Masina m;
	char buffer[256];
	fgets(buffer, 255, f);//citesc o linie in buffer

	char sep[3] = ",\n";

	m.id = atoi(strtok(buffer, sep));
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));

	char* aux;

	aux = strtok(NULL, sep); //acum avem sirul de caractere al modelului, ne ajuta ca putem sti dimensiunea cu strlen(aux)
	//aloc spatiu pt acest sir de caractere si dupa copiez
	m.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.model, aux);

	aux = strtok(NULL, sep); //acum avem sirul de caractere al modelului, ne ajuta ca putem sti dimensiunea cu strlen(aux)
	//aloc spatiu pt acest sir de caractere si dupa copiez
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.numeSofer, aux);


	m.serie = strtok(NULL, sep)[0];


	return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite)
{
	FILE* f = fopen(numeFisier, "r");
	
	Masina* vector = NULL;//NULL INITIAL PT CA EST EGOL NOI ACUM ABIA ADUAUGAM IN VECTOR!
	*nrMasiniCitite = 0;//!!!!!!

	if (f != NULL)
	{
		while (!feof(f))
		{
			adaugaMasinaInVector(&vector, nrMasiniCitite, citireMasinaFisier(f));
		}
		
	}
	else
	{
		printf("Eroare la deschiderea fisierului\n");
		//return NULL;
	}

	fclose(f);
	return vector;
}


int main()
{
	int nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);//nrMasini trb transmis prin adresa pt a se modifica!
	afisareVectorMasini(masini, nrMasini);

	dezalocare(&masini, &nrMasini);

	return 0;
}