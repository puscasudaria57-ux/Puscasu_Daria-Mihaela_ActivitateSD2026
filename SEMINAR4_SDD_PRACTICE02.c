#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraFilm
{
    int id;
    int durata;
    float rating;
    char* titlu;
    char* regizor;
    char gen;
};
typedef struct StructuraFilm Film;

typedef struct Nod Nod;
struct Nod
{
    Film info;
    Nod* next;
};

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

//functie care citeste un film din fisier si RETURNEAZA un film => tipul returnat: Film, parametrul primit: Fisierul
Film citireFilmDinFisier(FILE* f)
{
    char linie[256];
    fgets(linie, 255, f); //citirea unui rand (linii) din fisier in linie
    char sep[4] = ", \n";//delimitatorii din fisierul text 
    //strtok sparge un sir mare de caractere si returneaza bucati (tokens), mai intai id apoi durata etc

    Film film;
    film.id = atoi(strtok(linie, sep));
    film.durata = atoi(strtok(NULL, sep));
    film.rating = atof(strtok(NULL, sep));

    char* aux; //avem nevoie de acest aux ca sa aflam lungimea sirului de caractere titlu si regizor si sa putem aloca dinamic
    aux = strtok(NULL , sep);//aux acum va retine titlul
    film.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
    strcpy(film.titlu, aux);

    //continuam si facem la fel si pt regizor
    aux = strtok(NULL, sep); //aux acum va retine regizorul
    film.regizor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
    strcpy(film.regizor, aux);

    film.gen = strtok(NULL, sep)[0]; //fiind un char adica un singur caracter, avem 2 pozitii: caracterul si \n, dar pe noi
    //ne intereseaza doar primul caracter : [0]

    return film;
}

//afisarea listei de filme prin apelul functiei afisareFilm
void afisareListaFilme(Nod* cap)
{
    while (cap != NULL)
    {
        afisareFilm(cap->info);
        cap = cap->next;
    }
}

void adaugaFilmLaFinalulListei(Nod* *cap, Film filmNou)
{
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = filmNou;
    nou->next = NULL;

    if (*cap != NULL)
    {
        Nod* p = *cap;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = nou;
    }
    else
    {
        *cap = nou;
    }
}

void adaugaFilmLaInceputulListei(Nod** cap, Film filmNou)
{
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = filmNou;
    nou->next = *cap;
    *cap = nou;
}

//Citi?i dintr-un fi?ier text informa?iile despre filme (id, durat?, rating, titlu, regizor, gen) 
// ?i construi?i o list? simplu înl?n?uit?.
Nod* citireListaFilmeDinFisier(const char* numeFisier)
{
    Nod* cap = NULL; //lista goala initial
    FILE* f = fopen(numeFisier, "r");
    if (f)
    {
        while (!feof(f))
        {
            adaugaFilmLaFinalulListei(&cap, citireFilmDinFisier(f));
        }
    }
    else
    {
        printf("Fisierul este vid!\n\n");
    }

    fclose(f);
    return cap;//returnez lista intrucat functia este de tipul listei respective
}


void dezalocare(Nod** cap)
{
    //parcurg toate nodurile pana la ultimul
    while (*cap != NULL)
    {
        Nod* p = *cap; //cap pt ca inainteaza p ul va lua in continuare adresa pe care o retine in fiecare moment

        //cap inaiteaza
        *cap = (*cap)->next;//NU UITA SA DEREFERNTIEZI CAP

        if (p->info.titlu != NULL)
            free(p->info.titlu);
        if (p->info.regizor != NULL)
            free(p->info.regizor);
        free(p);
    }
   
}

float ratingMediu(Nod* cap)
{
    float suma = 0;
    int contor = 0;
    while (cap != NULL)
    {
        suma = suma + cap->info.rating;
        contor++;

        //NU UITA SA INAINTEZI
        cap = cap->next;
    }
    if (contor != 0)
    {
        return suma / contor;
    }
    return 0;
}


