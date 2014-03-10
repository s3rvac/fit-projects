/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		symbol_table.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Tabulka symbolů - implementace
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "debug.h"
#include "hashtable.h"
#include "symbol_table.h"
#include "data_type.h"
#include "constants.h"
#include "error.h"
#include "generator.h"
#include "symtable_stack.h"
#include "functions.h"

/*
 * Konstanta udávající, kolik místa se má vyhradit, při kontrole návratového
 * typu funkce a jejich argumentů. O tuto konstantu se také bude zvedat
 * alokovaná paměť, když místo pro kontrolu dojde
 */
#define SPACE_FOR_FUNC_TYPE 10

/*
 * Pomocné soukromé funkce tabulky symbolů
 */
static TErrCode free_memory_malloced_in_htable(THtable *t);
static TErrCode insert_var_to_htable(TSymtable *local, const char *name, TDataType type);
static TErrCode insert_func_type(TSymtable *t, TDataType type);
static TErrCode check_func_type(TSymtable *t, TDataType type);
static TErrCode check_end_of_arg(TSymtable *t);
static TErrCode symtable_insert_main(TSymtable *global);

/*
 * Vytvoří a inicializuje tabulku symbolů obsahující hashovací tabulku
 * velikosti definované v souboru constants.h. Pokud se tabulku nepodaří
 * vytvořit vrací NULL.
 */
TSymtable * symtable_init()
{
	TSymtable *new = malloc(sizeof(TSymtable));

	new->table = htable_init(HASH_TABLE_SIZE);
	new->argument_count = 0;
	new->var_count = 0;
	new->first_free_index = STACK_BOTTOM_SIZE;
	new->act = NULL;
	new->func_insert = false;
	new->definition = false;
	symtable_stack_init(&(new->stack));

	return new;
}

/*
 * Zruší všechny data v tabulce a uvolní paměť
 */
TErrCode symtable_free(TSymtable *t)
{
	assert(t != NULL);

	TErrCode err = free_memory_malloced_in_htable(t->table);
	htable_free(t->table);
	symtable_stack_free(&(t->stack));
	free(t);

	return err;
}

/*
 * Zruší všechna data v tabulce a reinicializuje ji, je možné s ní
 * nadále pracovat, jako by byla právě vytvořena
 */
TErrCode symtable_clear(TSymtable *t)
{
	assert(t != NULL);
	assert(symtable_stack_empty(t->stack));

	TErrCode err = free_memory_malloced_in_htable(t->table);
	htable_clear(t->table);
	symtable_stack_free(&(t->stack));
	t->argument_count = 0;
	t->var_count = 0;
	t->first_free_index = STACK_BOTTOM_SIZE;
	t->act = NULL;
	t->func_insert = false;
	t->definition = false;

	return err;
}

/*
 * Zjistí, zda je tabulka symbolů prázdná. Tzn. jestli je ve stavu v jakém je
 * po své inicializaci
 */
bool symtable_empty(TSymtable *t)
{
	assert(t != NULL);

	if (htable_empty(t->table) && (t->argument_count == 0) && (t->var_count == 0) &&
	    (t->first_free_index == STACK_BOTTOM_SIZE) && (t->act == NULL) &&
			(t->func_insert == false) && (t->definition == false) && (symtable_stack_empty(t->stack)))
		return true;

	return false;
}

/*
 * Vloží novou proměnnou do tabulky
 */
TErrCode symtable_insert_var(TSymtable *local, const char *name, TDataType type)
{
	assert(local != NULL);

	TErrCode err = insert_var_to_htable(local, name, type);
	if (err == ERR_FAILURE)
		return ERR_FAILURE;

	local->var_count++; // Vložili jsme novou proměnnou

	return ERR_SUCCESS;
}

/*
 * Vloží nový parametr do tabulky
 */
