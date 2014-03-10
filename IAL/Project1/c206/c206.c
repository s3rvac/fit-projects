f
/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2006
**
** Implementujte abstraktní datový typ dvousmìrnì vázaný lineární seznam.
** U¾iteèným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou
** typu tDLList (DL znamená Double-Linked a slou¾í pro odli¹ení
** jmen konstant, typù a funkcí od jmen u jednosmìrnì vázaného lineárního
** seznamu). Definici konstant a typù naleznete v hlavièkovém souboru c206.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu
** s vý¹e uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ
** obousmìrnì vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DLDisposeList ... zru¹ení v¹ech prvkù seznamu,
**      DLInsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      DLInsertLast .... vlo¾ení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zru¹í první prvek seznamu,
**      DLDeleteLast .... zru¹í poslední prvek seznamu,
**      DLPostDelete .... ru¹í prvek za aktivním prvkem,
**      DLPreDelete ..... ru¹í prvek pøed aktivním prvkem,
**      DLPostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vlo¾í nový prvek pøed aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal¹í prvek seznamu,
**      DLPred .......... posune aktivitu na pøedchozí prvek seznamu,
**      DLActive ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci jednotlivých funckí nevolejte ¾ádnou z funkcí
** implementovaných v rámci tohoto pøíkladu, není-li u funkce
** explicitnì uvedeno nìco jiného.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

// Debugging
#ifndef NDEBUG
#define NDEBUG
#include <assert.h>
#endif

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tuto funkci budete volat v rámci nìkterých dále implementovaných operací.
**/
    printf ("*ERROR* Chyba pøi práci se seznamem.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L)	{
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/
	assert(L != NULL);

	L->First = L->Last = L->Act = NULL;
}

void DLDisposeList (tDLList *L)	{
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free.
**/
	assert(L != NULL);

	tDLElemPtr item_backup;
	tDLElemPtr next_item = L->First;
	while ((item_backup = next_item) != NULL) {
		next_item = next_item->rptr;
		free(item_backup);
	}

	// Set list to the initialization state
	L->First = L->Last = L->Act = NULL;
}

void DLInsertFirst (tDLList *L, int val)	{
/*
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	assert(L != NULL);

	// Create new item
	tDLElemPtr new_item = malloc(sizeof(*new_item));
	if (!new_item){
		DLError();
 		return;
	}

	new_item->data = val;
	new_item->lptr = NULL;
	new_item->rptr = L->First;

	if (L->First) {
		// There is at least one item in the list
		L->First->lptr = new_item;
	}
	else {
		// There are no items in the list
		L->Last = new_item;
	}

	L->First = new_item;
}

void DLInsertLast(tDLList *L, int val)	{
/*
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	assert(L != NULL);

	// Create new item
	tDLElemPtr new_item = malloc(sizeof(*new_item));
	if (!new_item){
		DLError();
 		return;
	}

	new_item->data = val;
	new_item->lptr = L->Last;
	new_item->rptr = NULL;

	if (L->Last) {
		// There is at least one item in the list
		L->Last->rptr = new_item;
	}
	else {
		// There are no items in the list
		L->First = new_item;
	}

	L->Last = new_item;
}

void DLFirst (tDLList *L)	{
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	assert(L != NULL);

	L->Act = L->First;
}

void DLLast (tDLList *L)	{
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	assert(L != NULL);

	L->Act = L->Last;
}

int DLCopyFirst (tDLList *L)	{
/*
** Vrátí hodnotu prvního prvku seznamu L. Pokud je seznam L prázdný,
** volá funkci DLError.
**/
	assert(L != NULL);

	if (!L->First) {
		DLError();

		// Result is undefined, so I can return anything
		return 1;
	}

	return L->First->data;
}

int DLCopyLast (tDLList *L)	{
/*
** Vrátí hodnotu posledního prvku seznamu L. Pokud je seznam L prázdný,
** volá funkci DLError.
**/
	assert(L != NULL);

	if (!L->Last) {
		DLError();

		// Result is undefined, so I can return anything
		return 1;
	}

	return L->Last->data;
}

