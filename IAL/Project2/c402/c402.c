
/* c402.c: ********************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS
**                           Implementace: Petr Pøikryl, prosinec 1994
**                                 Úpravy: Petr Pøikryl, listopad 1997
**                                         Petr Pøikryl, kvìten 1998
**			  	 Pøepracované do jazyku c: Martin Tuèek, srpen 2005
**
** S využitím dynamického pøidìlování pamìti, implementujte
** následující operace nad binárním vyhledávacím stromem -- vše NEREKURZIVNÌ.
** (BT znamená Binary Tree; Tato pøedpona je u procedur uvedena kvùli možné
** kolizi jmen v ostatních pøíkladech):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vlo¾ení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní prùchod typu pre-order
**     BTInorder ....... nerekurzivní prùchod typu in-order
**     BTPostorder ..... nerekurzivní prùchod typu post-order
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech procedur, které využívají nìkterý z prùchodù stromem, deklarujte
** lokální proceduru nazvanou Leftmost -- nalezení nejlevìjšího uzlu
** v podstromu (viz pøednášky, kde se tato procedura jmenovala Nejlev).
**
** Definice typù naleznete v souboru c402.h; uzel stromu je typu tBTNode,
** ukazatel na nìj je typu tBTNodePtr, uzel obsahuje položku int Cont
** (obsah) a ukazatele LPtr a RPtr.
**
** Pøíklad slouží zejména k procvièení nerekurzivních zápisù algoritmù
** nad stromy. Než zaènete tento pøíklad øešit, prostudujte si dùkladnì
** principy pøevodu rekurzivních algoritmù na nerekurzivní. Programování
** je pøedevším inženýrská disciplína, kde opìtné objevování Ameriky nemá
** místo. Pokud se vám zdá, že by nìco šlo zapsat optimálnìji, promyslete
** si všechny detaily vašeho øešení. Povšimnìte si typického umístìní akcí
** pro rùzné typy prùchodù. Zamyslete se nad modifikací øešených algoritmù
** pro výpoèet poètu uzlù stromu, poètu listù stromu, výšky stromu a pro
** vytvoøení zrcadlového obrazu stromu (pouze popøehazování ukazatelù bez
** vytváøení nových uzlù a rušení starých).
**
** Pøi prùchodech stromem použijte ke zpracování uzlu proceduru BTWorkOut().
** Pro práci se zásobníky použijte rovnìž pøedem pøipravené procedury
** a funkce. K dispozici máte zásobníky pro hodnoty typu boolean
** a tBTNodePtr (SInit*, SPush*, STopPop*, SEmpty*, kde místo '*' použijete
** 'P' pro zásobník s ukazateli nebo 'B' pro zásobník s boolovskými
** hodnotami.
**
**
** !Upozornìní! Je tøeba, abyste spravnì rozlišovali, kdy použit dereferenèní
** operátor * na samotný ukazatel a kdy budeme pracovat pouze se samotným
** ukazatelem (prohledavaci fce). V techto úlohách poznáte tuto skuteènost predevším
** pomocí prototypu tìchto funkcí. V situaci, kdy pracujeme s ukazatelem na ukazatel, je
** tøeba použít dereferenci.
**
** -------------------------------------------------------------------------
**/
#include "c402.h"
int solved;
int errflg;

void BTWorkOut (tBTNodePtr Ptr)		{
/*           ---------
** Pomocná procedura používaná pøi prùchodech stromem.
** Zpracuje uzel, na který ukazuje Ptr.
** Nemodifikovat!
**/

	if (Ptr==NULL)
		printf("NULL\n");
    else
		printf("Vypis hodnoty daneho uzlu> %d\n",Ptr->Cont);
}

/* -------------------------------------------------------------------------}
{* Implementace zásobníkù je velmi zjednodušena. Zdrojový text je formátován
** s ohledem na úsporu místa a není pøíliš komentován (neberte si z toho
** pøíklad -- když, tak odstrašující). Definice datovych struktur, viz. hlavickovy
** soubor.
}*
**/

/*********** OPERACE SE ZÁSOBNÍKEM ************/


/* Zásobník ukazatelù: */

void SInitP (tStackP *S)
/* inicializace zásobníku ukazatelù */
{
	S->top = 0;
}


void SPushP (tStackP *S, tBTNodePtr ptr)
/*vloží hodnotu na vrchol zásobníku */
{
	if (S->top==MAXSTACK)
		printf("Pøeteèení zásobníku s ukazateli!\n");
	else {
		S->top++;
		S->a[S->top]=ptr;
	}
}