TErrCode symtable_insert_arg(TSymtable *local, const char *name, TDataType type)
{
	assert(local != NULL);
	assert(name != NULL);

	// Protože se parametr uvnitř funkce chová jako proměnná, nijak jej od
	// ostatních proměnných neodlišujeme
	TErrCode err = insert_var_to_htable(local, name, type);
	if (err == ERR_FAILURE)
		return ERR_FAILURE;

	local->argument_count++; // Vložili jsme nový parametr

	// Ukládání, nebo kontrola typů argumentů
	if (local->func_insert == true) {
		if (insert_func_type(local, type) != ERR_SUCCESS)
			return ERR_FAILURE;
	}
	else if (check_func_type(local, type) != ERR_SUCCESS)
		return ERR_FAILURE;

	return ERR_SUCCESS;
}

/*
 * Vloží nový parametr do tabulky. Volá se při deklaraci funkce.
 */
TErrCode symtable_insert_arg_type(TSymtable *global, TDataType type)
{
	assert(global != NULL);

	global->argument_count++; // Vložili jsme nový parametr

	// Ukládání, nebo kontrola typů argumentů
	if (global->func_insert == true) {
		if (insert_func_type(global, type) != ERR_SUCCESS)
			return ERR_FAILURE;
	}
	else if (check_func_type(global, type) != ERR_SUCCESS)
		return ERR_FAILURE;

	return ERR_SUCCESS;
}

/*
 * Uloží informaci o použití dočasné proměnné pro uložení hodnoty
 * mezivýrazu a vrátí její index
 */
int symtable_insert_tmp(TSymtable *local)
{
	assert(local != NULL);

	int tmp = local->first_free_index;
	local->first_free_index++;
	local->var_count++; // Proměnná bude zabírat místo na zásobníku, musíme s ní počítat

	return tmp;
}

/*
 * Začátek vkládání funkce, případně její sémantické kontroly. Pokud půjde
 * o deklaraci funkce, lokalní tabulka se po zavolání
 * symtable_stop_insert_func() vrátí do stejného stavu, v jakém byla před
 * vykonáním této funkce. V případě neúspěchu vrací ERR_FAILURE
 */
TErrCode symtable_start_insert_func(TSymtable *global, TSymtable *local, const char *name, TDataType ret, bool builtin)
{
	assert(global != NULL);
	assert(local != NULL);
	// Do tabulek se nesmí právě vkládat jiná funkce
	assert(global->act == NULL);
	assert(local->act == NULL);

	// Zjistíme, zda se v tabulce už nenachází funkce s daným jménem
	local->act = htable_search(global->table, name);

	if (local->act == NULL) {
		// Funkce ještě v tabulce není, budeme ji vkládat
		local->func_insert = global->func_insert = true;
		THtableItemData new = {.type = SYM_FUNCTION, .data = {.function = {.type = NULL, .instruction = NULL, .argument_count = 0, .var_count = 0, .declared = false, .builtin = builtin}}};
		// Startovací bod funkce vytváříme vždy hned při jejim prvním výskytu, i když nepůjde o definici
		new.data.function.instruction = generate_new_func();
		if (new.data.function.instruction == NULL)
			return ERR_FAILURE;

		local->act = htable_lookup(global->table, name, new); // Vložíme do hashovací tabulky
		if (local->act == NULL) {
			error(RAW, "semanticka analyza: Nedostatek pameti\n");
			return ERR_FAILURE;
		}

		// Uložení návratového typu
		local->act->data.function.type = malloc(SPACE_FOR_FUNC_TYPE * sizeof(char));
		if (local->act->data.function.type == NULL) {
			error(RAW, "semanticka analyza: Nedostatek pameti\n");
			return ERR_FAILURE;
		}

		if (insert_func_type(local, ret) != ERR_SUCCESS)
			return ERR_FAILURE;
	}
	else {
		// Funkce, již v tabulce je, budeme kontrolovat, jestli stejně deklarovaná
		if (local->act->type != SYM_FUNCTION) {
			// Pod tímto názvem není v tabulce funkce
			error(ROWONLY, "semanticka analyza: Identifikator %s jiz nemuze byt pouzit jako nazev funkce, protoze je jiz pouzivan!\n", name);
			return ERR_FAILURE;
		}

		// Zkontrolujeme, jestli se nepokoušíme deklarovat built-in funkci
		if (local->act->data.function.builtin == true) {
			error(ROWONLY, "semanticka analyza: Vestavenou funkci %s nelze deklarovat ani definovat!\n", name);
			return ERR_FAILURE;
		}

		// Protože už funkce v tabulce je, musíme tabulky správně nastavit
		local->func_insert = global->func_insert = false;

		//Kontrola návratového typu
		if (check_func_type(local, ret) != ERR_SUCCESS)
			return ERR_FAILURE;
	}

	// Ještě dekončíme přípravu tabulek na práci s touto funkcí
	global->act = local->act;
	global->argument_count = 0;
	global->definition = local->definition = false;

	return ERR_SUCCESS;
}

