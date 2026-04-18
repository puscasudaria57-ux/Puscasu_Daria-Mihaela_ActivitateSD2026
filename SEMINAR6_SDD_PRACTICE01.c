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


//STIVA (structura stivei)
//voi reprezenta o stiva prin LSI
typedef struct NodSimplu NodSimplu;
struct NodSimplu
{
	Masina info;
	NodSimplu* next;
};

void afisareMasina(Masina masina)
{
	if (masina.id == -1)
	{
		printf("Masina nu exista!\n");
		return;
	}
	
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}



//functie de adaugare a unei masini in stiva
//primim ca parametri stiva pe care vrem sa o modificam (**) si Masina pe care vrem sa o adaugam in stiva
void pushStack(NodSimplu** stack, Masina masina)
{
	NodSimplu* nodNou = (NodSimplu*)malloc(sizeof(NodSimplu));//aloc spatiu noului nod pe care il voi adauga in stiva

	nodNou->info = masina;
	nodNou->next = (*stack);
	(*stack) = nodNou;
}

//Masina = functia returneaza o masina
//popStack = scoate un element din stiva
//NodSimplu * *stack = primeste adresa stivei, ca sa poata modifica inceputul ei

//extragem + stergere de la inceput (de aceea avem ** pt ca modificam!)
Masina popStack(NodSimplu** stack)
{
	Masina rezultat;
	rezultat.id = -1; //in caz ca stiva este goala, marcam id ul ca fiind -1 si in momentul in care se va returna acest
	//rezultat, masina rezultata va avea id = -1 => in functia de afisare se va intra pe ramura de if si se va afisa
	//"masina nu exista", astfel stim ca stiva este goala

	//dar daca stiva nu este goala, stergem primul nod, care este, de asemenea, ultimul nod adaugat (LIFO)
	if (*stack != NULL)
	{
		NodSimplu* p = *stack; //MA MUT CU STACK SI STERG CU P NODUL
		*stack = p->next;

		//retin informatia (Masina) din nodul care va fi sters in Masina rezultat
		rezultat = p->info; //shallow copy
		free(p); //daau free doar la acest pointer, nu si la atributele din masina spre care pointeaza p, pentru ca 
		//cu o linie mai sus s a facyt shallow copy, iar masina rezultat trebuie sa aiba acele atribute, nu trebuie a fi sterse
	}

	return rezultat;
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
	aux = strtok(NULL, sep);
	m.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.model, aux);

	aux = strtok(NULL, sep);
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.numeSofer, aux);

	m.serie = strtok(NULL, sep)[0];

	return m;
}

NodSimplu* citireStackDinFisier(const char* numeFisier)
{
	NodSimplu* stack = NULL;
	FILE* f = fopen(numeFisier, "r");

	if (f)
	{
		while (!feof(f))
		{
			pushStack(&stack, citireMasinaFisier(f));
		}
	}

	fclose(f);
	return stack;
}



//metoda care determina daca stiva este sau nu nulla (va returna 0 sau 1)
char emptyStack(NodSimplu* stack)
{
	return stack == NULL;//daca stiva este goala va returna 1(true), altfel 0(false)
}


void dezalocare(NodSimplu** stack)
{
	while (*stack != NULL)
	{
		//folosesc functia popStack si scot fiecare element din stiva si apoi dezaloc

		Masina masina = popStack(stack);

		if (masina.model != NULL)
			free(masina.model);
		if (masina.numeSofer != NULL)
			free(masina.numeSofer);
	}
}


//aceasta functie returneaza numarul de elemente din stiva
int size(NodSimplu* stack)
{
	NodSimplu* aux = stack;
	int count = 0;

	while (aux != NULL)
	{
		count++;
		aux = aux->next;
	}

	return count;
}



//QUEUE - aleg LSD

typedef struct Nod Nod;
struct Nod
{
	Masina info;
	Nod* next;
	Nod* prev;
};

typedef struct Queue Queue;
struct Queue //lista dubla inlantuita
{
	Nod* start;
	Nod* end;
};

//enqueue adauga la inceput
//enqueue scoate de la final

//enqueue = adaug? un element în coad?, adaugare la inceput si se va scoate de la final
void enqueue(Queue* queue, Masina masinaNoua)
{
	//adaugam la inceput
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = queue->start;
	if (queue->start != NULL)
	{
		queue->start->prev = nou;
	}
	else
	{
		queue->end = nou;
	}
	queue->start = nou; //indiferent!
}



//extragem de la final
//dequeue scoate un element din coada
//Queue* queue = prime?te coada prin pointer, ca s - o poat? modifica
Masina dequeue(Queue* queue) //un singur * pt ca deja campurile structurii Queue sunt *, deci modificam aici!
{
	if (queue->start == NULL) //daca coada este nula
	{
		Masina masina;
		masina.id = -1;//Pentru c? func?ia trebuie s? returneze ceva de tip Masina, chiar dac? nu are ce scoate
		return masina;
	}

	//dc nu este nula, retinem ultima masina, o salvezi în variabila masina pentru c? urmeaz? s? ?tergi nodul
	Masina masina = queue->end->info;

	if (queue->start == queue->end) // daca exista un singur element
	{
		/*start
			?
			[X]
			?
		end*/

		free(queue->start); //stergi unicul nod, apoi coada devine goala

		queue->start = NULL;
		queue->end = NULL;
	}
	else //dc exista mai multe noduri in coada
	{
		queue->end = queue->end->prev; //va retine adresa penultimului nod queue->end
		free(queue->end->next);//sterg ultimul nod, am folosit next pt ca acum queue->end pointeaza la penultimul nod si vrem sa il stergem pe ultimul nod
		queue->end->next = NULL;
	}
	
	return masina;
}