tBTNodePtr STopPopP (tStackP *S)
/* vrací odstranìnou hodnotu */
{
	if (S->top==0)  {
		printf("Podteèení zásobníku s ukazateli!\n");
		return(NULL);
	}
	else {
		return (S->a[S->top--]);
	}
}


bool SEmptyP (tStackP *S)
/* test na prázdnost zásobníku */
{
	if (S->top==0)
		return(true);
	else
		return(false);
}



/* Zásobník boolovských hodnot: */

void SInitB (tStackB *S)
/* inicializace zásobníku */
{
	S->top = 0;
}


void SPushB (tStackB *S,bool val)
/*vloží hodnotu na vrchol zásobníku */
{
	if (S->top==MAXSTACK)
		printf("Pøeteèení zásobníku pro boolean!\n");
	else {
		S->top++;
		S->a[S->top]=val;
	}
}


bool STopPopB (tStackB *S) {
/* vrací odstranìnou hodnotu  */

	if (S->top==0) {
		printf("Podteèení zásobníku pro boolean!\n");
		return(NULL);
	}
	else {
		return(S->a[S->top--]);
	}
}


bool SEmptyB (tStackB *S) {
	if (S->top==0)
		return(true);
	else
		return(false);
}

/* -------------------------------------------------------------------------*/

void BTInit (tBTNodePtr *RootPtr)	{
/*           ------
** Inicializace stromu. Smí se volat pouze pøed prvním použitím
** konkrétního binárního stromu, protože neuvolòuje uzly neprázdného
** stromu (ani to dìlat nemù¾e). K rušení neprázdného stromu slouží
** procedura BTDisposeTree (viz dále).
**
**
**  Všimnìte si, že zde se poprvé v hlavièce objevuje typ ukazatel na ukazatel,
**	proto je tøeba pøi práci s RootPtr(pøiøazení) použít dereferenèní operator.
**	Ten bude ještì tøeba použít v procedurách BTInsert() a BTDisposeTree().
**
**/

	*RootPtr = NULL;
}

void BTInsert (tBTNodePtr *RootPtr, int Content)	{
/*          --------
** Vloží do stromu nový uzel s hodnotou 'Content'. Z pohledu vkládání
** chápejte vytváøený strom jako binární vyhledávací strom (BVS = BST),
** kde uzly s hodnotou menší než má otec leží v levém podstromu, uzly vìtší
** leží vpravo. Pokud vkládaný uzel již existuje, neprovádí se nic (hodnota
** se ve stromu vyskytuje maximálnì jedenkrát). Pokud se vytváøí nový uzel,
** vzniká vždy jako list stromu. Øešte nerekurzivnì.
**
**/

    tBTNodePtr fptr;  /* ukazatel na otcovský uzel F */
    tBTNodePtr ptr;   /* pomocný ukazatel */

    /* V pøípadì, kdy strom není prázdný, musíme vyhledat místo, kam
      by se nová hodnota mìla vložit. Pokud uzel se zadanou hodnotou
      již existuje, nedìláme nic. Pokud se bude uzel vytváøet, musíme
      si pamatovat ukazatel na otce, na kterého bude nový uzel pøipojen.*/

	ptr = *RootPtr;
	fptr = NULL;
	while (ptr) {
		// Save parent's address
		fptr =  ptr;

		if (Content < ptr->Cont)
			ptr = ptr->LPtr;
		else if (Content > ptr->Cont)
			ptr = ptr->RPtr;
		else {
			// Node exists -> do nothing
			return;
		}
	}

	// Insert node (create new one)
	ptr = malloc(sizeof(*ptr));
	if (!ptr) {
		fprintf(stderr, "Error: Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	ptr->Cont = Content;
	ptr->LPtr = ptr->RPtr = NULL;
	if (fptr) {
		// Set link from the father
		if (Content < fptr->Cont)
			fptr->LPtr = ptr;
		else
			fptr->RPtr = ptr;
	}
	else {
		// This is the first node in the tree
		*RootPtr = ptr;
	}
}

/* Nyní následuje sekce jednotlivých prùchodù stromem, pøed tvorbou Leftmost
ètìte vždy i popisek nasledujíci procedury = implikace øešení daného Leftmost */


/*                                  PREORDER                                  */


void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*           --------
** Lokální procedura BTPreorder -- funkce viz pøednášky.
**/
/* Pohybuje se po levé vìtvi podstromu, až narazí na jeho nejlevìjší uzel.
  Ukazatele na všechny navštívené uzly ukládá do zásobníku. Protože jde
  o prùchod typu preorder, je navštívený uzel souèasnì zpracován, použijte
  BTWorkOut(). */

	while (ptr) {
		SPushP(Stack, ptr);
		BTWorkOut(ptr);
		ptr = ptr->LPtr;
	}
}

void BTPreorder (tBTNodePtr RootPtr)	{
/*           ----------
** Samotný nerekurzivní prùchod typu pre-order, použijte dané Leftmost_Preorder
** dle prednášek.
**/

    tStackP Stack ;  /* zásobník ukazatelù */
    tBTNodePtr ptr;  /* pomocný ukazatel na uzel */

	// Initialize stack
	SInitP(&Stack);

	ptr = RootPtr;
	Leftmost_Preorder(ptr, &Stack);
	while (!SEmptyP(&Stack)) {
		ptr = STopPopP(&Stack);
		Leftmost_Preorder(ptr->RPtr, &Stack);
	}
}


/* 					IN ORDER			 	      */


void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*           --------
** Lokální procedura BTInorder -- funkce viz pøednášky.
**/
/*  Pohybuje se po levé vìtvi podstromu, až narazí na jeho nejlevìjší uzel.
   Ukazatele na všechny navštívené uzly ukládá do zásobníku. */

	while (ptr) {
		SPushP(Stack, ptr);
		ptr = ptr->LPtr;
	}
}



