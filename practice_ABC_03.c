//#define _CRT_SECURE_NO_WARNINGS
//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//
//
//
//typedef struct StructuraProdus Produs;
//struct StructuraProdus
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
//void adaugaProdusInArbore(Nod** radacina, Produs produsNou)
//{
//    if (*radacina)
//    {
//        if ((*radacina)->info.id > produsNou.id)
//        {
//            adaugaProdusInArbore(&(*radacina)->stanga,produsNou);
//        }
//        if ((*radacina)->info.id < produsNou.id)
//        {
//            adaugaProdusInArbore(&(*radacina)->dreapta, produsNou);
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
//Nod* citireArborDeProdusDinFisier(const char* numeFisier)
//{
//    FILE* f = fopen(numeFisier, "r");
//    if (!f)
//    {
//        printf("Eroare\n");
//        return NULL;
//    }
//    
//    Nod* radacina = NULL;
//
//    while (!feof(f))
//    {
//        adaugaProdusInArbore(&radacina, citireProdusDinFisier(f));
//    }
//    fclose(f);
//
//    return radacina;
//
//}
//
//
//
//
//// =============== AFISARI ==================
//// Preordine:   Rădăcină -> Stânga -> Dreapta
//// Inordine:    Stânga -> Rădăcină -> Dreapta
//// Postordine:  Stânga -> Dreapta -> Rădăcină
//
//void afisareProduseDinArborePreordine(Nod* radacina)
//{
//    if (radacina) {
//        // 1. afișăm informația din nodul curent
//        afisareProdus(radacina->info);
//
//        // 2. parcurgem subarborele stâng
//        afisareProduseDinArborePreordine(radacina->stanga);
//
//        // 3. parcurgem subarborele drept
//        afisareProduseDinArborePreordine(radacina->dreapta);
//    }
//}
//
//
//void afisareProduseDinArborePostordine(Nod* radacina)
//{
//    if (radacina)
//    {
//        // 1. parcurgem subarborele stâng
//        afisareProduseDinArborePostordine(radacina->stanga);
//
//        // 2. parcurgem subarborele drept
//        afisareProduseDinArborePostordine(radacina->dreapta);
//
//        // 3. afișăm rădăcina
//        afisareProdus(radacina->info);
//    }
//}
//
//
//void afisareProduseDinArboreInordine(Nod* radacina)
//{
//    if (radacina)
//    {
//        // 1. parcurgem subarborele stâng
//        afisareProduseDinArboreInordine(radacina->stanga);
//
//        // 2. afișăm rădăcina
//        afisareProdus(radacina->info);
//
//        // 3. parcurgem subarborele drept
//        afisareProduseDinArboreInordine(radacina->dreapta);
//    }
//}
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
//float calculeazaPretTotal(Nod* radacina)
//{
//	if (radacina)
//	{
//		// prețul total = preț curent + prețuri din dreapta + prețuri din stânga
//		return radacina->info.pret
//			+ calculeazaPretTotal(radacina->dreapta)
//			+ calculeazaPretTotal(radacina->stanga);
//	}
//
//	return 0; // dacă nodul este NULL, suma este 0
//}
//
//
//int main()
//{
//    Nod* radacina = citireArborDeProdusDinFisier("produse_arbore.txt");
//    afisareProduseDinArborePreordine(radacina);
//
//
//    return 0;
//}