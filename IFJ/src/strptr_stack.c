/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		strptr_stack.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Zasobnik ukazatelu na retezce.
******************************************************************************/

#include <stdlib.h>
#include "strptr_stack.h"

/*
 * Globalni ukazatel na vrchol zasobniku ukazatelu na retezce.
 */
static Tstrptr_stack_item *strptr_stack_top = NULL;

/*
 * Inicializace zasobniku.
 */
void strptr_stack_init(void)
{
	strptr_stack_top = NULL;

	return;
}

/*
 * Pridani jiz alokovaneho retezce do zasobniku.
 * V pripade chyby (malo pameti) vraci NULL.
 * Jinak vraci ukazatel na polozku v zasobniku.
 */
Tstrptr_stack_item *strptr_stack_push(char *string)
{
	Tstrptr_stack_item *new_item;

	/* Alokace polozky v zasobniku. */
	new_item = malloc(sizeof(*new_item));
	if (new_item == NULL)
		return NULL;

	/* Vlozeni do zasobniku. */
	new_item->ptr = string;
	new_item->next = strptr_stack_top;
	strptr_stack_top = new_item;

	return new_item;
}

/*
 * Alokace noveho retezce s vlozenim ukazatele do zasobniku.
 * V pripade chyby (malo pameti) vraci NULL (a zasobnik nerusi).
 */
void *strmalloc(size_t size)
{
	void *new_str;

	/* Alokace retezce. */
	new_str = malloc(size);
	if (new_str == NULL)
		return NULL;

	/* Pridani retezce do zasobniku. */
	if (strptr_stack_push(new_str) == NULL) {
		free(new_str);
		return NULL;
	}

	return new_str;
}

/*
 * Zruseni celeho zasobniku vcetne dealokace retezcu.
 */
void strptr_stack_free()
{
	Tstrptr_stack_item *free_item;

	while (strptr_stack_top != NULL) {
		/* Odpojeni uvolnovane polozky. */
		free_item = strptr_stack_top;
		strptr_stack_top = strptr_stack_top->next;
		/* Dealokace polozky + retezce */
		free(free_item->ptr);
		free(free_item);
	}

	return;
}

/* Konec souboru strptr_stack.c */
