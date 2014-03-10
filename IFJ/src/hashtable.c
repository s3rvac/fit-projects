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
	Popis:	Implementace hashovaci tabulky
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "debug.h"
#include "hashtable.h"

/*
 * Hashovaci (mapovaci) funkce.
 */
static unsigned int hash_function(const char *str, size_t htable_size)
{
	size_t h = 0;
	unsigned char *p;
	for (p = (unsigned char *) str; *p != '\0'; p++)
		h = 31*h + *p;
	return h % htable_size;
}

/*
 * Vytvori a inicializuje hashovaci tabulku velikosti 'size'.
 * Pokud se tabulku nepodari vytvorit, pak funkce vraci NULL.
 */
THtable *htable_init(size_t size)
{
	// Vytvoření tabulky
	THtable *t = malloc(sizeof(THtable) + size * sizeof(THtableItem *));

	if (t == NULL)
		return NULL;

	// Inicializace vytvořené tabulky
	t->size = size;
	for (unsigned int i = 0; i < size; i++)
	t->data[i] = NULL;

	return t;
}

/*
 * Zrusi vsechny prvky v tabulce a zrusi tabulku.
 */
void htable_free(THtable *t)
{
	assert (t != NULL);

	// Zrus vsechny polozky v tabulce
	htable_clear(t);

	// Dealokuj tabulku
 	free(t);
}

/*
 * Zrusi vsechny prvky v tabulce, ale nezrusi tabulku, takze je mozno s ni nadale pracovat.
 */
void htable_clear(THtable *t)
{
	assert (t != NULL);

	THtableItem *tmp;

	for (unsigned int i = 0; i < t->size; i++) {
		while (t->data[i] != NULL) {
			tmp = t->data[i];
			t->data[i] = t->data[i]->next;
			// dealokuj klic tabulky (retezec)
			free(tmp->key);

			// Dealokuj pamet pro polozku tabulky
			free(tmp);
		}
	}
}

/*
 * Vytvori novou polozku v tabulce a vrati ukazatel na ni
 * (nebo NULL, pokud se polozku nepodari vytvorit).
 */
static THtableItem *htable_newitem(const char *key, THtableItemData *data)
{
	// Alokace pameti pro polozku
	THtableItem *new_item = malloc(sizeof(*new_item));
	if (!new_item)
		return NULL;

	// Alokace pameti pro klic
	new_item->key = malloc((strlen(key) + 1) * sizeof(char));
	if (!new_item->key) {
		free(new_item);
		return NULL;
	}

	// Zkopiruje klic a ulozi data do nove vytvorene polozky
	strcpy(new_item->key, key);
	new_item->data = *data;
	new_item->next = NULL;

	return new_item;
}

/*
 * Vyhleda polozku podle klice a vrati:
 * a) polozka byla nalezena: true a ukazatel nastavi na danou polozku
 * b) polozka nebyla nalezena: false a ukazatel nastavi na posledni polozku v seznamu
 * V parametru 'index' bude nastaven index v tabulce (tj pro pripad, ze item bude NULL, tzn
 * nenalezli jsme ani 1 polozku)
 */
static bool htable_find(THtable *t, const char *key, THtableItem **item, size_t *index)
{
	assert ((t != NULL) && (key != NULL));

	// Zjisti a nastav index v tabulce
	size_t t_index = hash_function(key, t->size);
	if (index != NULL)
		*index = t_index;

	// Zjisti, zda uz se na danem indexu nachazi nejaka polozka
	if (t->data[t_index] == NULL) {
		// Nenachazi, index je nastaven
		if (item != NULL)
			*item = NULL;

		return false;
	}

	// Nachazi, takze zjisti, zda jedna z polozek je hledana polozka
	THtableItem *tmp = t->data[t_index];
	while (tmp != NULL) {
		if (strcmp(key, tmp->key) == 0) {
			// Toto je hledana polozka
			if (item != NULL)
				*item = tmp;
			return true;
		}
		else if (tmp->next == NULL)
			break;

		tmp = tmp->next;
	}

	// Neni, nastav ukazatel na posledni polozku
	if (item != NULL)
		*item = tmp;
	return false;
}

/*
 * Vyhleda polozku v tabulce podle klice. Pokud ji nalezne, vraci ukazatel na tuto polozku
 * (resp. na data teto polozky).
 * Pokud ne, tak ji prida do tabulky a vrati ukazatel na ni.
 * Pokud se nepodari vytvorit novou polozku v tabulce, funkce vraci NULL.
 */
THtableItemData *htable_lookup(THtable *t, const char *key, THtableItemData data)
{
	THtableItem *item = NULL;
	size_t index = 0;
	bool found = htable_find(t, key, &item, &index);

	if (found) {
		assert (item != NULL);

		// Polozka nalezena
		return &item->data;
	}
	else if (item != NULL) {
		// Nenalezli jsme, ale na danem indexu se nachazi min. 1 polozka,
		// takze vytvorime novou
		item->next = htable_newitem(key, &data);
		return (item->next != NULL) ? &item->next->data : NULL;
	}

	// Nenalezli jsme a na danem indexu se neneachazi zadna polozka
	t->data[index] = htable_newitem(key, &data);
	return (t->data[index] != NULL) ? &t->data[index]->data : NULL;
}

/*
 * Pouze vyhleda zadanou polozku v tabulce (na rozdil od htable_lookup),
 * pokud ji najde, vraci ukazatel na ni. Pokud ne, tak vraci NULL.
 */
THtableItemData *htable_search(THtable *t, const char *key)
{
	THtableItem *item = NULL;
	bool found = htable_find(t, key, &item, NULL);

	return (found && (item != NULL)) ? &item->data : NULL;
}

/*
 * Zrusi zadanou polozku a vraci true, jestlize polozka byla nalezena a zrusena.
 * Pokud dana polozka neexistuje, vraci false
 */
bool htable_delete(THtable *t, const char *key)
{
	THtableItem *item = NULL;
	size_t index = 0;
	bool found = htable_find(t, key, &item, &index);

	if (found) {
		assert (item != NULL);

		// Polozka nalezena, kvuli zachovani konzistence seznamu
		// se bude muset projit seznamem od pocatecniho indexu tabulky
		THtableItem *tmp = NULL; // Pomocny ukazatel na polozku pred hledanou polozkou
		THtableItem *item = t->data[index];

		while (strcmp(key, item->key) != 0) {
			tmp = item;
			item = item->next;
		}

		// item ukazuje na hledanou polozku, kterou budeme rusit
		// a tmp na polozkou pred hledanou polozkou
		tmp->next = item->next;
		free(item);
		return true;
	}

	// Nebyla nalezena, nic nerusime
	return false;
}

/*
 * Pokud je tabulka prázdná vrací true, jinak false
 */
bool htable_empty(THtable *t) {
	assert(t != NULL);
	assert(t->data != NULL);

  for (unsigned int i = 0; i < t->size; i++) {
		if (t->data[i] != NULL) // Tabulka něco obsahuje
			return false;
	}

	// Prošli jsme celou tabulku a byla prázdná
	return true;
}

/* Konec souboru hashtable.c */
