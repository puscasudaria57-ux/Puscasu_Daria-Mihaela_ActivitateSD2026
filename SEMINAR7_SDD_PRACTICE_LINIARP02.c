#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PAS_REDIMENSIONARE 5
#define DIM_INITIALA 5

typedef struct Film Film;
struct Film
{
		int id;
	    int durata;
	    float rating;
	    char* titlu;
	    char* regizor;
	    char gen;
};

typedef struct HashTable HashTable;
struct HashTable
{
	int dim;
	Film* vector; //vector de structuri Film
};


Film citireFilmDinFisier(FILE* f)
{
	Film film;
	char buffer[256];
	fgets(buffer, 255, f);
	char sep[3] = ",\n";

		film.id = atoi(strtok(buffer, sep));
		film.durata = atoi(strtok(NULL, sep));
	    film.rating = atof(strtok(NULL, sep));
	
	    char* aux; //avem nevoie de acest aux ca sa aflam lungimea sirului de caractere titlu si regizor si sa putem aloca dinamic
	    aux = strtok(NULL , sep);//aux acum va retine titlul
	    film.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	    strcpy(film.titlu, aux);
	
	    //continuam si facem la fel si pt regizor
	    aux = strtok(NULL, sep); //aux acum va retine regizorul
	    film.regizor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	    strcpy(film.regizor, aux);
	
	    film.gen = strtok(NULL, sep)[0]; //fiind un char adica un singur caracter, avem 2 pozitii: caracterul si \n, dar pe noi
	    //ne intereseaza doar primul caracter : [0]

	return film;
}


//functie care afiseaza un film
void afisareFilm(Film film)
{
    printf("ID: %d\n", film.id);
    printf("Durata: %d\n", film.durata);
    printf("Rating: %.2f\n", film.rating);
    printf("Titlu: %s\n", film.titlu);
    printf("Regizor: %s\n", film.regizor);
    printf("Gen: %c\n\n", film.gen);
}

HashTable initializareHash(int dimensiune)
{
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Film*)malloc(sizeof(Film)* dimensiune);//aloc dinamic tabela de dispersie
	for (int i; i < dimensiune; i++)
	{
		ht.vector[i].id = -1;
		ht.vector[i].durata = -1;
		ht.vector[i].rating = 0;
		ht.vector[i].titlu = NULL;
		ht.vector[i].regizor = NULL;
		ht.vector[i].gen = '-';
	}
	return ht;
}

int functieHash(const char* titlu, int dimensiune)
{
	int suma = 0;
	for (int i = 0; i < strlen(titlu); i++)
	{
		suma = suma + titlu[i];
	}
	return suma % dimensiune;
}


int inserareFilmInTabela(HashTable ht, Film film)
{
	int poz = functieHash(film.titlu, ht.dim);
	for (int i = poz; i < ht.dim; i++)
	{
		if (ht.vector[i].titlu == NULL) //daca acea pozitie este libera, voi insera filmul acolo
		{
			ht.vector[i] = film;
			return 1;
		}
	}
	return 0;
}

void dezalocaUnSingurFilm(Film* f)
{
	if (f->regizor != NULL)
	{
		free(f->regizor);
		f->regizor = NULL;
	}
	if (f->titlu != NULL)
	{
		free(f->titlu);
		f->titlu = NULL;
	}
}

void dezalocaTabela(HashTable* ht)
{
	for (int i = 0; i < ht->dim; i++)
	{
		dezalocaUnSingurFilm(&(ht->vector[i])); //ht->vector[0] este prima carte din tabela
	}
	free(ht->vector);
	ht->vector = NULL;
}

void afisareTabelaFilme(HashTable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i].titlu != NULL) //ne ducem la un atribut si vedem daca e null sau nu ca sa stim daca afisam
		{
			printf("Filmul de pe pe pozitia %d este: ", i);
			afisareFilm(ht.vector[i]);
		}
		else
		{
			printf("Pe pozitia %d nu exista film!\n", i);
		}
	}
}

//cauta filmul dupa titlu si afiseaza pozitia
int cautaFilmDupaTitlu(HashTable ht, const char* titluCautat)
{
	int poz = functieHash(titluCautat, ht.dim);

	for (int i = poz; i < ht.dim; i++)
	{
		if (ht.vector[i].titlu != NULL)
		{
			if (strcmp(ht.vector[i].titlu, titluCautat) == 0)
			{
				return i;
			}
		}
		else
		{
			// dac? g?se?ti pozi?ie liber? ? nu exist?
			return -1;
		}
	}

	return -1;
}

int main()
{
	return 0;
}