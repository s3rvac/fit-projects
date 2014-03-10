
/* c201.c *********************************************************************}
{* Téma: Jednosmìrný lineární seznam
**
**                     Návrh a referenèní implementace: Petr Pøikryl, øíjen 1994
**                                          Úpravy: Andrea Nìmcová listopad 1996
**                                                   Petr Pøikryl, listopad 1997
**                                Pøepracované zadání: Petr Pøikryl, bøezen 1998
**                                  Pøepis do jazyka C: Martin Tuèek, øíjen 2004
**	                                          Úpravy: Bohuslav Køena, øíjen 2006
**
** Implementujte abstraktní datový typ jednosmìrný lineární seznam.
** U¾iteèným obsahem prvku seznamu je celé èíslo typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou typu tList.
** Definici konstant a typù naleznete v hlavièkovém souboru c201.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu s vý¹e
** uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DisposeList ... zru¹ení v¹ech prvkù seznamu,
**      InsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zru¹í první prvek seznamu,
**      PostDelete .... ru¹í prvek za aktivním prvkem,
**      PostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      LCopy ......... vrací hodnotu aktivního prvku,
**      actualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      LSucc ......... posune aktivitu na dal¹í prvek seznamu,
**      active ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci funckí nevolejte ¾ádnou z funkcí implementovaných v rámci
** tohoto pøíkladu, není-li u funkce explicitnì uvedeno nìco jiného.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

// Debugging
#ifndef NDEBUG
#define NDEBUG
#include <assert.h>
#endif

int solved;
int errflg;

void Error()	{
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tuto funkci budete volat v rámci nìkterých dále implementovaných operací.
**/
    printf ("*ERROR* Chyba pøi práci se seznamem.\n");
    errflg = TRUE;                      /* globální promìnná -- pøíznak chyby */
}

void InitList (tList *L)	{
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/
	assert(L != NULL);

	L->act = L->frst = NULL;
}

void DisposeList (tList *L)	{
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. V¹echny prvky seznamu L budou korektnì uvolnìny voláním
** operace free.
***/
	assert(L != NULL);

	tElemPtr item_backup;
	tElemPtr next_item = L->frst;
	while ((item_backup = next_item) != NULL) {
		next_item = next_item->ptr;
		free(item_backup);
	}

	// Set list to the initialization state
	L->frst = L->act = NULL;
}

void InsertFirst (tList *L, int val)	{
/*
** Vlo¾í prvek s hodnotou val na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci Error().
**/
	assert(L != NULL);

	// Create new item
 	tElemPtr new_item = malloc(sizeof(*new_item));
	if (!new_item){
		Error();
 		return;
	}

	new_item->data = val;
	new_item->ptr = L->frst;
	L->frst = new_item;
}

void First (tList *L)		{
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný pøíkaz, ani¾ byste testovali,
** zda je seznam L prázdný.
**/
	assert(L != NULL);

	L->act = L->frst;
}

void CopyFirst (tList *L, int *val)	{
/*
** Vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	assert(L != NULL && val != NULL);

	// Check whether list is not empty
	if (!L->frst) {
		Error();
		return;
	}

	*val = L->frst->data;
}

void DeleteFirst (tList *L)	{
/*
** Ru¹í první prvek seznamu L. Pokud byl seznam L aktivní, aktivita se ztrácí.
** Pokud byl seznam L prázdný, nic se nedìje!
**/
	assert(L != NULL);

	// Check whether list is not empty
	if (!L->frst)
		return;

	// If the first item is active, list will become deactive
	if (L->act == L->frst)
		L->act = NULL;

	tElemPtr frst_item_backup = L->frst;
	L->frst = L->frst->ptr;
	free(frst_item_backup);
}

void PostDelete (tList *L)			{
/*
** Ru¹í prvek seznamu L za aktivním prvkem. Pokud není seznam L aktivní
** nebo pokud je aktivní poslední prvek seznamu L, nic se nedìje!
**/
	assert(L != NULL);

	// Check whether list is active
	// and we're not at the last item of the list
	if (!L->act || L->act->ptr == NULL)
		return;

	tElemPtr item_backup = L->act->ptr;
	L->act->ptr = L->act->ptr->ptr;
	free(item_backup);
}

void PostInsert (tList *L, int val)	{
/*
** Vlo¾í prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje!
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci Error().
**/
	assert(L != NULL);

	// Check whether list is active
	if (!L->act) {
		return;
	}

	// Create new item
	tElemPtr new_item = malloc(sizeof(*new_item));
	if (!new_item){
		Error();
 		return;
	}

	new_item->data = val;
	new_item->ptr = L->act->ptr;
	L->act->ptr = new_item;
}

void Copy (tList *L, int *val)		{
/*
** Vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, volá se funkce Error().
**/
	assert(L != NULL && val != NULL);

	// Check whether list is active
	if (!L->act) {
		Error();
		return;
	}

	*val = L->act->data;
}

void Actualize (tList *L, int val)	{
/*
** Pøepí¹e data aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic!
**/
	assert(L != NULL);

	// Check whether list is active
	if (!L->act)
		return;

	L->act->data = val;
}

void Succ (tList *L)	{
/*
** Posune aktivitu na následující prvek seznamu L.
** V¹imnìte si, ¾e touto operací se mù¾e aktivní seznam stát neaktivním.
** Pokud seznam L není aktivní, nedìlá nic!
**/
	assert(L != NULL);

	// Check whether list is active
	if (!L->act)
		return;

	L->act = L->act->ptr;
}

int Active (tList *L) 	{
/*
** Je-li seznam L aktivní, vrací True. V opaèném pøípadì vrací false.
** Tuto funkci implementujte jako jediný pøiøazovací pøíkaz,
**/
	assert(L != NULL);

	return (L->act != NULL) ? TRUE : FALSE;
}

/* Konec c201.c */
