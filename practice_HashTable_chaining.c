//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//
//typedef struct Produs Produs;
//struct Produs
//{
//    int cod;
//    float pret;
//    char* denumire;    // dinamic
//    char* categorie;   // dinamic
//    char tip;          // nou (ex: 'A' = alimentar, 'B', 'C')
//};
//
//typedef struct Nod Nod;
//struct Nod
//{
//    Produs info;
//    Nod* next;
//};
//
//typedef struct HashTable HashTable;
//struct HashTable
//{
//    int dim;
//    Nod** vector;
//};
//
//
//HashTable initializareHashTable(int dimensiune)
//{
//    HashTable ht;
//    ht.dim = dimensiune;
//    ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
//    for (int i = 0; i < dimensiune; i++)
//    {
//        ht.vector[i] = NULL;
//    }
//    return ht;
//}
//
//int calculeazaHash(int dimensiune, const char* denumire)
//{
//    int suma = 0;
//    for (int i = 0; i < strlen(denumire); i++)
//    {
//        suma = suma + denumire[i];
//    }
//    return suma % dimensiune;
//}
//
//
//Produs citireProdusDinFisier(FILE* f)
//{
//    Produs p;
//    char buffer[256];
//    fgets(buffer, 255, f);
//    char sep[3] = ",\n";
//
//    p.cod = atoi(strtok(buffer, sep));
//    p.pret = atof(strtok(NULL, sep));
//
//    char* aux;
//
//    aux = strtok(NULL, sep);
//    p.denumire = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
//    strcpy(p.denumire, aux);
//
//    aux = strtok(NULL, sep);
//    p.categorie = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
//    strcpy(p.categorie, aux);
//
//    p.tip = strtok(NULL, sep)[0];
//
//    return p;
//}
//
//void adaugaProdusLaFinalInLista(Nod** cap, Produs produsNou)
//{
//    Nod* nou = (Nod*)malloc(sizeof(Nod));
//    nou->info = produsNou;
//    nou->next = NULL;
//
//    if (*cap != NULL) //dc lista nu este goala, capul ramane la inceput si adaug nodul la final
//    {
//        Nod* aux = *cap;
//        while (aux->next != NULL)
//        {
//            aux = aux->next;
//        }
//        aux->next = nou;
//    }
//    else
//    {
//        //dc lista este goala, capul se va modifica pt ca nodul adaugat va fi primul nod
//        *cap = nou;
//    }
//    
//}
//
//
//void inserareProdusInHashTable(HashTable ht, Produs produs)
//{
//    int poz = calculeazaHash(ht.dim, produs.denumire);
//    adaugaProdusLaFinalInLista(&(ht.vector[poz]), produs);
//}
//
//
//HashTable citireHashTableDinFisier(const char* numeFisier, int dimensiune)
//{
//    HashTable ht = initializareHashTable(dimensiune);
//    FILE* f = fopen(numeFisier, "r");
//    if (f)
//    {
//        while (!feof(f))
//        {
//            inserareProdusInHashTable(ht, citireProdusDinFisier(f));
//        }
//    }
//
//    fclose(f);
//    return ht;
//}
//
//void afisareProdus(Produs p)
//{
//    printf("Cod: %d\n", p.cod);
//    printf("Pret: %.2f\n", p.pret);
//    printf("Denumire: %s\n", p.denumire);
//    printf("Categorie: %s\n", p.categorie);
//    printf("Tip: %c\n\n", p.tip);
//}
//
//void afisareListaDeLaInceput(Nod* cap)
//{
//    while (cap)
//    {
//        afisareProdus(cap->info);
//        cap = cap->next;
//    }
//}
//
//void afisareTabelaDeProduse(HashTable ht)
//{
//    for (int i = 0; i < ht.dim; i++)
//    {
//        if (ht.vector[i] != NULL)
//        {
//
//            printf("Pe pozitia %d avem urmatoarele produse (urmatorul cluster):\n", i);
//            afisareListaDeLaInceput(ht.vector[i]);
//        }
//        else
//        {
//            printf("Pe pozitia %d nu avem produse\n", i);
//        }
//    }
//}
//
//void dezalocareLista(Nod** cap)
//{
//    while (*cap)
//    {
//        Nod* aux = *cap;
//        *cap = aux->next;
//
//        if (aux->info.categorie != NULL)
//            free(aux->info.categorie);
//
//        if (aux->info.denumire != NULL)
//            free(aux->info.denumire);
//
//        free(aux);
//    }
//    *cap = NULL;
//}
//
//void dezalocareHashTable(HashTable* ht)
//{
//    for (int i = 0; i < ht->dim; i++)
//    {
//        dezalocareLista(&(ht->vector[i]));
//    }
//    free(ht->vector);
//    ht->dim = 0;
//    ht->vector = NULL;
//
//}
//
//
//float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere)
//{
//    (*nrClustere) = 0;
//    for (int i = 0; i < ht.dim; i++)
//    {
//        if (ht.vector[i] != NULL) //vom calcula pretul mediu doar pt acele pozitii unde nu este nulla valoarea
//        {
//            (*nrClustere)++; //acesta va fi nr de elemente al vectorului de float*
//        }
//    }
//
//    float* pMedii = (float*)malloc(sizeof(float) * (*nrClustere));
//    int k = 0;
//    for (int i = 0; i < ht.dim; i++)
//    {
//        if (ht.vector[i] != NULL) //vom calcula pretul mediu doar pt acele pozitii unde nu este nulla valoarea
//        {
//            Nod* aux = ht.vector[i];
//            float suma = 0;
//            int contor = 0;
//            while (aux)
//            {
//                suma = suma + aux->info.pret;
//                contor++;
//                aux = aux->next;
//            }
//
//            pMedii[k++] = suma / contor;
//        }
//    }
//
//    return pMedii;
//}
//
//
////cautarile in cadrul tabelei vor fi mult mai usor de facut fata de vector pt ca ne vom folosi de fctia calculeazaHash
//Produs getProdusDupaDenumire(HashTable ht, const char* denumireCautata)
//{
//    Produs p;
//    p.cod = -1;
//
//    int poz = calculeazaHash(ht.dim, denumireCautata);
//    if (poz >= 0 && poz < ht.dim)
//    {
//        Nod* aux = ht.vector[poz]; //acum aux va fi capul listei in care se afla produsul cu denumirea cautata
//        while (aux)
//        {
//            if (strcmp(aux->info.denumire, denumireCautata) == 0)
//            {
//                p = aux->info;
//
//                p.denumire = (char*)malloc(sizeof(char) * (strlen(aux->info.denumire) + 1));
//                strcpy(p.denumire, aux->info.denumire);
//
//                p.categorie = (char*)malloc(sizeof(char) * (strlen(aux->info.categorie) + 1));
//                strcpy(p.categorie, aux->info.categorie);
//            }
//            aux = aux->next;
//        }
//    }
//    return p;
//}
//
//int main()
//{
//    HashTable ht = citireHashTableDinFisier("produse.txt", 7);
//    afisareTabelaDeProduse(ht);
//
//    int nrElem;
//    float* pMedii = calculeazaPreturiMediiPerClustere(ht, &nrElem);
//    for (int i = 0; i < nrElem; i++)
//    {
//        printf("%.2f ", pMedii[i]);
//    }
//
//    Produs p = getProdusDupaDenumire(ht, "Laptop");
//    afisareProdus(p);
//
//    return 0;
//}