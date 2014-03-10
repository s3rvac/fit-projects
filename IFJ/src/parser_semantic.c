/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		parser_semantic.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Wrapper pro get_next_token() a dalsi funkce
			potrebne pri semanticke analyze - implementace.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "parser_define.h"
#include "parser_semantic.h"
#include "constants.h"
#include "generator.h"
#include "debug.h"
#include "data_type.h"
#include "scanner.h"
#include "token.h"

/*
 * Staticke globalni promenne
 */
/* Nazev posledni volane funkce (napr. po nacteni f( ulozi "f") */
static const char *last_called_func_name;
/* Aktualni funkce, jejiz deklaraci/definici provadime (nebo jsme zrovna provedli) */
static const char *current_func_name;
/* Posledni nacteny token */
static Ttoken last_token;

/*
 * Zjisti nazev funkce, jejiz deklaraci/definici provadime (nebo jsme zrovna provedli)
 */
inline const char *get_current_func_name(void)
{
	return current_func_name;
}

/*
 * Zjisti nazev posledne volane funkce (v tele jine funkce)
 */
inline const char *get_last_called_func_name(void)
{
	return last_called_func_name;
}

/*
 * Vrati posledni nacteny token
 */
inline Ttoken get_last_token(void)
{
	return last_token;
}

/*
 * Transformuje index na index globalni promenne
 * (viz constants.h)
 */
static inline int local_index2global_index(int index)
{
	return (-(index + GLOBAL_CONST_SHIFT));
}

/*
 * Zjisti index a datovy typ promenne a vrati ERR_SUCCESS, pokud byla nalezena
 * (a vysledek ulozi do index a data_type) nebo v pripade neuspechu vrati ERR_FAILURE.
 */
TErrCode get_var_data(Ttoken tok, int *index, TDataType *data_type, TSymtable *global_st, TSymtable *local_st)
{
	TErrCode rc = ERR_SUCCESS; // Navratovy kod

	if (tok.type != TOK_IDENTIFIER) {
		// Nejedna se o identifikator
		return ERR_FAILURE;
	}

	if (get_variable(local_st, index, data_type, tok.attribute.ptr) != ERR_SUCCESS) {
		if (get_variable(global_st, index, data_type, tok.attribute.ptr) != ERR_SUCCESS) {
 			error(ROWONLY, "Semanticka analyza: Promenna %s nebyla deklarovana.\n", tok.attribute.ptr);
			rc = ERR_FAILURE;
		}
		else {
			// Jedna se o globalni promennou, musim transformovat index
			*index = local_index2global_index(*index);
		}
	}

	return rc;
}

/*
 * Prevede datovy typ v tokenu na datovy typ z TDataType
 */
TDataType tok2data_type(Ttoken tok)
{
	switch (tok.type) {
		case TOK_INT:
			return TYPE_INT;
			break;
		case TOK_DOUBLE:
			return TYPE_DOUBLE;
			break;
		case TOK_STRING:
			return TYPE_STRING;
			break;
		default:
			break;
	}

	return TYPE_VOID;
}

/*
 * Vraci true, jestli je predany token datovy typ (mimo void)
 */
static inline bool is_var_data_type(Ttoken tok)
{
	return (tok.type == TOK_INT || tok.type == TOK_DOUBLE || tok.type == TOK_STRING);
}

/*
 * Vraci true, jestli je predany token navratovy datovy typ
 */
static inline bool is_return_data_type(Ttoken tok)
{
	return (tok.type == TOK_VOID || is_var_data_type(tok));
}

/*
 * Vraci true, pokud token obsahuje datovy typ (void, int, double, string)
 */
static inline bool is_data_type(Ttoken tok)
{
	return is_return_data_type(tok);
}

/*
 * Wrapper pro get_next_token() - vklada symboly do TS a kontroluje deklarace/definice
 */