/*
 * Narazili jsme na definici funkce. V případě, že funkci již není možné
 * definovat vrací chybu.
 */
TErrCode symtable_func_declar(TSymtable *local) {
	assert(local != NULL);
	assert(local->act != NULL);
	assert(local->act->type == SYM_FUNCTION);

	if (local->act->data.function.declared == true) {
		// Pokoušíme se znovu definovat funkci
		error(ROWONLY, "semanticka analyza: Funkce jiz byla jednou definovana!\n");
		return ERR_FAILURE;
	}

	// Nastavíme tabulku, aby si pamatoval, že definujeme funkci
	local->definition = true;

	// Přidáme mikroinstrukci do generovaného programu
	generator_ins_inst(local->act->data.function.instruction);
	local->act->data.function.declared = true; // Funkce je již součástí nudle, nesmíme ji uvolnit zvlášť

	return ERR_SUCCESS;
}

/*
 * Konec vkládání funkce. V ukazateli instruction funkce předá adresu
 * mikroinstrukce se začátkem funkce. V případě chyby vrací ERR_FAILURE.
 */
TErrCode symtable_stop_insert_func(TSymtable *global, TSymtable *local)
{
	assert(global != NULL);
	assert(local != NULL);
	assert(global->act == local->act); // Obě tabulky musí pracovat se stejnou funkcí
	assert(global->act != NULL);
	assert(global->act->type == SYM_FUNCTION);

	if (local->definition == true) { // Končí definice funkce
		// Museli jsme zkontrolovat všechny parametry funkce
		if (check_end_of_arg(local) != ERR_SUCCESS)
			return ERR_FAILURE;

		// Zapamatujeme si u funkce kolik potřebuje místa na zásobníku
		global->act->data.function.argument_count = local->argument_count;
		global->act->data.function.var_count = local->var_count;

		// Upravíme data v mikroinstrukci
		generator_edit_head_func(global->act->data.function.instruction, local->var_count, local->argument_count);
	}
	else if (check_end_of_arg(global) != ERR_SUCCESS)
		return ERR_FAILURE;

	// Zrušíme informace, o tom, že se vkládá funkce v globální tabulce, a vymažeme lokální
	TErrCode err = symtable_clear(local);
	global->act = NULL;
	global->func_insert = false;

	return err;
}

/*
 * Vloží nové návěstí do tabulky symbolů, toto návěstí však není prozatím
 *  definované.
 */
TErrCode symtable_insert_label_name(TSymtable *local, const char *name) {
	assert(local != NULL);
	assert(name != NULL);

	THtableItemData *label = htable_search(local->table, name);
	if (label != NULL) {
		// Symbol s tímto názvem již existuje
		if (label->type != SYM_LABEL) {
			// Název symbolu není návěstí
			error(ROWONLY, "semanticka analyza: Identifikator %s byl jiz na stejne urovni pouzit!\n", name);
			return ERR_FAILURE;
		}

		return ERR_SUCCESS;
	}

	// Vložíme nové návěstí do tabulky
	THtableItemData data = {.type = SYM_LABEL, .data = {.label = {.declared = false, .instruction = NULL}}};
	label = htable_lookup(local->table, name, data);
	if (label == NULL) {
		// Došla paměť
		error(RAW, "semanticka analyza: Nedostatek pameti!\n");
		return ERR_FAILURE;
	}

	// I když ještě není známo přesné umístění návěstí v nudli, alokujeme místo, aby jsme mohli provádět skoky
	label->data.label.instruction = generate_new_label();

	if (label->data.label.instruction == NULL) {
		return ERR_FAILURE;
	}

	return ERR_SUCCESS;
}

