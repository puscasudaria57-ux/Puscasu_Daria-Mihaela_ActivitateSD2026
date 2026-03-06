#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


struct StructuraFilm 
{
    int id;
    int durata;
    float rating;
    char* titlu;
    char* regizor;
    char gen;
};
typedef struct StructuraFilm Film; //in loc sa scriem "struct StructuraFilm" voi scrie "Film"


void afisareFilm(Film film)
{
    printf("ID: %d\n", film.id);
    printf("Durata: %d minute\n", film.durata);
    printf("Rating: %.2f\n", film.rating);
    printf("Titlu: %s\n", film.titlu);
    printf("Regizor: %s\n", film.regizor);
    printf("Gen: %c\n\n", film.gen);
}


void afisareVectorFilme(Film* filme, int nrFilme)
{
    if (filme == NULL || nrFilme == 0)
    {
        printf("Vectorul de filme este gol!\n");
        return;
    }
    else
    {
        for (int i = 0; i < nrFilme; i++)
        {
            afisareFilm(filme[i]);
        }
    }
}


void adaugaFilmInVector(Film** filme, int* nrFilme, Film FilmNou)
{
    Film* temp = (Film*)malloc(sizeof(Film) * ((*nrFilme) + 1)); //alocam memorie pentru vectorul temporar, alocam nrFilme+1! 
    //si pe pozitia lui nrFilme adaug FilmNou! apoi cresc nrFilme(de aceea este transmis prin adresa ca sa putem modifica valoarea, nu uita de dereferentiere!)

    for (int i = 0; i < (*nrFilme); i++)
    {
        temp[i] = (*filme)[i]; //facem shallow copy
    }
    //adaug elementul (filmul nou) pe ultima pozitie adica pe pozitia *nrFilme (indexare de la 0 la nrFilme-1)
    temp[*nrFilme] = FilmNou;

    free(*filme); //stergem tot ce se afla la adresa spre care pointeaza filme si acum in loc sa facem * filme=NULL, ii vom atribui 
    //vectorul temporar, iar cei doi pointeri vor pointa spre aceeasi zona de memorie, de aceea este ok ca am facut shallow copy,
    //indiferent ca am dezalocat intregul vector, nu am dezalocat si atributele char* alocate dinamic, 
    // de aceea ele vor ramane prin shallow copy in temp, respectiv in vectorul filme
    //Adica: nu se sterg titlu si regizor pt ca ei sunt pointeri alocati separat!
    (*filme) = temp;
    (*nrFilme)++; //crestem nr de elemente ale vectorului, prin dereferentiere se va modifica variabila originala
    //variabila temp se afla pe stack ul functiei, ea doar pointeaza spre heap, deci la finalul functiei variabila temp dispare
    //de pe stack, iar heapul nu este afectata cand functia se termina!
}


Film citireFilmFisier(FILE* file)
{
    Film f; //voi citi un film, am nevoie de asta

   //buffer / linie = citesc si salvez temporar intr un buffer, este de fapt o linie din fisierul acela text ( o linie = un film in acel fisier text)
   char buffer[256]; 
   char delimitatori[3] = ",\n";//avem ca delimitatori , si \n 
   fgets(buffer, 255, file); //file = stream ul din care vom citi, 255 pt ca ultimul caracter este pentru termiantorul de sir
   //deci acum prin fgets am citit o intreaga linie in buffer

   //vom sparge aceasta linie prin strtok, strtok returneaza un sir de caractere (un token = ce gaseste pana la primul separaator) 
   //strtok sparge un sir mare de caractere si returneaza bucati (tokens), va returna mai intai id ul etc..
   //strtok returneaz? un pointer(char*) c?tre primul caracter al token - ului g?sit în ?irul ini?ial

  f.id = atoi( strtok(buffer, delimitatori) ) ;//atoi = ascii to integer
  f.durata = atoi(strtok(NULL, delimitatori)); //prin acest NULL de fapt se uita la ultimul apel (exact unde am ramas) si sparge in continuare de unde a ramas 
  //daca i am dat tot buffer s ar duce la inceputul liniei si ar afisa la nesfarsit doar id ul 
  f.rating = atof(strtok(NULL, delimitatori));

  char* aux;
  aux = strtok(NULL, delimitatori); //voi avea sirul de caractere a titlului, ne folosim de aux ca sa aflam lungimea titlului si sa putem aloca dinamic
  f.titlu = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
  strcpy(f.titlu, aux);

  aux = strtok(NULL, delimitatori);
  f.regizor = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
  strcpy(f.regizor, aux);

  f.gen = strtok(NULL, delimitatori)[0]; //fiind un char adica un singur caracter, avem 2 pozitii: caracterul si \n, dar pe noi
  //ne intereseaza doar primul caracter : [0]

  return f; //functia returneaza o linie, o structura de tip film
}


//deschid fisierul o singura data si pt fiecare film in parte apelez functia de citire a unui singur film de mai sus
//iar fiecare film in parte il adaug in vectorul de filme prin functia de adaugare a unui film de mai sus
//Film* este un pointer adica trebuie ca functia sa mi returneze adresa unde eu am acel vector
Film* citireVectorFilmeDinFisier(const char* numeFisier, int* nrFilmeCitite)
{
    //functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
    //prin apelul repetat al functiei citireMasinaFisier()
    //numarul de masini este determinat prin numarul de citiri din fisier
    //ATENTIE - la final inchidem fisierul/stream-ul


    FILE* file = fopen(numeFisier, "r");//r = read, fisierul se deschide in modul citire

    if (!file)
    {
        printf("Eroare la deschiderea fisierului\n");
        return NULL;
    }
    else
    {
        Film* filme = NULL;
        (*nrFilmeCitite) = 0;
        //trb sa apelez functia de adaugare a unui film pana ajung la finalul fiserului
        //marcator/flag care ne arata ca am ajuns la finalul fisierului : feof
        
        while (!feof(file))
        {
            adaugaFilmInVector(&filme, nrFilmeCitite, citireFilmFisier(file));
            //ultimul param al fctiei adauga film este Film FilmNou si acest Film Nou va fi dat de functia de citire a unui film din fisier
        }
        fclose(file);
        return filme;
    }
}


void dezalocare(Film** filme,int* nrFilme)
{
    for (int i = 0; i < (*nrFilme); i++)
    {
        if ((*filme)[i].titlu != NULL)
        {
            free((*filme)[i].titlu);

        }
        if ((*filme)[i].regizor != NULL)
        {
            free((*filme)[i].regizor);

        }
    }
    free(*filme);
    *filme = NULL;
    (*nrFilme) = 0;
}

int main()
{
    int nrFilme = 0; //dimensiunea vectorului de filme, dimensiune initiala, ea va creste 
    Film* filme = citireVectorFilmeDinFisier("filme.txt",&nrFilme);
    afisareVectorFilme(filme, nrFilme);


    dezalocare(&filme, &nrFilme);

    return 0;
}