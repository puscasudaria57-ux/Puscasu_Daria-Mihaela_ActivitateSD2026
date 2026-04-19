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
	Nod* prev;
};

typedef struct Queue Queue;
struct Queue
{
	Nod* start;
	Nod* end;
};

//Realiza?i o func?ie care adaug? o ma?in? într-o coad? (enqueue). ADAUGARE LA INCEPUT
void enqueue(Queue* queue, Masina masinaNoua)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = queue->start;
	nou->prev = NULL;

	if (queue->start != NULL)//dc coada nu este goala trb sa facem legatura dintre primul nod si nodul pe care il adaugam nou
	{
		queue->start->prev = nou;
	}
	else
	{
		queue->end = nou;
	}
	queue->start = nou;//indiferent pt ca adaugi un nod la inceput si start trb sa pointeze spre acel nod!
}

//Realiza?i o func?ie care extrage o ma?in? din coad? (dequeue), tratând cazul în care coada este goal?.
Masina dequeue(Queue* queue)
{
	if (queue->start == NULL) //dc coada e goala setez id=-1 si se va afisa "Masina nu exista" la apelul fctiei afisareMasina
	{
		Masina masina;
		masina.id = -1;
		return masina;
	}

	//DACA COADA NU ESTE NULA => retin ultima masina care va fi stearsa (retin informatia din ultimul nod)
	Masina masinaDeSters = queue->end->info; //queue->end->info = masina din ultimul nod

	if (queue->start == queue->end)//dc in coada exista un singur nod
	{
		free(queue->start); //stergi unicul nod, apoi coada devine goala
		queue -> start = NULL;
		queue->end = NULL;
	}
	else
	{
		queue->end = queue->end->prev;
		free(queue->end->next);
		queue->end->next = NULL;
		
	}

	return masinaDeSters;
}

void dezalocare(Queue* queue)
{
	while (queue->start != NULL)
	{
		Masina masina = dequeue(queue);

		if (masina.model != NULL)
			free(masina.model);
		if (masina.numeSofer != NULL)
			free(masina.numeSofer);
	}
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


Queue citireQueueDinFisier(const char* numeFisier)
{
	Queue queue;
	queue.start = NULL; //INITIAL COADA ESTE GOALA
	queue.end = NULL;

	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			enqueue(&queue, citireMasinaFisier(f));
		}
	}
	else
	{
		printf("Eroare la deschiderea fisierului!\n");
	}

	return queue;
}

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

void afisareQueue(Queue* queue)
{
	Queue aux;
	aux.start = NULL;
	aux.end = NULL;

	if (queue->start == NULL)
	{
		Masina masina;
		masina.id = -1;
		afisareMasina(masina);
	}
	else
	{
		while (queue->start != NULL)
		{
			Masina masina = dequeue(queue);
			afisareMasina(masina);

			enqueue(&aux, masina);
		}
		queue->start = aux.start;
		queue->end = aux.end;
		
	}

}


Masina getMasinaByID(Queue* queue, int idCautat)
{
	Queue aux;
	aux.start = NULL;
	aux.end = NULL;

	Masina rezultat;
	rezultat.id = -1;

	while (queue->start != NULL)
	{
		Masina masina = dequeue(queue);

		if (masina.id == idCautat)
		{
			rezultat = masina;

			rezultat.model = (char*)malloc(sizeof(char) * (strlen(masina.model)+1));
			strcpy(rezultat.model, masina.model);

			rezultat.numeSofer = (char*)malloc(sizeof(char) * (strlen(masina.numeSofer) + 1));
			strcpy(rezultat.numeSofer, masina.numeSofer);

		}

		enqueue(&aux, masina);//ai grija sa nu il lasi in if, asta trebuie sa se execute la fiecare pas al while ului
	}
	queue->start = aux.start;
	queue->end = aux.end;

	return rezultat;
}


//Realiza?i o func?ie care returneaz? ma?ina cea mai scump? din coad?.
Masina getMasinaScumpa(Queue* queue)
{
	//verific daca coada este goala
	if (queue->start == NULL)
	{
		Masina m_vida = { 0, 0, -1.0f, NULL, NULL, 0 }; // Returnam o masina "invalida"
		return m_vida;
	}

	Queue aux;
	aux.start = NULL;
	aux.end = NULL;

	Masina masinaPretMax = dequeue(queue);//am scos ultima ma?in? din coad? (cea mai veche, prima introdus?)
	enqueue(&aux, masinaPretMax);// ?i am pus-o în coada auxiliar? ca s? nu o pierd
	//dequeue extrage nodurile de la final!

	while (queue->start != NULL)
	{
		Masina masina = dequeue(queue);

		if (masinaPretMax.pret < masina.pret)
		{
			masinaPretMax = masina;
		}

		enqueue(&aux, masina);
	}
	queue->start = aux.start;
	queue->end = aux.end;

	//ABIA ACUM facem Deep Copy pentru rezultatul final!!!!!!!!!!!!!!!!!!!!!!
	Masina deReturnat = masinaPretMax;

	if (masinaPretMax.model) {
		deReturnat.model = (char*)malloc(strlen(masinaPretMax.model) + 1);
		strcpy(deReturnat.model, masinaPretMax.model);
	}

	if (masinaPretMax.numeSofer) {
		deReturnat.numeSofer = (char*)malloc(strlen(masinaPretMax.numeSofer) + 1);
		strcpy(deReturnat.numeSofer, masinaPretMax.numeSofer);
	}

	return deReturnat;
}




//Realiza?i o func?ie care calculeaz? media pre?urilor ma?inilor din coad?.
float pretMediu(Queue* queue)
{
	if (queue->start == NULL)
		return 0;

	Queue aux;
	aux.start = NULL;
	aux.end = NULL;

	float total = 0;
	int contor = 0;
	while(queue->start != NULL)
	{
		Masina masina = dequeue(queue);

		total = total + masina.pret;
		contor++;

		enqueue(&aux, masina);
	}


	queue->start = aux.start;
	queue->end = aux.end;

	if(contor)
	{
		return total / contor;
	}
	

}


int main()
{
	Queue queue = citireQueueDinFisier("masini.txt");
	afisareQueue(&queue);

	Masina m1 = getMasinaByID(&queue, 10);
	afisareMasina(m1);

	if (m1.model != NULL) //am alocat in interiorul stivei asa ca dezaloc aici
				{
					free(m1.model);
					m1.model = NULL;
				}
				if (m1.numeSofer != NULL)
				{
					free(m1.numeSofer);
					m1.numeSofer = NULL;
				}

			Masina	m2 = getMasinaScumpa(&queue);
				afisareMasina(m2);

				if (m2.model != NULL)
				{
					free(m2.model);
					m2.model = NULL;
				}
				if (m2.numeSofer != NULL)
				{
					free(m2.numeSofer);
					m2.numeSofer = NULL;
				}


				float pretMed = pretMediu(&queue);
				printf("%.2f\n", pretMed);

				afisareQueue(&queue);//sa fiu sigura ca este coada corecta dupa modificari
	return 0;
}