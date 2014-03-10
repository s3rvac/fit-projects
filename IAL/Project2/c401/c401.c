
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS (Dynamické pridel.pam.)
**                           Vytvoøil: Petr Pøikryl, listopad 1994
**                           Úpravy:   Andrea Nìmcová, prosinec 1995
**                                     Petr Pøikryl, duben 1996
**                                     Petr Pøikryl, listopad 1997
**           Pøepracované do jazyku c: Martin Tuèek, rijen 2005
**
** Implementujte rekurzivním zpùsobem operace nad binárním vyhledávacím
** stromem (BVS; v angliètinì BST -- Binary Search Tree).
**
** Klíèem uzlu stromu je jeden znak (obecnì jím mùže být cokoliv, podle
** èeho se vyhledává). Užiteèným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíèem leží vlevo, uzly s vìtším klíèem leží ve stromu
** vpravo. Využijte dynamického pøidìlování pamìti.
** Rekurzivním zpùsobem øešte následující procedury a funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíèem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... ru¹ení uzlu se zadaným klíèem
**   BSTDispose ... ru¹ení celého stromu
**
** Pøesnou definici typù naleznete v souboru c401.h; pro pøehled --
** ADT BVS je reprezentováno koøenovým ukazatelem stromu typu tBSTNodePtr.
** Uzel stromu (struktura typu tBSTNode) obsahuje ukazatele LPtr a RPtr na levý
** a pravý podstrom, složku char KEY -- klíè, podle kterého se vyhledává,
** a int BSTNodeCont -- obsah uzlu (demonstrativne integer).
**
** !Upozorneni! Je treba, abyste spravne rozlisovali, kdy pouzit dereferencni
** operator * na samotny ukazatel(tj. kdyz budeme chtit zapsat urcitou hodnotu do tohoto
** pointeru, typicky modifikacni operace) a kdy budeme pracovat pouze se samotnym
** ukazatelem (vyhledavaci fce). V techto ulohach poznate tuto skutecnost predevsim
** pomoci prototypu techto fci. V situaci, kdy pracujeme s ukazatelem na ukazatel, je
** treba pouzit dereferenci.
**
** Poznámka: nepoužívejte nestandardní pøíkazy (exit(),...) a operace.
**
**/

