#include<stdio.h>
#include<malloc.h>

struct Telefon
{
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};


struct Telefon  initializare(int id,int RAM, const char* producator, float pret, char serie)
{
	struct Telefon t;

	t.id = id;
	t.RAM = RAM;
	t.pret = pret;
	t.serie = serie;

	t.producator = (char*)malloc(sizeof(char) * (strlen(producator) + 1));
	strcpy(t.producator, producator);

	

	return t;
}

void afisare(struct Telefon t) 
{
	printf("ID: %d\n", t.id);
	printf("RAM: %d\n", t.RAM);
	printf("PRODUCATOR %s\n", t.producator);
	printf("PRET: %5.2f\n", t.pret);
	printf("SERIE: %c\n", t.serie);
	printf("\n");
}



//vector este pointer la primul element dintr-un vector de structuri Telefon
//struct Telefon* vector este pointer la primul Telefon
//in main trb sa faci un vector de telefoane pt care sa apelezi functia de afisare a vectorului, iar functia va primi un vector de telefoane si numarul de elemente al vectorului
void afisareVector(struct Telefon* vector, int nrElemente) 
{
	for (int i = 0; i < nrElemente; i++)
	{
		afisare(vector[i]);
	}
}


//copierea trebuie realizata cu DEEP COPY
//Functia returneaza un pointer la struct Telefon adica adresa unui nou vector de structuri (returneaza inceputul unui vector alocat pe heap)
//struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) 
//{
//	if (nrElementeCopiate > nrElemente)
//		nrElementeCopiate = nrElemente; //in felul asta daca vrem sa copiam mai multe elemente fata de cate avem deja
//	//in vector, vom copia pur si simplu tot vectorul in vectorulNou
//
//	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
//	//aloc memorie pentru noul vector care va contine elementele copiate din vectorul initial (cel primit ca parametru)
//	struct Telefon* vectorNou = NULL;
//	vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
//
//	for (int i = 0; i < nrElementeCopiate; i++)
//	{
//		vectorNou[i] = vector[i];//dar in acest caz pentru atributul producator se va realiza shallow copy
//		//va trebui sa fac peste acel shallow copy un deep copy pentru producator, astfel incat atributul producator din 
//		//vectorul nou sa pointeze spre o zona de memorie diferita fata de atributul producator din vectorul de baza
//		vectorNou[i].producator = (char*)malloc(sizeof(char) * (strlen(vectorNou[i].producator) + 1));
//		strcpy(vectorNou[i].producator, vector[i].producator);
//	}
//
//	return vectorNou;
//}

//SAU

struct Telefon copy_telefon(struct Telefon t)
{
	/*aloca memorie noua
	copiaza stringul
	creeaza obiect independent
	astfel incat sa se realizeze deep copy, ci nu shallow copy*/

	struct Telefon tel;
	tel = initializare(t.id, t.RAM, t.producator, t.pret, t.serie);
		return tel;
}

//acest *vector este un pointer transmis prin valoare, de aceea nu trebuie sa il dereferentiam!!!!!!!
//Dereferențierea vector (*vector) nu trebuie făcută în această funcție pentru că tu nu vrei să modifici elementele prin pointer, ci doar să citești elementele și să le copiezi
struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate)
{
	if (nrElemente < nrElementeCopiate)
	{
		nrElementeCopiate = nrElemente;
	}

	struct Telefon* vectorNou = NULL;
	vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);

	for (int i = 0; i < nrElementeCopiate; i++)
	{
		vectorNou[i] = copy_telefon(vector[i]); //prin acest copy_telefon se face deep copy
		//se aseamana cu constructorul de copiere din poo c++
	}

	return vectorNou;
}

void dezalocare(struct Telefon** vector, int* nrElemente) 
{
	//dezalocam elementele din vector si vectorul
	/*int x = 10;
	int* p = &x;
	int** pp = &p;*/

	for (int i = 0; i < (*nrElemente); i++)
	{
		if ((*vector)[i].producator != NULL)
		{
			free((*vector)[i].producator);
		}
	}

	free(*vector);
	*vector = NULL;
	*nrElemente = 0; 
}


