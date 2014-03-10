/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		symtable_stack.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Tabulka symbolů - rozhraní zásobníku pro volání funkcí
******************************************************************************/

#ifndef SYMTABLE_STACK_H
#define SYMTABLE_STACK_H

#include <stdbool.h>
#include "error.h"

typedef struct TSymtableStack{
	const char *name;                 // Jméno funkce
	const char *type;                 // Návratový typ a typy argumentů
	int arg_check;                    // Pořadí argumentu, který se má právě zkontrolovat
	struct TSymtableStack *next;      // Ukazatel na další položku v zásobníku
} TSymtableStack;

/*
 * Inicializuje zásobník
 */
void symtable_stack_init(TSymtableStack **stack);

/*
 * Kontroluje, zda zásobník není prázdný
 */
bool symtable_stack_empty(TSymtableStack *stack);

/*
 * Přidá na zásobník položku.
 */
TErrCode symtable_stack_push(TSymtableStack **stack, const char *name, const char *type);

/*
 * Vrátí ukazatel na řetězec reprezentující typ aktuálně kontrolované funkce.
 */
const char * symtable_stack_top_type(TSymtableStack *stack);

/*
 * Vrátí řetězec na ukazatel se jménem funkce.
 */
const char * symtable_stack_top_name(TSymtableStack *stack);

/*
 * Vrátí pořadí argumentu, který se má právě kontrolovat.
 */
int symtable_stack_top_arg(TSymtableStack *stack);

/*
 * Zvýší počet zkontrolovaných argumentů.
 */
void symtable_stack_top_checked(TSymtableStack *stack);

/*
 * Odstraní položku ze zásobníku.
 */
void symtable_stack_pop(TSymtableStack **stack);

/*
 * Zruší zásobník - uvolní paměť
 */
void symtable_stack_free(TSymtableStack **stack);

#endif /* #ifndef SYMTABLE_STACK_H */

/* Konec souboru symtable_stack.h */
