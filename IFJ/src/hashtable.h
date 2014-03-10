/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		hashtable.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Hashovaci tabulka - rozhrani
******************************************************************************/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "instructions.h"
#include "data_type.h"

/*
 * Oznčení toho, co symbol reprezentuje
 */
typedef enum {
	SYM_FUNCTION,       // Funkce
	SYM_LABEL,          // Návěstí
	SYM_VARIABLE,       // Proměnná
} EHTableType;

/*
 * Informace, které je potřeba uchovávat u funkce
 */
typedef struct {
	char *type;                          // Uchováva návratový typ funkce a typy parametrů
	TMicroInstruction *instruction;      // Ukazatel na odpovídající mikroinstrukci, pokud dosud není známá - NULL
	int argument_count;                  // Počet parametrů funkce
	int var_count;                       // Počet lokálních a pomocných proměnných potřebných ve funkci
	bool declared;                       // Informace o tom, zda již byla funkce definovaná
	bool builtin;                        // Informace o tom, zda se jedná o vestavěnou funkci
} SymbolFuncData;

/*
 * Informace, které je potřeba uchovávat u návěstí
 */
typedef struct {
	TMicroInstruction *instruction;      // Ukazatel na odpovídající mikroinstrukci
	bool declared;                       // Informace o tom, zda již bylo návěstí deklarováno
} SymbolLabelData;

/*
 * Informace, které je potřeba uchovávat u proměnné
 */
typedef struct {
	TDataType type;                      // Typ proměnné
	int index;                           // Index proměnné na zásobníku
} SymbolVarData;

/*
 * Union obsahující informace o funkci, návěstí, nebo proměnné
 */
typedef union {
	SymbolFuncData function;
	SymbolLabelData label;
	SymbolVarData var;
} UHtableData;

/*
 * Vlastní data položky
 */
typedef struct {
	EHTableType type;    // Typ symbolu
	UHtableData data;    // Potřebná data k danému symbolu
} THtableItemData;

/*
 * Prvek (polozka) hashovaci tabulky
 */
typedef struct htable_item {
	char *key;                // Klic, podle ktereho se bude vyhledavat
	THtableItemData data;      // Vlastni data polozky
	struct htable_item *next; // Ukazatel na dalsi polozku na danem indexu
} THtableItem;

/*
 * Hashovaci tabulka
 */
typedef struct {
	size_t size;         // Velikost tabulky
	THtableItem *data[]; // Pole seznamu, ktere tvori zaklad tabulky
} THtable;

/*
 * Vytvori a inicializuje hashovaci tabulku velikosti 'size'.
 * Pokud se tabulku nepodari vytvorit (nedostatek pameti), pak funkce vraci NULL.
 */
THtable *htable_init(size_t size);

/*
 * Zrusi vsechny prvky v tabulce a zrusi tabulku.
 */
void htable_free(THtable *t);

/*
 * Zrusi vsechny prvky v tabulce, ale nezrusi tabulku, takze je mozno s ni nadale pracovat.
 */
void htable_clear(THtable *t);

/*
 * Vyhleda polozku v tabulce podle klice. Pokud ji nalezne, vraci ukazatel na tuto polozku
 * (resp. na data teto polozky).
 * Pokud ne, tak ji prida do tabulky a vrati ukazatel na ni.
 * Pokud se nepodari vytvorit novou polozku v tabulce, funkce vraci NULL.
 */
THtableItemData *htable_lookup(THtable *t, const char *key, THtableItemData data);

/*
 * Pouze vyhleda zadanou polozku v tabulce (na rozdil od htable_lookup),
 * pokud ji najde, vraci ukazatel na ni. Pokud ne, tak vraci NULL.
 */
THtableItemData *htable_search(THtable *t, const char *key);

/*
 * Zrusi zadanou polozku a vraci true, jestlize polozka byla nalezena a zrusena.
 * Pokud dana polozka neexistuje, vraci false
 */
bool htable_delete(THtable *t, const char *key);

/*
 * Pokud je tabulka prázdná vrací true, jinak false
 */
bool htable_empty(THtable *t);

#endif /* #ifndef HASHTABLE_H */

/* Konec souboru hashtable.h */
