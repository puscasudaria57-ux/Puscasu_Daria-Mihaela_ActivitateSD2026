#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Film
{
    int id;
    char* titlu;
    float durata;
    char gen; //un singur caracter
    int rating;//intre 0-10
};

//aceasta functie de initializare va returna o variabila de tip struct Film
struct Film initializare(int id, char* titlu, float durata, char gen, int rating)
{
    struct Film f;

    f.id = id;

    f.titlu = (char*)malloc(sizeof(char) * (strlen(titlu) + 1));
    strcpy(f.titlu, titlu);

    f.durata = durata;
    f.gen = gen;
    f.rating = rating;

    return f;
    //return f trimite o copie a structurii catre apelant (main)
    //Variabila locala f dispare dupa ce functia se termina, dar copia returnata ramane in main
}

void afisare(struct Film f)
{
    printf("ID ul filmului este: %d\n", f.id);
    printf("Titlul filmului este: %s\n", f.titlu);
    printf("Durata filmului este: %5.2f\n", f.durata);
    printf("Genul filmului este : %c\n", f.gen);
    printf("Rating ul filmului este: %d\n \n", f.rating);
}



void dezalocare(struct Film* f)
{
    if (f->titlu != NULL)
    {
        free(f->titlu);
        f->titlu = NULL;
    }
}


void modifica_Titlu(struct Film* f, char* nouTitlu)
{
    free(f->titlu); //eliberez memoria din heap

    f->titlu = (char*)malloc(sizeof(char) * (strlen(nouTitlu) + 1));//realoc pentru noul titlu
    strcpy(f->titlu, nouTitlu);

}

void modifica_Durata(struct Film* f, float nouaDurata)
{
    f->durata = nouaDurata;
}


int esteFilmBun(struct Film f)
{
    if (f.rating >= 5)
        return 1;
    return 0;

    //sau:  return (f.rating >= 5);
}

int main()
{
    struct Film f1;
    f1 = initializare(1, "Vizavi", 120, 'D', 6); //se apeleaza functia de initializare
    afisare(f1);

    modifica_Titlu(&f1, "His and Hers");
    afisare(f1);

    modifica_Durata(&f1, 98.345);
    afisare(f1);

    if (esteFilmBun(f1) == 1) //se apeleaza functia pentru structura f1
        printf("Filmul este bun!\n");
    else
        printf("Filmul nu este bun!\n");

    dezalocare(&f1);

    return 0;
}