void BTInorder (tBTNodePtr RootPtr)	{
/*           ---------
** Nerekurzivní prùchod typu in-order. Pro zpracování uzlu volejte
** proceduru BTWorkOut a pracujte s LeftMost_Inorder.
**/
    tStackP Stack;  /* zásobník ukazatelù */
    tBTNodePtr ptr; /* pomocný ukazatel na uzel */

	// Initialize stack
	SInitP(&Stack);

	ptr = RootPtr;
	Leftmost_Inorder(ptr, &Stack);
	while (!SEmptyP(&Stack)) {
		ptr = STopPopP(&Stack);
		BTWorkOut(ptr);
		Leftmost_Inorder(ptr->RPtr, &Stack);
	}
}


/*					Post Order						*/


void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Lokální procedura pro Postorder -- funkce viz pøednášky.
**/
/* Pohybuje se po levé vìtvi podstromu, až narazí na jeho nejlevìjší uzel.
  Ukazatele na všechny navštívené uzly ukládá do zásobníku. Protože jde
  o prùchod typu postorder, je souèasnì do boolovského zásobníku ukládána
  hodnota, která øíká, že uzel byl navštíven poprvé (pøi prùchodu do
  levého podstromu) a že se tedy ještì nemá zpracovávat. */

	while (ptr) {
		SPushP(StackP, ptr);
		SPushB(StackB, true);
		ptr = ptr->LPtr;
	}
}



void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Nerekurzivní prùchod typu post-order. Pro zpracování uzlu volejte
** proceduru BTWorkOut.
**/

    tStackP StackP;  /* zásobník ukazatelù */
    tBTNodePtr ptr;  /* pomocný ukazatel na uzel */
    tStackB StackB;  /* zásobník boolovských hodnot */

	// Initialize stacks
	SInitP(&StackP);
	SInitB(&StackB);

	// Am I back from the left side ?
	bool from_left = false;

	ptr = RootPtr;
	Leftmost_Postorder(ptr, &StackP, &StackB);
	while (!SEmptyP(&StackP)) {
		ptr = STopPopP(&StackP);
		SPushP(&StackP, ptr);
		from_left = STopPopB(&StackB);

		if (from_left) {
			// I'm here from left, so go right
			SPushB(&StackB, false);
			Leftmost_Postorder(ptr->RPtr, &StackP, &StackB);
		}
		else {
			// I'm here from right, so do workout
			STopPopP(&StackP);
			BTWorkOut(ptr);
		}
	}
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*           -------------
** Zruší všechny uzly stromu -- uvolní jimi zabíraný prostor voláním
** standardní funkce free.
**
** Poznámka: pøi rušení uzlù stromu není dùležité, v jakém poøadí se
** ~~~~~~~~~ budou rušit, proto mùžete uvažovat i jiné varianty, než
** klasické typy prùchodu stromem.
**
**/

    tStackP S;       /* zásobník ukazatelù */
    tBTNodePtr ptr;  /* pomocný ukazatel na uzel */

	// Initialize stack
	SInitP(&S);

	do {
		if (*RootPtr == NULL) {
			// Load next pointer
			if (!SEmptyP(&S))
				*RootPtr = STopPopP(&S);
		}
		else {
			if ((*RootPtr)->RPtr != NULL) {
				// Save right node
				SPushP(&S, (*RootPtr)->RPtr);
			}

			// Delete node
			ptr = *RootPtr;
			*RootPtr = (*RootPtr)->LPtr;
			free(ptr);
		}
	} while (!(SEmptyP(&S) && (*RootPtr == NULL)));

	// Set root node to initialized state
	*RootPtr = NULL;
}

/* konec c402.c */