int gnt_iist(FILE *stream, TPhases *phase, TSymtable *global_st, TSymtable *local_st, Ttoken *tok)
{
	/* Posledni nacteny datovy typ */
	static TDataType last_data_type;
	/* Indikuje, zda minuly token byl identifikator (a nebyl "pouzit") */
	static bool prev_token_was_id = false;

	TErrCode rc = ERR_SUCCESS; // Aktualni navratovy kod

	// Ziska token od lexikalni analyzy
	*tok = get_next_token(stream);

	if (tok->type == TOK_SCANNERERROR) {
		// Chyba ve scanneru
		rc = ERR_FAILURE;
	}
	else if (*phase == PHASE_UNKNOWN && tok->type == TOK_VAR) {
		// Zmena faze
		*phase = PHASE_GLOBAL_VAR;
	}
	else if (*phase != PHASE_BODY) {
		// Deklaracni a definicni cast
		if (tok->type == TOK_IDENTIFIER) {
			switch (*phase) {
				case PHASE_DECLAR:
					// Vlozit parametr do lokalni tabulky (defice)
					if (last_token.type == TOK_VOID) {
						// Toto by mel pohlidat parser - zde jen z duvodu bezpecnosti
						error(ROWONLY, "Semanticka analyza: Parametr %s nemuze mit datovy typ void.\n",
							tok->attribute.ptr);
						rc = ERR_FAILURE;
					}
					else if (!is_var_data_type(last_token)) {
						error(ROWONLY, "Semanticka analyza: Parametr %s nema urceny datovy typ.\n",
							tok->attribute.ptr);
						rc = ERR_FAILURE;
					}
					else {
						// Uloz informace do TS
 						rc = symtable_insert_arg(local_st, tok->attribute.ptr, last_data_type);
 						prev_token_was_id = false;
 					}
					break;

				case PHASE_GLOBAL_VAR:
					// Vlozit globalni promennou
					if (last_token.type == TOK_VOID) {
						// Toto by mel pohlidat parser - zde jen z duvodu bezpecnosti
						error(ROWONLY, "Semanticka analyza: Globalni promenna %s nemuze mit datovy typ void.\n",
							tok->attribute.ptr);
						rc = ERR_FAILURE;
					}
					else if (!is_var_data_type(last_token)) {
						error(ROWONLY, "Semanticka analyza: Globalni promenna %s nema urceny datovy typ.\n",
							tok->attribute.ptr);
						rc = ERR_FAILURE;
					}
					else {
						// Uloz informace do TS
						rc = symtable_insert_var(global_st, tok->attribute.ptr, last_data_type);
						prev_token_was_id = false;
					}
					break;

				case PHASE_LOCAL_VAR:
					// Vlozit lokalni promennou (definice)
					if (last_token.type == TOK_VOID) {
						// Toto by mel pohlidat parser - zde jen z duvodu bezpecnosti
						error(ROWONLY, "Semanticka analyza: Lokalni promenna %s nemuze mit datovy typ void.\n",
							tok->attribute.ptr);
						rc = ERR_FAILURE;
					}
					else if (!is_var_data_type(last_token)) {
						error(ROWONLY, "Semanticka analyza: Lokalni promenna %s nema urceny datovy typ.\n",
							tok->attribute.ptr);
						rc = ERR_FAILURE;
					}
					else {
						// Uloz informace do TS
						rc = symtable_insert_var(local_st, tok->attribute.ptr, last_data_type);
						prev_token_was_id = false;
					}
					break;

				default: // PHASE_UNKNOWN - faze zatim nebyla urcena - bude se jednat o volani funkce
					*phase = PHASE_DECLAR;
					prev_token_was_id = true;
					break;
			}
		}
		else if (is_data_type(*tok)) {
			// Datovy typ
			last_data_type = tok2data_type(*tok);
		}
		else if (*phase == PHASE_DECLAR) {
			if (tok->type == TOK_LBRACKET && prev_token_was_id) {
				// Jedna se o deklaraci/definici funkce (zatim nevime)
				prev_token_was_id = false;

				rc = symtable_start_insert_func(global_st, local_st, last_token.attribute.ptr, last_data_type, false);

				// Ulozim si aktualni nazev
				current_func_name = last_token.attribute.ptr;
			}
			else if (tok->type == TOK_SEMICOLON) {
				if (last_token.type == TOK_RBRACKET) {
					// Konec deklarace funkce
					rc = symtable_stop_insert_func(global_st, local_st);

					// Konec deklarace - nutne nastavit spravne fazi
					*phase = PHASE_UNKNOWN;
				}
				else if (is_data_type(last_token)) {
					// Parametr funkce pri deklaraci (jen typ)
					rc = symtable_insert_arg_type(global_st, last_data_type);
				}
			}
			else if (tok->type == TOK_RBRACKET && is_data_type(last_token)) {
				// Posledni parametr pri deklaraci pred ')'
				rc = symtable_insert_arg_type(global_st, last_data_type);
			}
			else if ( tok->type == TOK_VAR ||tok->type == TOK_LVINCULUM) {
					// Zacatek tela funkce - kontrola, jestli je mozne funkci definovat
					// (tzn. nebyla dosud definovana)
					rc = symtable_func_declar(local_st);
			}
		}

	}
	else {
		// Telo funkce
		if (prev_token_was_id) {
			// Predchazejici token byl identifikator -> podle aktualniho tokenu zjistim,
			// "co mam kontrolovat"
			if (tok->type == TOK_LABEL) {
				// Navesti
				prev_token_was_id = false;
				// Vlozime do TS
				rc = symtable_insert_label(local_st, last_token.attribute.ptr);
				if (rc != ERR_SUCCESS) {
					last_token = *tok;
					return ERR_FAILURE;
				}

				// Vlozeni navesti do posloupnosti instrukci
				generator_ins_inst(symtable_get_label_addr(local_st, last_token.attribute.ptr));
			}
			else if (tok->type == TOK_LBRACKET) {
				// Volani funkce
				prev_token_was_id = false;
				// Zkontrolovat, zda nebyla funkce prekryta lokalni promennou
				if (get_variable(local_st, NULL, NULL, last_token.attribute.ptr) == ERR_SUCCESS) {
					error(ROWONLY, "Semanticka analyza: Lokalni promenna %s zastinila funkci %s - neni mozne pouzit volani teto funkce.\n",
						last_token.attribute.ptr, last_token.attribute.ptr);
					rc = ERR_FAILURE;
				}
				// Zkontrolovat, zda funkce byla deklarovana
				if (!symtable_func_exists(global_st, last_token.attribute.ptr)) {
					error(ROWONLY, "Semanticka analyza: Funkce %s nebyla deklarovana.\n",
						last_token.attribute.ptr);
					rc = ERR_FAILURE;
				}

				// Ulozit nazev volane funkce do globalni staticke promenne,
				// aby ji bylo mozno zjistit pri volani precedencni SA
				last_called_func_name = last_token.attribute.ptr;
			}
			else if (tok->type == TOK_ASSIGNMENT) {
				// Prirazeni - zkontrolovat, jestli se jedna o promennou a ne o funkci
				// (pripadne jestli neexistuje lokalni promenna, ktera by zastinila funkci)
				if (get_variable(local_st, NULL, NULL, last_token.attribute.ptr) != ERR_SUCCESS) {
					if (get_variable(global_st, NULL, NULL, last_token.attribute.ptr) != ERR_SUCCESS) {
						if (symtable_func_exists(global_st, last_token.attribute.ptr)) {
							error(ROWONLY, "Semanticka analyza: Funkci (%s) nelze pouzit na leve strane prirazovaciho prikazu.\n",
								last_token.attribute.ptr);
							rc = ERR_FAILURE;
						}
						else {
							error(ROWONLY, "Semanticka analyza: Promenna %s nebyla deklarovana.\n",
								last_token.attribute.ptr);
							rc = ERR_FAILURE;
						}
					}
				}
				// Existuje jako promenna
				prev_token_was_id = false;
			}
			else {
				// Promenna
				prev_token_was_id = false;
				// Zkontrolovat, zda promenna byla deklarovana
				if (!symtable_variable_exists(global_st, local_st, last_token.attribute.ptr)) {
					error(ROWONLY, "Semanticka analyza: Promenna %s nebyla deklarovana.\n",
						last_token.attribute.ptr);
					rc = ERR_FAILURE;
				}
			}
		}
		else if (tok->type == TOK_IDENTIFIER) {
			if (last_token.type == TOK_GOTO) {
				// Goto
				prev_token_was_id = false;
				// Vlozeni navesti a kontrola
				rc = symtable_insert_label_name(local_st, tok->attribute.ptr);

				// Instrukce se vklada v predictive_sa.c
			}
			else {
				// O jaky typ identifikatoru se jedna budeme vedet az podle pristiho tokenu
				prev_token_was_id = true;
			}
		}
		else if (tok->type == TOK_RVINCULUM) {
			// Konec definice funkce
			rc = symtable_stop_insert_func(global_st, local_st);
			// Zmena faze
			*phase = PHASE_UNKNOWN;

			// Pokud ma funkce navratovy typ VOID, pak } znamena return;
			TDataType ret_type;
			symtable_get_func_rettype(global_st, get_current_func_name(), &ret_type);
			if (ret_type == TYPE_VOID) {
				// Navratova hodnota se nebude vyzvedavat, takze je jedno co tam je
				if (generate_return(RETURN_VALUE_INDEX) != ERR_SUCCESS) {
					last_token = *tok;
					return ERR_FAILURE;
				}
			}
			else {
				// Nagenerovani ERROR funkce, pro pripad, ze by se zde dostal interpret
				if (generate_error() != ERR_SUCCESS) {
					last_token = *tok;
					return ERR_FAILURE;
				}
			}
		}
		else if (tok->type == TOK_SEMICOLON && last_token.type == TOK_RETURN) {
				// return; -> musi se resit zde
				// Zkontrolovat, jestli je navratovy typ funkce void
				TDataType ret_type;
				if ((symtable_get_func_rettype(global_st, get_current_func_name(), &ret_type) != ERR_SUCCESS) ||
					ret_type != TYPE_VOID) {
					// Navratovy typ neni void nebo funkce neexistuje - chyba
					error(ROWONLY, "Semanticka chyba: Funkce %s nema navratovy typ void, nelze volat return; "
						"bez navratove hodnoty.\n",
						get_current_func_name());
					last_token = *tok;
					return ERR_FAILURE;
				}

				// Navratova hodnota se nebude vyzvedavat, takze je jedno co tam je
				if (generate_return(RETURN_VALUE_INDEX) != ERR_SUCCESS) {
					last_token = *tok;
					return ERR_FAILURE;
				}
		}
	}

	// Nastav stav (pokud nastala zmena)
	if (tok->type == TOK_LVINCULUM) {
		*phase = PHASE_BODY;
	}
	else if (tok->type == TOK_SEMICOLON && *phase == PHASE_GLOBAL_VAR) {
		// Prosli jsme pres globalni var
		*phase = PHASE_UNKNOWN;
	}
	else if (last_token.type == TOK_RBRACKET && tok->type == TOK_VAR) {
		*phase = PHASE_LOCAL_VAR;
	}

	last_token = *tok;
	return rc;
}

/* Konec souboru parser_semantic.c */
