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
typedef struct StructuraMasina Masina; //alias, in loc sa scrii peste tot struct StructuraMasina vei scrie: Masina 


//Realiza?i o func?ie care ini?ializeaz? o ma?in? cu valori primite ca parametri.
//
//aloca?i dinamic memorie pentru model ?i numeSofer
//copia?i valorile primite

//functia va returna o Masina, valorile campurilor vor fi primite ca parametru
Masina initializare(int id, int nrUsi, float pret, const char* model, const char* numeSofer, unsigned char serie)
{
	Masina m; //declar o masina m de tipul Masina

	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	m.serie = serie;

	m.model = (char*)malloc(sizeof(char) * (strlen(model) + 1));
	strcpy(m.model, model);

	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(numeSofer) + 1));
	strcpy(m.numeSofer, numeSofer);

	return m; //returnez masina pt ca asta returneaza functia
}

void afisareMasina(Masina masina)//void ca nu returneaza nimic, folosim printf, primeste ca parametru o Masina pt ca asta vrem sa afisam
{
		printf("ID: %d\n", masina.id);
		printf("Nr.usi : %d\n", masina.nrUsi);
		printf("Pret: %.2f\n", masina.pret);
		printf("Model: %s\n", masina.model);
		printf("Nume sofer: %s\n", masina.numeSofer);
		printf("Serie: %c\n\n", masina.serie);
}


void dezalocare(Masina* m) //pointer pt ca modifici, m acum este un pointer, adica retine o adresa de memorie => deci ca tu
//sa ajungi la informatia de la acea adresa trebuie sa faci dereferentiere si accesare ca sa o poti accesa si sa verifici daca NULLA
{
	if ((*m).model != NULL)
	{
		free((*m).model);
		m->model = NULL;
	}
		
	if (m->numeSofer != NULL)
	{
		free(m->numeSofer);
		m->numeSofer = NULL;
	}
		
}


//Realiza?i o func?ie care modific? modelul unei ma?ini.
//PASII:
//1)elibera?i memoria veche : free(m->model);
//2)aloca?i memorie nou? : m->model = malloc(...);
//3)copia?i noul model : strcpy(m->model, modelNou);
void modificaModel(Masina* m, char* modelNou) 
{
	//eliberez memoria
	free(m->model);
	
	//aloc dinamic memorie modelului pt noul model
	m->model = (char*)malloc(sizeof(char) * (strlen(modelNou) + 1));

	//copiez in model in structura modelul nou
	strcpy(m->model, modelNou);
}
//modelNou = sursa(ce prime?ti)
//m->model = destina?ia(unde salvezi)

//Realiza?i o func?ie care modific? numele ?oferului unei ma?ini.
void modificaNumeSofer(Masina* m, char* numeNou)
{
	free(m->numeSofer);
	m->numeSofer = (char*)malloc(sizeof(char) * (strlen(numeNou) + 1));
	strcpy(m->numeSofer, numeNou);
}



int main()
{
	Masina m1 = initializare(1, 3, 5000, "Astra", "Ionescu", 'A');
	afisareMasina(m1);

	modificaModel(&m1, "Audi");
	afisareMasina(m1);

	//dezalocare(&m1);
	//afisareMasina(m1);

	return 0;

}