Queue citireCoadaMasiniDinFisier(const char* numeFisier)
{
	Queue queue;
	queue.end = NULL; //NU UITA CA INITIAL COADA, LISTA, STIVA SUNT GOALE
	queue.start = NULL;

	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		enqueue(&queue, citireMasinaFisier(f));
	}

	fclose(f);
	return queue;
}


void dezalocareCoadaDeMasini(Queue* queue) 
{
	//sunt dezalocate toate masinile si coada de elemente

	while (queue->start!=NULL) //cat timp coada nu e goala
	{
		Masina masina = dequeue(queue); //scoate un element din coada, face free pe nod si returneaza masina scoasa din coasa
		//ca mai apoi sa poti dezaloca atributele nume si model mai jos!
		/*Ce face dequeue :

			scoate ultimul nod
			face free pe nod
			returneaz? Masina*/

		if (masina.model != NULL)
			free(masina.model);
		if (masina.numeSofer != NULL)
			free(masina.numeSofer);


		/* de ce mai faci free ?
			if (masina.model != NULL)
				free(masina.model);

		?? pentru c? :

		? nodul a fost ?ters
			? dar string - urile din Masina NU

			?? ele sunt pe heap :

			model
			numeSofer*/
	}
}
//metode de procesare
Masina getMasinaByID(Queue* queue, int id)
{
	//se va folosi dequeue care modifica coada, dar eu imi doresc sa caut noduri dupa id, nu sa sterg =>voi folosi o coada
	//auxiliara aux
	Queue aux;
	aux.start = NULL;
	aux.end = NULL;

	Masina rezultat;
	rezultat.id = -1;

	while (queue->start != NULL)
	{
		Masina masina = dequeue(queue);
		if (masina.id == id)
		{
			rezultat = masina;
			rezultat.model = (char*)malloc(strlen(masina.model) + 1);
			strcpy(rezultat.model, masina.model);
			rezultat.numeSofer = (char*)malloc(strlen(masina.numeSofer) + 1);
			strcpy(rezultat.numeSofer, masina.numeSofer);
		}
		enqueue(&aux, masina); //refac coada, doar ca o voi face in aux

	}
	//la final refac si pointerii de la start si end pentru queue folosindu ne de aux
	queue->start = aux.start;
	queue->end = aux.end;

	return rezultat;
}


//varianta de functie fara distrugere
Masina getMasinaByID2(Queue* queue, int id)
{
	Nod* aux = queue->start; // pointer de parcurgere

	Masina rezultat;
	rezultat.id = -1;

	while (aux != NULL)
	{
		if (aux->info.id == id)
		{
			rezultat = aux->info;

			// deep copy
			rezultat.model = (char*)malloc(strlen(aux->info.model) + 1);
			strcpy(rezultat.model, aux->info.model);

			rezultat.numeSofer = (char*)malloc(strlen(aux->info.numeSofer) + 1);
			strcpy(rezultat.numeSofer, aux->info.numeSofer);
		}

		aux = aux->next;
	}

	return rezultat;
}


float calculeazaPretTotal(NodSimplu** stack) //vom face tot asa pop (voi distruge si reface stiva)
{
	NodSimplu* aux = NULL; //voi reface lista in acest nod si voi distruge stack prin pop

	int total = 0;
	while (*stack != NULL)
	{
		Masina masina = popStack(stack); //popStack returneaza o masina pe care o salvam in "masina"
		total = total + masina.pret;

		//refac lista in aux
		pushStack(&aux, masina);
	}
	while (aux != NULL)
	{
		pushStack(stack, popStack(&aux)); //introduc inapoi in stiva nodurile pe care scot cu push din aux
	}
	
	return total;
}


//functia de mai sus fara distrugere
float calculeazaPretTotal2(NodSimplu* stack)
{
	float total = 0;
	NodSimplu* aux = stack; // pointer de parcurgere

	while (aux != NULL)
	{
		total += aux->info.pret;
		aux = aux->next;
	}

	return total;
}


int main()
{
	Queue queue;
	queue = citireCoadaMasiniDinFisier("masini.txt");
	//scoate o masina din coada si o afiseaza
	Masina masina2 = dequeue(&queue);
	afisareMasina(masina2);
	//SAU afisareMasina(dequeue(&queue)); 

	Masina masina = getMasinaByID(&queue, 9);
	afisareMasina(masina);
	// SAU afisareMasina(getMasinaByID(&queue, 9));
	free(masina.model);//pt ca am alocat dinamic in getMasinaByID
	free(masina.numeSofer);

	return 0;
}