#include "c401.h"
int solved;
int errflg;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Poèáteèní inicializace stromu pøed prvním použitím datové struktury.
** Poèáteèní testování ukazatele není možné, protože obsahuje nedefinovanou
** (tj. libovolnou) hodnotu, která ovšem neodráží reálný stav.
**
**  Všimnìte si, ze zde se poprvé v hlavièce objevuje typ ukazatel na ukazatel,
**  proto je treba pri práci s RootPtr(pøiøazení) použít dereferenèní operátor.
**  Ten bude ještì tøeba použít v procedurách BSTDelete, BSTInsert a BSTDispose.
**/

	*RootPtr = NULL;
}

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
**
** Vyhledávání uzlu v BVS podle zadaného klíèe K. Pokud je nalezen, vrací
** funkce hodnotu TRUE a v promìnné Content se vrací obsah pøíslušného uzlu.
** Pokud pøíslušný uzel není nalezen, vrací funkce hodnotu FALSE a obsah
** promìnné Content není definován (to znamená, že do ní nebudete nic
** pøiøazovat). Pøi vyhledávání v binárním stromu bychom typicky použili
** cyklus ukonèený testem zahrnujícím stav dosažení listu nebo nalezení
** uzlu s klíèem. V tomto pøípadì ovšem test nepoužijte a problém øešte
** rekurzivním volání této funkce (nedeklarujte žádnou pomocnou proceduru
** nebo funkci).
**/

	if (RootPtr != NULL) {
		// Move to the child according to the key value
		// (or return true if it was found)
		if (K < RootPtr->Key) {
			return BSTSearch(RootPtr->LPtr, K, Content);
		}
		else if (K > RootPtr->Key ) {
			return BSTSearch(RootPtr->RPtr, K, Content);
		}
		else {
			// Node was found
			*Content = RootPtr->BSTNodeCont;
			return TRUE;
		}
	}

	return FALSE;
}


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{
/*   ---------
** Vloží do stromu hodnotu Content s klíèem K. Pokud již uzel
** se zadaným klíèem existuje, nahradí se obsah uzlu novou hodnotou.
** Novì vytvoøený uzel nech je vždy listem stromu. Øešte rekurzivnì.
**
** Tato rekurzivní implementace je ponìkud neefektivní, protože se pøi
** každém rekurzivním zanoøení musí kopírovat celý integer "Content" (obecnì
** obsah uzlu). V praxi by se tento problém øešil napøíklad jedním
** z tìcho zpùsobù:
**  - pøedáváním promìnné "Content" odkazem (v tom pøípadì je nutné dosazovat
**    pøi volání promìnnou a není možné pøímo zapsat hodnotu);
**  - deklarací vnitøní procedury, které by se parametr pøedal odkazem;
**    vnìjší procedura by sloužila jen jako obal (nevolala by se
**    rekurzivnì);
**  - pøi využití pøedchozí varianty by se do rekurzivní procedury pøedával
**    pøedem naplnìný nový uzel, který by se na závìr zrušil v pøípadì,
**    že se uzel nepodaøilo zaøadit (pokud už uzel s tímto klíèem existoval,
**    pøepsal by se jen obsah, pøípadnì by se uzly vymìnily a ke zrušení
**    by se pøedal starý uzel);
**
** Nerekurzivní varianta by v tomto pøípadì byla efektivnìjší jak z hlediska
** rychlosti, tak z hlediska pamìových nárokù. Zde však jde o školní
** pøíklad. Nedeklarujte žádnou pomocnou proceduru nebo funkci, problém
** øešte rekurzivním voláním procedury samé.
**
**
** POZOR: Vzhledem k jisté složitosti rekurzívního volání této fce zde uvádím
** pøíklad jak funkci zavolat (když jsme pøijali RootPtr jako ukazatel na ukazatel).
** Správné zavolání napø. na levý podstrom:
** BSTInsert(&((*RootPtr)->LPtr), K, Content)
**/

	// Are we at the end of the tree ?
	if (*RootPtr == NULL) {
		*RootPtr = malloc(sizeof(**RootPtr));
		if (!(*RootPtr)) {
			fprintf(stderr, "Error: Not enough memory.\n");
			exit(EXIT_FAILURE);
		}

		// Save data
		(*RootPtr)->LPtr = (*RootPtr)->RPtr = NULL;
		(*RootPtr)->BSTNodeCont = Content;
		(*RootPtr)->Key = K;
	}
	else {
		// No, so move to the child according to the key value
		if (K < (*RootPtr)->Key) {
			BSTInsert(&((*RootPtr)->LPtr), K, Content);
		}
		else if (K > (*RootPtr)->Key ) {
			BSTInsert(&((*RootPtr)->RPtr), K, Content);
		}
		else {
			// Node exists, so just actualize it
			(*RootPtr)->BSTNodeCont = Content;
		}
	}
}


/*
**         POZOR NASLEDUJÍCÍ PROCEDURA BUDE POUŽITA V DÁLE
**         PREÈTÌTE SI PROTO PODROBNÌ NEJPRVE KOMENTÁØ K PROCEDUØE BSTDELETE(),
**         NEŽ ZAÈNETE VYTVÁØET REPLACEBYRIGHTMOST().
**/


