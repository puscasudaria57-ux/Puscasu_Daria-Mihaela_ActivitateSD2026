#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ARBORE AVL = arbore binar de cautare echilibrat

typedef struct Masina Masina;
struct Masina
{
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};

// structura nodului este la fel ca la ABC
typedef struct Nod Nod;

struct Nod
{
    Masina info;   // informatia din nod
    Nod* stanga;  // copil stanga: id-uri mai mici
    Nod* dreapta; // copil dreapta: id-uri mai mari
};

void afisareMasina(Masina masina)
{
    printf("Id: %d\n", masina.id);
    printf("Nr usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

Masina citireMasinaDinFisier(FILE* file)
{
    Masina m;

    char buffer[256];
    char sep[3] = ",\n";

    fgets(buffer, 255, file);

    m.id = atoi(strtok(buffer, sep));
    m.nrUsi = atoi(strtok(NULL, sep));
    m.pret = atof(strtok(NULL, sep));

    char* aux;

    aux = strtok(NULL, sep);
    m.model = (char*)malloc(strlen(aux) + 1);
    strcpy(m.model, aux);

    aux = strtok(NULL, sep);
    m.numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m.numeSofer, aux);

    m.serie = strtok(NULL, sep)[0];

    return m;
}

// functie comuna cu ABC
// calculeaza inaltimea arborelui/subarborelui
int calculeazaInaltimeArbore(Nod* radacina)
{
    if (radacina)
    {
        // calculam inaltimea subarborelui stang
        int st = calculeazaInaltimeArbore(radacina->stanga);

        // calculam inaltimea subarborelui drept
        int dr = calculeazaInaltimeArbore(radacina->dreapta);

        // inaltimea = 1 + maximul dintre stanga si dreapta
        return 1 + (st > dr ? st : dr); //max(st,dr);
    }

    return 0; // daca nodul este NULL, inaltimea este 0
}

// functie specifica AVL
// calculeaza gradul de echilibru al unui nod
int verificareEchilibru(Nod* radacina)
{
    // inaltimea subarborelui stang
    int st = calculeazaInaltimeArbore(radacina->stanga);

    // inaltimea subarborelui drept
    int dr = calculeazaInaltimeArbore(radacina->dreapta);

    // grad echilibru = stanga - dreapta
    return st - dr;
}

// functie specifica AVL
// se foloseste cand arborele este dezechilibrat spre dreapta
void rotireStanga(Nod** radacina)
{
    // retinem copilul din dreapta
    Nod* aux = (*radacina)->dreapta;

    // stanga lui aux devine dreapta vechii radacini
    (*radacina)->dreapta = aux->stanga;

    // vechea radacina devine copilul stang al lui aux
    aux->stanga = *radacina;

    // aux devine noua radacina
    *radacina = aux;
}

// functie specifica AVL
// se foloseste cand arborele este dezechilibrat spre stanga
void rotireDreapta(Nod** radacina)
{
    // retinem copilul din stanga
    Nod* aux = (*radacina)->stanga;

    // dreapta lui aux devine stanga vechii radacini
    (*radacina)->stanga = aux->dreapta;

    // vechea radacina devine copilul drept al lui aux
    aux->dreapta = *radacina;

    // aux devine noua radacina
    *radacina = aux;
}

// functie specifica AVL
// insereaza masina ca intr-un ABC, apoi echilibreaza arborele
void adaugaMasinaInArboreEchilibrat(Nod** radacina, Masina masinaNoua)
{
    if (*radacina == NULL)
    {
        // daca pozitia este libera, cream nod nou
        Nod* nou = (Nod*)malloc(sizeof(Nod));

        nou->info = masinaNoua;
        nou->stanga = NULL;
        nou->dreapta = NULL;

        *radacina = nou; // legam nodul nou in arbore
    }
    else
    {
        // inserare normala ca in ABC
        if (masinaNoua.id > (*radacina)->info.id)
        {
            // id mai mare -> dreapta
            adaugaMasinaInArboreEchilibrat(&(*radacina)->dreapta, masinaNoua);
        }
        else if (masinaNoua.id < (*radacina)->info.id)
        {
            // id mai mic -> stanga
            adaugaMasinaInArboreEchilibrat(&(*radacina)->stanga, masinaNoua);
        }

        // dupa inserare verificam echilibrul nodului curent
        int gradEchilibru = verificareEchilibru(*radacina);

        // grad -2 inseamna dezechilibru pe dreapta
        if (gradEchilibru == -2)
        {
            if (verificareEchilibru((*radacina)->dreapta) <= 0)
            {
                // caz dreapta-dreapta
                rotireStanga(radacina);
            }
            else
            {
                // caz dreapta-stanga
                rotireDreapta(&(*radacina)->dreapta);
                rotireStanga(radacina);
            }
        }

        // grad 2 inseamna dezechilibru pe stanga
        if (gradEchilibru == 2)
        {
            if (verificareEchilibru((*radacina)->stanga) >= 0)
            {
                // caz stanga-stanga
                rotireDreapta(radacina);
            }
            else
            {
                // caz stanga-dreapta
                rotireStanga(&(*radacina)->stanga);
                rotireDreapta(radacina);
            }
        }
    }
}

// aproape la fel ca la ABC
// diferenta: apelam adaugaMasinaInArboreEchilibrat
Nod* citireArboreDeMasiniDinFisier(const char* numeFisier)
{
    FILE* file = fopen(numeFisier, "r");

    Nod* radacina = NULL;

    if (!file)
    {
        printf("Eroare deschidere fisier!\n");
        return NULL;
    }

    while (!feof(file))
    {
        Masina masina = citireMasinaDinFisier(file);

        // aici este diferenta fata de ABC:
        // inseram in arbore echilibrat
        adaugaMasinaInArboreEchilibrat(&radacina, masina);
    }

    fclose(file);

    return radacina;
}

// afisare in preordine: radacina -> stanga -> dreapta
void afisareMasiniDinArborePreordine(Nod* radacina)
{
    if (radacina)
    {
        afisareMasina(radacina->info);
        afisareMasiniDinArborePreordine(radacina->stanga);
        afisareMasiniDinArborePreordine(radacina->dreapta);
    }
}

// afisare in inordine: stanga -> radacina -> dreapta
void afisareMasiniDinArboreInordine(Nod* radacina)
{
    if (radacina)
    {
        afisareMasiniDinArboreInordine(radacina->stanga);
        afisareMasina(radacina->info);
        afisareMasiniDinArboreInordine(radacina->dreapta);
    }
}

// afisare in postordine: stanga -> dreapta -> radacina
void afisareMasiniDinArborePostordine(Nod* radacina)
{
    if (radacina)
    {
        afisareMasiniDinArborePostordine(radacina->stanga);
        afisareMasiniDinArborePostordine(radacina->dreapta);
        afisareMasina(radacina->info);
    }
}

// la fel ca la ABC
void dezalocareArboreDeMasini(Nod** radacina)
{
    if (*radacina)
    {
        dezalocareArboreDeMasini(&(*radacina)->stanga);
        dezalocareArboreDeMasini(&(*radacina)->dreapta);

        free((*radacina)->info.model);
        free((*radacina)->info.numeSofer);

        free(*radacina);
        *radacina = NULL;
    }
}

// la fel ca la ABC
Masina getMasinaByID(Nod* radacina, int id)
{
    if (radacina)
    {
        if (radacina->info.id == id)
        {
            Masina m;

            m.id = radacina->info.id;
            m.nrUsi = radacina->info.nrUsi;
            m.pret = radacina->info.pret;
            m.serie = radacina->info.serie;

            m.model = (char*)malloc(strlen(radacina->info.model) + 1);
            strcpy(m.model, radacina->info.model);

            m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
            strcpy(m.numeSofer, radacina->info.numeSofer);

            return m;
        }

        if (radacina->info.id < id)
        {
            return getMasinaByID(radacina->dreapta, id);
        }
        else
        {
            return getMasinaByID(radacina->stanga, id);
        }
    }

    Masina m;
    m.id = -1;
    m.nrUsi = 0;
    m.pret = 0;
    m.model = NULL;
    m.numeSofer = NULL;
    m.serie = '-';

    return m;
}

// la fel ca la ABC
int determinaNumarNoduri(Nod* radacina)
{
    if (radacina)
    {
        return 1
            + determinaNumarNoduri(radacina->stanga)
            + determinaNumarNoduri(radacina->dreapta);
    }

    return 0;
}

// la fel ca la ABC
float calculeazaPretTotal(Nod* radacina)
{
    if (radacina)
    {
        return radacina->info.pret
            + calculeazaPretTotal(radacina->stanga)
            + calculeazaPretTotal(radacina->dreapta);
    }

    return 0;
}

// la fel ca la ABC
float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer)
{
    if (radacina)
    {
        float suma = calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer)
            + calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);

        if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
        {
            suma += radacina->info.pret;
        }

        return suma;
    }

    return 0;
}

int main()
{
    Nod* radacina = citireArboreDeMasiniDinFisier("masini.txt");

    printf("Preordine:\n");
    afisareMasiniDinArborePreordine(radacina);

    printf("Numar noduri: %d\n", determinaNumarNoduri(radacina));
    printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
    printf("Pret total: %.2f\n", calculeazaPretTotal(radacina));
    printf("Pret masini sofer Gigel: %.2f\n", calculeazaPretulMasinilorUnuiSofer(radacina, "Gigel"));

    Masina m = getMasinaByID(radacina, 7);

    if (m.id != -1)
    {
        printf("\nMasina gasita:\n");
        afisareMasina(m);

        // pentru ca getMasinaByID face malloc, dezalocam copia
        free(m.model);
        free(m.numeSofer);
    }
    else
    {
        printf("\nMasina nu a fost gasita.\n");
    }

    dezalocareArboreDeMasini(&radacina);

    return 0;
}