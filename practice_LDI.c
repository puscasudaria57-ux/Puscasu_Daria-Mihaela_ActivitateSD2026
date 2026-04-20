#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Film Film;
struct Film
{
    int id;
    int durata;
    float rating;
    char* titlu;
    char* regizor;
    char gen;
};

typedef struct Nod Nod;
struct Nod
{
    Film info;
    Nod* prev;
    Nod* next;

};

typedef struct ListaDubla ListaDubla;
struct ListaDubla
{
    Nod* start;
    Nod* end;
};


//Să se implementeze o funcție care citește un film dintr-o linie a unui fișier text și construiește un obiect de tip Film
Film citireFilmDinFisier(FILE* file)
{
    Film f;

    char buffer[256];
    fgets(buffer, 255, file);
    char sep[4] = " ,\n";

    f.id = atoi(strtok(buffer, sep));
    f.durata = atoi(strtok(NULL, sep));
    f.rating = atof(strtok(NULL, sep));

    char* aux;
    
    aux = strtok(NULL, sep);
    f.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
    strcpy(f.titlu, aux);

    aux = strtok(NULL, sep);
    f.regizor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
    strcpy(f.regizor, aux);

    f.gen = strtok(NULL, sep)[0];

    return f;
}


//functie care afiseaza un film
void afisareFilm(Film film)
{
    printf("ID: %d\n", film.id);
    printf("Durata: %d\n", film.durata);
    printf("Rating: %.2f\n", film.rating);
    printf("Titlu: %s\n", film.titlu);
    printf("Regizor: %s\n", film.regizor);
    printf("Gen: %c\n\n", film.gen);
}

//afisare lista de la inceput
void afisareListaDeLaInceput(ListaDubla listaDubla)
{
    Nod* aux = listaDubla.start;
    while (aux != NULL)
    {
        afisareFilm(aux->info);
        aux = aux->next;
    }
}

//afisare lista de la final
void afisareListaDeLaFinal(ListaDubla listaDubla)
{
    Nod* aux = listaDubla.end;
    while (aux != NULL)
    {
        afisareFilm(aux->info);
        aux = aux->prev;
    }
}

//Să se implementeze o funcție care adaugă un nou nod la începutul listei dublu înlănțuite.
//Inserarea unui film la începutul listei
void adaugaFilmInListaLaInceput(ListaDubla* listaDubla, Film filmNou)
{
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = filmNou;
    nou->prev = NULL;
    nou->next = listaDubla->start;
    if (listaDubla->start != NULL)
    {
        listaDubla->start->prev = nou;
    }
    else
    {
        listaDubla->end = nou;
    }
    listaDubla->start = nou;
}

//Inserarea unui film la finalul listei
void adaugaFilmInListaLaFinal(ListaDubla* listaDubla, Film filmNou)
{
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = filmNou;
    nou->next = NULL;
    nou->prev = listaDubla->end;
    if (listaDubla->end != NULL)
    {
        listaDubla->end->next = nou;
    }
    else
    {
        listaDubla->start = nou;
    }
    listaDubla->end = nou;
}


//Să se implementeze o funcție care citește toate filmele dintr-un fișier text și le inserează într-o listă dublu înlănțuită.
ListaDubla citireFilmeDinFisierInLD(const char* numeFisier)
{
    ListaDubla listaDubla;
    listaDubla.start = NULL;
    listaDubla.end = NULL;

    FILE* f = fopen(numeFisier, "r");
    if (f)
    {
        while (!feof(f))
        {
            adaugaFilmInListaLaFinal(&listaDubla, citireFilmDinFisier(f));
        }
    }

    fclose(f);
    return listaDubla;
}


void dezalocare2(ListaDubla* listaDubla)
{
    Nod* p = listaDubla->start;
    while (p!=NULL)
    {
        Nod* aux = p;
        p = p->next; //INAINTEZ CU P SI STERG CU AUX
        
        if (aux->info.regizor != NULL)
            free(aux->info.regizor);
        if (aux->info.titlu != NULL)
            free(aux->info.titlu);
        free(aux);

    }
    listaDubla->start = NULL;
    listaDubla->end = NULL;
}

//SAU
void dezalocare(ListaDubla* listaDubla)
{
    while (listaDubla->start != NULL)
    {
        Nod* aux = listaDubla->start;
        listaDubla->start = aux->next; //INAINTEZ CAPUL LISTEI SI STERG CU AUX

        if (aux->info.regizor != NULL)
            free(aux->info.regizor);
        if (aux->info.titlu != NULL)
            free(aux->info.titlu);
        free(aux);

    }
    listaDubla->start = NULL;
    listaDubla->end = NULL;
}


//Să se implementeze o funcție care calculează și returnează ratingul mediu al tuturor filmelor din lista dublu înlănțuită.
float getRatingMediu(ListaDubla listaDubla)
{
    float suma = 0;
    int contor = 0;
    Nod* aux = listaDubla.start;
    while (aux != NULL)
    {
        suma = suma + aux->info.rating;
        contor++;

        aux = aux->next;
    }

    if (contor != 0)
        return suma / contor;

    return 0;
}



