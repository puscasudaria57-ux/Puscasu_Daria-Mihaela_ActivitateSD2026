#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMasina
{
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod Nod;
struct Nod
{
	Masina info;
	Nod* next;
	Nod* prev; //avem previous si next (lista dubla retine adresa urmatorului nod si a nodului precedent)
};


//Crearea strcuturii pt lista dubla!
typedef struct ListaDubla ListaDubla;
struct ListaDubla
{
	Nod* cap;
	Nod* coada;  
};


void afisareMasina(Masina masina)
{
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

// listaDubla.cap->info; informatia din primul nod
void afisareMasinaDeLaInceput(ListaDubla listaDubla) //la LSI: Nod* cap, dar aici avem nodurile in structura respectiva
//si in acea structura ei sunt deja pointeri
{
	Nod* aux = listaDubla.cap;  //pointer cu care ne deplasam, aux ia adresa primului nod (capul listei)
	while (aux != NULL)
	{
		afisareMasina(aux->info); 
		aux = aux->next; //avansez in lista de la inceput spre final

	}
}



void afisareMasinaDeLaFinal(ListaDubla listaDubla)
{
	Nod* aux = listaDubla.coada; //parcurgem de la coada spre cap

	while (aux != NULL) //la nodul de la inceput avem NULL, acolo ar trebui sa se opreasca parcurgerea listei
	{
		afisareMasina(aux->info);
		aux = aux->prev; //avansez in lista de la final spre inceput
	}
}


//este in oglinda cu adaugarea de mai jos
void adaugaMasinaLaInceputulListei(ListaDubla* listaDubla, Masina masinaNoua)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = listaDubla->cap;

	if (listaDubla->cap != NULL)//daca exista nod in stanga nodului adaugat, capul va pointa spre primul nod nou adaugat 
	{
		listaDubla->cap->prev = nou;//nou este deja o adresa, trebuie next pt ca trebuie sa facem legatura (last practic este un nod si nextul lui trb sa fie setat
	}
	else
	{
		//dc noul nod este singurul din lista=>va pointa si coada spre el
		listaDubla->coada = nou;//(*listaDubla).first = nou;
	}
	listaDubla->cap = nou;//se executa pt ambele cazuri pt ca ai adaugat un nod la inceput si trb cap sa pointeze spre el


}



//este in oglinda cu adaugarea de mai sus
void adaugaMasinaLaSfarsitulListei(ListaDubla* listaDubla, Masina masinaNoua)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = listaDubla->coada;

	//ACUM COADA NU MAI POINTEAZA UNDE TREBUIE, ACUM VA TREBUIE SA POINTEZE CATRE ULTIMUL NOD (adica catre nodul nou!)
	//deci daca suntem in situatia in care lista are noduri => dupa ce adaugi nodul in lista, coada va pointa spre
	//penultimul nod => listaDubla -> coada va fi diferit de NULL si daca se intampla asta vrem coada sa pointeze
	//spre ultimul nod (noul nod adaugat)
	if (listaDubla->coada != NULL)
	{
		listaDubla->coada->next = nou;
	}
	else
	{
		//aici se intra cand lista este goala si adaugam un element (adica listaDubla->coada si listaDubla->cap sunt NULL)!
		//si cap si coada vor pointa spre acelasi nod
		
		listaDubla->cap = nou;
	}
	listaDubla->coada = nou; //se executa no matter what
	 
}



Masina citireMasinaFisier(FILE* f)
{
	Masina m;

	char buffer[256];
	fgets(buffer, 255, f);
	char sep[3] = ",\n";

	m.id = atoi(strtok(buffer, sep));
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));

	char* aux;

	aux = strtok(NULL, sep); //acum avem sirul de caractere al modelului, ne ajuta ca putem sti dimensiunea cu strlen(aux)
	//aloc spatiu pt acest sir de caractere si dupa copiez
	m.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.model, aux);

	aux = strtok(NULL, sep); //acum avem sirul de caractere al modelului, ne ajuta ca putem sti dimensiunea cu strlen(aux)
	//aloc spatiu pt acest sir de caractere si dupa copiez
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.numeSofer, aux);


	m.serie = strtok(NULL, sep)[0];

	return m;
}


//void* (Nod*) dar listaDubla deja contine pointeri!
ListaDubla citireLDMasiniDinFisier(const char* numeFisier)
{
	ListaDubla listaMasini;
	listaMasini.cap = NULL; //lista va fi initial NULLA!
	listaMasini.coada = NULL;


	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			adaugaMasinaLaSfarsitulListei(&listaMasini, citireMasinaFisier(f));
		}
	}
	else
	{
		printf("Eroare la deschiderea fiserului!\n");
	}

	fclose(f);
	return listaMasini;
}

void dezalocare(ListaDubla* listaDubla)
{
	Nod* p = listaDubla->cap;  //= (*listDubla).cap
	while (p != NULL)
	{
		Nod* aux = p;
		p = p->next;//inaintez cu p si sterg cu aux

		if (aux->info.model != NULL)
			free(aux->info.model);
		if (aux->info.numeSofer != NULL)
			free(aux->info.numeSofer);

		//acum stergem efectiv nodul
		free(aux);
	}
	//la iesirea din while au fost sterse toate nodurile
	//acum si pointerii cap si coada trb sa fie nulle
	listaDubla->cap = NULL;
	listaDubla->coada = NULL;
	
}


