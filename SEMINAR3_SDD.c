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


void afisareVectorMasini(Masina* masini, int nrMasini)
{
	//afiseaza toate elemente de tip masina din vector
	//prin apelarea functiei afisareMasina()
	if (masini == NULL || nrMasini == 0)
	{
		printf("Vectorul de masini este gol\n");
		return;
	}

	for (int i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua)
{
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;
	//primesc pointerul prin adresa ca sa pot modifica vectorul

	//temp pt ca este un vector temporar, nu mai folosim vectorul dupa aceasta functie, avem tipul Masina* ca sa alocam dinamic
	//vectorul nou stocheaza masini, iar masinile fiecare in parte au cate 2 pointeri(char* model; char* numeSofer;)

	Masina* temp = (Masina*)malloc(((*nrMasini) + 1) * sizeof(Masina));
	for (int i = 0; i < *nrMasini; i++)
	{
		temp[i] = (*masini)[i];
	}
	temp[*nrMasini] = masinaNoua; //shallow copy, vom avea doi pointeri din 2 vectori diferiti care voi pointa spre aceeasi zona de memorie
	free(*masini);

	*masini = temp; //fara & la temp pt ca temp este deja o adresa
	(*nrMasini)++;
	//temp este pe stiva functiei si dispare la finalul functiei
}


Masina citireMasinaFisier(FILE* file)
{
	//functia citeste o masina dintr-un stream deja deschis
	//masina citita este returnata;

	//buffer / linie = citesc si salvez temporar intr un buffer, este de fapt o linie din fisierul acela text ( o linie = o masina in acel fisier text)
	char linie[256];// 256 este acoperator pentru toate variantele, alocare statica?

	fgets(linie, 255, file); //file = stream ul din care vom citi, 255 pt ca ultimul caracter este pentru termiantorul de sir

	char delimitator[3] = ",\n";//delimitatorii din fisierul text 
	//strtok sparge un sir mare de caractere si returneaza bucati (tokens), va returna mai intai id ul etc..
	Masina masina;
	masina.id = atoi(strtok(linie, delimitator)); //strtok returneaza un string, id este int => trb sa facem cast, nu putem face cast explicit, facem cu atoi = ASCII to int (face conversie)
	masina.nrUsi = atoi(strtok(NULL, delimitator)); //dc null?
	masina.pret = atof(strtok(NULL, delimitator));

	char* aux = strtok(NULL, delimitator); //avem nevoie de acest aux ca sa stim dimensiunea
	masina.model = (char*)malloc((sizeof(char)) * (strlen(aux) + 1));
	strcpy(masina.model, aux);
	//model este pointer deci trb sa ii alocam spatiu

	aux = strtok(NULL, delimitator);
	masina.numeSofer = (char*)malloc((sizeof(char)) * (strlen(aux) + 1));
	strcpy(masina.numeSofer, aux);

	masina.serie = strtok(NULL, delimitator)[0];

	return masina;
}


Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite)
{
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* file = fopen(numeFisier, "r"); //r e modul de citire
	if (!file)
	{
		printf("Eroare la deschiderea fisierului\n");
		return NULL;
	}
	else
	{
		Masina* vectorMasini = NULL;
		*nrMasiniCitite = 0;
		while (!feof(file))
		{
			Masina masina = citireMasinaFisier(file); //returneaza o masina si o stocam in vectorul de masini (voi folosi pt asta o functie care adauga masina)
			adaugaMasinaInVector(&vectorMasini, nrMasiniCitite, masina);
		}
		return vectorMasini;
	}

}



int main()
{
	int nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);

	afisareVectorMasini(masini, nrMasini);

	dezalocareVectorMasini(&masini, &nrMasini);

	//afisareVectorMasini(masini, nrMasini); ruleaza ca sa vezi ca s a realizat dezalocarea corect

	return 0;
}