void ReplaceByRightmost (tBSTNodePtr PtrReplaced,tBSTNodePtr *RootPtr)	{
/*   ------------------
**
** Pomocná procedura pro vyhledání, pøestìhování a uvolnìní nejpravìj¹ího
** uzlu v podstromu urèeném ukazatelem RootPtr. Na vstupu se pøedpokládá
** hodnota ukazatele RootPtr rùzná od NULL (zajistìte to testováním pøed
** volání procedury). Dále se pøedpokládá, že pomocný ukazatel PtrReplaced
** ukazuje na uzel, který se má naplnit hodnotami vyhledaného uzlu.
**
**
**/

 	tBSTNodePtr ptr; /* používejte tento pomocný ukazatel */
 	ptr=NULL;

	if ((*RootPtr)->RPtr != NULL) {
		ReplaceByRightmost(PtrReplaced, &((*RootPtr)->RPtr));
	}
	else {
		// Save data (we're at the rightest node)
 		PtrReplaced->Key = (*RootPtr)->Key;
 		PtrReplaced->BSTNodeCont = (*RootPtr)->BSTNodeCont;

 		// Destroy node and change it to the left one
  		ptr = *RootPtr;
 		*RootPtr = (*RootPtr)->LPtr;
  		free(ptr);
	}
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíè K. Pokud uzel se zadaným klíèem
** neexistuje, nedìlá nic. Veškeré manipulace øešte rekurzivnì.
**
** Pokud má rušený uzel jen jeden podstrom, pak jej zdìdí otec. Pokud má
** oba podstromy, pak je rušený uzel nahrazen nejpravìjším uzlem levého
** podstromu. Pozor! Nejpravìjší uzel nemusí být listem. Pro tuto operaci
** jsme deklarovali proceduru ReplaceByRightmost -- viz. její komentáø
** uveden výše.
** POZOR: Vzhledem k jisté složitosti rekurzívního volání této fce zde uvádím
** pøíklad jak funkci zavolat (když jsme pøijali RootPtr jako ukazatel na ukazatel).
** Správné zavolání napø. na levý podstrom:
** BSTDelete(&((*RootPtr)->LPtr), K).
** Podobnì je tomu tak i u ReplaceByRightMost().
** Napøíklad: ReplaceByRightmost(*RootPtr, (&((*RootPtr)->LPtr)));
**/

    tBSTNodePtr ptr;  /* používejte tento pomocný ukazatel */
    ptr=NULL;

	if ((*RootPtr) != NULL) {
		// Find node according to the key
		if (K < (*RootPtr)->Key) {
			BSTDelete(&((*RootPtr)->LPtr), K);
		}
		else if (K > (*RootPtr)->Key) {
			BSTDelete(&((*RootPtr)->RPtr), K);
		}
		else {
			// This node has to be deleted
			ptr = (*RootPtr);

			if (ptr->RPtr == NULL) {
				// This node doesn't have right child
				(*RootPtr) = ptr->LPtr;
				free(ptr);
			}
			else if (ptr->LPtr == NULL) {
				// This node has only right child
				(*RootPtr) = ptr->RPtr;
				free(ptr);
			}
			else {
				// It has both childs, so make replacement
				ReplaceByRightmost(*RootPtr, &ptr->LPtr);
			}
		}
	}
}

void BSTDispose (tBSTNodePtr *RootPtr)		{
/*   ----------
** Korektnì zruší celý binární vyhledávací strom. Zvolte nejvýhodnìjší
** druh rekurzívního prùchodu stromem. Nedeklarujte žádné pomocné procedury
** nebo funkce.
** POZOR: Vzhledem k jisté složitosti rekurzívního volání této fce zde uvádím
** pøíklad jak funkci zavolat (když jsme pøijali RootPtr jako ukazatel na ukazatel).
** Správné zavolání napø. na levý podstrom:
** BSTDispose(&(*RootPtr)->LPtr).
**/

	if (*RootPtr == NULL) {
		// We are at the end of branch
		return;
	}
	else {
		// Postorder pass
		BSTDispose(&(*RootPtr)->LPtr);
		BSTDispose(&(*RootPtr)->RPtr);
		free(*RootPtr);
		*RootPtr = NULL;
	}
}

/* konec c401.c */

