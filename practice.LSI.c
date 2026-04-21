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


//S? se implementeze o func?ie care cite?te un produs dintr-o linie a unui fi?ier text ?i returneaz? un obiect de tip Produs
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


void afisareProdus(Produs p)
{
    printf("Cod: %d\n", p.cod);
    printf("Pret: %.2f\n", p.pret);
    printf("Denumire: %s\n", p.denumire);
    printf("Categorie: %s\n", p.categorie);
    printf("Tip: %c\n\n", p.tip);
}

void adaugaProdusLaInceputulListei(Nod** cap, Produs produs)
{
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = produs;
    nou->next = *cap;
    *cap = nou;
}

void adaugaProdusLaFinalulListei(Nod** cap, Produs produs)
{
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = produs;
    nou->next = NULL;

    if (*cap == NULL) //dc lista este goala, cap va pointa spre nodul pe care il adaugam
    {
        *cap = nou;
    }
    else
    {
        //dc nu,vom parcurge lista cat timp next!=NULL, cand am ajuns pe next = NULL, trebuie sa legam ultimul nod de noul nod

        Nod* aux = *cap;
        while (aux->next != NULL)
        {
            aux = aux->next;
        }
        aux->next = nou;
    }
}

//S? se citeasc? produsele din fi?ier ?i s? se construiasc? o list? simplu înl?n?uit?.
Nod* citireListaProduseDinFisier(const char* numeFisier)
{
    Nod* cap = NULL; //initial goala lista

    FILE* f = fopen(numeFisier, "r");
    if (f)
    {
        while (!feof(f))
        {
            adaugaProdusLaFinalulListei(&cap, citireProdusDinFisier(f));
        }
    }

    fclose(f);//inchid fisierul
    return cap;
}

void afisareListaProduse(Nod* cap)
{
    if (cap == NULL)
    {
        printf("Lista este goala\n");
        return;
    }
    else
    {
        while (cap != NULL)//parcurgem cu cap pt ca modificam capul
        {
            afisareProdus(cap->info);
            cap = cap->next;
        }
    }
}


void dezalocare(Nod** cap)
{
    while (*cap != NULL)
    {
        Nod* aux = *cap;
        *cap = aux->next; //inaintez cu cap si sterg cu aux

        if (aux->info.categorie != NULL)
            free(aux->info.categorie);
        if (aux->info.denumire != NULL)
            free(aux->info.denumire);

        free(aux);
    }
    *cap = NULL;
}


//S? se calculeze pre?ul mediu al produselor din list?.
float pretMediu(Nod* cap)
{
    float suma = 0;
    int contor = 0;
    while (cap)
    {
        suma = suma + cap->info.pret;
        contor++;
        cap = cap->next;
    }
    if (contor != 0)
    {
        return suma / contor;
    }
    return 0;
}

//S? se afi?eze produsele care apar?in unei categorii date.
Nod* afisareProduseDupaCategorie(Nod* cap, const char* categorieCautata)
{
    Nod* lista = NULL;
    while (cap)
    {
        if (strcmp(cap->info.categorie, categorieCautata) == 0)
        {

            adaugaProdusLaFinalulListei(&lista, cap->info);
        }
        cap = cap->next;
    }
    return lista;
}


//S? se returneze produsul cu un anumit cod prin deep copy.
//codul este unic
Produs getProdusByCod(Nod* cap, int codCautat)
{
    Produs p;
    p.cod = -1;
    while (cap)
    {
        if (cap->info.cod == codCautat)
        {
            p = cap->info;

            p.denumire = (char*)malloc(sizeof(char) * (strlen(cap->info.denumire) + 1));
            strcpy(p.denumire, cap->info.denumire);

       
            p.categorie = (char*)malloc(sizeof(char) * (strlen(cap->info.categorie) + 1));
            strcpy(p.categorie, cap->info.categorie);
        }
        cap = cap->next;
    }
    return p;
}


