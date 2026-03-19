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

//creare structura pentru un nod dintr-o lista dublu inlantuita

//creare structura pentru Lista Dubla 

struct Nod
{
	struct Nod* prev; //previous
	struct Nod* next;
	Masina info;
};
typedef struct Nod Nod;


struct ListaDubla
{
	Nod* first;
	Nod* last;
};
typedef struct ListaDubla ListaDubla;



Masina citireMasinaDinFisier(FILE* file)
{
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


void afisareMasina(Masina masina)
{
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniDeLaInceput(/*lista dubla de masini*/ ListaDubla listaDubla)
{
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()

	//afisarea listei se poate face ori de la inceput ori de la sfarsit
	Nod* aux = listaDubla.first;  //pointer cu care ne deplasam
	while (aux)
	{
		afisareMasina(aux->info);
		aux = aux->next;
	}
}


void afisareListaMasiniDeLaSfarsit(/*lista dubla de masini*/ ListaDubla listaDubla)
{
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()

	//afisarea listei se poate face ori de la inceput ori de la sfarsit
	Nod* aux = listaDubla.last;  //pointer cu care ne deplasam
	while (aux)
	{
		afisareMasina(aux->info);
		aux = aux->prev;
	}
}

//* ca sa modificam
void adaugaMasinaInListaLaFinal(/*lista dubla de masini*/ ListaDubla* listaDubla, Masina masinaNoua)
{
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou;
	nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua; //shallow copy pt ca ..
	nou->next = NULL;
	nou->prev = listaDubla->last;
	if (listaDubla->last != NULL)
	{
		listaDubla->last->next = nou; //nou este deja o adresa, trebuie next pt ca trebuie sa facem legatura (last practic este un nod si nextul lui trb sa fie setat)
		//listaDubla->last = nou;//poate fi pe ambele ramuri
	}
	else
	{
		listaDubla->first = nou;
	}
	listaDubla->last = nou;
}

//va fi in oglinda cu functia de mai sus
void adaugaLaInceputInLista(/*lista dubla de masini*/ ListaDubla* listaDubla, Masina masinaNoua)
{
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* nou;
	nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = listaDubla->first;
	if (listaDubla->first != NULL)
	{
		listaDubla->first->prev = nou; //nou este deja o adresa, trebuie next pt ca trebuie sa facem legatura (last practic este un nod si nextul lui trb sa fie setat)
		//listaDubla->last = nou;//poate fi pe ambele ramuri
	}
	else
	{

		listaDubla->last = nou;
	}
	listaDubla->first = nou; //(*listaDubla).first = nou;

}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier)
{
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()


	FILE* fisier = fopen(numeFisier, "r");
	Masina masina;
	ListaDubla listaMasini;
	listaMasini.first = NULL;
	listaMasini.last = NULL;
	while (!feof(fisier))
	{
		masina = citireMasinaDinFisier(fisier);
		adaugaLaInceputInLista(&listaMasini, masina); //o adaugam cu shallow copy, de ce?
	}
	//ATENTIE - la final inchidem fisierul/stream-ul
	fclose(fisier);
	return listaMasini;

}

void dezalocareLDMasini(/*lista dubla de masini*/ListaDubla* listaMasini)
{
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente

	Nod* aux;
	if (listaMasini->first->next)
	{
		aux = listaMasini->first->next;

		while (aux)
		{
			free((aux->prev->info).model);
			free((aux->prev->info).numeSofer);
			free(aux->prev);

			aux = aux->next;
		}
		free(listaMasini->last->info.model);
		free(listaMasini->last->info.numeSofer);
		free(listaMasini->last);
		listaMasini->first = NULL;
		listaMasini->last = NULL;

	}
}

float calculeazaPretMediu(/*lista de masini*/ListaDubla listaMasini)
{
	//calculeaza pretul mediu al masinilor din lista.
	Nod* masina = listaMasini.first;
	float Suma = 0, nr = 0;
	while (masina)
	{
		nr++;
		Suma += masina->info.pret;
		masina = masina->next;
	}
	if (nr != 0)
		return Suma / nr;

	return 0;
}

void stergeMasinaDupaID(/*lista masini*/ int id)
{
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/)
{
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}



int main()
{

	ListaDubla listaMasini = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(listaMasini);

	dezalocareLDMasini(&listaMasini);
	afisareListaMasiniDeLaInceput(listaMasini);


	return 0;
}