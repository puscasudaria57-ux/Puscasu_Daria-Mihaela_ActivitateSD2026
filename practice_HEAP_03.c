//#define _CRT_SECURE_NO_WARNINGS
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//
//
//typedef struct Articol Articol;
//struct Articol
//{
//    int id;
//    int cantitate;
//    float pret;
//    char* denumire;
//    unsigned char categorie;
//};
//
//
//typedef struct Heap Heap;
//struct Heap
//{
//    int lungime;
//    Articol* vector;
//    int nrElemente;
//};
//
//void afisareArticol(Articol articol)
//{
//    printf("ID: %d\n", articol.id);
//    printf("Cantitate: %d\n", articol.cantitate);
//    printf("Pret: %.2f\n", articol.pret);
//    printf("Denumire: %s\n", articol.denumire);
//    printf("Categorie: %c\n\n", articol.categorie);
//}
//
//
//Articol citireArticolDinFisier(FILE* f)
//{
//    Articol articol;
//
//    char buffer[256];
//    char sep[3] = ",\n";
//
//    fgets(buffer, 255, f);
//
//    articol.id = atoi(strtok(buffer, sep));
//    articol.cantitate = atoi(strtok(NULL, sep));
//    articol.pret = atof(strtok(NULL, sep));
//
//    char* aux;
//
//    aux = strtok(NULL, sep);
//    articol.denumire = (char*)malloc(strlen(aux) + 1);
//    strcpy(articol.denumire, aux);
//
//    articol.categorie = strtok(NULL, sep)[0];
//
//    return articol;
//}
//
//
//Heap initializareHeap(int lungime)
//{
//    Heap h;
//    h.lungime = lungime;
//    h.nrElemente = 0;
//    h.vector = (Articol*)malloc(sizeof(Articol) * lungime);
//
//    return h;
//}
//
//void filtreazaHeap(Heap heap, int pozitieNod)
//{
//    int pozFiuStg = 2 * pozitieNod + 1;
//    int pozFiuDr = 2 * pozitieNod + 2;
//    int pozMax = pozitieNod;
//
//    //MAX HEAP
//    if (pozFiuDr < heap.nrElemente && heap.vector[pozFiuDr].id > heap.vector[pozMax].id)
//    {
//        pozMax = pozFiuDr;
//    }
//    if (pozFiuStg <heap.nrElemente && heap.vector[pozFiuStg].id > heap.vector[pozMax].id)
//    {
//        pozMax = pozFiuStg;
//    }
//
//    if (pozMax != pozitieNod)
//    {
//        Articol aux = heap.vector[pozMax];
//        heap.vector[pozMax] = heap.vector[pozitieNod];
//        heap.vector[pozitieNod] = aux;
//
//        if (pozMax <= (heap.nrElemente - 2) / 2)
//        {
//            filtreazaHeap(heap, pozMax);
//        }
//    }
//}
//
//Heap citireHeapDinFisier(const char* numeFisier)
//{
//    FILE* f = fopen(numeFisier, "r");
//    Heap heap = initializareHeap(10);
//    if (!f)
//    {
//        printf("err");
//        return heap;
//    }
//
//    while (!feof(f))
//    {
//        heap.vector[heap.nrElemente++] = citireArticolDinFisier(f);
//    }
//    fclose(f);
//
//    for (int j = (heap.nrElemente - 2) / 2; j >= 0; j--)
//    {
//        filtreazaHeap(heap, j);
//    }
//
//    return heap;
//}
//
//
//Articol extrageArticol(Heap* heap)
//{
//    if (heap->nrElemente)
//    {
//        //MUT MAXIMUL LA FINAL
//        Articol aux = heap->vector[0];
//        heap->vector[0] = heap->vector[heap->nrElemente - 1];
//        heap->vector[heap->nrElemente - 1] = aux;
//
//        heap->nrElemente--; //SCAD NUMAERUL DE ELEMENTE (ASCUND ELEMENTUL MAXIM PE CARE L AM MUTAT, ELEMENTELE ASCUNSE VOR FI IN ORDINE CRESCATOARE ASTFEL)
//
//        for (int j = (heap->nrElemente - 2) / 2; j >= 0; j--)
//        {
//            filtreazaHeap(*heap, j);
//        }
//
//        return aux;//!!!!!!
//    }
//    Articol a;
//    a.id = -1;
//    a.denumire = NULL;
//    return a;
//}
//
//void afisareHeap(Heap heap)
//{
//    for (int i = 0; i < heap.nrElemente; i++)
//    {
//        afisareArticol(heap.vector[i]);
//    }
//}
//
//void afisareHeapAscuns(Heap heap)
//{
//    for (int i = heap.nrElemente; i < heap.lungime; i++)
//    {
//        afisareArticol(heap.vector[i]);
//    }
//}
//
//
//void dezalocareHeap(Heap* heap)
//{
//    for (int i = 0; i < heap->lungime; i++)
//    {
//        free(heap->vector[i].denumire);
//    }
//    free(heap->vector);
//    heap->nrElemente = 0;
//    heap->lungime = 0;
//    heap->vector = NULL;
//}
//
//
//int main()
//{
//    Heap heap = citireHeapDinFisier("articole.txt");
//
//    afisareHeap(heap);
//
//    printf("Extras:\n");
//    Articol extras = extrageArticol(&heap);
//    afisareArticol(extras);
//
//    printf("Heap dupa extragere:\n");
//    afisareHeap(heap);
//
//    printf("Elemente ascunse:\n");
//    afisareHeapAscuns(heap);
//
//    dezalocareHeap(&heap);
//
//    return 0;
//}