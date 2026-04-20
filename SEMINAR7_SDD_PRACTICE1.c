#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita
typedef struct Nod Nod;
struct Nod
{
	Masina info;
	Nod* next;
};

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
typedef struct HashTable HashTable;
struct HashTable
{
	int dim;
	Nod** vector; //*vector de pointeri Nod*
};


Masina citireMasinaDinFisier(FILE* file) 
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) 
{
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//primim pointer la primul nod ca sa avem acces dupa la toate nodurile din lista
void afisareListaMasini(Nod* cap)//aceasta fctie va fi folosita in cazul in care vrem sa afisam masinile dintr un 
//cluster, un chain (o lista simplu inlantuita), masinile de pe o anumita pozitie din hash table
{
	while (cap != NULL)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

//folosesc aceasta fctie cand am COLIZIUNE si vreau sa adaug un nod nou in lista, adaugarea va fi facuta la final, deci 
//nu mai transmit prin adresa (Nod** cap) ci transmit prin valoare (Nod* cap), pt ca nu fom modifica capul listei
void adaugaMasinaInListaLaFinal(Nod** cap, Masina masinaNoua)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;

	if (*cap != NULL) //dc in hash table nu avem pe acea pozitie NULL (adica daca lista nu este goala) adaugam la final nodul in lista
	{
		Nod* p = *cap; //imi iau un nod auxiliar cu care sa parcurg lista, ca sa nu modific capul
		while (p -> next != NULL)
		{

			p = p -> next;
		}
		p->next = nou;
	}
	else
	{
		*cap = nou;	//modific cap
	}
}

//trebuie sa initializez vectorul (hash table ul) si sa marcam faptul ca el este gol (NULL)
//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL;
HashTable initializareHashTable(int dimensiune)
{
	//trebuie sa ii aloc spatiu
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune); //alocam tabela(vectorul). tabela = vector de pointeri
	//dup ce aloc spatiu trebuie sa initializez toate acele pozitii din vector (tabela) cu valoarea NULL
	for (int i = 0;i < dimensiune;i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}

//voi tine cont in calcularea hash ului de numele soferului (trebuie sa fie un atribut unic pt fiecare masina in parte)
// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
int calculeazaHash(const char* numeSofer, int dimensiune)
{
	int suma = 0; //suma codurilor ascii din fiecare nume
	for (int i = 0; i < strlen(numeSofer); i++) //vector de caractere
	{
		suma = suma + numeSofer[i];
	}
	return suma % dimensiune;

	//sau return strlen(numeSofer)%dimensiune; //dar asa ai avea multe coliziuni
}

//dc ai fi avut fctia hash de calculat in functie de id ar fi aratat asa:
//int calculeazaHash(int id, int dimensiune) 
// {
//		id = id * 23 + 123; //sa fie cat mai diferit astfel incat sa evitam coliziunea
//		return id % dimensiune;
//}


//functia de inserare a unei masini intr o tabela hash
void inserareMasinaInTabela(HashTable hash, Masina masina)
{
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	//pozitia se determina prin apelul fctiei hash
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
	adaugaMasinaInListaLaFinal(&(hash.vector[pozitie]), masina);// &()

	//Nod** vector; --> hash.vector (vector de pointeri la Nod)
	//Nod* --> hash.vector[pozitie] (adresa primului nod din lista)
	//void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) functia are **, iar tu aici:hash.vector[pozitie] ai Nod* => &
}



//citirea tuturor masinilor dintr un fisier pe care sa le punem intr un HashTable!
HashTable citireMasiniDinFisierInHashTable(const char* numeFisier)
{
	//HASH TABLE UL TREBUIE INITIALIZAT, deci initiliaziarea tabelei de dispersie va avea loc aici
	HashTable ht = initializareHashTable(7);

	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			inserareMasinaInTabela(ht, citireMasinaDinFisier(f));
		}
	}
	else
	{
		printf("Eroare la deschiderea fisierului!\n");
	}
	fclose(f);//ATENTIE SA INCHIZI
	return ht;
}

//sunt afisate toate masinile cu evidentierea clusterelor realizate: "aceste masini sunt pe pozitia 0, 1, 2 etc "
//sa precizam si pozitia!
void afisareTabelaDeMasini(HashTable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL) //dc pozitia din tabela nu este goala 
		{
			printf("Masinile (Cluster ul) de pe pozitia %d sunt:\n", i);
			afisareListaMasini(ht.vector[i]);
		}
		else
		{
			printf("\nPe pozitia %d nu avem masini\n", i);
		}
	}
}

void dezalocareListaMasini(Nod** cap)
{
	while (*cap != NULL)
	{
		Nod* aux = *cap;
		*cap = aux->next; //inaintez capul si sterg cu aux

		if (aux->info.model != NULL)
			free(aux->info.model);
		if (aux->info.numeSofer != NULL)
			free(aux->info.numeSofer);

		free(aux);
			
	}
	*cap = NULL; //lista este goala
}


