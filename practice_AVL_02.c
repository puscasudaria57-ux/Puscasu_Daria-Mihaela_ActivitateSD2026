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
//typedef struct Nod Nod;
//struct Nod
//{
//    Film info;
//    Nod* stanga;
//    Nod* dreapta;
//};
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
////În citireFilmDinFisier, trebuie s? faci strtok în aceea?i ordine în care apar valorile pe fiecare linie din fi?ier!!
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
//    film.rating = atof(strtok(NULL, sep));
//
//    char* aux;
//
//    aux = strtok(NULL, sep);
//    film.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
//    strcpy(film.titlu, aux);
//
//    aux = strtok(NULL, sep);
//    film.regizor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
//    strcpy(film.regizor, aux);
//
//    film.gen = strtok(NULL, sep)[0];
//
//    return film;
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
//
//// functie specifica AVL
//// calculeaza gradul de echilibru al unui nod
//int verificareEchilibru(Nod* radacina)
//{
//    int st = calculeazaInaltimeArbore(radacina->stanga);
//    int dr = calculeazaInaltimeArbore(radacina->dreapta);
//
//    return st - dr;
//}
//
//
//void rotireStanga(Nod** radacina)
//{
//    Nod* aux = (*radacina)->dreapta;//retinem nodul din dreapta
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
//}
//
//
//void adaugaFilmInArboreDeEchilibru(Nod** radacina, Film filmNou)
//{
//    if (*radacina)
//    {
//        if ((*radacina)->info.id > filmNou.id)
//        {
//            adaugaFilmInArboreDeEchilibru(&(*radacina)->stanga, filmNou);
//        }
//        if ((*radacina)->info.id < filmNou.id)
//        {
//            adaugaFilmInArboreDeEchilibru(&(*radacina)->dreapta, filmNou);
//        }
//
//        int gradDeEchilibru = verificareEchilibru(*radacina);
//
//        if (gradDeEchilibru == -2)//DEZECHILIBRU PE DREAPTA
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
//        if (gradDeEchilibru == 2)
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
//        
//        }
//
//
//    }
//    else
//    {
//        Nod* nou = (Nod*)malloc(sizeof(Nod));
//        nou->info = filmNou;
//        nou->dreapta = NULL;
//        nou->stanga = NULL;
//        *radacina = nou;
//    }
//}
//
//
//Nod* citireArboreDeFilmeDinFisier(const char* numeFisier)
//{
//    FILE* f = fopen(numeFisier, "r");
//    Nod* radacina = NULL;
//
//    if (!f)
//    {
//        printf("EROARE!\n");
//        return NULL;
//    }
//
//    while (!feof(f))
//    {
//        adaugaFilmInArboreDeEchilibru(&radacina, citireFilmDinFisier(f));
//    }
//    fclose(f);
//
//    return radacina;
//}
//
//
//void afisareArboreInPreordine(Nod* radacina)
//{
//    if (radacina)
//    {
//        afisareFilm(radacina->info);
//        afisareArboreInPreordine(radacina->stanga);
//        afisareArboreInPreordine(radacina->dreapta);
//    }
//}
//
//int main()
//{
//    Nod* radacina = citireArboreDeFilmeDinFisier("filme.txt");
//    afisareArboreInPreordine(radacina);
//
//    return 0;
//}