//Determina?i filmul (=> functia va returna un FILM (informatia utila) cu rating maxim din list?.
Film ratingMaxim(Nod* cap)
{
    
    //daca lista este vida? =>rating 0
    if (cap == NULL)
    {
        Film f;
        f.rating = 0;
        return f;
    }
    else
    {
        //presupun ca ratingul maxim este al primului film
        Film filmRatingMaxim = cap->info;
        while (cap != NULL)
        {
            if (cap->info.rating > filmRatingMaxim.rating)
                filmRatingMaxim = cap->info;

            cap = cap->next;
        }

        return filmRatingMaxim;
    }
}

//Realiza?i o func?ie care afi?eaz? doar filmele (=> VOI FOLOSI FUNCTIA DE AFISARE A UNEI STRUCTURI FILM, VOI APELA 
// FUNCTIA: afisareFilm) care au ratingul mai mare decât o valoare dat?.
void afisareFilmeRatingMaiMare(Nod* cap, float prag)
{
    while (cap != NULL)
    {
        if (cap->info.rating > prag)
            afisareFilm(cap->info); //cap->info este de fapt un FILM!

        cap = cap->next;//inaintez in lista
    }
}


//?terge?i toate filmele care au un anumit gen(primit ca parametru).
//
//trata?i cazul în care elementele sunt la începutul listei
//trata?i cazul în care sunt în mijloc sau la final
//pot exista mai multe filme de ?ters
void stergeFilmDupaGen(Nod** cap, char genCautat)
{
    //cap != NULL pt ca lista poate fi formata doar din genul respectiv, deci am ajunge la final 
    while (*cap != NULL && (*cap)->info.gen == genCautat)
    {
        Nod* p = *cap;
        *cap = p->next;

        if (p->info.titlu != NULL)
            free(p->info.titlu);
        if (p->info.regizor != NULL)
            free(p->info.regizor);
        free(p);
    }

    //in acest moment ai nevoie de un cap "auxiliar", o copie cu care sa parcurgi lista, in cazul in care ai o situatie de genul:
    //nod bun, nod de sters, nod bun, intrucat deja la inceput nu mai ai sigur noduri de sters pt ca mai sus in acel while le ai sters
    Nod* aux = *cap;
    while (aux != NULL)
    {
        //cat timp exista un nod urmator (next) si genul este diferit de genul cautat, la iesirea din bucla vom salva nodul
        //la care am ramas, nodul cu genul cautat intr un nod temporar temp si il vom sterge, mai apoi vom lega aux de nodul 
        //din dreapta nodului sters, in aceste momente cap nu va mai suferi modificari, intrucat ne am asigurat inca de la 
        //inceput ca nu avem noduri cu genul cautat la inceputul listei
        while (aux->next && aux->next->info.gen != genCautat)
        {
            aux = aux->next;
        }
        if (aux->next!= NULL)
        {
            Nod* temp = aux->next;//nodul de sters!

            //voi lega nodul bun de urmatorul nod bun dupa ce am sters
            aux->next = aux->next->next; //sau temp->next, ca temp stim ca este nodul de sters
            //nod bun, nod de sters, nod bun

            if (temp->info.titlu != NULL)
                free(temp->info.titlu);
            if (temp->info.regizor != NULL)
                free(temp->info.regizor);
            free(temp);

            
        }
        else
        {
            aux = NULL;//ca sa iesim din while ul initial
        }
        

    }
}


int main()
{
    Nod* cap = citireListaFilmeDinFisier("filme.txt");
    afisareListaFilme(cap);

    //dezalocare(&cap);
    //afisareListaFilme(cap);

    printf("Ratingul mediu al filmelor este: %.2f\n\n", ratingMediu(cap));

    printf("Filmul cu rating maxim este: \n");
    Film filmRMaxim = ratingMaxim(cap);
    afisareFilm(filmRMaxim);

    printf("Filmele cu rating mai mare decat pragul dat sunt: \n");
    afisareFilmeRatingMaiMare(cap, 8.0);

    printf("Stergere filme dupa gen:\n");
    stergeFilmDupaGen(&cap, 'S');
    afisareListaFilme(cap);

    return 0;

}