//dezaloc atat elementele din tabela cat si tabela, de aceea o vom transmite prin *
void dezalocareTabelaDeDispersie(HashTable* ht)
{
	for (int i = 0; i < ht->dim; i++)
	{
		dezalocareListaMasini(&(ht->vector[i]));
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

//calculeaza pretul mediu al masinilor din fiecare cluster => va rezulta un pointer la float (vector de preturii medii)
//avem nevoie de urmatorii parametri: tabela ht de dispersie si numarul de clustere, adica pozitiile din tabela de dispersie
//in care pointerii sunt diferiti de NULL, acest *nrClustere va fi dimensiunea vectorului de float* pe care il vom returna
float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere)
{
	float* preturiMedii; //momentan nu stiu cat spatiu sa ii aloc dinamic, deci trebuie sa trec prin toata tabela si sa
	//aflu un de am pozitia diferita de NULL

	(*nrClustere) = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			(*nrClustere)++;
		}
	}
	preturiMedii = (float*)malloc(sizeof(float) * (*nrClustere));

	
	int k = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			float suma = 0;
			int contor = 0;
			Nod* aux = ht.vector[i];//acest este capul fiecarei liste de masini in parte
			while (aux != NULL)
			{
				suma = suma + aux->info.pret;
				contor++;
				aux = aux->next;
			}
			preturiMedii[k++] = suma / contor;
		}
		
	}
	
	return preturiMedii;
	
}

Masina getMasinaByNumeDinLista(Nod* cap, const char* nume)
{
	Masina m;
	m.id = -1;

	while (cap && strcmp(cap->info.numeSofer,nume)!=0)
	{
		cap = cap->next;
	}
	if (cap != NULL) //inseamna ca mi a gasit o masina si de aceea a iesit din while, nu ca nu am gasit numele
	{
		m = cap->info; //shallow copy

		m.model = (char*)malloc(sizeof(char) * (strlen(cap->info.model) + 1));
		strcpy(m.model, cap->info.model);


		m.numeSofer = (char*)malloc(sizeof(char) * (strlen(cap->info.numeSofer) + 1));
		strcpy(m.numeSofer, cap->info.numeSofer);

	}

	return m;
}


//cautarile in cadrul tabelei vor fi mult mai usor de facut fata de vector, cheia aici este numele soferului
//getMasinaDupaCheie
//DEEP COPY
Masina getMasinaDupaNumeSofer(HashTable ht, const char* numeCautat)
{
	Masina m;
	m.id = -1;

	//nu are rost sa iteram prin tot tabelul => voi folosi fctia hash
	int poz = calculeazaHash(numeCautat, ht.dim); //in acest poz se va retina pozitia la care clusterul contine acea masina
	
	if (poz >= 0 && poz < ht.dim)
	{
		//functie de cautare a masinii respective in lista
		m = getMasinaByNumeDinLista(ht.vector[poz], numeCautat);
	}

	return m;
}

//SAU

Masina getMasinaByNume(HashTable ht, const char* numeCautat)
{
	Masina m;
	m.id = -1;
	//calculez pozitia pe care s ar putea afla clusterul ce contine masina cu numele cautat
	int poz = calculeazaHash(numeCautat, ht.dim);

	if (poz >= 0 && poz < ht.dim)
	{
		Nod* aux = ht.vector[poz]; //si acum aux va fi capul listei in care se afla masina cu numele cautat si acum cautam in lista
		while (aux)
		{
			if (strcmp(aux->info.numeSofer, numeCautat) == 0)
			{
				m = aux->info;

				//deep copy
				m.model = (char*)malloc(sizeof(char) * (strlen(aux->info.model) + 1));
				strcpy(m.model, aux->info.model);

				m.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux->info.numeSofer) + 1));
				strcpy(m.numeSofer, aux->info.numeSofer);
			}
			aux = aux->next;
		}
	}
	return m;
}

int main()
{
	//HashTable ht = initializareHashTable(7);//mi a initializat tabela cu o dimensiune de 7, dar am facut aceasta initializare 
	//in functia citireMasiniDinFisierInHashTable

	HashTable ht = citireMasiniDinFisierInHashTable("masini.txt");

	afisareTabelaDeMasini(ht);

	
	printf("Vectorul de preturi medii este:\n");
	int nrClustere; //nu trb sa initializezi cu 0 ca oricum initializarea asta are loc in functie
	float* pMedii = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	for (int i = 0; i < nrClustere; i++)
	{
		printf("%.2f\n", pMedii[i]);
	}

	printf("\n");

	Masina m = getMasinaByNume(ht, "Ionescu Vasile");
	afisareMasina(m);

	dezalocareTabelaDeDispersie(&ht);

	return 0;
}