float calculeazaPretMediu(ListaDubla listaDubla)
{
	Nod* aux = listaDubla.cap;
	int contor = 0;
	float suma = 0;
	while (aux != NULL)
	{
		suma = suma + aux->info.pret;
		contor++;

		aux = aux->next;
	}

	if (contor != 0)
	{
		return suma / contor;
	}

	return 0;
}

//id ul fiind unic se va sterge la fiecare apel o singura masina, ci nu mai multe cum era la LSI (ca nu stergi dupa sofer etc)
void stergeMasinaDupaID(ListaDubla* listaDubla, int idCautat)//trebuie sa primim si id ul dupa care vrem sa stergem masina (nodul)
{
	//trebuie tratata posibilitatea ca masina pe care vrem sa o stergem se afla pe prima sau ultima pozitie

	//exista noduri in lista sau nu?
	if (listaDubla->cap == NULL)
	{
		return; //dc lista este goala intrerupem direct parcurgerea (optimizare dpdv a timpului)
	}

	Nod* p = listaDubla->cap;
	while (p!=NULL && p->info.id != idCautat)//ma deplasez pana cand dam de masina cu id de sters, 
	//ne putem pozitiona chiar pe ea, ci nu inainte de ea cum faceam la LSI, si facem p!=NULL, ca in cazul in care nu avem in 
    //lista un nod cu id ul respectiv, vrem ca acest while sa se opreasca din cautari, sa nu cicleze la infinit
	//voi verifica asta mai jos cu p == NULL
	{
		p = p->next;
	}
	

	if (p == NULL)//suntem in situatia in care am cautat pana la finalul listei si n am gasit o masina cu acel id
	{
		return; //intrerupem direct executia functiei pt ca nu avem ce sterge
	}

	//DACA AVEMM CE STERGE:
	//avem posibilitatea ca nodul sa fie: fie primul nod, fie la inceput, fie la mijloc
	if (p->prev == NULL) //daca prevul este nul, inseamna ca este vorba de exact primul nod din lista! : PRIMUL NOD
	{
		//mut capul listei
		listaDubla->cap = p->next;
		if (listaDubla->cap != NULL)
		{
			//trebuie sa sterg legatura spre nodul sters
			listaDubla->cap->prev = NULL;
		}
	}
	else //pe aceasta ramura se intra daca e vorba de un nod de la mijloc sau de la final
	{
		p->prev->next = p->next; //nod bun, nod de sters, nod bun, primul nod bun trebuie sa aiba next ul celui de al doilea nod bun
		
	}
	if (p->next != NULL)
	{
//iar cel de al doilea nod bun trebuie sa aiba prev ul primului nod bun, dar trebuie sa verificam daca cel de al treilea nod exista
		p->next->prev = p->prev;
	}
	else //daca p->next este null inseamna ca primul nod bun trebuie sa fie ultimul adica coada sa pointeze acolo
	{
		//daca ajungi aici inseamna ca stergi exact ULTIMUL NOD
		listaDubla->coada = p->prev;
	}

	//dupa ce am trecut de toate aceste verificari stergem nodul
	//acum putem sterge p ul nodul acela de sters, de la dezalocare
	if (p->info.model != NULL)
		free(p->info.model);
	if (p->info.numeSofer != NULL)
		free(p->info.numeSofer);

	//acum stergem efectiv nodul
	free(p);

}

//char* pt ca returneaza numele care este char*!!!
char* getNumeMasinaScumpa(ListaDubla listDubla)
{
	//trebuie sa verificam daca exista noduri in lista!
	if (listDubla.cap != NULL)
	{
		Nod* max = listDubla.cap; //presupun ca nodul cu masina cea mai scumpa este primul nod
		Nod* aux = listDubla.cap; //nod cu care sa ma deplasez
		while (aux != NULL)
		{
			if (aux->info.pret > max->info.pret)
			{
				max = aux; //max va retine constant adresa nodului cu pret mai mare!
			}
			aux = aux->next;
		}

		//acum in max voi avea masina cu pretul cel mai mare si vrem sa luam de aici numele soferului, voi face deep copy
		char* numeMaxim = (char*)malloc(sizeof(char) * (strlen(max->info.numeSofer)));
		strcpy(numeMaxim, max->info.numeSofer);

		return numeMaxim;//se va returna char* pt ca char* numeMaxim, iar tipul returnat este char*
	}
	else
		return NULL;
	
}

int main()
{

	ListaDubla ld = citireLDMasiniDinFisier("masini.txt");
	//afisareMasinaDeLaFinal(ld);
	//afisareMasinaDeLaInceput(ld);

	float pretMediu = calculeazaPretMediu(ld);
	printf("Pretul mediul al masinilor este de: %.2f\n", pretMediu);

	stergeMasinaDupaID(&ld, 1);
	stergeMasinaDupaID(&ld, 5);
	stergeMasinaDupaID(&ld, 10);
	afisareMasinaDeLaInceput(ld);

	char* numeSoferMasinaScumpa = getNumeMasinaScumpa(ld);
	printf("Soferul cu cea mai scumpa masina este: %s\n", numeSoferMasinaScumpa);
	if (numeSoferMasinaScumpa != NULL) //pt ca am facut deep copy in functie trebuie sa dezalocam!
		free(numeSoferMasinaScumpa);

	dezalocare(&ld);

	return 0;
}