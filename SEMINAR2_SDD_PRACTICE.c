//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include<malloc.h>
//#include <string.h>
//#include <stdlib.h>
//
//
////free(pointer) eliberează din heap blocul de memorie care a fost alocat cu malloc / calloc / realloc și la care indică pointerul;
////memoria devine disponibilă pentru reutilizare, iar pointerul rămâne cu o adresă invalidă(dangling),
////de aceea de obicei îl setăm manual la NULL pentru a arăta că nu mai pointează spre memorie validă.
//
//struct Biblioteca
//{
//	int id;
//	int nrCarti;
//	char* nume;
//	float suprafata;
//	char sector;
//};
//
//
////aceasta functie returneaza un tip de date de tipul struct Biblioteca
//struct Biblioteca initializare(int id, int nrCarti, const char* nume, float suprafata, char sector)
//{
//	struct Biblioteca b;
//
//	b.id = id;
//	b.nrCarti = nrCarti;
//	b.suprafata = suprafata;
//	b.sector = sector;
//
//	b.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
//	strcpy(b.nume, nume);
//
//	return b;
//
//}
//
//
//void afisareBiblioteca(struct Biblioteca b)
//{
//	printf("ID-ul: %d\t\n Numarul de carti: %d\n Denumire: %s\n Suprafata: %5.2f\n Sector: %c\n\n", b.id, b.nrCarti, b.nume, b.suprafata, b.sector);
//}
//
//
//void afisareVector(struct Biblioteca* vector ,  int nrBiblioteci) //primesti ca parametri vectorul pe care vrei sa il afisezi si numarul lui de elemente
//{
//	for (int i = 0; i < nrBiblioteci; i++) //parcurgi vectorul de biblioteci si apelezi pentru fiecare biblioteca in parte functia de afisare a unei singure biblioteci scrisa mai sus
//	{
//		afisareBiblioteca(vector[i]);
//	}
//}
//
//struct Biblioteca copy_biblioteca_01(struct Biblioteca b)
//{
//	struct Biblioteca biblio;
//
//	biblio = initializare(b.id, b.nrCarti, b.nume, b.suprafata, b.sector);
//
//	return biblio;
//}
////sau
//struct Biblioteca copy_biblioteca_02(struct Biblioteca b)
//{
//	return initializare(b.id, b.nrCarti, b.nume, b.suprafata, b.sector);//se returneaza o biblioteca
//}
////sau
//struct Biblioteca copy_biblioteca_03(struct Biblioteca b)
//{
//	struct Biblioteca biblio;
//
//	biblio.id = b.id;
//	biblio.nrCarti = b.nrCarti;
//	biblio.suprafata = b.suprafata;
//	biblio.sector = b.sector;
//
//	biblio.nume = (char*)malloc(strlen(b.nume) + 1);
//	strcpy(biblio.nume, b.nume);
//
//	return biblio;
//}
//
////Antetul struct Biblioteca inseamna ca functia returneaza o structura de tip Biblioteca
////Parametrul(struct Biblioteca b) înseamna ca functia primeste o biblioteca
////În interiorul functiei se creeaza o biblioteca noua si se copiază valorile din biblioteca primita(id, număr de cărți, nume, suprafață, sector).
////Astfel se obține o copie a bibliotecii într - o zonă nouă de memorie
//
//
////functia returneaza un pointer catre prima structura de tip Biblioteca(returneaza adresa primului elem dintr-un vector de structuri Biblioteca)
////Pt ca functia va crea un vector nou de structuri, iar în C un vector este reprezentat prin adresa primului element
//struct Biblioteca* copiazaPrimeleNElemente(struct Biblioteca* vector, int nrElemente, int nrElementeCopiate)
//{
//	if (nrElementeCopiate > nrElemente)
//	{
//		nrElementeCopiate = nrElemente;
//	}
//
//	//aloc memorie pt noul vector in care se vor copia primele n elemente
//	struct Biblioteca* vectorNou = NULL;
//	vectorNou = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * nrElementeCopiate);
//
//	for (int i = 0; i < nrElementeCopiate; i++)
//	{
//		vectorNou[i] = copy_biblioteca_01(vector[i]);
//	}
//
//	return vectorNou;
//}
//
//
//void dezalocare(struct Biblioteca** vector, int* nrElemente)
//{
//	/*Funcția eliberează memoria unui vector de structuri 
//
//		Mai exact :
//
//	    eliberează NUME din fiecare structură --> printr un for!
//
//		eliberează vectorul de biblioteci : free((*vector));
//
//		pune pointerul pe NULL : *vector = NULL;
//
//		pune numărul de elemente la 0
//
//		Adică „curăță” complet vectorul*/
//
//	for (int i = 0; i < (*nrElemente); i++)
//	{
//		if ((*vector)[i].nume != NULL)
//		{
//			free((*vector)[i].nume);
//		}
//	}
//
//	free((*vector));
//	*vector = NULL;
//	*nrElemente = 0;
//}
//
//
////struct Biblioteca** vectorNou -> adica functia primeste adresa pointerului din main
//void copiazaBibCuSupMare(struct Biblioteca* vector, char nrElemente, float suprafata, struct Biblioteca** vectorNou, int* dimensiune) 
//{
//	//trebuie sa aloc dinamic memorie pentru noul vector, dar pt asta trebuie sa aflu mai intai numarul de elemente al vectorului
//	*dimensiune = 0;
//	for (int i = 0; i < nrElemente; i++)
//	{
//		if (vector[i].suprafata > suprafata)
//		{
//			(*dimensiune)++;
//		}
//	}
//
//	if ((*dimensiune) == 0)
//	{
//		printf("Nu exista biblioteci cu suprafete atat de mari!\n\n");
//		return;
//	}
//
//	//*vectorNou are tipul struct Biblioteca*!!
//	(*vectorNou) = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * (*dimensiune));
//
//	int contor = 0;
//	for (int i = 0; i < nrElemente; i++)
//	{
//		if (vector[i].suprafata > suprafata)
//		{
//			(*vectorNou)[contor] = copy_biblioteca_01(vector[i]);
//			contor++;
//		}
//	}
//
//}
//
//
//struct Biblioteca getPrimaBibCuNumele(struct Biblioteca* vector, int nrEelemente, const char* nume)
//{
//	struct Biblioteca biblio;
//	int gasit = 0;
//
//	for (int i = 0; i < nrEelemente && gasit == 0 ; i++)
//	{
//		if (strcmp(nume, vector[i].nume) == 0)
//		{
//			biblio = vector[i];
//			biblio.nume = (char*)malloc(sizeof(char) * (strlen(vector[i].nume) + 1));
//			strcpy(biblio.nume, vector[i].nume);
//			gasit = 1;
//		}
//	}
//
//	/*if (gasit == 0)
//	{
//		printf("Biblioteca cautat nu exista!\n");
//		return; iti va da eroare daca incerci sa faci acest return pt ca functia trebuie sa afiseze un tip de data struct Biblioteca neaparat!
//	}*/
//		
//
//	return biblio;
//}
//
//
//void dezalocare_o_sg_biblioteca(struct Biblioteca* b)
//{
//	if (b->nume != NULL)
//	{
//		free(b->nume);
//		b->nume = NULL;
//	}
//}
//
//int main()
//{
//	struct Biblioteca b;
//	b = initializare(1, 431, "Biblioteca Mea", 482.95, '5');
//	afisareBiblioteca(b);
//
//
//	//vector de biblioteci
//	//Se rezervă în memorie spațiu pt 5 structuri Biblioteca consecutive, iar biblioteci reține adresa primei structuri din acest vector
//	printf("Vectorul de biblioteci este: \n");
//	int nrBiblioteci = 5;
//	struct Biblioteca* biblioteci = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * (nrBiblioteci));
//
//	//variabila nrBiblioteci spune că vrem 5 biblioteci în vecto
//	//malloc alocă dinamic în memorie un spațiu suficient pentru 5 structuri de tip Biblioteca
//	//sizeof(struct Biblioteca) calculează câtă memorie ocupă o singură bibliotecă
//	//valoarea returnată de malloc este adresa primei biblioteci din vector, iar această adresă este salvată în pointerul biblioteci
//
//	biblioteci[0] = copy_biblioteca_01(b); //fara shallow copy! adica deep copy
//	//biblioteci[0] = b; // shallow copy
//	biblioteci[1] = initializare(2, 6, "Apropo", 13, '1');
//	biblioteci[2] = initializare(3, 127, "Creanga", 938, '2');
//	biblioteci[3] = initializare(4, 921, "Slavici", 210, '5');
//	biblioteci[4] = initializare(5, 73, "Mihai Viteazu", 97, '2');
//
//	afisareVector(biblioteci, nrBiblioteci);
//
//
//	printf("Apelul functiei care copiaza primele n elemente(BIBLIOTECI) din vectorul de biblioteci initial:\n");
//	int nrElementeCopiate = 3;
//	struct Biblioteca* rezultat = NULL;
//	//rezultat = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * nrElementeCopiate); nu ai de ce sa mai aloci dinamic aici pentru ca deja ai alocat dinamic in functie!
//	rezultat = copiazaPrimeleNElemente(biblioteci, nrBiblioteci, nrElementeCopiate);
//	afisareVector(rezultat, nrElementeCopiate);
//
//	printf("Bibliotecile cu suprafata mare sunt:\n");
//	//void copiazaBibCuSupMare(struct Sablon* vector, char nrElemente, float suprafata, struct Sablon** vectorNou, int* dimensiune)
//	struct Biblioteca* vBibcuSupMAre = NULL; //cum vectorul este regasit in antent trebuie sa il initializam in main
//	int dimensiune = 0;
//	copiazaBibCuSupMare(biblioteci, nrBiblioteci, 10000, &vBibcuSupMAre, &dimensiune); //trimitem adresa ca sa putem modifica valorile de la adresele respective
//	afisareVector(vBibcuSupMAre, dimensiune);
//
//
//	printf("Prima biblioteca cu numele cautat va fi:\n");
//	struct Biblioteca BibCuNumeleSlavici;
//	BibCuNumeleSlavici = getPrimaBibCuNumele(biblioteci, nrBiblioteci, "Slavici");
//	afisareBiblioteca(BibCuNumeleSlavici);
//
//	dezalocare(&rezultat,&nrElementeCopiate);
//	dezalocare(&biblioteci, &nrBiblioteci);
//	dezalocare(&vBibcuSupMAre, &dimensiune);
//
//	//dezalocare pentru o singura biblioteca (pentru numele bibliotecii intrucat este alocat dinamic acest atribut)
//	dezalocare_o_sg_biblioteca(&b);
//	dezalocare_o_sg_biblioteca(&BibCuNumeleSlavici);
//	//DAR ATENTIE: nu mai poti lasa shallow copy aici: biblioteci[0] = b; pentru ca ar aparea eroarea data de double free (dezalocare dubla)
//
//
//
//	return 0;
//}