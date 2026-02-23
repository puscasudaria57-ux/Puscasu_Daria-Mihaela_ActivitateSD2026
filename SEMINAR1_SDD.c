#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



//struct este un tip de date definit de utilizator care grupeaza mai multe variabile sub un singur nume
struct Produs
{
	int id;
	int stoc;
	char* denumire; 
	float pret;
	char categorie;
};

//const char* pentru a nu putea fi modificata informatia de la acea adresa (informatia primita ca input)
struct Produs initializare(int id, int stoc,const char* denumire, float pret, char categorie)
{
	struct Produs p; //se creeaza o variabila locala(pe stack) p de tip struct Produs

	p.id = id;
	p.pret = pret;
	p.stoc = stoc;
	p.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1)); //cast explicit (char*) , +1 pt terminatorul de sir
	strcpy(p.denumire, denumire);
	p.categorie = categorie;

	return p;
}

void afisare(struct Produs p)
{
	printf("ID: %d\n", p.id);
	printf("Stoc: %d\n", p.stoc);
	printf("Denumire: %s\n", p.denumire);
	printf("Pret: %5.2f\n", p.pret);
	printf("Categorie: %c\n", p.categorie);
}


void modifica_Atribut(struct Produs* p, char* nouaDenumire)
{
	//struct Produs* p => se modifica direct structura originala din apelant

	//eliberez memoria (daca nu fac asta => memory leak)
	free((*p).denumire); // sageata face dereferntiere si accesare (accesarea este facuta cu punct  de obicei)

	//aloc spatiu nou pe heap pentru string ul nouaDenumire
	p->denumire = (char*)malloc(sizeof(char) * (strlen(nouaDenumire) + 1));
	strcpy(p->denumire, nouaDenumire);
}

void dezalocare(struct Produs* p)
{
	if (p->denumire != NULL)
	{
		free(p->denumire);
		p->denumire = NULL; 
	}

}

float calculeazaTotal(struct Produs p)
{
	return p.stoc * p.pret;
}



int main()
{
	struct Produs p;

	p = initializare(1, 20, "Telefon \n smart", 2000, 'A'); //' ' ca sa nu puna si caracterul null
	afisare(p);

	modifica_Atribut(&p, "Mouse");
	afisare(p);

	//%s = sir de caractere, %c = un singur caracter
	printf("Valoarea totala pentru %s este %5.3f RON", p.denumire, calculeazaTotal(p));
	


	
	struct Produs* ps = &p; 
	struct Produs* pp = malloc(sizeof(struct Produs)); 

	free(pp);//functioneaza, sterge ce am pe heap
	pp = NULL; //trebuie sa il pui NULL daca ai de gand sa il mai folosesti in viitor

	//free(ps); //imi da eroare pt ca ps pointeaza la p care este pe stiva 
//si nu poti face free pe memorie de pe stack pt ca aceasta memorie este gestionata automat de compilator


	dezalocare(&p);

	/*char* c;
	printf("Un pointer ocupa: %d BYTES", sizeof(c));

	va afisa:
	 Pe sistem 32-bit: pointer = 4 bytes
	 Pe sistem 64-bit (asta inseamna ca procesorul foloseste adrese pe 64 de biti): pointer = 8 bytes

	1 bytes(octet) = 8 biti


	char* c;
	int* p;
	double* d;

Toate ocupa aceeasi dimensiune. Pentru ca un pointer stocheaza o adresa de memorie
Dimensiunea adresei depinde doar de arhitectura (32 sau 64 bit), nu de tipul variabilei
	*/

	return 0;
}