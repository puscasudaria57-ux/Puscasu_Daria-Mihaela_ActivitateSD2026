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
    Nod* prev;

};

typedef struct Queue Queue;
struct Queue
{
    Nod* start;
    Nod* end;
};


//S? se implementeze o func?ie care adaug? un produs într - o coad?(enqueue), folosind o list? dublu înl?n?uit?.
void enqueue(Queue* queue, Produs produs)
{
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = produs;
    nou->prev = NULL;
    nou->next = queue->start;
    if (queue->start != NULL)
    {
        queue->start->prev = nou;
    }
    else
    {
        queue->end = nou;
    }
    queue->start = nou;
}

//S? se implementeze o func?ie care extrage un produs din coad? (dequeue), tratând cazul în care coada este goal?.
Produs dequeue(Queue* queue)
{
    if (queue->start == NULL) //daca coada e goala
    {
        Produs p;
        p.cod = -1;
        return p;
    }
   
    Produs deSters = queue->end->info;//dc coada nu este goala retin produsul de sters, ultimul produs pt ca stergem de la final
    if (queue->start == queue->end) //dc in coada avem un singur
    {
        free(queue->start);
        queue->start = NULL;
        queue->end = NULL;
    }
    else
    {
        queue->end = queue->end->prev;
        (queue->end->next);
        queue->end->next = NULL;
    }
    

    return deSters;
}


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
    p.denumire = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
    strcpy(p.denumire, aux);

    aux = strtok(NULL, sep);
    p.categorie = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
    strcpy(p.categorie, aux);

    p.tip = strtok(NULL, sep)[0];

    return p;
}


Queue citireQueueDinFisier(const char* numeFisier)
{
    Queue queue;
    FILE* f = fopen(numeFisier, "r");
    if (f)
    {
        while (!feof(f))
        {
            enqueue(&queue, citireProdusDinFisier(f));
        }
    }

    fclose(f);
    return queue;
}


void afisareProdus(Produs p)
{
    if (p.cod == -1)
    {
        printf("Produsul nu exista!\n");
        return;
    }
       
    
    printf("Cod: %d\n", p.cod);
    printf("Pret: %.2f\n", p.pret);
    printf("Denumire: %s\n", p.denumire);
    printf("Categorie: %s\n", p.categorie);
    printf("Tip: %c\n\n", p.tip);
}


void afisareQueue(Queue* queue)
{
    Queue aux;
    aux.start = NULL;
    aux.end = NULL;

    if (queue->start == NULL)
    {
        Produs p;
        p.cod = -1;
        afisareProdus(p);
    }

    while (queue->start != NULL)
    {
        Produs p = dequeue(queue);
        afisareProdus(p);

        enqueue(&aux, p);
    }
    queue->start = aux.start;
    queue->end = aux.end;
}


void dezalocare(Queue* queue)
{
    while (queue->start != NULL)
    {
        Produs p = dequeue(queue);

        if (p.categorie != NULL)
            free(p.categorie);
        if (p.denumire != NULL)
            free(p.denumire);
    }
    queue->start = NULL;
    queue->end = NULL;
}


float pretMediu(Queue* queue)
{
    Queue aux;
    aux.start = NULL;
    aux.end = NULL;

    float suma = 0;
    int contor = 0;
    while (queue->start != NULL)
    {
        Produs p = dequeue(queue);

        suma = suma + p.pret;
        contor++;

        enqueue(&aux, p);
    }
    queue->start = aux.start;
    queue->end = aux.end;

    if(contor)
         return suma / contor;

    return 0;
}


//S? se implementeze o func?ie care caut? un produs dup? cod ?i îl returneaz? prin deep copy.
//Coada trebuie ref?cut? dup? parcurgere.
Produs getProdusByCOD(Queue* queue, int codCautat)
{
    Queue aux;
    aux.start = NULL;
    aux.end = NULL;

    Produs deReturnat;
    deReturnat.cod = -1;

    int gasit = 0;
    while (queue->start!=NULL && gasit == 0)
    {
        Produs p = dequeue(queue);
        if (p.cod == codCautat)
        {
            deReturnat = p;

            deReturnat.denumire = (char*)malloc(sizeof(char) * (strlen(p.denumire) + 1));
            strcpy(deReturnat.denumire, p.denumire);

            deReturnat.categorie = (char*)malloc(sizeof(char) * (strlen(p.categorie) + 1));
            strcpy(deReturnat.categorie, p.categorie);

            gasit = 1;

        }
        enqueue(&aux, p);

    }
    queue->start = aux.start;
    queue->end = aux.end;

    return deReturnat;
}

int main()
{
    Queue queue = citireQueueDinFisier("produse.txt");
   // afisareQueue(&queue);

    float pM = pretMediu(&queue);
   // printf("%.2f", pM);

    Produs pCod = getProdusByCOD(&queue, 44);
    afisareProdus(pCod);

    dezalocare(&queue);
    //afisareQueue(&queue);


    return 0;
}

