#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PAS_REDIMENSIONARE 5
#define DIM_INITIALA 5

struct StructuraCarte
{
	int id;
	int nrPagini;
	float pret;
	char* titlu;
	char* autor;
	unsigned char categorie;
};
typedef struct StructuraCarte Carte;

struct HashTable
{
	int dim;
	Carte* vector;
};
typedef struct HashTable HashTable;

Carte citireCarteDinFisier(FILE* file)
{
	char linie[100];
	fgets(linie, 100, file);

	char separator[3] = ",\n";
	Carte c;

	c.id = atoi(strtok(linie, separator));
	c.nrPagini = atoi(strtok(NULL, separator));
	c.pret = atof(strtok(NULL, separator));

	char* aux;

	aux = strtok(NULL, separator);
	c.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(c.titlu, aux);

	aux = strtok(NULL, separator);
	c.autor = (char*)malloc(strlen(aux) + 1);
	strcpy(c.autor, aux);

	c.categorie = strtok(NULL, separator)[0];

	return c;
}
void afisareCarte(Carte carte)
{
	printf("Id: %d\n", carte.id);
	printf("Nr. pagini: %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Categorie: %c\n\n", carte.categorie);
}
HashTable initializareTabelaDispersie(int dimensiune)
{
	HashTable hash;
	hash.dim = dimensiune;
	hash.vector = (Carte*)malloc(sizeof(Carte) * dimensiune);
	for (int i = 0; i < hash.dim; i++)
	{
		hash.vector[i].titlu = NULL;
		hash.vector[i].autor = NULL;
		hash.vector[i].id = -1;
		hash.vector[i].pret = 0;
		hash.vector[i].nrPagini = 0;
		hash.vector[i].categorie = '-';
	}
	return hash;
}
int calculeazaHash(const char* titlu, int dimensiune)
{
	int suma = 0;
	for (int i = 0; i < strlen(titlu); i++)
	{
		suma += titlu[i];
	}
	return suma % dimensiune;
}
int inserareCarteInTabela(HashTable ht, Carte carte)
{
	int poz = calculeazaHash(carte.titlu, ht.dim);
	//cauta in dreapta sa o puna
	for (int i = poz; i < ht.dim; i++)
	{
		if (ht.vector[i].titlu == NULL)
		{
			ht.vector[i] = carte;
			return 1;
		}
	}

	return 0;
}
int cautaCarteInTabela(HashTable ht, const char* titlu)
{
	int poz = calculeazaHash(titlu, ht.dim);

	for (int i = poz; i < ht.dim; i++)
	{
		if (ht.vector[i].titlu == NULL)
		{
			return -1;
		}
		else if (strcmp(ht.vector[i].titlu, titlu) == 0)
		{
			return i;
		}
	}

	return -1;
}
void afisareTabelaDeCarti(HashTable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i].titlu != NULL)
		{
			printf("\nPozitia %d:\n", i);
			afisareCarte(ht.vector[i]);
		}
		else
		{
			printf("\nPe pozitia %d nu exista carti\n", i);
		}
	}
}
HashTable citireCartiDinFisier(const char* numeFisier, int dim)
{
	FILE* file = fopen(numeFisier, "r");
	HashTable ht = initializareTabelaDispersie(dim);

	if (file)
	{
		while (!feof(file))
		{
			Carte carte = citireCarteDinFisier(file);
			int inserat = inserareCarteInTabela(ht, carte); //se incearca inserarea 
			int dimNoua = ht.dim;

			while (inserat == 0) //nu se gaseste loc pentru carte, deci se redimensioneaza tabela;   cat timp nu se gaseste, se mareste tabela
			{
				HashTable htNou = initializareTabelaDispersie(dimNoua + PAS_REDIMENSIONARE);
				dimNoua += PAS_REDIMENSIONARE;

				inserat = 1;
				//mut toate elementele vechi in noua tabela
				for (int i = 0; i < ht.dim && inserat == 1; i++) //facem asta atata timp cat tptul functioneaza normal adica inserat=1
				{
					if (ht.vector[i].titlu != NULL)
					{
						inserat = inserareCarteInTabela(htNou, ht.vector[i]);
					}
				}

				if (inserat == 0) //daca tabela noua nu este buna nici macar pentru vechile elemente
				{
					free(htNou.vector);
				}
				else //daca mutarea a mers
				{
					free(ht.vector);
					ht.vector = htNou.vector;
					ht.dim = htNou.dim;

					inserat = inserareCarteInTabela(ht, carte); //se incearca din nou inserarea ; daca nu merge se reia while-ul
				}
			}
		}
		fclose(file);
	}

	return ht;
}
void dezalocareTabelaDeDispersie(HashTable* ht)
{
	for (int i = 0; i < ht->dim; i++)
	{
		if (ht->vector[i].titlu != NULL)
		{
			free(ht->vector[i].titlu);
		}
		if (ht->vector[i].autor != NULL)
		{
			free(ht->vector[i].autor);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}
int main()
{
	HashTable ht = citireCartiDinFisier("carti.txt", DIM_INITIALA);

	printf("\nTabela initiala:\n");
	afisareTabelaDeCarti(ht);

	dezalocareTabelaDeDispersie(&ht);
	return 0;
}