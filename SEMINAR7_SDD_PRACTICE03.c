#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct Nod Nod;
struct Nod
{
	Film info;
	Nod* next; //pointer la urmatorul nod din lista
};

//STRUCTURA PT TABELA DE DISPERSIE
typedef struct HashTable HashTable;
struct HashTable
{
	int dim;
	Nod** vector; //vector de pointeri la Nod (la primul nod al fiecarei liste de pe fiecare pozitie)
};


//citirea unui film din fisier
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


void afisareListaFilme(Nod* cap)
{
	while (cap)
	{
		afisareFilm(cap->info);
		cap = cap->next;
	}
}

void adaugaFilmInListaLaFinal(Nod** cap, Film filmNou)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = filmNou;
	nou->next = NULL;
	if (*cap != NULL) //daca lista nu este goala, cap ramane la locul lui
	{
		Nod* aux = *cap;
		while (aux->next!=NULL)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
	else
	{
		*cap = nou;//cap va pointa spre primul nod din lista
	}
}


HashTable initializareHashTable(int dimensiune)
{
	HashTable ht;

	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune); //am alocat spatiu dinamic pentru tabela (ht.vector)
	for (int i = 0; i < dimensiune; i++)
	{
		ht.vector[i] = NULL; //toate elementele din tabela vor fi NULL
	}

	return ht;
}

int functieHash(int id, int dimensiune)
{
	id = id * 45 + 234;
	return id % dimensiune;
}

void inserareFilmInTabela(HashTable ht, Film film)
{
	int poz = functieHash(film.id, ht.dim);
	adaugaFilmInListaLaFinal(&(ht.vector[poz]), film);

}

HashTable citireTabelaDeFilmeDinFisier(const char* numeFisier, int dimensiune)
{
	HashTable ht = initializareHashTable(dimensiune); //INITIALIZAREA TABELEI VA AVEA LOC AICI	
	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			inserareFilmInTabela(ht, citireFilmDinFisier(f));
		}
	}

	fclose(f);
	return ht;
}


//afisareTabelaDeMasini
void afisareTabelaDeFilme(HashTable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			printf("Pe pozitia %d avem urmatorul cluster:\n", i);
			afisareListaFilme(ht.vector[i]);
		}
		else
		{
			printf("\nPe pozitia %d nu avem niciun film!\n", i);
		}
	}
}

void dezalocareLista(Nod** cap)
{
	while (*cap != NULL)
	{
		Nod* aux = *cap;
		*cap = aux->next;

		if (aux->info.titlu != NULL)
			free(aux->info.titlu);
		if (aux->info.regizor != NULL)
			free(aux->info.regizor);

		free(aux);
		
	}
	*cap = NULL;
}

void dezalocareTabela(HashTable* ht)
{
	for (int i = 0; i < ht->dim; i++)
	{
		dezalocareLista(&(ht->vector[i]));
	}

	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}


//C?utarea unui film dup? ID
Film getFilmByID(HashTable ht, int idCautat)
{
	Film f;
	f.id = -1;
	//aflu mai intai pozitia pe care s ar putea afla clusterul cu filmul cu id cautat
	int poz = functieHash(idCautat, ht.dim);
	if (poz >= 0 && poz < ht.dim) //oricum nu are cum sa nu treaca de acest if pt ca ne am asigurat in functia de hash de asta
	{
		Nod* aux = ht.vector[poz];
		while (aux)
		{
			if (aux->info.id == idCautat)
			{
				f = aux->info;

				//deep copy
				f.regizor = (char*)malloc(sizeof(char) * (strlen(aux->info.regizor) + 1));
				strcpy(f.regizor, aux->info.regizor);

				f.titlu = (char*)malloc(sizeof(char) * (strlen(aux->info.titlu) + 1));
				strcpy(f.titlu, aux->info.titlu);

			}
			aux = aux->next;

		}
	}
	return f;
}

//S? se calculeze ratingul mediu al filmelor din fiecare cluster al tabelei de dispersie.Func?ia va returna un vector de valori reale
float* getRatingMediu(HashTable ht, int* nrClustere)
{
	float* ratinguriMedii;

	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			(*nrClustere)++;
		}
	}
	//acum aloc dinamic stiind pt cat spatiu trebuie sa aloc dinamic
	ratinguriMedii = (float*)malloc(sizeof(float) * (*nrClustere));


	int k = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			int contor = 0;
			float suma = 0;
			Nod* aux = ht.vector[i];
			while (aux)
			{
				suma = suma + aux->info.rating;
				contor++;
				aux = aux->next;
			}

			ratinguriMedii[k++] = suma / contor;
		}
	}

	return ratinguriMedii;
}


//S? se determine num?rul de filme aflate în fiecare cluster al tabelei de dispersie.
int* getNrFilme(HashTable ht, int* nrClustere)
{
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			(*nrClustere)++;
		}
	}

	int* nrFilme = (int*)malloc(sizeof(int) * (*nrClustere));
	int k = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			int contor = 0;
			Nod* aux = ht.vector[i];
			while (aux)
			{
				contor++;
				aux = aux->next;
			}
			nrFilme[k++] = contor;
		}
	}
	return nrFilme;
}


//S? se determine filmul cu rating maxim din întreaga tabel? de dispersie.
Film getFilmRatingMaxim(HashTable ht)
{
	Film max;
	max.id = -1;
	max.rating = -1; //pp ca acest film are rating ul maxim si compar cu el urmatoarele filme

	for (int i = 0; i < ht.dim; i++)
	{
		Nod* aux = ht.vector[i];
		while (aux)
		{
			if (aux->info.rating > max.rating)
			{
				max = aux->info;//shallow copy

			}

			aux = aux->next;
		}
	}

	//daca nu exist? niciun film
	if (max.id == -1)
		return max;

	Film deReturnat = max; //deep copy
	deReturnat.regizor = (char*)malloc(sizeof(char) * (strlen(max.regizor) + 1));
	strcpy(deReturnat.regizor, max.regizor);

	deReturnat.titlu = (char*)malloc(sizeof(char) * (strlen(max.titlu) + 1));
	strcpy(deReturnat.titlu, max.titlu);


	return deReturnat;
}


int main()
{
	HashTable ht = citireTabelaDeFilmeDinFisier("filme.txt", 7);
	afisareTabelaDeFilme(ht);

	printf("===============================\n\n");
	Film f = getFilmByID(ht, 1);
	afisareFilm(f);

	int nrClustere;
	float* rMed = getRatingMediu(ht, &nrClustere);
		for (int i = 0; i < nrClustere; i++)
		{
			printf("%.2f\n", rMed[i]);
		}
	

		int nrElem;
		int* nrFilme = getNrFilme(ht, &nrElem);
		for (int i = 0; i < nrElem; i++)
		{
			printf("%d\n", nrFilme[i]);
		}

		Film rMax = getFilmRatingMaxim(ht);
		afisareFilm(rMax);
	return 0;
}