#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct StructuraFilm Film;
struct StructuraFilm 
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
    Nod* stanga;
    Nod* dreapta;
};


void afisareFilm(Film film)
{
    printf("ID: %d\n", film.id);
    printf("Durata: %d minute\n", film.durata);
    printf("Rating: %.2f\n", film.rating);
    printf("Titlu: %s\n", film.titlu);
    printf("Regizor: %s\n", film.regizor);
    printf("Gen: %c\n\n", film.gen);
}


//În citireFilmDinFisier, trebuie s? faci strtok în aceea?i ordine în care apar valorile pe fiecare linie din fi?ier!!
Film citireFilmDinFisier(FILE* f)
{
    Film film;

    char buffer[256];
    char sep[3] = ",\n";
    fgets(buffer, 255, f);

    film.id = atoi(strtok(buffer, sep));
    film.durata = atoi(strtok(NULL, sep));
    film.rating = atof(strtok(NULL, sep));

    char* aux;

    aux = strtok(NULL, sep);
    film.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
    strcpy(film.titlu, aux);

    aux = strtok(NULL, sep);
    film.regizor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
    strcpy(film.regizor, aux);

    film.gen = strtok(NULL, sep)[0];

    return film;
}


// adauga o noua masina pe care o primim ca parametru in arbore,
// astfel incat sa respecte principiile de arbore binar de cautare
// dupa o anumita cheie pe care o decideti - aici folosim ID
void adaugaFilmInArbore(Nod** radacina, Film filmNou)
{
    if (*radacina)
    {
        if ((*radacina)->info.id < filmNou.id)
        {
            adaugaFilmInArbore(&(*radacina)->dreapta, filmNou);
        }
        if ((*radacina)->info.id > filmNou.id)
        {
            adaugaFilmInArbore(&(*radacina)->stanga, filmNou);
        }
    }
    else
    {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->dreapta = NULL;
        nou->stanga = NULL;
        nou->info = filmNou;
        *radacina = nou; // leg?m nodul nou în arbore
    }
}


Nod* citireArboreDeFilmeDinFisier(const char* numeFisier)
{
    FILE* f = fopen(numeFisier, "r");

    Nod* radacina = NULL;

    if (!f)
    {
        printf("Eroare la deschiderea fisierului!");
        return NULL;
    }

    while (!feof(f))
    {
        adaugaFilmInArbore(&radacina, citireFilmDinFisier(f));
    }

    fclose(f);

    return radacina;
}




//AFISARI
void afisareFilmeInPreordine(Nod* radacina)
{
    if (radacina)
    {
        afisareFilm(radacina->info);
        afisareFilmeInPreordine(radacina->stanga);
        afisareFilmeInPreordine(radacina->dreapta);
    }
   
}

void afisareFilmInPostordine(Nod* radacina)
{
    if (radacina)
    {
        afisareFilmInPostordine(radacina->stanga);
        afisareFilmInPostordine(radacina->dreapta);
        afisareFilm(radacina->info);

    }
}

void afisareFilmeInInordine(Nod* radacina)
{
    if (radacina)
    {
        afisareFilmeInInordine(radacina->stanga);
        afisareFilm(radacina->info);
        afisareFilmeInInordine(radacina->dreapta);
    }
}


void dezalocareArboreDeFilme(Nod** radacina)
{
    if (*radacina) { // dac? nodul exist?

        // dezaloc?m mai întâi subarborele stâng
        dezalocareArboreDeFilme(&(*radacina)->stanga);

        // dezaloc?m apoi subarborele drept
        dezalocareArboreDeFilme(&(*radacina)->dreapta);

        // eliber?m câmpurile alocate dinamic din Masina
        free((*radacina)->info.titlu);
        free((*radacina)->info.regizor);

        // eliber?m nodul curent
        free(*radacina);

        // set?m pointerul la NULL dup? dezalocare
        *radacina = NULL;
    }

}

int nrDeNoduriDinArbore(Nod* radacina)
{
    if (radacina)
    {
        return 1 + nrDeNoduriDinArbore(radacina->dreapta) + nrDeNoduriDinArbore(radacina->stanga);
    }
    return 0;
}

int calculeazaInaltimeArbore(Nod* radacina)
{
    if (radacina)
    {
        int st = calculeazaInaltimeArbore(radacina->stanga);
        int dr = calculeazaInaltimeArbore(radacina->dreapta);

        return 1 + max(st, dr); //sau (st > dr ? st : dr); 

    }

    return 0;
}

float calculeazaDurataTotala(Nod* radacina)
{
    if (radacina)
    {
        return radacina->info.durata + calculeazaDurataTotala(radacina->dreapta) + calculeazaDurataTotala(radacina->stanga);
    }
    return 0;
}


Film getFilmById(Nod* radacina, int id)
{
 
    if (radacina)
    {
        if (radacina->info.id == id)
        {
            Film f = radacina->info;

            f.regizor = (char*)malloc(sizeof(char) * (strlen(radacina->info.regizor) + 1));
            strcpy(f.regizor, radacina->info.regizor);

            f.titlu = (char*)malloc(sizeof(char) * (strlen(radacina->info.titlu) + 1));
            strcpy(f.titlu, radacina->info.titlu);

            return f;
        }

        if (radacina->info.id > id)
        {
           return getFilmById(radacina->stanga, id);
        }

        if (radacina->info.id < id)
        {
           return getFilmById(radacina->dreapta, id);
        }

   
    }

    Film f;
    f.id = -1;
    f.regizor = NULL;
    f.titlu = NULL;

    return f;
}



int main()
{
    Nod* radacina = citireArboreDeFilmeDinFisier("filme_arbore.txt");

    afisareFilmeInPreordine(radacina);

    printf("Nr de noduri: %d\n", nrDeNoduriDinArbore(radacina));
    printf("Inaltimea arborelui este: %d\n\n", calculeazaInaltimeArbore(radacina));


    Film f = getFilmById(radacina, 2);
    afisareFilm(f);

    free(f.titlu);
    free(f.regizor);


    return 0;
}