//S? se ?tearg? produsul cu un anumit cod(unic).
void stergeProdusDupaCod(Nod** cap, int codDeSters)
{
    if (*cap == NULL) //dc lista este goala se opreste executia imediat
        return;

    //primul nod din lista trebuie sters, ne am asigurat ca lista nu este goala, acum verificam dc primul nod este de sters
    if ((*cap)->info.cod == codDeSters)
    {
        //sterg nodul

        Nod* temp = *cap;
        *cap = temp->next; //inaintez capul si sterg nodul retinut in temp

        if (temp->info.categorie != NULL)
            free(temp->info.categorie);
        if (temp->info.denumire != NULL)
            free(temp->info.denumire);

        free(temp);
        return;//ies direct pt ca nu mai exista alt nod cu acest cod (codul este unic)
    }

    //suntem in situatia in care codul este pe undeva la mijloc 
    Nod* aux = *cap;

    while (aux->next != NULL && aux->next->info.cod != codDeSters)//trebuie sa ne oprim inaintea nodului de sters
    {
        aux = aux->next; //in aux va ramane nodul dinaintea nodului cu codul de sters
    }

    if (aux == NULL) //trb sa verificam pt cazul in care in lista nu exista produs cu acel cod (si daca nu exista aux ar ajunge NULL)
    {
        return; //iesim din functie pt ca nu am gasit un produs cu acest cod
    }
    else
    {
        //sterg produsul
        Nod* temp = aux ->next; //nodul de sters este aux->next
        aux->next = temp->next;

        if (temp->info.categorie != NULL)
            free(temp->info.categorie);

        if (temp->info.denumire != NULL)
            free(temp->info.denumire);

        free(temp);
    }

}


//S? se elimine toate produsele dintr - o anumit? categorie
void stergeProdusDupaCategorie(Nod** cap, const char* categorieDeSters)
{
    if (*cap == NULL)
    {
        return;
    }

    while (*cap != NULL && strcmp((*cap)->info.categorie, categorieDeSters) == 0)
    {
        Nod* temp = *cap;
        *cap = temp->next;//inaintez cu cap si sterg cu temp

        if (temp->info.categorie != NULL)
            free(temp->info.categorie);

        if (temp->info.denumire != NULL)
            free(temp->info.denumire);

        free(temp);
    }

    //acum ne am asigurat ca la inceputul listei nu mai avem produs cu acea categorie de sters
    Nod* aux = *cap;
    while (aux)//acest while este pentru a parcurge pana la final toata lista ca sa stergem toate nodurile cu acea categorie
    {
        while (aux->next != NULL && strcmp(aux->next->info.categorie, categorieDeSters) != 0) //ne pozitionam inaintea nodului de sters
        {
            aux = aux->next;//in aux va ramane nodul din stanga nodului de sters
        }

        if (aux->next == NULL)//in cazul asta am ajuns la finalul listei si nu am gasit categoria respectiva
        {
            aux = NULL;//ca sa iesim din bucla initiala while
        }
        else
        {
            Nod* temp = aux->next; //salvez nodul de sters intr un nod temporar
            aux->next = temp->next;

            if (temp->info.categorie != NULL)
                free(temp->info.categorie);

            if (temp->info.denumire != NULL)
                free(temp->info.denumire);

            free(temp);
        }
    }
    


}

int main()
{
    Nod* lista = citireListaProduseDinFisier("produse.txt");
    //afisareListaProduse(lista);

    float pMediu = pretMediu(lista);
    printf("Pretul mediu al produselor este de: %.2f\n\n",pMediu);

   // Nod* cap = afisareProduseDupaCategorie(lista, "Alimente");
   // afisareListaProduse(cap);

    Produs p = getProdusByCod(lista, 10);
    //afisareProdus(p);

    stergeProdusDupaCategorie(&lista, "Alimente");
    afisareListaProduse(lista);

    
   /* stergeProdusDupaCod(&lista, 1);
    stergeProdusDupaCod(&lista, 5);
    stergeProdusDupaCod(&lista, 10);
    afisareListaProduse(lista);*/


    dezalocare(&lista);
    return 0;
}








