//#define _CRT_SECURE_NO_WARNINGS
//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//
//
//
//typedef struct Produs Produs;
//struct Produs
//{
//    int id;
//    int stoc;
//    float pret;
//    char* denumire;
//    char categorie;
//};
//
//
//typedef struct Nod Nod;
//struct Nod
//{
//    Produs info;
//    Nod* stanga;
//    Nod* dreapta;
//};
//
//
//void afisareProdus(Produs produs)
//{
//    printf("ID: %d\n", produs.id);
//    printf("Stoc: %d\n", produs.stoc);
//    printf("Pret: %.2f\n", produs.pret);
//    printf("Denumire: %s\n", produs.denumire);
//    printf("Categorie: %c\n\n", produs.categorie);
//}
//
//
//Produs citireProdusDinFisier(FILE* f)
//{
//    Produs produs;
//
//    char buffer[256];
//    char sep[3] = ",\n";
//    fgets(buffer, 255, f);
//
//    produs.id = atoi(strtok(buffer, sep));
//    produs.stoc = atoi(strtok(NULL, sep));
//    produs.pret = atof(strtok(NULL, sep));
//
//    char* aux = strtok(NULL, sep);
//    produs.denumire = (char*)malloc(strlen(aux) + 1);
//    strcpy(produs.denumire, aux);
//
//    produs.categorie = strtok(NULL, sep)[0];
//
//    return produs;
//}
//
//
//int calculeazaInaltimeArbore(Nod* radacina)
//{
//    if (radacina)
//    {
//        int st = calculeazaInaltimeArbore(radacina->stanga);
//        int dr = calculeazaInaltimeArbore(radacina->dreapta);
//
//        return 1 + max(st, dr);
//    }
//    return 0;
//}
//
//int verificareEchilibru(Nod* radacina)
//{
//    if (radacina)
//    {
//        int st = calculeazaInaltimeArbore(radacina->stanga);
//        int dr = calculeazaInaltimeArbore(radacina->dreapta);
//
//        return st - dr;
//    }
//    return 0;
//}
//
//
//void rotireStanga(Nod** radacina)
//{
//    Nod* aux = (*radacina)->dreapta;
//    (*radacina)->dreapta = aux->stanga;
//    aux->stanga = *radacina;
//    *radacina = aux;
//
//}
//
//
//void rotireDreapta(Nod** radacina)
//{
//    Nod* aux = (*radacina)->stanga;
//    (*radacina)->stanga = aux->dreapta;
//    aux->dreapta = *radacina;
//    *radacina = aux;
//
//}
//
//void adaugaProduseInArboreDeEchilibru(Nod** radacina, Produs produsNou)
//{
//    if (*radacina)
//    {
//        if ((*radacina)->info.id > produsNou.id)
//        {
//            adaugaProduseInArboreDeEchilibru(&(*radacina)->stanga, produsNou);
//        }
//        if ((*radacina)->info.id < produsNou.id)
//        {
//            adaugaProduseInArboreDeEchilibru(&(*radacina)->dreapta, produsNou);
//        }
//
//        int gradEchilibru = verificareEchilibru(*radacina);
//        if (gradEchilibru == -2)
//        {
//            if (verificareEchilibru((*radacina)->dreapta) <= 0)
//            {
//                rotireStanga(radacina);
//            }
//            else
//            {
//                rotireDreapta(&(*radacina)->dreapta);
//                rotireStanga(radacina);
//            }
//        }
//        
//        if (gradEchilibru == 2)
//        {
//            if (verificareEchilibru((*radacina)->stanga) >= 0)
//            {
//                rotireDreapta(radacina);
//            }
//            else
//            {
//                rotireStanga(&(*radacina)->stanga);
//                rotireDreapta(radacina);
//            }
//        }
//    }
//    else
//    {
//        Nod* nou = (Nod*)malloc(sizeof(Nod));
//        nou->info = produsNou;
//        nou->dreapta = NULL;
//        nou->stanga = NULL;
//        *radacina = nou;
//    }
//}
//
//
//Nod* citireArboreDeEchilibruDinFisier(const char* numeFisier)
//{
//    FILE* f = fopen(numeFisier, "r");
//    Nod* radacina = NULL;
//
//    if (!f)
//    {
//        printf("Eroare la deschiderea fisierului!\n");
//        return NULL;
//    }
//
//    while (!feof(f))
//    {
//        adaugaProduseInArboreDeEchilibru(&radacina, citireProdusDinFisier(f));
//    }
//    fclose(f);
//
//    return radacina;
//}
//
//
//void afisareProduseDinArborePreordine(Nod* radacina)
//{
//    if (radacina)
//    {
//        afisareProdus(radacina->info);
//        afisareProduseDinArborePreordine(radacina->stanga);
//        afisareProduseDinArborePreordine(radacina->dreapta);
//    }
//}
//
//int nrNoduriArbore(Nod* radacina)
//{
//    if (radacina)
//    {
//        return 1 + nrNoduriArbore(radacina->dreapta) + nrNoduriArbore(radacina->stanga);
//    }
//    return 0;
//}
//
//
//float calculeazaPretTotal(Nod* radacina) 
//{
//    if(radacina)
//    {
//        return radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
//    }
//    return 0;
//}
//
//
//
//void dezalocareArbore(Nod** radacina)
//{
//    if (*radacina)
//    {
//        dezalocareArbore(&(*radacina)->stanga);
//        dezalocareArbore(&(*radacina)->dreapta);
//
//        free((*radacina)->info.denumire);
//
//        free(*radacina);
//        *radacina = NULL;
//    }
//}
//
//
//
//
//int main()
//{
//    Nod* radacina = citireArboreDeEchilibruDinFisier("produse.txt");
//    afisareProduseDinArborePreordine(radacina);
//
//    printf("Pretul total al produselor este: %.2f\n", calculeazaPretTotal(radacina));
//
//    dezalocareArbore(&radacina);
//    afisareProduseDinArborePreordine(radacina);
//    
//    return 0;
//}