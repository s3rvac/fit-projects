/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		parser.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Syntakticky a semanticky analyzator - je rozdelen na 2 casti
			a to prediktivni SA, ktera prochazi deklarace/definice funkci
			a paramteru a precedencni SA, ktera prochazi vyrazy.
			Implementace.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "parser_define.h"
#include "parser_semantic.h"
#include "predictive_sa.h"
#include "functions.h"
#include "error.h"
#include "constants.h"
#include "token.h"
#include "token_debug.h"
#include "symbol_table.h"
#include "scanner.h"

/*
 * Vypisuje chybove hlaseni (podle predaneho tokenu)
 */
static void print_parse_error(Ttoken tok)
{
	switch (tok.type) {
		case TOK_CONSTINT:
			error(ROWONLY, "Syntakticka analyza: Posledni token: %d\n", tok.attribute.i);
			break;
		case TOK_CONSTDOUBLE:
			error(ROWONLY, "Syntakticka analyza: Posledni token: %f\n", tok.attribute.d);
			break;
		default:
 			error(ROWONLY, "Syntakticka analyza: Posledni token: %s\n", tok2str(tok));
			break;
	}
}

/*
 * Syntakticka a semanticka analyza zdrojoveho souboru.
 * Pokud je nalezena chyba, vraci ERR_FAILURE a je vypsana chybova hlaska,
 * jinak ERR_SUCCESS. Postupne bude generovat instrukce (ty se budou ukladat do
 * modulu code_list a pak vykonavat, neni tudiz treba predavat parametr na tuto strukturu,
 * protoze bude zapouzdrena v danem modulu).
 */
TErrCode parser(FILE *input_file)
{
	TErrCode rc; // Navratovy kod

	// Vytvoreni + inicializace globalni a lokalni TS
	TSymtable *local_st = symtable_init();
	TSymtable *global_st = symtable_init();

	// Vlozeni deklarace funkce void main(); a deklaraci vestavenych funkci
	// Funkci main() musi obsahovat kazdy program a musi se shodovat s vyse uvedenou deklaraci
	if (symtable_insert_initial_data(global_st) != ERR_SUCCESS) {
		// Chyba
		symtable_free(global_st);
		symtable_free(local_st);
		return ERR_FAILURE;
	}

	// Pocatecni nonterminal musi byt "zacatek programu" a koncovy token EOF
	TNonTerm starting_nonterm = NT_PROG;
	Ttoken end_token = {.type = TOK_EOF};

	// Nastaveni faze - zatim neurcita
	TPhases phase = PHASE_UNKNOWN;

	// Ziskej prvni token
	Ttoken tok;
	rc = gnt_iist(input_file, &phase, global_st, local_st, &tok);
	if (rc != ERR_SUCCESS) {
		// Chyba
		print_parse_error(tok);
		symtable_free(global_st);
		symtable_free(local_st);
		return ERR_FAILURE;
	}

	// Vola se prediktivni SA
	// V tok bude ulozen posledni nacteny token
	rc = predictive_SA(input_file, &phase, global_st, local_st,
		tok, end_token, &tok, starting_nonterm, NULL, NULL);
	if (rc != ERR_SUCCESS) {
		// Chyba
		print_parse_error(tok);
		symtable_free(global_st);
		symtable_free(local_st);
		return ERR_FAILURE;
	}

	// Ukonceni analyzy zdrojoveho kodu a pridani potrebnych
	// informaci do posloupnosti instrukci
	if (symtable_end_src(global_st) != ERR_SUCCESS) {
		// Chyba
		print_parse_error(tok);
		symtable_free(global_st);
		symtable_free(local_st);
		return ERR_FAILURE;
	}

	// Uvolneni pameti pro obe 2 tabulky
	if (symtable_free(global_st) != ERR_SUCCESS) {
		symtable_free(local_st);
		return ERR_FAILURE;
	}
	if (symtable_free(local_st) != ERR_SUCCESS) {
		return ERR_FAILURE;
	}

    return ERR_SUCCESS;
}

/* Konec souboru parser.c */