//        Cerință:
//Să se implementeze o funcție care șterge din lista dublu înlănțuită filmul cu un anumit id.
//Se vor trata corect toate cazurile :
//
//ștergerea primului nod
//ștergerea ultimului nod
//ștergerea unui nod din mijloc
//lista goală
//film inexistent
void stergeFilmDupaID(ListaDubla* listaDubla, int idCautat)
{
    if (listaDubla->start == NULL)
    {
        printf("Lista este goala!\n");
        return;
    }

    Nod* aux = listaDubla->start;
    while (aux != NULL && (aux->info.id != idCautat) ) //cat timp nu am ajuns la finalul listei si nu am gasit film cu id ul cautat, sa se inainteze
    {
        aux = aux->next;
    }
    
    if (aux == NULL)//daca aux a ajuns NULL => nu exista film cu acel id pe care il cautam
    {
        printf("Nu exista film cu id ul cautat!\n");
        return;
    }
    else //dc am gasit film cu acel id il stergem
    {
        if (aux->prev == NULL) //dc nodul gasit este primul nod
        {
            listaDubla->start = aux->next; //mut capul la urmatorul nod
            if (listaDubla->start != NULL) //dc nodul de sters avea nod in stanga lui, sterg legatura de la nodul bun la cel de sters
            {
               // trebuie sa sterg legatura spre nodul sters
			    listaDubla->start->prev = NULL;
            }
        }
        else
        {
            aux->prev->next = aux->next; //sterg legaturile cu nodul din stanga pt ca acela sigur exista, verific dupa dc exista nod si in dreapta nodului de sters
           

            if (aux->next != NULL)//dc exista nod in stanga nodului de sters
            {
                aux->next->prev = aux->prev;

            }
            else
            {
                //daca ajungi aici inseamna ca stergi exact ULTIMUL NOD
                listaDubla->end = aux->prev;//coada va fi nodul din stanga nodului de sters
            }
                
        }

        if (aux->info.titlu)
            free(aux->info.titlu);
        if (aux->info.regizor)
            free(aux->info.regizor);

        free(aux);
    }
}

//Determinarea filmului cu rating maxim
Film getFilmRatingMaxim(ListaDubla listaDubla)
{
    Film fMax = listaDubla.start->info; //shallow copy, pp ca primul film din lista este cel cu rating maxim
    
    Nod* aux = listaDubla.start;
    while (aux)
    {
        if (aux->info.rating > fMax.rating)
        {
            fMax = aux->info; //shallow copy
        }

        aux = aux->next;

    }

    Film deReturnat = fMax; 

    //deep copy
    deReturnat.titlu = (char*)malloc(sizeof(char) * (strlen(fMax.titlu) + 1));
    strcpy(deReturnat.titlu, fMax.titlu);

    deReturnat.regizor = (char*)malloc(sizeof(char) * (strlen(fMax.regizor) + 1));
    strcpy(deReturnat.regizor, fMax.regizor);

    return deReturnat;
}



//Să se implementeze o funcție care caută un film în lista dublu înlănțuită după id și îl returnează prin deep copy.
Film getFilmByID(ListaDubla listaDubla, int idCautat)
{
    Film f;
    f.id = -1;

    Nod* aux = listaDubla.start;
    while (aux)
    {
        if (aux->info.id == idCautat)
        {
            f = aux->info; //aux->info = un film din lista dubla

            //deep copy
            f.titlu = (char*)malloc(sizeof(char) * (strlen(aux->info.titlu) + 1));
            strcpy(f.titlu, aux->info.titlu);

            f.regizor = (char*)malloc(sizeof(char) * (strlen(aux->info.regizor) + 1));
            strcpy(f.regizor, aux->info.regizor);
            
        }
        aux = aux->next;
    }

    return f;
}


//Să se implementeze o funcție care returnează titlul filmului cu durata cea mai mare din listă.
//Titlul va fi returnat prin deep copy.
char* getTitluDurataMax(ListaDubla ld)
{
    Film durataMax = ld.start->info;

    Nod* aux = ld.start;
    while (aux)
    {
        if (durataMax.durata < aux->info.durata)
        {
            durataMax = aux->info;
        }
        aux = aux->next;
    }

    Film f = durataMax;

    f.titlu = (char*)malloc(sizeof(char) * (strlen(durataMax.titlu) + 1));
    strcpy(f.titlu, durataMax.titlu);

    f.regizor = (char*)malloc(sizeof(char) * (strlen(durataMax.regizor) + 1));
    strcpy(f.regizor, durataMax.regizor);

    return f.titlu;
}


//Să se determine numărul de filme din listă care aparțin unui anumit regizor.
int numarFilmeRegizor(ListaDubla ld, const char* numeRegizor)
{
    int contor = 0;

    Nod* aux = ld.start;
    while (aux != NULL)
    {
        if (aux->info.regizor != NULL && strcmp(aux->info.regizor, numeRegizor) == 0)
        {
            contor++;
        }
        aux = aux->next;
    }

    return contor;
}

int main()
{

    ListaDubla ld = citireFilmeDinFisierInLD("filme.txt");
    //afisareListaDeLaFinal(ld);

    float rMediu = getRatingMediu(ld);
    //printf("Rating mediu: %.2f\n", rMediu);

    /*stergeFilmDupaID(&ld, 1);
    stergeFilmDupaID(&ld, 3);
    stergeFilmDupaID(&ld, 5);
    afisareListaDeLaInceput(ld);*/

    Film f = getFilmRatingMaxim(ld);
    afisareFilm(f);
    if (f.titlu != NULL)
        free(f.titlu);
    if (f.regizor != NULL)
        free(f.regizor);

    Film f2 = getFilmByID(ld, 1);
    afisareFilm(f2);
    if (f2.titlu) free(f2.titlu);
    if (f2.regizor) free(f2.regizor);

    char* titlu = getTitluDurataMax(ld);
    printf("Titlul filmului cu durata maxima este: %s", titlu);

    return 0;
}