/*
 * Vloží nové návěstí do tabulky symbolů. Používá se v případě jeho definice.
 */
TErrCode symtable_insert_label(TSymtable *t, const char *name)
{
	assert(t != NULL);

	THtableItemData *label = htable_search(t->table, name);
	if (label != NULL) {
		// Symbol s tímto názvem již existuje
		if (label->type != SYM_LABEL) {
			// Název symbolu není návěstí
			error(ROWONLY, "semanticka analyza: Identifikator %s byl jiz na stejne urovni pouzit!\n", name);
			return ERR_FAILURE;
		}

		if (label->data.label.declared == true) {
			// Snažíme se definovat návěstí se stejným jménem napodruhé
			error(ROWONLY, "semanticka analyza: Navesti %s se v teto funkci jiz vyskytuje!\n", name);
			return ERR_FAILURE;
		}

		// Označíme návěstí jako definované
		label->data.label.declared = true;
		return ERR_SUCCESS;
	}

	// Vložíme nové návěstí do tabulky
	THtableItemData data = {.type = SYM_LABEL, .data = {.label = {.declared = true, .instruction = NULL}}};

	label = htable_lookup(t->table, name, data);
	if (label == NULL) {
		// Došla paměť
		error(RAW, "semanticka analyza: Nedostatek pameti!\n");
		return ERR_FAILURE;
	}

	// Vytvoříme mikroinstrukci cíle skoku
	label->data.label.instruction = generate_new_label();
	if (label->data.label.instruction == NULL) {
		return ERR_FAILURE;
	}

	return ERR_SUCCESS;
}

/*
 * V tabulce t najde proměnnou var_name. Do index uloží index, na kterém se
 * proměnná nachází, do type její typ. Pokud nás index nebo type nezajímá,
 * očekává NULL pointer. Pokud identifikátor neexistuje, nebo se
 * nejedná o proměnnou vrací chybu.
 */
TErrCode get_variable(TSymtable *t, int *index, TDataType *type, const char *var_name) {
	assert(t != NULL);
	assert(var_name != NULL);

	THtableItemData *symbol = htable_search(t->table, var_name);

	if (symbol == NULL) {
		// Identifikátor neexistuje
		return ERR_FAILURE;
	}

	if (symbol->type != SYM_VARIABLE) {
		// Identifikátor nereprezentuje proměnnou
		return ERR_FAILURE;
	}

	if (index != NULL)
		*index = symbol->data.var.index;
	if (type != NULL)
		*type = symbol->data.var.type;

	return ERR_SUCCESS;
}

/*
 * V tabulce t najde funkci name. Do data_type uloží její návratový typ.
 * Pokud identifikátor neexistuje, nebo se nejedná o funkci vrací chybu.
 */
TErrCode symtable_get_func_rettype(TSymtable *t, const char *name, TDataType *data_type) {
	assert(t != NULL);
	assert(name != NULL);

	THtableItemData *symbol = htable_search(t->table, name);

	if (symbol == NULL) {
		// Identifikátor neexistuje
		return ERR_FAILURE;
	}

	if (symbol->type != SYM_FUNCTION) {
		// Identifikátor nereprezentuje funkci
		return ERR_FAILURE;
	}

	assert(symbol->data.function.type != NULL);

	*data_type = (TDataType) symbol->data.function.type[0];

	return ERR_SUCCESS;
}

/*
 * Zjistí zda existuje funkce se zadaným jménem. Vrací true pokud ano, jinak false.
 */
bool symtable_func_exists(TSymtable *global, const char *name) {
	assert(global != NULL);
	assert(name != NULL);

	THtableItemData *symbol = htable_search(global->table, name);

	if ((symbol == NULL) || (symbol->type != SYM_FUNCTION))
		return false;

	return true;
}

/*
 * Zjistí zda existuje symbol s předaným identifikátorem. Vrací true pokud ano,
 * jinak false
 */
bool symtable_id_exists(TSymtable *t, const char *name) {
	assert(t !=NULL);
	assert(name != NULL);

	if (htable_search(t->table, name) == NULL)
		return false;

	return true;
}