//acest *vector este un pointer transmis prin valoare, de aceea nu trebuie sa il dereferentiam!!!!!!!
//Dereferențierea vector (*vector) nu trebuie făcută în această funcție pentru că tu nu vrei să modifici elementele prin pointer, ci doar să citești elementele și să le copiezi
//k va fi dimensiunea vectorului nou iar k va creste o data ce punem elemente in vectorul nou
//adica o data ce gasim telefoane mai scumpe fata de pragul minim ( pretul minim ) de 2000 de lei
void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pret_minim, struct Telefon** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie


	//trebuie sa aflu mai intai dimensiunea vectorului nou
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].pret > pret_minim)
		{
			(*dimensiune)++;
		}
	}

	if ((*vectorNou) != NULL)
	{
		free((*vectorNou));
	}

	//aloc memorie noua si dupa mai jos copiez telefoanele scumpe in noul vector
	*vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * (*dimensiune));

	//iteram iar prin vectorul de baza si vedem care sunt telefoanele cu pretul > pret_minim 
	// si le copiam in vectorul nou, apoi crestem k cand gasim
	int k = 0; //contorul vectorului nou
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].pret > pret_minim)
		{
			(*vectorNou)[k] = copy_telefon(vector[i]);
			k++;
		}
	}
}

//tipul returnat este un telefon (ne intereseaza primul telefon din vector de la producatorul primit ca parametru)
struct Telefon getPrimulTelefonDeLaProducator(struct Telefon* vector, int nrElemente, const char* producator) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Telefon t; //un singur telefon (doar atat vrem, primul tel care indeplineste conditia respectiva)
	
	t.producator = NULL; //in caz ca nu este gasit acel producator sa ii dea NULL
	for (int i = 0; i < nrElemente; i++)
	{
		if (strcmp(vector[i].producator , producator) == 0)
		{
			t = copy_telefon(vector[i]);
		}
	}

	return t;
}
	


int main() 
{
	struct Telefon t;
	t = initializare(3, 100, "Samsung", 2500.0, 'S');
	//afisare(t);

	struct Telefon* telefoane; //vector de structuri de tip Telefon
	int nrTelefoane = 3;

	telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0] = initializare(1, 200, "Apple",1500, 'A');
	telefoane[1] = initializare(2, 200, "Huawei", 3000, 'C');
	telefoane[2] = t; //shallow copy

	afisareVector(telefoane, nrTelefoane);


	struct Telefon* rezultat;
	int nrElementeCopiate = 2;
	rezultat = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrElementeCopiate);
	printf("Primele %d telefoane copiate sunt:\n", nrElementeCopiate);
	afisareVector(rezultat, nrElementeCopiate); //afisez VECTORUL (si acesta tot un vector va fi)
	//vectorul cu primele N elemente copiate va fi vectorul REZULTAT

	dezalocare(&rezultat, &nrElementeCopiate);
	//afisareVector(rezultat, nrElementeCopiate); //ruleaza asta ca sa vezi ca nu ti va mai afisa nimic
	//pt ca tu ai facut deja dezalocarea

	printf("Telefoanele scumpe sunt: \n");
	struct Telefon* TelefoaneScumpe = NULL; //un vector null initial, in acesta se va face copierea telefoanelor scumpe
	int nrTelefoaneScumpe = 0; //dimensiune
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, 2000, &TelefoaneScumpe, &nrTelefoaneScumpe);//telefoane este transmis prin valoare
	afisareVector(TelefoaneScumpe, nrTelefoaneScumpe);
	//nrTelefoaneScumpe trebuie trimis ca pointer (&nrTelefoaneScumpe) deoarece functia il modifica
	
	dezalocare(&TelefoaneScumpe, &nrTelefoaneScumpe);
	//afisareVector(TelefoaneScumpe, nrTelefoaneScumpe); //ruleaza asta ca sa vezi ca nu ti va mai afisa nimic
	

	
	struct Telefon t3 = getPrimulTelefonDeLaProducator(telefoane,nrTelefoane,"Apple");
	printf("Primul telefon din vectorul de telefoane care este de la firma %s este:\n ", t3.producator);
	afisare(t3);


	return 0;
}