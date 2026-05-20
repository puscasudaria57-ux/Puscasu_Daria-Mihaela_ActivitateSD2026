#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct Articol Articol;
struct Articol
{
    int id;
    int cantitate;
    float pret;
    char* denumire;
    unsigned char categorie;
};


typedef struct Nod Nod;
struct Nod
{
    Articol info;
    Nod* stanga;
    Nod* dreapta;
};

void afisareArticol(Articol articol)
{
    printf("ID: %d\n", articol.id);
    printf("Cantitate: %d\n", articol.cantitate);
    printf("Pret: %.2f\n", articol.pret);
    printf("Denumire: %s\n", articol.denumire);
    printf("Categorie: %c\n\n", articol.categorie);
}


Articol citireArticolDinFisier(FILE* f)
{
    Articol articol;

    char buffer[256];
    char sep[3] = ",\n";

    fgets(buffer, 255, f);

    articol.id = atoi(strtok(buffer, sep));
    articol.cantitate = atoi(strtok(NULL, sep));
    articol.pret = atof(strtok(NULL, sep));

    char* aux;

    aux = strtok(NULL, sep);
    articol.denumire = (char*)malloc(strlen(aux) + 1);
    strcpy(articol.denumire, aux);

    articol.categorie = strtok(NULL, sep)[0];

    return articol;
}

int calculeazaInaltimea(Nod* radacina)
{
    if (radacina)
    {
        int st = calculeazaInaltimea(radacina->stanga);
        int dr = calculeazaInaltimea(radacina->dreapta);

        return 1 + max(st, dr);
    }
    return 0;
}

int verificaEchilibru(Nod* radacina)
{
    if (radacina)
    {
        int st = calculeazaInaltimea(radacina->stanga);
        int dr = calculeazaInaltimea(radacina->dreapta);

        return st-dr;
    }
    return 0;
}


void rotireStanga(Nod** radacina)
{
    Nod* aux = (*radacina)->dreapta;
    (*radacina)->dreapta = aux->stanga;
    aux->stanga = *radacina;
    *radacina = aux;
}

void rotireDreapta(Nod** radacina)
{
    Nod* aux = (*radacina)->stanga;
    (*radacina)->stanga = aux->dreapta;
    aux->dreapta = *radacina;
    *radacina = aux;
}

void adaugaArticolInArboreDeEchilibru(Nod** radacina, Articol articolNou)
{
    if (*radacina)
    {
        if ((*radacina)->info.id > articolNou.id)
        {
            adaugaArticolInArboreDeEchilibru(&(*radacina)->stanga, articolNou);
        }
        if ((*radacina)->info.id < articolNou.id)
        {
            adaugaArticolInArboreDeEchilibru(&(*radacina)->dreapta, articolNou);
        }

        int gradEchilibru = verificaEchilibru(*radacina);
        if (gradEchilibru == -2)
        {
            if (verificaEchilibru((*radacina)->dreapta) <= 0)
            {
                rotireStanga(radacina);
            }
            else
            {
                rotireDreapta(&(*radacina)->dreapta);
                rotireStanga(radacina);
            }
        }
        if (gradEchilibru == 2)
        {
            if (verificaEchilibru((*radacina)->stanga) >= 0)
            {
                rotireDreapta(radacina);
            }
            else
            {
                rotireStanga(&(*radacina)->stanga);
                rotireDreapta(radacina);
            }
        }

    }
    else
    {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->stanga = NULL;
        nou->dreapta = NULL;
        nou->info = articolNou;
        *radacina = nou;
    }
}


Nod* citireArboreDeEchilibruDinFisier(const char* numeFisier)
{
    FILE* f = fopen(numeFisier, "r");
    if (!f)
    {
        printf("err");
        return NULL;
    }
    Nod* radacina = NULL;

    while (!feof(f))
    {
        adaugaArticolInArboreDeEchilibru(&radacina, citireArticolDinFisier(f));
    }

    return radacina;
}


void afisareArboreInPreordine(Nod* radacina)
{
    if (radacina)
    {
        afisareArticol(radacina->info);
        afisareArboreInPreordine(radacina->stanga);
        afisareArboreInPreordine(radacina->dreapta);
    }
   
}


int nrNoduri(Nod* radacina)
{
    if (radacina)
    {
        return 1 + nrNoduri(radacina->stanga) + nrNoduri(radacina->dreapta);
    }
    return 0;
}

float calcPretArticole(Nod* radacina)
{
    if (radacina)
    {
        return radacina->info.pret + calcPretArticole(radacina->stanga) + calcPretArticole(radacina->dreapta);
    }
    return 0;
}


void dezalocareArbore(Nod** radacina)
{
    if (*radacina)
    {
        dezalocareArbore(&(*radacina)->dreapta);
        dezalocareArbore(&(*radacina)->stanga);

        free((*radacina)->info.denumire);

        free(*radacina);
        *radacina = NULL;
    }
}

Articol getArticolBYID(Nod* radacina, int id)
{
    if (radacina)
    {
        if (radacina->info.id == id)
        {
            Articol a = radacina->info;

            a.denumire = (char*)malloc(sizeof(char) * (strlen(radacina->info.denumire) + 1));
            strcpy(a.denumire, radacina->info.denumire);

            return a;
        }

        if (radacina->info.id > id)
        {
            return getArticolBYID(radacina->stanga, id);
        }
        if (radacina->info.id < id)
        {
            return getArticolBYID(radacina->dreapta, id);
        }
    }

    Articol a;
    a.id = -1;
    a.denumire = NULL;
    return a;
}

int main()
{
    Nod* radacina = citireArboreDeEchilibruDinFisier("articole.txt");
    afisareArboreInPreordine(radacina);


    Articol a = getArticolBYID(radacina, 2);
    if (a.id != -1)
    {
        printf("Articolul cu id ul cautat este:\n");
        afisareArticol(a);
        free(a.denumire);
    }
    else
    {
        printf("Articolul nu a fost gasit.\n");
    }


    dezalocareArbore(&radacina);

    

    return 0;
}