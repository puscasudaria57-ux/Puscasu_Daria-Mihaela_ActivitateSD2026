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

typedef struct NodSimplu NodSimplu;
struct NodSimplu
{
	Masina info;
	NodSimplu* next;
};


//Realiza?i o func?ie care adaug? o ma?in? într-o stiv? implementat? ca list? simplu înl?n?uit?.
//adaugare la inceput
void pushStack(NodSimplu** stack, Masina masinaNoua)//** pt ca modificam stiva
{
	//creez un nod nou si il aloc dinamic
	NodSimplu* nou = (NodSimplu*)malloc(sizeof(NodSimplu));
	nou->info = masinaNoua;
	nou->next = *stack;
	*stack = nou;

}

//Realiza?i o func?ie care extrage o ma?in? din stiv? (opera?ia pop), tratând cazul în care stiva este goal?.
Masina popStack(NodSimplu** stack)
{
	Masina masina;
	masina.id = -1; //in cazul in care lista este goala, la afisare se va apela functia afisareMasina si mesajul "Masina nu exista"

	if (*stack != NULL)
	{
		NodSimplu* p = *stack; //inaintez cu stack si sterg cu p
		*stack = p->next;

		//retin masina de sters in "masina"
		masina = p->info;
		free(p); //sterg nodul
	}

	return masina;
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

//Realiza?i o func?ie care determin? dac? stiva este goal?.
char emptyStack(NodSimplu* stack)
{
	return stack == NULL;
}

//Realiza?i o func?ie care dezaloc? complet stiva de ma?ini.
void dezalocare(NodSimplu** stack)
{
	while (*stack != NULL)
	{
		Masina masina = popStack(stack); //scot masina din stiva

		if (masina.model != NULL)
		{
			free(masina.model);
			masina.model = NULL;
		}
		if (masina.numeSofer != NULL)
		{
			free(masina.numeSofer);
			masina.numeSofer = NULL;
		}
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
	else
	{
		printf("Eroare la deschiderea fisierului!\n");
	}

	return stack;
}

//Realiza?i o func?ie care determin? num?rul de elemente din stiv?.
int size(NodSimplu** stack)
{
	int contor = 0;
	NodSimplu* aux = NULL;

	while (*stack != NULL)
	{
		Masina masina = popStack(stack);//distrug stack
		contor++;

		pushStack(&aux, masina); //refac stack ul in aux
	}

	//refac stiva initiala
	while (aux != NULL)
	{
		pushStack(stack, popStack(&aux));//o sa adaug in stack, scotand cu pop din aux
	}

	return contor;
}

//Realiza?i o func?ie care calculeaz? suma total? a pre?urilor ma?inilor din stiv?:
float sumaTotala(NodSimplu** stack)
{
	float total = 0;
	NodSimplu* aux = *stack;

	while (*stack != NULL)
	{
		Masina masina = popStack(stack);
		total = total + masina.pret;

		pushStack(&aux, masina);
	}

	//refac stiva initiala
	while (aux != NULL)
	{
		pushStack(stack, popStack(&aux));
	}

	return total;
}


//Realiza?i o func?ie care determin? pre?ul maxim din stiv?.
float pretMaxim(NodSimplu** stack)
{
	if (*stack == NULL)
		return 0; // sau alt caz special


	NodSimplu* aux = NULL;
	Masina masina = popStack(stack);
	float pretMax = masina.pret; //pp ca masina pretul cel mai mare apartine primei masini

	pushStack(&aux, masina); //TREBUIE SA PUN SI ACEASTA MASINA SCOASA DIN STIVA IN AUX PT CA ALTFEL O PIERD

	while (*stack != NULL)
	{
		Masina m = popStack(stack);
		if (m.pret > pretMax)
		{
			pretMax = m.pret;
		}

		pushStack(&aux, m);
	}

	while (aux != NULL)
	{
		pushStack(stack, popStack(&aux));
	}
	return pretMax;
}



//Realiza?i o func?ie care prime?te o stiv? de ma?ini ?i un ID ?i verific? existen?a unei ma?ini cu acel ID.
// În cazul în care aceasta este g?sit?, se va afi?a informa?ia asociat?.
// În caz contrar, se va afi?a un mesaj corespunz?tor. Stiva trebuie ref?cut? dup? parcurgere.
Masina getMasinaByID(NodSimplu** stack, int idCautat)
{
	NodSimplu* aux = NULL; //auxiliarul in care vom reface stiva, initial este null!

	Masina rezultat;
	rezultat.id = -1;//in caz ca nu se gaseste o masina cu id ul cautat va avea -1 => masina nu exista
	rezultat.model = NULL;
	rezultat.numeSofer = NULL;

	while (*stack)
	{
		Masina masina = popStack(stack);
		if (masina.id == idCautat)
		{
			rezultat = masina; //shallow copy

			//deep copy pt elementele alocate dinamic
			rezultat.model = (char*)malloc(sizeof(char) * (strlen(masina.model) + 1));
			strcpy(rezultat.model, masina.model);

			rezultat.numeSofer = (char*)malloc(sizeof(char) * (strlen(masina.numeSofer) + 1));
			strcpy(rezultat.numeSofer, masina.numeSofer);
		}

		pushStack(&aux, masina);
	}
	
	while (aux)
	{
		pushStack(stack, popStack(&aux));
	}

	return rezultat;

}

//functia nu returneaza nimic!, sa afiseaza o masina prin apelul afisareMasina, dar nu se returneaza masina!
void afisareStack(NodSimplu** stack)
{
	NodSimplu* aux = NULL;

	while (*stack)
	{
		Masina masina = popStack(stack);
		afisareMasina(masina);

		pushStack(&aux, masina);
	}

	while (aux)
	{
		pushStack(stack, popStack(&aux));
	}
}


int main()
{
	NodSimplu* stack = citireStackDinFisier("masini.txt");
	afisareStack(&stack);

	Masina m1 = getMasinaByID(&stack, 1);
	afisareMasina(m1);

	float pretMax = pretMaxim(&stack);
	printf("Pretul maxim al masinilor este de: %.2f\n", pretMax);

	int nrElementeStack = size(&stack);
	printf("Nr de elemente din stiva: %d\n", nrElementeStack);

	return 0;
}