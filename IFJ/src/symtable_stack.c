/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		symtable_stack.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Tabulka symbolů - implementace zásobníku pro volání funkcí
******************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include "symtable_stack.h"
#include "debug.h"
#include "error.h"

/*
 * Inicializuje zásobník
 */
void symtable_stack_init(TSymtableStack **stack) {
	assert(stack != NULL);

	*stack = NULL;
}

/*
 * Kontroluje, zda zásobník není prázdný
 */
bool symtable_stack_empty(TSymtableStack *stack) {
	if (stack == NULL)
		return true;

	return false;
}

/*
 * Přidá na zásobník položku.
 */
TErrCode symtable_stack_push(TSymtableStack **stack, const char *name, const char *type) {
	assert(stack != NULL);
	assert(name != NULL);
	assert(type != NULL);

	TSymtableStack *tmp = *stack;
	*stack = malloc(sizeof(TSymtableStack));
	if (*stack == NULL) {
		error(RAW, "semanticka analyza: Nedostatek pameti!\n");
		return ERR_FAILURE;
	}

	(*stack)->name = name;
	(*stack)->type = type;
	(*stack)->arg_check = 1;
	(*stack)->next=tmp;

	return ERR_SUCCESS;
}

/*
 * Vrátí ukazatel na řetězec reprezentující typ aktuálně kontrolované funkce.
 */
const char * symtable_stack_top_type(TSymtableStack *stack) {
	assert(stack != NULL);
	assert(stack->type != NULL);

	return stack->type;
}

/*
 * Vrátí řetězec na ukazatel se jménem funkce.
 */
const char * symtable_stack_top_name(TSymtableStack *stack) {
	assert(stack != NULL);
	assert(stack->name != NULL);

	return stack->name;
}


/*
 * Vrátí pořadí argumentu, který se má právě kontrolovat.
 */
int symtable_stack_top_arg(TSymtableStack *stack) {
	assert(stack != NULL);

	return stack->arg_check;
}

/*
 * Zvýší počet zkontrolovaných argumentů.
 */
void symtable_stack_top_checked(TSymtableStack *stack) {
	assert(stack != NULL);

	stack->arg_check++;
}

/*
 * Odstraní položku ze zásobníku.
 */
void symtable_stack_pop(TSymtableStack **stack) {
	assert(stack != NULL);
	assert(*stack != NULL);

	TSymtableStack *tmp = *stack;
	*stack = (*stack)->next;
	free(tmp);
}

/*
 * Zruší zásobník - uvolní paměť
 */
void symtable_stack_free(TSymtableStack **stack) {
	assert(stack != NULL);

	TSymtableStack *tmp = NULL;

	while (!symtable_stack_empty(*stack)) {
		tmp = *stack;
		*stack = (*stack)->next;
		free(tmp);
	}
}

/* Konec souboru symtable_stack.c */