/*
 * Zjistí, zda existuje proměnná daného jména v globální nebo lokální tabulce.
 * Vrací true pokud ano, jinak false.
 */
bool symtable_variable_exists(TSymtable *global, TSymtable *local, const char *name) {
	assert(global != NULL);
	assert(local != NULL);
	assert(name != NULL);

	if (get_variable(local, NULL, NULL, name) == ERR_SUCCESS)
		return true; // Proměnná existuje v lokální tabulce

	if (get_variable(global, NULL, NULL, name) == ERR_SUCCESS)
		return true; // Proměnná existuje v globální tabulce

	return false; // Proměnnou jsme nenašli
}

/*
 * Zjistí, zda je v tabulce definovaná funkce main. Pokud ano vrátí ukazatel
 * na odpovídající mikroinstrukci. Jinak NULL.
 */
TMicroInstruction *symtable_find_main(TSymtable *global) {
	assert(global != NULL);

	THtableItemData *tmp = htable_search(global->table, "main");

	if (tmp == NULL)
		return NULL;

	if (tmp->data.function.declared == true)
		return tmp->data.function.instruction;

	// Funkce main() nebyla definovaná
	return NULL;
}

/*
 * Funkce vloží do globální tabulky počáteční data - vestavěné funkce a
 * prototyp funkce main(). V případě chyby vrací chybový kód.
 */
TErrCode symtable_insert_initial_data(TSymtable *global) {
	if (symtable_insert_main(global) != ERR_SUCCESS)
		return ERR_FAILURE;

	return insert_all_builtin_functions(global);
}

/*
 * Připraví se na začátek volání funkce, vyhledá jí v hashovací tabulce a uloží
 * její typ na pomocný zásobník. V případě chyby vypíše důvod a vrátí chybový
 * kód.
 */
TErrCode symtable_start_call_func(TSymtable *global, const char *name) {
	assert(global != NULL);
	assert(name != NULL);

	THtableItemData *symbol = htable_search(global->table, name);

	if (symbol == NULL) {
		error(ROWONLY, "semanticka analyza: Funkce %s jeste nebyla definovana ani deklarovana!\n", name);
		return ERR_FAILURE;
	}

	if (symbol->type != SYM_FUNCTION) {
		error(ROWONLY, "semanticka analyza: %s neni funkce!\n", name);
		return ERR_FAILURE;
	}

	symtable_stack_push(&(global->stack), name, symbol->data.function.type);

	return ERR_SUCCESS;
}

/*
 * Zjistí datový typ dalšího argumentu právě zpracovávané funkce a porovná jej
 * s adresou předanou pointerem arg_type. V případě chyby ji vypíše a vrátí
 * chybový kód.
 */
TErrCode symtable_call_arg(TSymtable *global, TDataType arg_type) {
	assert(global != NULL);

	// Návratový typ funkce na vrcholu zásobníku a pořadí argumentu se kterým se pracuje
	const char *func_type = symtable_stack_top_type(global->stack);
	int arg = symtable_stack_top_arg(global->stack);

	if (func_type[arg] != (char) arg_type) {
		const char *func_name = symtable_stack_top_name(global->stack);
		// Nesprávný počet/typ argumentu
		if (func_type[arg] == '\0') {
			error(ROWONLY, "semanticka analyza: Funkce %s ocekava pouze %d parametru!\n", func_name, arg - 1);
			return ERR_FAILURE;
		}
		else {
			error(ROWONLY, "semanticka analyza: %d. parametr funkce %s je nespravneho typu!\n", arg, func_name);
			return ERR_FAILURE;
		}
	}

	// Příště se bude kontrolovat další parametr
	symtable_stack_top_checked(global->stack);

	return ERR_SUCCESS;
}

/*
 * Zkontroluje zda nebyla funkce zavolána s menším počtem parametrů a odebere
 * právě volanou funkci ze zásobníku a vrátí ukazatel na mikroinstrukci s jejim
 * začátkem. V případě chyby vypíše chybovou zprávu a vrátí NULL.
 */
