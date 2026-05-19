//#define _CRT_SECURE_NO_WARNINGS
//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
//
//typedef struct StructuraFilm Film;
//struct StructuraFilm 
//{
//    int id;
//    int durata;
//    float rating;
//    char* titlu;
//    char* regizor;
//    char gen;
//};
//
//
//typedef struct Heap Heap;
//struct Heap
//{
//    int lungime;
//    Film* vector;
//    int nrElemente;
//};
//
//
//Heap initializareHeap(int lungime)
//{
//    Heap h;
//
//    h.lungime = lungime;
//    h.nrElemente = 0;
//    h.vector = (Film*)malloc(sizeof(Film) * lungime);
//
//    return h;
//}
//
//
//void afisareFilm(Film film)
//{
//    printf("ID: %d\n", film.id);
//    printf("Durata: %d minute\n", film.durata);
//    printf("Rating: %.2f\n", film.rating);
//    printf("Titlu: %s\n", film.titlu);
//    printf("Regizor: %s\n", film.regizor);
//    printf("Gen: %c\n\n", film.gen);
//}
//
//
//Film citireFilmDinFisier(FILE* f)
//{
//    Film film;
//
//    char buffer[256];
//    char sep[3] = ",\n";
//    fgets(buffer, 255, f);
//
//    film.id = atoi(strtok(buffer, sep));
//    film.durata = atoi(strtok(NULL, sep));
//    film.rating= atof(strtok(NULL, sep));
//
//    char* aux;
//    aux = strtok(NULL, sep);
//    film.regizor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
//    strcpy(film.regizor, aux);
//
//    aux = strtok(NULL, sep);
//    film.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
//    strcpy(film.titlu, aux);
//
//    film.gen = strtok(NULL, sep)[0];
//
//    return film;
//}
//
//void afisareHeap(Heap heap)
//{
//    for (int i = 0; i < heap.nrElemente; i++)
//    {
//        afisareFilm(heap.vector[i]);
//    }
//}
//
//void afisareHeapAscuns(Heap heap)
//{
//    for (int i = heap.nrElemente; i < heap.lungime; i++)
//    {
//        afisareFilm(heap.vector[i]);
//    }
//}
//
//
//void filtreazaHeap(Heap heap, int pozitieNod)
//{
//    int pozFiuStg = 2 * pozitieNod + 1;
//    int pozFiuDr = 2 * pozitieNod + 2;
//    int pozMax = pozitieNod;
//
//    if (pozFiuDr < heap.nrElemente && heap.vector[pozFiuDr].id > heap.vector[pozMax].id)
//    {
//        pozMax = pozFiuDr;
//    }
//
//    if (pozFiuStg<heap.nrElemente && heap.vector[pozFiuStg].id > heap.vector[pozMax].id)
//    {
//        pozMax = pozFiuStg;
//    }
//
//    if (pozMax != pozitieNod)
//    {
//        Film aux = heap.vector[pozMax];
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
//    Heap heap = initializareHeap(5);
//
//    while (!feof(f))
//    {
//        heap.vector[heap.nrElemente++] = citireFilmDinFisier(f);
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
//Film extrageFilm(Heap* heap)
//{
//    //in caz ca heap ul este gol
//    Film f;
//    f.durata = 0;
//    f.gen = '-';
//    f.id = -1;
//    f.rating = 0;
//    f.titlu = NULL;
//    f.regizor = NULL;
//
//    if (heap->nrElemente > 0)
//    {
//        Film aux = heap->vector[0];
//        heap->vector[0] = heap->vector[heap->nrElemente - 1];
//        heap->vector[heap->nrElemente - 1] = aux;
//        heap->nrElemente--;//ca sa ascunzi acel ultim element dupa interschimbare scazi din nr de elemente
//
//        for (int j = (heap->nrElemente - 2) / 2; j >= 0; j--)
//        {
//            filtreazaHeap(*heap, j);
//        }
//        return aux;
//    }
//
//    return f;
//}
//
//void dezalocare(Heap* heap)
//{
//    for (int i = 0; i < heap->lungime; i++)
//    {
//        free(heap->vector[i].regizor);
//        free(heap->vector[i].titlu);
//    }
//    free(heap->vector);
//    heap->vector = NULL;
//    heap->nrElemente = 0;
//    heap->lungime = 0;
//}
//
//
//int main()
//{
//
//    Heap heap = citireHeapDinFisier("filme.txt");
//    afisareHeap(heap);
//
//    printf("==================EXTRAGERE FILM===================\n");
//
//    afisareFilm(extrageFilm(&heap));
//    afisareFilm(extrageFilm(&heap));
//    afisareFilm(extrageFilm(&heap));
//
//    printf("====================AFISARE HEAP ASCUNS===============\n ");
//    afisareHeapAscuns(heap);//SE VOR AFISA IN ORDINE CRESCATOARE ELEMENTELE "ASCUNSE"
//
//    dezalocare(&heap);
//	return 0;
//}