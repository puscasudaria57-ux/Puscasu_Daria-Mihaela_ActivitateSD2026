//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//typedef struct Farmacie Farmacie;
//struct Farmacie
//{
//	int id;
//	float pret;
//	const char* denumire;
//	char tip;
//};
//
//
//typedef struct NodSimplu NodSimplu;
//struct NodSimplu
//{
//	Farmacie info;
//	NodSimplu* next;
//};
//
//void pushStack(NodSimplu** stack, Farmacie f)
//{
//	NodSimplu* nou = (NodSimplu*)malloc(sizeof(NodSimplu));
//	nou->info = f;
//	nou->next = (*stack);
//	*stack = nou;
//}
//
//Farmacie popStack(NodSimplu** stack)
//{
//	Farmacie f;
//	f.id = -1;
//
//	if (*stack != NULL)
//	{
//		NodSimplu* aux = *stack;
//		*stack = aux->next;
//
//		f = aux->info;
//
//		free(aux);
//	}
//
//	return f;
//
//}
//
//
//Farmacie citireFarmacieDinFisier(FILE* f)
//{
//	Farmacie farmacie;
//
//	char buffer[256];
//	fgets(buffer, 255, f);
//	char sep[3] = ",\n";
//
//	farmacie.id = atoi(strtok(buffer, sep));
//	farmacie.pret = atof(strtok(NULL, sep));
//
//	char* aux = strtok(NULL, sep);
//
//	farmacie.denumire = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
//	strcpy((char*)farmacie.denumire, aux);
//
//	farmacie.tip = strtok(NULL, sep)[0];
//
//	return farmacie;
//}
//
//
//NodSimplu* citireStackDinFisier(const char* numeFisier)
//{
//	NodSimplu* stack = NULL;
//	FILE* f = fopen(numeFisier, "r");
//	if (f)
//	{
//		while (!feof(f))
//		{
//			pushStack(&stack, citireFarmacieDinFisier(f));
//		}
//	}
//	fclose(f);
//	return stack;
//}
//
//
////cate farmacii au denumirea cautata
//int getNrFarmacii(NodSimplu** stack, const char* denumireCautata)
//{
//	if (*stack == NULL)
//		return 0;
//
//	int nr = 0;
//	NodSimplu* aux = NULL;
//
//	while (*stack != NULL)
//	{
//		Farmacie f = popStack(stack);
//
//		if (strcmp(f.denumire, denumireCautata) == 0)
//		{
//			nr++;
//		}
//
//		pushStack(&aux, f);
//	}
//
//	while (aux)
//	{
//		pushStack(stack, popStack(&aux));
//	}
//
//	return nr;
//}
//
//
//
//typedef struct Nod Nod;
//struct Nod
//{
//	Farmacie info;
//	Nod* next;
//	Nod* prev;
//};
//typedef struct Queue Queue;
//struct Queue
//{
//	Nod* start;
//	Nod* end;
//};
//
//
//
//
//void enqueue(Queue* queue, Farmacie farmacieNoua)
//{
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = farmacieNoua;
//	nou->prev = NULL;
//	nou->next = queue->start;
//
//	if (queue->start != NULL)
//	{
//		queue->start->prev = nou;
//	}
//	else
//	{
//		queue->end = nou;
//	}
//	queue->start = nou;
//
//}
//
//Farmacie dequeue(Queue* queue)
//{
//	Farmacie f;
//	f.id = -1;
//
//	if (queue->start == NULL)
//	{
//		return f;
//	}
//
//	if (queue->start == queue->end)
//	{
//		free(queue->start);
//		queue->start = NULL;
//		queue->end = NULL;
//	}
//	else
//	{
//		queue->end = queue->end->prev;
//		queue->end->next = NULL;
//		free(queue->end->next);
//	}
//
//}
//
//
//Queue citireQueueDinFisier(const char* numeFisier)
//{
//	Queue queue;
//	queue.start = NULL;
//	queue.end = NULL;
//	FILE* f = fopen(numeFisier, "r");
//	while (!feof(f))
//	{
//		enqueue(&queue, citireFarmacieDinFisier(f));
//	}
//	return queue;
//}
//
//
//void afisareFarmacie(Farmacie f)
//{
//
//}
//
//void afisareQueue(Queue* queue)
//{
//	Queue aux;
//	aux.end = NULL;
//	aux.start = NULL;
//
//	while (queue->start != NULL)
//	{
//		Farmacie f = dequeue(queue);
//
//		afisareFarmacie(f);
//
//		enqueue(&aux, f);
//	}
//	queue->start = aux.start;
//	queue->end = aux.end;
//}
//
//
//int main()
//{
//	return 0;
//}