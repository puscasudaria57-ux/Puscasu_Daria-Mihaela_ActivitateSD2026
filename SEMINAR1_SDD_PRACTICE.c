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




int main()
{
    struct Film f1;
    f1 = initializare(1, "Vizavi", 120, 'D', 6); //se apeleaza functia de initializare
    afisare(f1);


    dezalocare(&f1);

    return 0;
}