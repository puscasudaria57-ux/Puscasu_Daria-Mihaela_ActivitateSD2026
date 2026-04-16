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


//Realizați o funcție care inițializează o mașină:
Masina initializare(int _id, int _nrUsi, float _pret, const char* _model, const char* _numeSofer, unsigned char _serie)
{
	Masina m;
	
	m.id = _id;
	m.nrUsi = _nrUsi;
	m.pret = _pret;
	
	m.model = (char*)malloc(sizeof(char) * (strlen(_model) + 1));
	strcpy(m.model, _model);

	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(_numeSofer) + 1));
	strcpy(m.numeSofer, _numeSofer);

	m.serie = _serie;


	return m;
}

//Realizați o funcție care afișează o mașină.
void afisareMasina(Masina masina)
{
		printf("ID: %d\n", masina.id);
		printf("Nr usi: %d\n", masina.nrUsi);
		printf("Pret: %5.2f\n", masina.pret);
		printf("Model: %s\n", masina.model);
		printf("Nume sofer: %s\n", masina.numeSofer);
		printf("Seria: %c\n\n", masina.serie);
}

//Realizați o funcție care afișează un vector de mașini.
//Masina* masini = vectorul de masini (masini este un pointer care retine adresa de memorie a primului element din vector
void afisareVectorDeMasini(Masina* masini, int nrMasini)//un singur * pt ca nu modifici vectorul, vrei doar sa afisezi!
{
	if (masini == NULL || nrMasini == 0)
	{
		printf("Vectorul este gol!\n");
		return; //ies din start din functie
	}

	printf("Vectorul de masini:\n");
	for (int i = 0; i < nrMasini; i++) //parcurg vectorul de masini si pentru fiecare masina apelez functia de afisareMasina
	{
		afisareMasina(masini[i]);
	}
}

//Realizați o funcție care dezalocă complet un vector de mașini
void dezalocare(Masina** m, int* nrMasini)//e ca la lista, ai nevoie de ** ca sa modifici!
{
	for (int i = 0; i < *nrMasini; i++)
	{
		if ((*m)[i].model != NULL)
		{
			free((*m)[i].model);
		}
		if ((*m)[i].numeSofer != NULL)
		{
			free((*m)[i].numeSofer);
		}
	}

	free(*m); //sterg intregul vector la final

	*m = NULL; //m nu va mai pointa spre nimic
	*nrMasini = 0; //nr de elemente va fi 0
}

Masina copy_masina(Masina masina)
{
	return initializare(masina.id, masina.nrUsi,masina.pret, masina.model, masina.numeSofer, masina.serie);
}

//Realizați o funcție care copiază primele N elemente dintr-un vector de mașini într-un vector nou.
//deep copy!
//functia va returna un vector de masini : Masina* !!!
Masina* copiazaPrimeleMasiniDinVector(Masina* vector, int nrElemente, int nrElementeCopiate)
{
	if (nrElementeCopiate > nrElemente)
	{
		nrElementeCopiate = nrElemente;
	}


	Masina* vectorNou = NULL; //VECTOR NOU, NULL INITIAL
	vectorNou = (Masina*)malloc(sizeof(Masina) * nrElementeCopiate);//ALOC MEMORIE PT VECTORUL NOU

	for (int i = 0; i < nrElementeCopiate; i++)
	{
		vectorNou[i] = copy_masina(vector[i]);
	}

	return vectorNou;
}


//Realizați o funcție care creează un nou vector ce conține doar mașinile cu prețul mai mare decât o valoare dată.
//primim in antet ca parametru direct vectorul cu ** astfel incat sa il putem modifica
void copiazaMasiniScumpe(Masina* masini, int nrMasini, float pragPret, Masina** vectorNou, int* dimensiune)
{
	*dimensiune = 0;
	for (int i = 0; i < nrMasini; i++)
	{
		//aflu dimensiunea noului vector!
		if (masini[i].pret > pragPret)
			(*dimensiune)++;
	}

	if ((*vectorNou) != NULL) //extra masura nu e neaparat
			{
				free((*vectorNou));
			}
		
			//aloc memorie noua si dupa mai jos copiez masinile scumpe in noul vector
			*vectorNou = (Masina*)malloc(sizeof(Masina) * (*dimensiune));

	int k = 0;
	for (int i = 0; i < nrMasini; i++)
	{
		if (masini[i].pret > pragPret)
		{
			(*vectorNou)[k++] = copy_masina(masini[i]);
		}
	}
}


//Realizați o funcție care returnează prima mașină condusă de un anumit șofer.
Masina getPrimaMasinaSoferDat(Masina* masini, int nrMasini, const char* soferDat)
{
	Masina masina;
	int gasit = 0;
	for (int i = 0; i < nrMasini&& gasit == 0; i++)
	{
		if (strcmp(masini[i].numeSofer, soferDat) == 0)
		{
			return masini[i];
			gasit = 1;
		}
	}

}

//SAU

//Realizați o funcție care returnează prima mașină condusă de un anumit șofer.
Masina getPrimaMasinaSoferDat2(Masina* masini, int nrMasini, const char* soferDat)
{
	Masina masina;
	int gasit = 0;

	masina.numeSofer = NULL; // in caz ca nu exista numele respectiv sa se afiseze null
	
	for (int i = 0; i < nrMasini && gasit == 0; i++)
	{
		if (strcmp(masini[i].numeSofer, soferDat) == 0)
		{
			masina = copy_masina(masini[i]);
			gasit = 1;
		}
	}
	return masina;

}

int main()
{
	int nrMasini = 2;
	Masina* m = (Masina*)malloc(sizeof(Masina) * nrMasini);
	m[0] = initializare(1, 3, 5000, "Astra", "Ionescu", 'A');
	m[1] = initializare(2, 4, 4500, "Audi", "Ionescu", 'S');

	afisareVectorDeMasini(m,nrMasini);

	
	int nrElemCopiate = 1;
	Masina* primeleMasini = copiazaPrimeleMasiniDinVector(m, nrMasini, nrElemCopiate);
	afisareVectorDeMasini(primeleMasini,nrElemCopiate);
	
	//dezalocare(&m, nrMasini);
	/*dezalocare(&primeleMasini, nrElemCopiate);
	afisareVectorDeMasini(m,nrMasini);
	afisareVectorDeMasini(primeleMasini, nrElemCopiate);*/

	int dimensiune = 0;
	Masina* masiniScumpe = NULL;//nu mai aloc dinamic ca deja am alocat in fctie
	copiazaMasiniScumpe(m, nrMasini, 4700, &masiniScumpe, &dimensiune);//nu mai retin asta in nimic pt ca se modifica direct
	//atributele cu &
	afisareVectorDeMasini(masiniScumpe,dimensiune);

	printf("Masina cautata dupa un nume\n");
	Masina masina = getPrimaMasinaSoferDat2(m, nrMasini, "Ionescu");
	afisareMasina(masina);

	return 0;

}