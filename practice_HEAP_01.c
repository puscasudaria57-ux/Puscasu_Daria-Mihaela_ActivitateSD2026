//#define _CRT_SECURE_NO_WARNINGS
//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
//
//
//typedef struct Masina Masina;
//struct Masina
//{
//	int id;
//	int nrUsi;
//	float pret;
//	char* model;
//	char* numeSofer;
//	unsigned char serie;
//};
//
//typedef struct Heap Heap;
//struct Heap
//{
//	int lungime; //lungimea vectorului de masini (toate elementele, INCLUSIV cele ascunse)
//	Masina* vector; //vector de masini
//	int nrElemente; //numarul de elemente FARA elementele ascunse!
//};
//
//Heap initializareHeap(int lungime)
//{
//	Heap h;
//
//	h.lungime = lungime;
//	h.nrElemente = 0;
//	h.vector = (Masina*)(malloc(sizeof(Masina) * lungime));
//
//	return h;
//}
//
//void afisareMasina(Masina masina)
//{
//	printf("Id: %d\n", masina.id);
//	printf("Nr usi: %d\n", masina.nrUsi);
//	printf("Pret: %.2f\n", masina.pret);
//	printf("Model: %s\n", masina.model);
//	printf("Nume sofer: %s\n", masina.numeSofer);
//	printf("Serie: %c\n\n", masina.serie);
//}
//
//Masina citireMasinaFisier(FILE* f)
//{
//	Masina m;
//
//	char buffer[256];
//	char sep[3] = ",\n"; //virgula si new line(\n) + null terminator => sep[3]
//	fgets(buffer, 255, f);
//
//	m.id = atoi(strtok(buffer, sep));
//	m.nrUsi = atoi(strtok(NULL, sep));
//	m.pret = atof(strtok(NULL, sep));
//	
//	char* aux;
//	aux = strtok(NULL, sep);
//	m.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
//	strcpy(m.model, aux);
//
//	aux = strtok(NULL, sep);
//	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
//	strcpy(m.numeSofer, aux);
//
//	m.serie = strtok(NULL, sep)[0];
//
//	return m;
//}
//
//
//
////filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru
//void filtreazaHeap(Heap heap, int pozitieNod)
//{
//	int pozFiuSt = 2 * pozitieNod + 1;
//	int pozFiuDr = 2 * pozitieNod + 2;
//	int pozMax = pozitieNod; //pornesc cu presupunerea ca nodul este mai mai mare decat fiul din stanga si fiul din dreapta
//
//	//acum verific dc pp mea initiala este corecta + verific daca pozitiile calculate mai sus exista si in vector 
//	//dc vrei sa faci prioritizare pt altceva in afara de id, in loc de id scrii pret etc !!!!!!!!!!!!!!
//	if (pozFiuSt < heap.nrElemente && heap.vector[pozMax].id < heap.vector[pozFiuSt].id)
//	{
//		pozMax = pozFiuSt;
//	}
//	if (pozFiuDr < heap.nrElemente && heap.vector[pozMax].id < heap.vector[pozFiuDr].id)
//	{
//		pozMax = pozFiuDr;
//	}
//
//	//SINGURA DIFERENTA INTRE MIN HEAP SI MAX HEAP ESTE SEMNUL :
//	//
//	/*if (pozFiuSt < heap.nrElemente && heap.vector[pozMax].id > heap.vector[pozFiuSt].id)
//	{
//		pozMax = pozFiuSt;
//	}
//	if (pozFiuDr < heap.nrElemente && heap.vector[pozMax].id > heap.vector[pozFiuDr].id)
//	{
//		pozMax = pozFiuDr;
//	}*/
//
//	
//
//	//daca presupunerea noastra nu a fost corecta (adica pozMax s a schimbat in urma celor doua if uri de mai sus)
//	//inseamna ca am gasit un maxim si trebuie sa facem o inversare (folosim metoda paharelor)
//	if (pozMax != pozitieNod)
//	{
//		Masina aux = heap.vector[pozMax];
//		heap.vector[pozMax] = heap.vector[pozitieNod];
//		heap.vector[pozitieNod] = aux;
//
//		if (pozMax <= (heap.nrElemente - 2) / 2)//!
//		{
//			filtreazaHeap(heap, pozMax); //repetam procesul 
//		}
//	}
//
//}
//
//
//
////dupa ce citim masinile le si filtram
//Heap citireHeapDinFisier(const char* numeFisier)
//{
//	FILE* f = fopen(numeFisier, "r");
//	Heap heap = initializareHeap(10); //intrucat am 10 masini initializez direct cu 10
//
//	while (!feof(f))
//	{
//		heap.vector[heap.nrElemente++] = citireMasinaFisier(f);
//		//nrElemente = 0 initial, apoi creste cu ++ pana la 10 masini
//	}
//	fclose(f);
//
//		for (int j = (heap.nrElemente - 2)/2;j >= 0;j--)//va filtra intregul heap 
//		{
//			filtreazaHeap(heap, j);
//		}
//
//	return heap;
//
//}
//
//
////nrElemente = elementele vectorului FARA elementele ascunse
//void afisareHeap(Heap heap)
//{
//	for (int i = 0; i < heap.nrElemente; i++)
//	{
//		afisareMasina(heap.vector[i]);
//	}
//}
//
//
////lungime = numarul de elemente ale vectorului cu tot cu elementele ascunse ale vectorului
//void afisareHeapAscuns(Heap heap)
//{
//	for (int i = heap.nrElemente; i < heap.lungime; i++)
//	{
//		afisareMasina(heap.vector[i]);
//	}
//
//}
//
//
////ne int sa extragem valoarea maxima din acest heap
////extrage si returneaza masina de pe prima pozitie
////elementul extras nu il stergem...doar il ascundem
////la final va ramane un vector sortat crescator (MAX HEAP) sau sortat descrescator (MIN HEAP)
//Masina extrageMasina(Heap* heap)
//{
//	//IN CAZUL IN CARE HEAP UL ESTE GOL
//	Masina m;
//	m.id = -1;
//	m.nrUsi = 0;
//	m.pret = 0;
//	m.model = NULL;
//	m.numeSofer = NULL;
//	m.serie = '-';
//
//	if (heap->nrElemente > 0)//trb sa verific asta altfle nu am ce sa extrag!
//	{
//		//facem interschimbarea de pe prima pozitie pe ultima
//		Masina aux = heap->vector[0];
//		heap->vector[0] = heap->vector[heap->nrElemente - 1];
//		heap->vector[heap->nrElemente - 1] = aux;
//		heap->nrElemente--;//ca sa ascunzi acel ultim element dupa interschimbare scazi din nr de elemente
//
//		//acum trb sa filtrez
//		for (int j = (heap->nrElemente - 2) / 2; j >= 0; j--)
//		{
//			filtreazaHeap(*heap, j);
//		}
//
//		return aux; //shallow copy
//	}
//
//	return m;//IN CAZ CA HEAP UL ESTE GOL
//}
//
//
//void dezalocare(Heap* heap)
//{
//	for (int i = 0; i < heap->lungime; i++)
//	{
//		free(heap->vector[i].model);
//		free(heap->vector[i].numeSofer);
//	}
//
//	free(heap->vector);
//	heap->vector = NULL;
//	heap->lungime = 0;
//	heap->nrElemente = 0;
//}
//
//
//int main()
//{
//	Heap heap = citireHeapDinFisier("masini.txt");
//	//afisareHeap(heap);
//
//		printf("==========================================================\n");
//		printf("Extrageri:\n");
//		afisareMasina(extrageMasina(&heap));
//		afisareMasina(extrageMasina(&heap));
//		afisareMasina(extrageMasina(&heap));
//		printf("==========================================================\n");
//		printf("Heap ascuns:\n");
//		afisareHeapAscuns(heap); //heap ul ascuns va fi sortat crescator! (MAX HEAP)
//
//	dezalocare(&heap);
//
//	return 0;
//}
//
//
