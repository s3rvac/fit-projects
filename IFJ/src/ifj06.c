/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		if06.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Interpret imperativniho jazyka IFJ06
			Program ocekava jako 1. parametr nazev zdrojoveho souboru
			nebo '-h','--help' pro zobrazeni napovedy
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include "debug.h"
#include "code_list.h"
#include "error.h"
#include "parser.h"
#include "interpret.h"
#include "strptr_stack.h"

/*
 * Nutny pocet parametru programu ke spusteni
 * (nazev souboru nebo pozadavek o zobrazeni napovedy)
 */
#define REQ_PARAM_COUNT 2

/*
 * Minimalni delka parametru obsahujici volbu k programu
 */
#define MIN_OPTIONS_PARAM_LENGTH 2

/*
 * Vraci true, jestlize je predany retezec
 * volbou k programu (zacina bud na '-' nebo '--')
 */
static inline bool is_options_param(const char *param)
{
	return (param != NULL &&
		(strlen(param) >= MIN_OPTIONS_PARAM_LENGTH) &&
		(param[0] == '-'));
}

/*
 * Vraci true, jestlize je predany retezec
 * pozadavek na zobrazeni napovedy
 */
static inline bool is_help_param(const char *param)
{
	return (param != NULL &&
		((strcmp("-h", param) == 0) || (strcmp("--help", param) == 0)));
}

/*
 * Vypise napovedu k programu (na standardni vystup)
 */
static void print_help(void)
{
	printf("Interpret programovaciho jazyka IFJ06 (2006)\n"
		"Autori:\n"
 		"    Lukas Marsik, xmarsi03@stud.fit.vutbr.cz\n"
		"    Libor Polcak, xpolca03@stud.fit.vutbr.cz\n"
		"    Boris Prochazka, xproch63@stud.fit.vutbr.cz\n"
		"    Martin Tomec, xtomec05@stud.fit.vutbr.cz\n"
		"    Petr Zemek, xzemek02@stud.fit.vutbr.cz\n"
		"\n"
		"Pouziti: ifj06 [volby] zdrojovy_soubor\n"
		"Volby:\n"
		"  -h --help  Zobrazi tuto napovedu k programu\n");
}

/*
 * Main
 */
int main(int argc, char *argv[])
{
	// Kontrola parametru
	if (argc != REQ_PARAM_COUNT) {
		// Nebyl zadan vstupni soubor
		error(RAW, "Nebyl zadan vstupni soubor nebo zadano vice nez %d parametry\n"
			"Pro zobrazeni napovedy spuste program s parametrem -h\n", REQ_PARAM_COUNT);
		return ERR_FAILURE;
	}
	else if (is_options_param(argv[1])) {
		// Kontrola volby
		if (is_help_param(argv[1])) {
			// Pozadavek na zobrazeni napovedy
			print_help();
			return ERR_SUCCESS;
		}
		else {
			error(RAW, "Neplatna volba: %s\n"
				"Pro zobrazeni napovedy spuste program s parametrem -h\n", argv[1]);
			return ERR_FAILURE;
		}
	}

	// Prvnim parametrem by mel byt nazev vstupniho souboru
	// -> pokus o otevreni
	FILE *input_file = fopen(argv[1], "r");
	if (!input_file) {
		error(RAW, "Nepodarilo se otevrit vstupni soubor: %s\n", argv[1]);
		return ERR_FAILURE;
	}

	// Ulozeni funkci strptr_stack_free(); a code_list_clean();
	// do atexit() posloupnosti, aby se na ni nezapomelo
 	atexit(strptr_stack_free);
 	atexit(code_list_clean);

	// Inicializace modulu pro ukladani posloupnsti instrukci
	if (code_list_init() != ERR_SUCCESS) {
		// Nedostatek pameti
		fclose(input_file);
		return ERR_FAILURE;
	}

	/*
 	* Syntakticka a semanticka analyza zdrojoveho souboru.
 	* Pokud je nalezena chyba, vraci ERR_FAILURE a je vypsana chybova hlaska,
 	* jinak ERR_SUCCESS. Postupne bude generovat instrukce (ty se budou ukladat do
 	* modulu code_list a pak vykonavat, neni tudiz treba predavat parametr na tuto strukturu,
 	* protoze bude zapouzdrena v danem modulu).
 	*/
	if (parser(input_file) != ERR_SUCCESS) {
		// Chyba
		fclose(input_file);
		return ERR_FAILURE;
	}

	/*
	 * Interpretace instrukci
	 */
	if (interpret() != ERR_SUCCESS) {
		// Chyba
		fclose(input_file);
		return ERR_FAILURE;
	}

	fclose(input_file);

    return ERR_SUCCESS;
}

/* Konec souboru if06.c */