void DLDeleteFirst (tDLList *L)	{
/*
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/
	assert(L != NULL);

	if (!L->First)
		return;

	// If the first item is active, list will become deactive
	if (L->Act == L->First)
		L->Act = NULL;

	// Delete first item
	tDLElemPtr first_item = L->First;
	L->First = L->First->rptr;
	L->First->lptr = NULL;
	free(first_item);
}

void DLDeleteLast (tDLList *L)	{
/*
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/
	assert(L != NULL);

	if (!L->Last)
		return;

	// If the last item is active, list will become deactive
	if (L->Act == L->Last)
		L->Act = NULL;

	// Delete last item
	tDLElemPtr last_item = L->Last;
	L->Last = L->Last->lptr;
	if (L->Last != NULL)
		L->Last->rptr = NULL;

	free(last_item);
}

void DLPostDelete (tDLList *L)	{
/*
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud nebyl seznam L aktivní, nic se nedìje.
**/
	assert(L != NULL);

	if (!L->Act || (L->Act == L->Last))
		return;

	// Delete item next to the active item
	tDLElemPtr backup_item = L->Act->rptr;
	L->Act->rptr = L->Act->rptr->rptr;

	if (backup_item == L->Last) {
		// Deleted item is the last one
		L->Last = L->Act;
	}
	else {
		L->Act->rptr->lptr = L->Act;
	}
	free(backup_item);
}

void DLPreDelete (tDLList *L)	{
/*
** Zru¹í prvek seznamu L pøed aktivním prvkem.
** Pokud nebyl seznam L aktivní, nic se nedìje.
**/
	assert(L != NULL);

	if (!L->Act || (L->Act == L->First))
		return;

	// Delete item before the active item
	tDLElemPtr backup_item = L->Act->lptr;
	L->Act->lptr = L->Act->lptr->lptr;

 	if (backup_item == L->First) {
		// Deleted item is the first one
		L->First = L->Act;
	}
	else {
		L->Act->lptr->rptr = L->Act;
	}
	free(backup_item);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	assert(L != NULL);

	if (!L->Act)
		return;

	// Create new item
	tDLElemPtr new_item = malloc(sizeof(*new_item));
	if (!new_item){
		DLError();
 		return;
	}

	new_item->data = val;
	new_item->rptr = L->Act->rptr;
	new_item->lptr = L->Act;
	L->Act->rptr = new_item;

	// Correct pointer to the last item
	if (L->Act == L->Last)
		L->Last = new_item;
	else
		new_item->rptr->lptr = new_item;
}

void DLPreInsert (tDLList *L, int val)		{
/*
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	assert(L != NULL);

	if (!L->Act)
		return;

	// Create new item
	tDLElemPtr new_item = malloc(sizeof(*new_item));
	if (!new_item){
		DLError();
 		return;
	}

	new_item->data = val;
	new_item->lptr = L->Act->lptr;
	new_item->rptr = L->Act;
	L->Act->lptr = new_item;

	// Correct pointer to the first item
	if (L->Act == L->First)
		L->First = new_item;
	else
		new_item->lptr->rptr = new_item;
}

int DLCopy (tDLList *L)	{
/*
** Vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError.
**/
	assert(L != NULL);

	if (!L->Act) {
		DLError();

		// Result is undefined, so I can return anything
		return 1;
	}

	return L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
**/
	assert(L != NULL);

	if (!L->Act) {
		return;
	}

	L->Act->data = val;
}

void DLSucc (tDLList *L)	{
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
**/
	assert(L != NULL);

	if (!L->Act) {
		return;
	}

	L->Act = L->Act->rptr;
}


void DLPred (tDLList *L)	{
/*
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
**/
	assert(L != NULL);

	if (!L->Act) {
		return;
	}

	L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivní, vrací true. V opaèném pøípadì vrací false.
** Funkci implementujte jako jediný pøíkaz.
**/
	assert(L != NULL);

	return (L->Act != NULL) ? TRUE : FALSE;
}

/* Konec c206.c*/
