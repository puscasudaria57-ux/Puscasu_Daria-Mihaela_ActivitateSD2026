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

//STACK (LIFO - ADAUGA LA INCEPUT IN LISTA (push), STERGE DE LA INCEPUT DIN LISTA(pop) )
typedef struct NodSimplu NodSimplu;
struct NodSimplu
{
    Produs info;
    NodSimplu* next;
};


//S? se implementeze o func?ie care adaug? un produs într-o stiv? implementat? ca list? simplu înl?n?uit?.
//adaugarea unui nod nou la inceputul listei
void pushStack(NodSimplu** stack, Produs produsNou)
{
    NodSimplu* nou = (NodSimplu*)malloc(sizeof(NodSimplu));
    nou->info = produsNou;
    nou->next = *stack;
    *stack = nou;
}

//S? se implementeze o func?ie care extrage un produs din stiv?(opera?ia pop), tratând cazul în care stiva este goal?.
Produs popStack(NodSimplu** stack)
{
    Produs p; //dc stiva este goala se va return acest produs
    p.cod = -1;

    if (*stack != NULL)
    {
        NodSimplu* aux = *stack;
        *stack = aux->next; //inaintez cu stack si sterg cu aux

        p = aux->info; //retin produsul sters

        free(aux); //sterg nodul, pur si simplu stergi nodul, nu dezaloci nimic, dezalocarea o vei face in functia de dezalocare
    }

    return p; //sterg produsul
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

//S? se implementeze o func?ie care dezaloc? complet stiva de produse.
void dezalocareStiva(NodSimplu** stack)
{
    while (*stack != NULL)
    {
        Produs produs = popStack(stack); //scot produsul din stiva

        if (produs.categorie != NULL)
            free(produs.categorie);
        if (produs.denumire != NULL)
            free(produs.denumire);

       
       // *stack = (*stack)->next; NU ESTE NEVOIE SA FACI ASTA CA ORICUM INAINTEZI CU STACK UL IN TIMP CE STERGI NODURI
        //FOLOSINF FUNCTIA POP!
    }
    *stack = NULL;
}


//S? se implementeze o func?ie care cite?te un produs dintr-un fi?ier text.
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

//S? se construiasc? o stiv? de produse citind din fi?ier.
NodSimplu* citireStivaDinFisier(const char* numeFisier)
{
    NodSimplu* stack = NULL;
    FILE* f = fopen(numeFisier, "r");
    if (f)
    {
        while (!feof(f))
        {
            pushStack(&stack, citireProdusDinFisier(f));
        }
    }
    fclose(f);
    return stack;
}

//S? se afi?eze toate produsele din stiv? f?r? a pierde elementele (stiva trebuie ref?cut?).
void afisareStack(NodSimplu** stack)
{
    if (*stack == NULL)
    {
        Produs p;
        p.cod = -1;
        afisareProdus(p);
        return;
    }
   
    //imi iau un nod auxiliar cu care sa imi refac stiva
    NodSimplu* aux = NULL; //initial stiva asta e goala
    while (*stack != NULL)
    {
        Produs p = popStack(stack); //stric stiva
        afisareProdus(p); //afisez produsul scos din stiva

        pushStack(&aux, p); //refac stiva in aux
    }
    while (aux)
    {
        pushStack(stack, popStack(&aux)); //scot din aux si push in stack
    }
    

   
}

//S? se determine câte produse apar?in unei categorii date.
int getNrProduseCategorie(NodSimplu** stack, const char* categorieCautata)
{
    int nr = 0;

    NodSimplu* aux = NULL;
    while (*stack)
    {
        Produs p = popStack(stack);

        if (strcmp(p.categorie, categorieCautata) == 0)
        {
            nr++;
        }

        pushStack(&aux, p);
    }

    while (aux)
    {
        pushStack(stack, popStack(&aux));
    }

    return nr;
}


//Realizati o fctie care det pretul maxim din stiva
float getaPretMaxim(NodSimplu** stack)
{
    if (*stack == NULL)
    {
        return 0;
    }

    NodSimplu* aux = NULL; //ESTE NULLA STIVA AUXILIARA INITIAL
    Produs pMax = popStack(stack); //presupun ca aceasta este produsul cu pret maxim
    float pretMax = pMax.pret;
    pushStack(&aux, pMax);
    while (*stack)
    {
        Produs p = popStack(stack);
        if (pretMax < p.pret)
        {
            pretMax = p.pret;
        }
        pushStack(&aux, p);
    }
    while (aux)
    {
        pushStack(stack, popStack(&aux));
    }

    return pretMax;

}

int main()
{
    NodSimplu* stack = citireStivaDinFisier("produse.txt");
    afisareStack(&stack);

    printf("Numarul de produse din categoria selectata este: %d\n\n", getNrProduseCategorie(&stack, "Alimente"));

    float pMax = getaPretMaxim(&stack);
    printf("%.2f", pMax);

    return 0;
}