//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
////folosesc fisierul masini.txt
////sau va creati un alt fisier cu alte date
//
//struct StructuraMasina 
//{
//	int id;
//	int nrUsi;
//	float pret;
//	char* model;
//	char* numeSofer;
//	unsigned char serie;
//};
//typedef struct StructuraMasina Masina;
//
//
////creare structura pentru un nod dintr-o lista simplu inlantuita
//
////struct Nod
////{
////	Masina info;
////	struct Nod* next;//pointer la nod, tipul de date spre care pointeaza next este struct Nod
////};
//
////sau cu typedef
//
//typedef struct Nod Nod;
////typedef: Creeaz? un alias (un alt nume) pentru un tip de date => De acum înainte, struct Nod poate fi scrit simplu ca Nod
////acest typedef trb scris inainte de a crea structura !!!!
//struct Nod
//{
//	Masina info;
//	Nod* next;//pointer la urmatorul nod
//};
//
////aceasta functie citeste o singura masina din fisier
//Masina citireMasinaDinFisier(FILE* file) 
//{
//	char buffer[100];
//	char sep[3] = ",\n";
//	fgets(buffer, 100, file);
//	char* aux;
//	Masina m1;
//	aux = strtok(buffer, sep);
//	m1.id = atoi(aux);
//	m1.nrUsi = atoi(strtok(NULL, sep));
//	m1.pret= atof(strtok(NULL, sep));
//	aux = strtok(NULL, sep);
//	m1.model = malloc(strlen(aux) + 1);
//	strcpy_s(m1.model, strlen(aux) + 1, aux);
//
//	aux = strtok(NULL, sep);
//	m1.numeSofer = malloc(strlen(aux) + 1);
//	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
//
//	m1.serie = *strtok(NULL, sep);
//	return m1;
//}
//
//
//void afisareMasina(Masina masina) 
//{
//	printf("Id: %d\n", masina.id);
//	printf("Nr. usi : %d\n", masina.nrUsi);
//	printf("Pret: %.2f\n", masina.pret);
//	printf("Model: %s\n", masina.model);
//	printf("Nume sofer: %s\n", masina.numeSofer);
//	printf("Serie: %c\n\n", masina.serie);
//}
//
////P ul din desenul profesorului este "capul listei" cu ajutorul lui po parcurge toata lista, p ul este initial un pointer
////care pointeaza la primul element din lista (de aceea il vom numi cap), iar el mai apoi va retine adresa celui de al doilea
////element din lista, celui de al treilea etc. => ma voi deplasa de la un nod la altul pana p ul sau "cap" va ajunge null =>
////voi folosi un while pt ca nu imi este cunoscut de la inceput nr ul de pasi,iar la fiecare iteratie voi apela functia afisareMasina()
//void afisareListaMasini(/*lista de masini*/ Nod* cap) //cap = P
//{
//	//afiseaza toate elemente de tip masina din lista simplu inlantuita
//	//prin apelarea functiei afisareMasina()
//
//	while (cap !=  NULL)
//	{
//		afisareMasina(cap->info);//aceasta functie primeste o masina, facem dereferentiere si accesare cu ajutorul pointerului cap
//		//ca sa ajungem la informatia utila a fiecarui nod (adica la Masina noastra, la o structura Masina)
//		cap = cap->next; //cap va trece la urmatorul nod, va pointa spre adresa celui de al doilea nod
//
//		//(*cap).info = cap->info
//	}
//}
//
//
////poti adauga o masina in lista la inceput sau la final
//void adaugaLaFinalMasinaInLista(/*lista de masini*/Nod* *cap, Masina masinaNoua)
//{
//	//adauga la final in lista primita o noua masina pe care o primim ca parametru
//
//	Nod* nou = (Nod*)malloc(sizeof(Nod));//nou este pointer la un nod nou
//	nou->info = masinaNoua; //shallow copy
//	nou->next = NULL; //NULL pt ca adaugam nodul la final
//
//	//sunt 2 cazuri: cap este diferit de NULL pt ca lista nue este goala SAU cap este NULL pt ca lista este vida
//	if (*cap != NULL)
//	{
//		Nod* p = *cap;//p este un pointer auxiliar (copie a lui cap) folosit ca s? parcurgem lista f?r? s? modific?m capul
//		while (p->next != NULL)
//		{
//			p = p->next;
//		}
//		p->next = nou; //nou este un pointer catre un nod, iar p->next fix asta este 
//
//	}
//	else
//	{
//		*cap = nou;//avem Nod* *cap pentru ca trebuie ca pointerul cap sa fie transmis prin adresa ca se poata modifica
//		//pt ca in acest caz lista fiind goala, cap = NULL, iar noi vrem sa modificam cap sa pointeze spre adresa
//		//de memorie a noului nod si cum nou este pointer catre noul nod, cap va retine direct acest pointer care este adresa
//		//nodului nou
//		//simplu zis: tu in nou ai adresa noului nod
//	}
//}
//
//
////poti adauga o masina in lista la inceput sau la final
////aici pointerul cap va fi modificat cu siguranta pentru ca facem inserare la inceputul listei
//void adaugaLaInceputInLista(/*lista de masini*/Nod* *cap, Masina masinaNoua) 
//{
//	//adauga la inceputul listei o noua masina pe care o primim ca parametru
//
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = masinaNoua;
//	nou->next = *(cap);
//	(*cap) = nou;//daca nu faci asta cap va retine adresa celui de al doilea element din lista, dar vrem ca el sa retina 
//	//adresa primului element, se modifica capul pt ca nu mai este acela primul nod, ci am adaugat altul la inceputul listei
//	
//}
//
//
////aceasta functie va citi toate masinile din fisier si le va adauga intr o lista (astfel imi va returna acea lista)
////de aceea functia va returna Nod* pt ca va returna un pointer la lista pe care o formam
//Nod* citireListaMasiniDinFisier(const char* numeFisier)
//{
//	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
//	//prin apelul repetat al functiei citireMasinaDinFisier()
//	//ATENTIE - la final inchidem fisierul/stream-ul
//
//	//cap este NULL initial pt ca nu avem nicio masina in lista
//	Nod* cap = NULL; //functia va returna acest cap, de aceea tipul returnat al functiei este Nod*
//
//	FILE* f = fopen(numeFisier, "r");//deschid fisierul
//	if (f)
//	{
//		while (!feof(f)) //cat timp nu am ajuns la finalul fisierului
//		{
//			adaugaLaFinalMasinaInLista(&cap, citireMasinaDinFisier(f));
//			//adaug masini in lista, modific cap, masina o preluez rand pe rand prin apelul functiei citireMasinaDinFisier
//			//functie care citeste O SINGURA masina din fisier
//		}
//
//	}
//
//	fclose(f);//inchidem, inainte sa returnam!!!
//	return cap;//returnez capul, daca am masini in fisier returnez lista, daca nu am returnez NULL pt ca cap este NULL initial
//	
//}
//
//
////cap se va modifica! tu vei dezaloca(sterge) toata informatia utila(masinile), mai apoi stergi si nodul in sine pt ca si 
////el este alocat dinamic, de aceea cap se modifica!
//void dezalocareListaMasini(Nod** cap)
//{
//	while (*cap != NULL)
//	{
//		Nod* p = *cap;        // salvezi nodul curent
//		*cap = (*cap)->next;       // mergi mai departe, echivalent cu *cap = p->next;
//
//		if (p->info.model != NULL)//sterg infromatia utila din nod
//			free(p->info.model);
//		if (p->info.numeSofer != NULL)
//			free(p->info.numeSofer);
//
//		free(p);              // stergi nodul cu totul
//	}
//}
//
//
////vom calcula suma tuturor preturilor masinilor din lista si vom returna media acestor preturi
//float calculeazaPretMediu(/*lista de masini*/Nod* cap)
//{
//	//calculeaza pretul mediu al masinilor din lista.
//
//	float suma = 0;
//	int contor = 0;
//	while (cap != NULL)
//	{
//		suma = cap->info.pret + suma;
//		contor++;
//
//		cap = cap->next; //asa parcurg lista!
//	}
//
//	if (contor != 0)
//	{
//		return suma / contor; //asta este media, dar se face doar daca exista masini in lista (contor!=0)
//	}
//
//	return 0;
//}
//
//
////stergerea masinilor dintr o anumita serie, se sterg noduri din lista existenta, lista se modifica pe loc
////aici pb se pune cand exista mai multe masini cu aceeasi serie si trebuie sterse TOATE
//void stergeMasiniDinSeria(/*lista masini*/Nod* *cap, char serieCautata) 
//{
//	//sterge toate masinile din lista care au seria primita ca parametru.
//	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
//
//	//situatia in care masina sau masinile se afla pe prima pozitie (sau pe primele pozitii)
//	//dupa ce se iese din while stim sigur ca la inceputul listei nu mai exista masini cu seriaCautata
//	while (*cap != NULL && (*cap)->info.serie == serieCautata)
//	{
//		//ca la dezalocare
//		Nod* p = *cap;        // salvezi nodul curent
//		*cap = (*cap)->next;       // mergi mai departe, echivalent cu *cap = p->next;
//
//		if (p->info.model != NULL)//sterg infromatia utila din nod
//			free(p->info.model);
//		if (p->info.numeSofer != NULL)
//			free(p->info.numeSofer);
//
//		free(p);// stergi nodul cu totul
//	}
//
//	//acum trebuie sa cautam nodurile din interiorul listei cu seriaCautata
//	//vom lua iar un pointer auxiliar pentru ca nu ne dorim ca pointerul cap sa isi modifice adresa pe care o retine
//	//trebuie sa retina primul nod din lista pt ca acel prim nod ne ama asigurat ca nu va avea seriaCautata
//		Nod* aux = *cap;
//		while (aux)
//		{
//			while (aux->next != NULL && aux->next->info.serie != serieCautata)
//			{
//				aux = aux->next; //cat timp exista nodul urmator si seria este diferita de seria cautata ne deplasam
//			}
//			if (aux->next) //sa vedem daca nodul exista totusi (nodul pe care vrem sa il stergem)
//			{
//				//salvez nodul intr un pointer temporar
//				Nod* temp = aux->next; //aux->next stiu sigur ca exista ca am verificat in if
//
//				//acum voi face ca nodul spre care pointeaza p sa retina adresa de memorie a urmatorului nod, nu cel 
//				//care are seria cautata, ci urmatorul
//				aux->next = aux->next->next; //SAU aux->next = temp->next (pt ca temp = aux->next)
//
//				//stergem temp
//				if (temp->info.model != NULL)//sterg infromatia utila din nod
//					free(temp->info.model);
//				if (temp->info.numeSofer != NULL)
//					free(temp->info.numeSofer);
//
//				free(temp);// stergi nodul cu totul
//
//			}
//			else
//			{
//				aux = NULL;// ca sa iasa din primul while: while(aux) , avem nevoie de acel while(aux) ca sa stergem toate 
//				//nodurile care respecta conditia cu seria cautata
//			}
//		}
//		
//	
//
//
//}
//
//
////un sofer poate avea mai multe masini, primim ca parametru numele soferului care ne intereseaza 
//// si calculam pretul masinilor pe care le detine. Ionescu are 3 masini: Passat, Logan, Astra
//float calculeazaPretulMasinilorUnuiSofer(/*lista masini*/Nod* cap, const char* numeSofer)
//{
//	//calculeaza pretul tuturor masinilor unui sofer.
//	float suma = 0;
//	while (cap != NULL)
//	{
//		if (strcmp(cap->info.numeSofer, numeSofer) == 0)
//		{
//			suma = suma + cap->info.pret;
//		}
//
//		cap = cap->next;//DEPLASAREA!
//	}
//	return suma;
//}
//
//
//
//int main() 
//{
//	//formez lista:
//	Nod* cap = citireListaMasiniDinFisier("masini.txt");
//	//afisareListaMasini(cap);
//
//	//dezalocareListaMasini(&cap);
//	//afisareListaMasini(cap);
//
//	float pretMediuListaMasini = calculeazaPretMediu(cap);
//	printf("Pretul mediu al masinilor din lista este de: %.2f\n", pretMediuListaMasini);
//
//	float pretMasiniUnSgSofer = calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu");
//	printf("Pretul masinilor soferului respectiv este de: %.2f\n", pretMasiniUnSgSofer);
//
//	stergeMasiniDinSeria(&cap, 'A');
//	afisareListaMasini(cap);
//
//	stergeMasiniDinSeria(&cap, 'B');
//	afisareListaMasini(cap);
//
//	return 0;
//}