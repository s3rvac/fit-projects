/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		symbol_table.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Tabulka symbolů - rozhraní
******************************************************************************/

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>
#include "hashtable.h"
#include "error.h"
#include "data_type.h"
#include "symtable_stack.h"

/*
 * Vlastní tabulka symbolů
 */
typedef struct {
	THtable *table;         // Hashovací tabulka se symbolama
	int argument_count;     // Počet parametrů dané funkce u lokální tabulky, u globální vždy 0
	int var_count;          // Počet lokálních, případně globálních proměnných
	int first_free_index;   // První volný index pro proměnnou
	THtableItemData *act;   // Ukazatel na položku v tabulce, nebo i odpovídající globální tabulce, se kterou se právě pracuje
	bool func_insert;       // Obsahuje informaci, zda pravě vkládáme funkci, nebo jen kontrolujeme podle předešlé deklarace/definice
	bool definition;        // Obsahuje informaci, zda jsme se dostali do definice, nebo jsme zustali pouze u deklarace
	TSymtableStack *stack;  // Zásobník, který je potřeba při kontrole argumentů při volání funkcí
} TSymtable;

/*
 * Vytvoří a inicializuje tabulku symbolů obsahující hashovací tabulku
 * velikosti definované v souboru constants.h. Pokud se tabulku nepodaří
 * vytvořit vrací NULL.
 */
TSymtable * symtable_init();

/*
 * Zruší všechny data v tabulce a uvolní paměť
 */
TErrCode symtable_free(TSymtable *t);

/*
 * Zruší všechna data v tabulce a reinicializuje ji, je možné s ní
 * nadále pracovat, jako by byla právě vytvořena
 */
TErrCode symtable_clear(TSymtable *t);

/*
 * Zjistí, zda je tabulka symbolů prázdná. Tzn. jestli je ve stavu v jakém je
 * po své inicializaci
 */
bool symtable_empty(TSymtable *t);

/*
 * Vloží novou proměnnou do tabulky
 */
TErrCode symtable_insert_var(TSymtable *local, const char *name, TDataType type);

/*
 * Vloží nový parametr do tabulky. Volá se při definici funkce.
 */
TErrCode symtable_insert_arg(TSymtable *local, const char *name, TDataType type);

/*
 * Vloží nový parametr do tabulky. Volá se při deklaraci funkce.
 */
TErrCode symtable_insert_arg_type(TSymtable *global, TDataType type);

/*
 * Uloží informaci o použití dočasné proměnné pro uložení hodnoty
 * mezivýrazu a vrátí její index
 */
int symtable_insert_tmp(TSymtable *local);

/*
 * Začátek vkládání funkce, případně její sémantické kontroly. Pokud
 * půjde o deklaraci funkce, lokalní tabulka se po zavolání
 * symtable_stop_insert_func() do stejného stavu, v jakém byla před vykonáním
 * této funkce. V případě neúspěchu vrací ERR_FAILURE
 */
TErrCode symtable_start_insert_func(TSymtable *global, TSymtable *local, const char *name, TDataType ret, bool builtin);

/*
 * Narazili jsme na definici funkce. V případě, že funkci již není možné
 * definovat vrací chybu.
 */
TErrCode symtable_func_declar(TSymtable *local);

/*
 * Konec vkládání funkce. V ukazateli instruction funkce předá adresu
 * mikroinstrukce se začátkem funkce. V případě chyby vrací ERR_FAILURE.
 */
TErrCode symtable_stop_insert_func(TSymtable *global, TSymtable *local);

/*
 * Vloží nové návěstí do tabulky symbolů, toto návěstí však není prozatím
 * definované.
 */
TErrCode symtable_insert_label_name(TSymtable *local, const char *name);

/*
 * Vloží nové návěstí do tabulky symbolů. Používá se v případě jeho definice.
 */
TErrCode symtable_insert_label(TSymtable *local, const char *name);

/*
 * V tabulce t najde proměnnou var_name. Do index uloží index, na kterém se
 * proměnná nachází, do type její typ. Pokud nás index nebo type nezajímá,
 * očekává NULL pointer. Pokud identifikátor neexistuje, nebo se
 * nejedná o proměnnou vrací chybu.
 */
TErrCode get_variable(TSymtable *t, int *index, TDataType *type, const char *var_name);

/*
 * V tabulce t najde funkci name. Do data_type uloží její návratový typ.
 * Pokud identifikátor neexistuje, nebo se nejedná o funkci vrací chybu.
 */
TErrCode symtable_get_func_rettype(TSymtable *t, const char *name, TDataType *data_type);

/*
 * Zjistí, zda existuje funkce se zadaným jménem. Vrací true pokud ano, jinak false.
 */
bool symtable_func_exists(TSymtable *global, const char *name);

/*
 * Zjistí zda existuje symbol s předaným identifikátorem. Vrací true pokud ano,
 * jinak false
 */
bool symtable_id_exists(TSymtable *t, const char *name);

/*
 * Zjistí, zda existuje proměnná daného jména v globální nebo lokální tabulce.
 * Vrací true pokud ano, jinak false.
 */
bool symtable_variable_exists(TSymtable *global, TSymtable *local, const char *name);

/*
 * Zjistí, zda je v tabulce definovaná funkce main. Pokud ano vrátí na ukazatel
 * na odpovídající mikroinstrukci. Jinak NULL.
 */
TMicroInstruction *symtable_find_main(TSymtable *global);

/*
 * Funkce vloží do globální tabulky počáteční data - vestavěné funkce a
 * prototyp funkce main(). V případě chyby vrací chybový kód.
 */
TErrCode symtable_insert_initial_data(TSymtable *global);

/*
 * Připraví se na začátek volání funkce, vyhledájí v hashovací tabulce a uloží
 * její typ na pomocný zásobník. V případě chyby vypíše důvod a vrátí chybový
 * kód.
 */
TErrCode symtable_start_call_func(TSymtable *global, const char *name);

/*
 * Zjistí datový typ dalšího argumentu právě zpracovávané funkce a porovná jej
 * na adresu předanou pointerem arg_type. V případě chyby ji vypíše a vrátí
 * chybový kód.
 */
TErrCode symtable_call_arg(TSymtable *global, TDataType arg_type);

/*
 * Zkontroluje zda nebyla funkce zavolána s menším počtem parametrů a odebere
 * právě volanou funkci ze zásobníku a vrátí ukazatel na mikroinstrukci s jejim
 * začátkem. V případě chyby vypíše chybovou zprávu a vrátí NULL.
 */
TMicroInstruction * symtable_stop_call_func(TSymtable *global);

/*
 * Vrací ukazatel na mikroinstrukci návěstí.
 */
TMicroInstruction * symtable_get_label_addr(TSymtable *local, const char *name);

/*
 * Vrací ukazatel na mikroinstrukci začátku funkce.
 */
TMicroInstruction * symtable_get_func_addr(TSymtable *global, const char *name);

/*
 * Vloží do kódu mikroinstrukcí počáteční instrukci.
 */
TErrCode symtable_end_src(TSymtable *global);

#endif /* #ifndef SYMBOL_TABLE_H */

/* Konec souboru symbol_table.h */
