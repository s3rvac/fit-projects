
/* c403.c: **********************************************************}
{* Téma: Vyhledávací tabulka v neseøazeném poli se zar. (optimalizace)
**         První implementace: Petr Pøikryl, prosinec 1994
**         Úpravy: Petr Pøikryl, listopad 1997, kvìten 1998
**		   Další úprava: Martin Tuèek, srpen 2005 (jazyk C)
**		   Další úprava: Roman Lukáš, øíjen 2006
**
** Implementujte vyhledávací tabulku podle následujících požadavkù.
** Tabulka je implementována v poli, obsah prvkù pole není seøazen.
** Pøi vyhledávání se sekvenènì prochází celé využité pole s využitím
** takzvané zarážky. Maximální kapacita tabulky je tedy o jednièku nižší,
** než maximální vyu¾itelná kapacita pole. Implementujte následující
** procedury a funkce (zkratka AS pochází ze slova Array = pole a
** Sentinel = zde zarážka):
**
**   ASInit ..... inicializace tabulky
**   ASSearch ... vyhledávání se zará¾kou v neseøazeném poli
**   ASInsert ... vkládání do neseøazeného pole s vyu¾itím ASSearch
**
** Pøi každém vyhledání se optimalizuje doba vyhledávání èasto hledaných
** položek tím, že se nalezená položka vždy posunuje o jedno místo smìrem
** k zaèátku pole.
**
** Definici typù naleznete v souboru c403.h. Tabulka je reprezentována
** datovou strukturou typu tASTable, která se skládá z pole 'arr' a indexu
** poslední vyu¾ité polo¾ky pole 'last'. Polo¾ky pole jsou tvoøeny záznamy
** typu tASData, ve kterých se nachází slo¾ka Key (klíèem je pro jednoduchost
** èíslo typu integer) a obsah Cont (demonstrativne int). Pøi implementaci tìl
** øešených procedur uvažujte maximální rozmìr pole ASSize (viz poznámka
** v c403.h).
**
** Dùležitým rysem vyhledávacích metod je poèet porovnávání vyhledávaného
** klíèe s klíèi prohledávaných položek pole. K porovnávání využívejte
** povinnì funkce ASCompare (viz dále). Poèet porovnávání omezte
** na minimum. To znamená, že nebudete volat podruhé funkci ASCompare
** pro stejné klíèe tam, kde jste si mohli výsledek porovnání zapamatovat
** v pomocné promìnné.
**
**/

#include "c403.h"
int ASCompNum;
int solved;
int errflg;

int ASCompare (int k1, int k2) {
/*  --------- */
	ASCompNum = ASCompNum + 1;      /* poèet porovnání */
	if (k1 < k2) 			/* porovnání dvou klíèù */
		return(-1);
	else if (k1 == k2)
		return(0);
	else /*k1 > k2*/
		return(1);
}

void ASError()	{
/*   ------- */
	printf("Chyba: Položka již nemùže být vložena\n");
	errflg = TRUE;
}
/* -------------------------------------------------------------------------*/

void ASInit(tASTable *Tab)	{
/*           ------ */
/* Inicializace tabulky (výsledkem je prázdná tabulka).
   Inicializaci tabulky se provede tak, že se index prvního prvku
   nastaví na hodnotu -1. Hodnoty prvkù ani klíèù se v tabulce nemìní.
*/
	const int INIT_VALUE = -1;

	Tab->last = INIT_VALUE;
}


int ASSearch (tASTable *Tab, int key, int* ind)	 	{
/*          --------
** Vyhledávání v NESEØAZENÉM poli se zarážkou a s pøesunem nalezené
** položky o jednu pozici dopøedu (výmìna s pøedchozí položkou).
**
** Vyhledávacím klíèem je hodnota key. Funkce vrací pøíznak, zda byl prvek
** nalezen. Pokud ne (vrací false), pak se v promìnné 'ind' vrací první
** volná pozice, na kterou se mùže prvek vložit.
** Pokud byl prvek nalezen (vrací true), došlo k jeho posunutí o jednu
** pozici dopøedu a v promìnné ind se vrací nová pozice nalezeného prvku.
**
** Pro porovnání dvou klíèù používejte pøeddefinovanou funkci ASCompare
** (viz. výše). Vzhledem k tomu, že se pøi vyhledávání vždy používá
** zarážka, bude minimální poèet porovnání (pøi prázdné tabulce) roven 1.
**
** POZOR!!!
** Pøi vložení zarážky se hodnota 'last' NEMÌNÍ! Zarážka se tedy nachází
** na pozici 'last' + 1.
** Zarážka mùže obsahovat obecnì libovolnou hodnotu, vy ale hodnotu
** zarážky nastavte na -1, aby se vám shodovaly výsledky testù!!!
**/
	int SENTINEL = -1;

	// Set sentinel
	Tab->arr[Tab->last + 1].Cont = SENTINEL;
	Tab->arr[Tab->last + 1].Key = key;

	int current_index = 0; // We are starting from the beginning
	while (ASCompare(Tab->arr[current_index].Key, key) != 0) {
		current_index++;
	}

	// Is we have found sentinel, we haven't found the item we were looking for
	if (current_index == (Tab->last + 1)) {
		*ind = current_index;
		return FALSE;
	}

	// We have found, so move current item one index left (if it's possible)
	if (current_index > 0) {
		tASData tmp_content = Tab->arr[current_index];
		Tab->arr[current_index] = Tab->arr[current_index - 1];
		Tab->arr[current_index - 1] = tmp_content;
		*ind = current_index - 1;
	}
	else {
		*ind = current_index;
	}

	return TRUE;
}

void ASInsert (tASTable *Tab, int Key, int Content)	{
/*          --------
** Vloží novou položku s obsahem Content a klíèem Key do tabulky Tab.
**
** Pokud by vložením další položky došlo k pøeplnìní Tab (pokud by nešlo
** pøi dalším vyhledávání uložit zarážku), volejte proceduru ASError()
** a novou položku nevkládejte.
**
** Pokud daná položka (udaná klíèem Key) v poli již existuje,
** pøiøadí se do odpovídající složky záznamu nový obsah.
** Pøi vyhledávání již existující položky využijte døíve implementovanou
** funkci ASSearch (to znamená, že se existující a modifikovaná polo¾ka
** automaticky posune dopøedu).
**/

	int index;
	int found = ASSearch(Tab, Key, &index);
	if (found) {
		// Actualize item
		Tab->arr[index].Cont = Content;
	}
	else {
		// Can we add at least one more item ?
		if (Tab->last == (ASSize - 2)) {
			// No, there isn't enough space
			ASError();
			return;
		}

		// Add new item
		Tab->arr[index].Cont = Content;
		Tab->last++;
	}
}

/* konec c403.c */

