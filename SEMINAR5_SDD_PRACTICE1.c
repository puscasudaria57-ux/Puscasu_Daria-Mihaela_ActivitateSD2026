#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

typedef struct Nod Nod;
struct Nod
{
	Masina info;
	Nod* next;
	Nod* prev; //avem previous si next (lista dubla retine adresa urmatorului nod si a nodului precedent)
};


//Crearea strcuturii pt lista dubla!
typedef struct ListaDubla ListaDubla;
struct ListaDubla
{
	Nod* cap;
	Nod* coada;  
};


void afisareMasina(Masina masina)
{
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

// listaDubla.cap->info; informatia din primul nod
void afisareMasinaDeLaInceput(ListaDubla listaDubla) //la LSI: Nod* cap, dar aici avem nodurile in structura respectiva
//si in acea structura ei sunt deja pointeri
{
	Nod* aux = listaDubla.cap;  //pointer cu care ne deplasam, aux ia adresa primului nod (capul listei)
	while (aux != NULL)
	{
		afisareMasina(aux->info); 
		aux = aux->next; //avansez in lista de la inceput spre final

	}
}



void afisareMasinaDeLaFinal(ListaDubla listaDubla)
{
	Nod* aux = listaDubla.coada; //parcurgem de la coada spre cap

	while (aux != NULL) //la nodul de la inceput avem NULL, acolo ar trebui sa se opreasca parcurgerea listei
	{
		afisareMasina(aux->info);
		aux = aux->prev; //avansez in lista de la final spre inceput
	}
}


//este in oglinda cu adaugarea de mai jos
void adaugaMasinaLaInceputulListei(ListaDubla* listaDubla, Masina masinaNoua)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = listaDubla->cap;

	if (listaDubla->cap != NULL)//daca exista nod in stanga nodului adaugat, capul va pointa spre primul nod nou adaugat 
	{
		listaDubla->cap->prev = nou;//nou este deja o adresa, trebuie next pt ca trebuie sa facem legatura (last practic este un nod si nextul lui trb sa fie setat
	}
	else
	{
		//dc noul nod este singurul din lista=>va pointa si coada spre el
		listaDubla->coada = nou;//(*listaDubla).first = nou;
	}
	listaDubla->cap = nou;//se executa pt ambele cazuri pt ca ai adaugat un nod la inceput si trb cap sa pointeze spre el


}



//este in oglinda cu adaugarea de mai sus
void adaugaMasinaLaSfarsitulListei(ListaDubla* listaDubla, Masina masinaNoua)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = listaDubla->coada;

	//ACUM COADA NU MAI POINTEAZA UNDE TREBUIE, ACUM VA TREBUIE SA POINTEZE CATRE ULTIMUL NOD (adica catre nodul nou!)
	//deci daca suntem in situatia in care lista are noduri => dupa ce adaugi nodul in lista, coada va pointa spre
	//penultimul nod => listaDubla -> coada va fi diferit de NULL si daca se intampla asta vrem coada sa pointeze
	//spre ultimul nod (noul nod adaugat)
	if (listaDubla->coada != NULL)
	{
		listaDubla->coada->next = nou;
	}
	else
	{
		//aici se intra cand lista este goala si adaugam un element (adica listaDubla->coada si listaDubla->cap sunt NULL)!
		//si cap si coada vor pointa spre acelasi nod
		
		listaDubla->cap = nou;
	}
	listaDubla->coada = nou; //se executa no matter what
	 
}



Masina citireMasinaFisier(FILE* f)
{
	Masina m;

	char buffer[256];
	fgets(buffer, 255, f);
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


//void* (Nod*) dar listaDubla deja contine pointeri!
ListaDubla citireLDMasiniDinFisier(const char* numeFisier)
{
	ListaDubla listaMasini;
	listaMasini.cap = NULL; //lista va fi initial NULLA!
	listaMasini.coada = NULL;


	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			adaugaMasinaLaSfarsitulListei(&listaMasini, citireMasinaFisier(f));
		}
	}
	else
	{
		printf("Eroare la deschiderea fiserului!\n");
	}

	fclose(f);
	return listaMasini;
}

void dezalocare(ListaDubla* listaDubla)
{
	Nod* p = listaDubla->cap;  //= (*listDubla).cap
	while (p != NULL)
	{
		Nod* aux = p;
		p = p->next;//inaintez cu p si sterg cu aux

		if (aux->info.model != NULL)
			free(aux->info.model);
		if (aux->info.numeSofer != NULL)
			free(aux->info.numeSofer);

		//acum stergem efectiv nodul
		free(aux);
	}
	//la iesirea din while au fost sterse toate nodurile
	//acum si pointerii cap si coada trb sa fie nulle
	listaDubla->cap = NULL;
	listaDubla->coada = NULL;
	
}




int main()
{

	ListaDubla ld = citireLDMasiniDinFisier("masini.txt");
	afisareMasinaDeLaFinal(ld);
	afisareMasinaDeLaInceput(ld);

	return 0;
}