TMicroInstruction * symtable_stop_call_func(TSymtable *global) {
	assert(global != NULL);

	// Návratový typ funkce na vrcholu zásobníku a pořadí argumentu se kterým se pracuje
	const char *func_type = symtable_stack_top_type(global->stack);
	int arg = symtable_stack_top_arg(global->stack);

	if (func_type[arg] != '\0') {
		const char *func_name = symtable_stack_top_name(global->stack);
		error(ROWONLY, "semanticka analyza: Pocet parametru funkce %s: %d, ocekavany pocet: %d!\n", func_name, arg-1, strlen(func_type)-1);
		return NULL;
	}

	// Předání mikroinstrukce startu funkce
	THtableItemData *func= htable_search(global->table, symtable_stack_top_name(global->stack));
	assert(func != NULL);

	symtable_stack_pop(&(global->stack)); // Odstranění funkce ze zásobníku

	return func->data.function.instruction;
}

/*
 * Vrací ukazatel na mikroinstrukci návěstí.
 */
TMicroInstruction * symtable_get_label_addr(TSymtable *local, const char *name) {
	assert(local != NULL);
	assert(name != NULL);

	THtableItemData *label = htable_search(local->table, name);

	assert(label->type == SYM_LABEL);
	return label->data.label.instruction;
}

/*
 * Vrací ukazatel na mikroinstrukci začátku funkce.
 */
TMicroInstruction * symtable_get_func_addr(TSymtable *global, const char *name) {
	assert(global != NULL);
	assert(name != NULL);

	THtableItemData *func = htable_search(global->table, name);

	if (func == NULL)
		return NULL;

	assert(func->type == SYM_FUNCTION);
	return func->data.function.instruction;
}

/*
 * Vloží do kódu mikroinstrukcí počáteční instrukci.
 */
TErrCode symtable_end_src(TSymtable *global) {
	assert(global != NULL);

	TMicroInstruction *imain = symtable_find_main(global);
	if (imain == NULL) {
		error(RAW, "Chybi definice funkce main()!\n");
		return ERR_FAILURE;
	}

	return generate_start_point(imain, global->var_count);
}


//*********************** Pomocné funkce ***********************//

/*
 * Pomocná funkce, která uvolňuje paměť, která již nebude potřeba,
 * mallocovanou symbol_table.c v hashovací tabulce. Za touto funkcí
 * by měla vždy následovat htable_clear(), nebo htable_free()
 */
static TErrCode free_memory_malloced_in_htable(THtable *t)
{
	assert (t != NULL);

	THtableItem *tmp;
	TErrCode err = ERR_SUCCESS; // V případě neúspěchu se nastaví na chybu

	for (unsigned int i = 0; i < t->size; i++) {
		// Postupně projdeme celou hashovací tabulku
		tmp = t->data[i];
		while (tmp != NULL) {
			// Dealokujeme pameť v položce
			if (tmp->data.type == SYM_FUNCTION) {
				free(tmp->data.data.function.type);
				if (tmp->data.data.function.declared == false) {
					// Dealokujeme jen pokud není součástí nudle
					free(tmp->data.data.function.instruction);
					error(RAW, "semanticka analyza: Funkce %s nebyla definovana!\n", tmp->key);
					err = ERR_FAILURE;
				}
			}

			if ((tmp->data.type == SYM_LABEL) && (tmp->data.data.label.declared == false)) {
				// Dealokujeme start návěstí, jen pokud není součástí nudle
				free(tmp->data.data.label.instruction);
				error(RAW, "semanticka analyza: Navesti %s nebylo deklarovano!\n", tmp->key);
				err = ERR_FAILURE;
			}

			tmp = tmp->next;
		}
	}

	return err;
}

/*
 * Pomocná funkce, která se volá při vkládání proměnné i parametru
 * funkce
 */
static TErrCode insert_var_to_htable(TSymtable *local, const char *name, TDataType type)
{
	assert(local != NULL);
	assert(name != NULL);

	// Obsah vkládané proměnné
	THtableItemData data = {.type = SYM_VARIABLE, .data = {.var = {.type = type, .index = local->first_free_index}}};

	if (htable_search(local->table, name) != NULL) {
		// Název symbolu je již používán
		error(ROWONLY, "semanticka analyza: Identifikator %s byl jiz na stejne urovni pouzit!\n", name);
		return ERR_FAILURE;
	}

	THtableItemData *symbol = htable_lookup(local->table, name, data);
	if (symbol == NULL) {
		// Došla paměť
		error(RAW, "semanticka analyza: Nedostatek pameti!\n");
		return ERR_FAILURE;
	}

	local->first_free_index++; // Vložili jsme novou proměnnou, která bude zabírat místo na zásobníku

	return ERR_SUCCESS;
}

