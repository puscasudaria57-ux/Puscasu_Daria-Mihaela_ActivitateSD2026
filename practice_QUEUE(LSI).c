#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Produs Produs;
struct Produs
{
    int cod;
    float pret;
    char* denumire;    // dinamic
    char* categorie;   // dinamic
    char tip;          // nou (ex: 'A' = alimentar, 'B', 'C')
};

typedef struct Nod Nod;
struct Nod
{
    Produs info;
    Nod* next;
};


Produs citireProdusDinFisier(FILE* f)
{
    Produs p;
    char buffer[256];
    fgets(buffer, 255, f);
    char sep[3] = ",\n";

    p.cod = atoi(strtok(buffer, sep));
    p.pret = atof(strtok(NULL, sep));

    char* aux;

    aux = strtok(NULL, sep);
    p.denumire = (char*)malloc(strlen(aux) + 1);
    strcpy(p.denumire, aux);

    aux = strtok(NULL, sep);
    p.categorie = (char*)malloc(strlen(aux) + 1);
    strcpy(p.categorie, aux);

    p.tip = strtok(NULL, sep)[0];

    return p;
}



void afisareProdus(Produs p)
{
    if (p.cod == -1)
    {
        printf("Produs inexistent!\n");
        return;
    }

    printf("Cod: %d\n", p.cod);
    printf("Pret: %.2f\n", p.pret);
    printf("Denumire: %s\n", p.denumire);
    printf("Categorie: %s\n", p.categorie);
    printf("Tip: %c\n\n", p.tip);
}

//adaugi la inceput
void enqueue(Nod** queue, Produs produsNou)
{
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = produsNou;
    nou->next = *queue;
    *queue = nou;
}

Produs dequeue(Nod** queue)
{
    if (*queue == NULL)
    {
        Produs p;
        p.cod = -1;
        p.denumire = NULL;
        p.categorie = NULL;
        return p;
    }

    if ((*queue)->next == NULL) //dc exista un singur nod in lista, il stergem
    {
        Nod* aux = *queue;
        *queue = NULL; //mu mai retine nimic coada

        Produs p;
        p = aux->info;

        free(aux);
        return p;
    }

    Nod* temp = *queue;
    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }
    Nod* aux = temp->next; //temp->next este nodul de sters (ULTIMUL NOD)
    Produs p;
    p = aux->info;
    temp->next = NULL; //sa nu se mai lege de ultimul nod pe care il vom sterge mai jos
    free(aux); //stergem ultimul nod
    

    return p;
}


Nod* citireCoadaDinFisier(const char* numeFisier)
{
    Nod* coada = NULL;
    FILE* f = fopen(numeFisier, "r");

    if (f)
    {
        while (!feof(f))
        {
            enqueue(&coada, citireProdusDinFisier(f));
        }
    }

    fclose(f);
    return coada;
}


void dezalocareCoada(Nod** coada)
{
    while (*coada)
    {
        Produs p = dequeue(coada);

        if (p.denumire)
            free(p.denumire);
        if (p.categorie)
            free(p.categorie);
    }
}


void afisareCoadaCuRefacere(Nod** coada)
{
    if (*coada == NULL)
    {
        Produs p;
        p.cod = -1;
        afisareProdus(p);
        return;
    }

    Nod* aux = NULL; // coada auxiliara

    while (*coada != NULL)
    {
        Produs p = dequeue(coada); // ?? scot din coada originala
        afisareProdus(p);

        enqueue(&aux, p); // ?? refac in aux
    }

    // ?? refac coada initiala
    while (aux != NULL)
    {
        enqueue(coada, dequeue(&aux));
    }
}


int main()
{
    Nod* coada = citireCoadaDinFisier("produse.txt");

    printf("Coada:\n");
    afisareCoadaCuRefacere(&coada);

    dezalocareCoada(&coada);

    return 0;
}