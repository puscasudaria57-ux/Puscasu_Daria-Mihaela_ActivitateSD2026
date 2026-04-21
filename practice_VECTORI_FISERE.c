#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Produs Produs;
struct Produs
{
    int cod;
    float pret;
    char* denumire;
    char* categorie;
    char tip;
};


void afisareProdus(Produs p)
{
    printf("Cod: %d\n", p.cod);
    printf("Pret: %.2f\n", p.pret);
    printf("Denumire: %s\n", p.denumire);
    printf("Categorie: %s\n", p.categorie);
    printf("Tip: %c\n\n", p.tip);
}

//S? se afi?eze un vector de produse.
void afisareVectorProduse(Produs* vector,int nrElemente)
{
    if (vector == NULL || nrElemente == 0)
    {
        printf("Vectorul este gol");
        return;
    }

    for (int i = 0; i < nrElemente; i++)
    {
        afisareProdus(vector[i]);
    }
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

void adaugaProdusInvector(Produs** vector, int* nrElemente, Produs produsNou)
{
    Produs* temp = (Produs*)malloc(sizeof(Produs) * ((*nrElemente)+1));//aloc dinamic pt nrElemente + 1 elemente
    for (int i = 0; i < *nrElemente; i++)
    {
        temp[i] = (*vector)[i]; //copiez in vectorul temporar toate elemente, FARA DEEP COPY!

    }
    temp[*nrElemente] = produsNou; //pe pozitia asta pun noul produs
    (*nrElemente)++;

    free(*vector);//sterg tot ce aveam inainte in vector!!!!!!!!!!!!!!!!!!!
    *vector = temp;

}



//S? se citeasc? toate produsele din fi?ier într-un vector dinamic.
Produs* citireVectorDeProduseDinFisier(const char* numeFisier, int* nrProduse)
{
    Produs* vector = NULL;
    *nrProduse = 0;

    FILE* f = fopen(numeFisier, "r");
    if (f)
    {
        while (!feof(f))
        {
            adaugaProdusInvector(&vector,nrProduse, citireProdusDinFisier(f));
        }
    }

    fclose(f);
    return vector;
}

void dezalocare(Produs** vector, int* nrElemente)
{
    for (int i = 0; i < (*nrElemente); i++)
    {
        if ((*vector)[i].categorie != NULL)
            free((*vector)[i].categorie);

        if ((*vector)[i].denumire != NULL)
            free((*vector)[i].denumire);
    }
    free(*vector); //sterg vectorul
    *vector = NULL;
    *nrElemente = 0;
}

Produs deep_copy(Produs produs)
{
    Produs p;

    p.cod = produs.cod;
    p.pret = produs.pret;
    p.tip = produs.tip;

    p.categorie = (char*)malloc(sizeof(char) * (strlen(produs.categorie) + 1));
    strcpy(p.categorie, produs.categorie);

    p.denumire = (char*)malloc(sizeof(char) * (strlen(produs.denumire) + 1));
    strcpy(p.denumire, produs.denumire);


    return p;
}


//S? se creeze un vector nou cu primele N produse.
Produs* primeleNProduse(Produs* vector, int nrElemente, int nrElementeCopiate)
{
    Produs* produse = (Produs*)malloc(sizeof(Produs) * nrElementeCopiate); //aloc dinamic acest vector!
    
    for (int i = 0; i < nrElementeCopiate; i++)
    {
        produse[i] = deep_copy(vector[i]);
    }

    return produse;
}

//S? se creeze un vector nou care con?ine doar produsele cu pre? mai mare decât un prag dat.
void pretMaiMareDecatPrag(Produs* vector, int nrElemente, Produs** vectorRezultat, int* elemVecRez, float pretPrag)
{
    (*elemVecRez) = 0; //INITIALIZEZI CU 0 CA ESTE PO SUMA
    for (int i = 0; i < nrElemente; i++)
    {
        if (vector[i].pret > pretPrag)
        {
            (*elemVecRez)++; //aflu dimensiunea acestui vector rezultat ca sa pot sa il aloc dinamic
        }
    }

    *vectorRezultat = (Produs*)malloc(sizeof(Produs) * (*elemVecRez));//ALOC DINAMIC PT ACEST VECTOR REZULTAT

    int k = 0;
    for (int i = 0; i < nrElemente; i++)
    {
        if (vector[i].pret > pretPrag)
        {
            (*vectorRezultat)[k++] = deep_copy(vector[i]);
        }
    }
}


//daca trebuie pur si simplu sa afisezi, ci nu sa creezi un vector nou, faci asa:
void afisareProdusePretMaiMare(Produs* vector, int nrElemente, float prag)
{
    if (vector == NULL || nrElemente == 0)
    {
        printf("Nu exista produse\n");
        return;
    }

    for (int i = 0; i < nrElemente; i++)
    {
        if (vector[i].pret > prag)
        {
            afisareProdus(vector[i]);
        }
    }
}


//S? se returneze primul produs cu o denumire dat?(deep copy).
Produs getProdusByDenumire(Produs* produse, int nrElem, const char* denumireCautata)
{
    Produs p;
    p.cod = -1;
    p.denumire = NULL;
    p.categorie = NULL;

    int ok = 0;
    for (int i = 0; ok == 0 && i < nrElem; i++)
    {
        if (strcmp(produse[i].denumire, denumireCautata) == 0)
        {
            ok = 1;
            p = produse[i];

            p.categorie = (char*)malloc(sizeof(char) * (strlen(produse[i].categorie) + 1));
            strcpy(p.categorie, produse[i].categorie);

            p.denumire = (char*)malloc(sizeof(char) * (strlen(produse[i].denumire) + 1));
            strcpy(p.denumire, produse[i].denumire);
        }
    }
    return p;
}

//produsul cel mai scump
Produs produsMaxim(Produs* vector, int nrElemente)
{
    Produs max;
    max.cod = -1;
    max.pret = -1;

    for (int i = 0; i < nrElemente; i++)
    {
        if (vector[i].pret > max.pret)
        {
            max = vector[i]; // shallow copy
        }
    }

    Produs rezultat = max;

    // deep copy
    rezultat.denumire = (char*)malloc(strlen(max.denumire) + 1);
    strcpy(rezultat.denumire, max.denumire);

    rezultat.categorie = (char*)malloc(strlen(max.categorie) + 1);
    strcpy(rezultat.categorie, max.categorie);

    return rezultat;
}



void afisareProduseCategorie(Produs* vector, int nrElemente, const char* categorie)
{
    for (int i = 0; i < nrElemente; i++)
    {
        if (vector[i].categorie != NULL &&
            strcmp(vector[i].categorie, categorie) == 0)
        {
            afisareProdus(vector[i]);
        }
    }
}

int main()
{
    int nrProduse;
    Produs* produse = citireVectorDeProduseDinFisier("produse.txt", &nrProduse);
    //afisareVectorProduse(produse, nrProduse);
    
    int nrElementeCopiate = 3;
    Produs* p = primeleNProduse(produse, nrProduse, nrElementeCopiate);
    //afisareVectorProduse(p, nrElementeCopiate);

    int elem;
    Produs* pretMare = NULL;
    pretMaiMareDecatPrag(produse, nrProduse, &pretMare, &elem, 100);
    //afisareVectorProduse(pretMare,elem);

    Produs p2 = getProdusByDenumire(produse, nrProduse, "Lapte");
    //afisareProdus(p2);

    afisareProduseCategorie(produse, nrProduse, "Alimente");

    return 0;
}