/*
 * Funkce ukládá typ návratové hodnoty funkce a její parametry, stará se
 * o dynamické zvětšování přidělené paměti. Vrací chybu pokud dojde paměť.
 */
static TErrCode insert_func_type(TSymtable *t, TDataType type) {
	assert(t != NULL);
	assert(t->act != NULL);

	// Kontrola, zda je ještě alokováno dostatek prostoru
	if (((t->argument_count + 1) % SPACE_FOR_FUNC_TYPE) == 0) {
		// Je potřeba zvětšit místo
		char *tmp = realloc(t->act->data.function.type, t->argument_count + SPACE_FOR_FUNC_TYPE + 1);
		if (tmp == NULL) {
			// realloc() se nepovedl
			error(RAW, "semanticka analyza: Nedostatek pameti!\n");
			return ERR_FAILURE;
		}

		t->act->data.function.type = tmp;
	}

	t->act->data.function.type[t->argument_count] = (char) type;
	t->act->data.function.type[t->argument_count + 1] = '\0';

	return ERR_SUCCESS;
}

/*
 * Funkce kontroluje typ návratové hodnoty a její parametry, pokud nesouhlasí
 * vypíše důvod chyby a vrátí chybovou hodnotu.
 */
static TErrCode check_func_type(TSymtable *t, TDataType type) {
	assert(t != NULL);
	assert(t->act != NULL);

	if (t->act->data.function.type[t->argument_count] == (char) type)
		return ERR_SUCCESS;

	// Zjistíme co nesouhlasí
	if (t->argument_count == 0)
		error(ROWONLY, "semanticka analyza: Nesouhlasi navratovy typ funkce s deklaraci nebo definici!\n");
	else if (t->act->data.function.type[t->argument_count] == '\0')
		error(ROWONLY, "semanticka analyza: Pocet parametru funkce pri deklaraci nebo definici: %d!\n", t->argument_count - 1);
	else
		error(ROWONLY, "semanticka analyza: %d. parametr funkce je nespravneho typu!\n", t->argument_count);

	return ERR_FAILURE;
}

/*
 * Funkce zjišťuje, zda jsme nepoužili menší počet parametrů funkce než
 * posledně. Pokud ano, vypíše chybu a vrátí chybu.
 */
static TErrCode check_end_of_arg(TSymtable *t) {
	assert(t != NULL);
	assert(t->act != NULL);

	if (t->act->data.function.type[t->argument_count + 1] == '\0')
		return ERR_SUCCESS;

	error(ROWONLY, "semanticka analyza: Pocet parametru funkce: %d, ocekavany pocet: %d!\n", t->argument_count, strlen(t->act->data.function.type)-1);
	return ERR_FAILURE;
}

static TErrCode symtable_insert_main(TSymtable *global) {
	assert(global != NULL);

	// Obsah položky identifikující funkci main
	THtableItemData new = {.type = SYM_FUNCTION, .data = {.function = {.type = NULL, .instruction = NULL, .argument_count = 0, .var_count = 0, .declared = false, .builtin = false}}};
	new.data.function.instruction = generate_new_func();
	if (new.data.function.instruction == NULL) {
		return ERR_FAILURE;
	}

	// Natavíme správný návratový typ a parametry
	new.data.function.type = malloc(2 * sizeof(char));
	if (new.data.function.type == NULL) {
		error(RAW, "semanticka analyza: Nedostatek pameti!\n");
		return ERR_FAILURE;
	}
	strcpy(new.data.function.type, "v"); // main() je typu void

	if (htable_lookup(global->table, "main", new) == NULL) {
		error(RAW, "semanticka analyza: Nedostatek pameti!\n");
		return ERR_FAILURE;
	}

	return ERR_SUCCESS;
}

/* Konec souboru symbol_table.c */
