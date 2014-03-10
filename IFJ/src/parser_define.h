/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		parser_define.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Definice typu potrebnych v syntakticke analyze
******************************************************************************/

#ifndef _PARSER_DEFINE_H_
#define _PARSER_DEFINE_H_

#include "data_type.h"
#include "token.h"

/* Non-terminaly */
typedef enum {
	ERR = -1, // Signalizuje chybu
	// Prediktivni SA
	// Deklarace a definice
	NT_PROG,
	NT_VAR_DECLAR,
	NT_VAR_ITEM,
	NT_VAR_LIST,
	NT_FUNCTIONS,
	NT_FUNC_UNIT,
	NT_FUNC_UNIT2,
	NT_FUNC_NON_PARAMS,
	NT_FUNC_PARAMS,
	NT_FUNC_PARAM_DECLAR_LIST,
	NT_FUNC_PARAM_DEF_LIST,
	NT_TYPE_SPECIFIER,
	NT_RETURN_TYPE_SPECIFIER,
	// Telo funkci
	NT_STATEMENT_LIST,
	NT_STATEMENT,
	NT_ST_NOT_BEGIN_ID,
	NT_ST_BEGIN_ID,
	NT_CONDITION_STATEMENT,
	NT_JUMP_STATEMENT,
	NT_RETURN_VALUE,
	NT_ASSIGNMENT,
	NT_FUNC_CALL,
	NT_FUNC_ARG_LIST,
	NT_EXP_IN_FUNC,
	NT_EXPRESSION,
	// Precedencni SA
	E = 0
} TNonTerm;

/*
 * Mozne akce v precedencni tabulce
 */
enum prec_actions {
	NA = -1, // zadna akce - chyba
	SH,      // < shift
	RE,      // > redukce
	EQ,      // =
	FU       // Volani funkce
};

/*
 * Nonterminal - datovy typ a index (lokalni promenne)
 */
typedef struct {
	TNonTerm non_term;   // O jaky non-terminal se jedna
	TDataType data_type; // Datovy typ
	int index;
} TNonTermData;

/*
 * Polozka na zasobniku nebo na prave strane pravidla (terminaly a non-terminaly).
 * T_RULE je cislo pravidla
 * T_END je ukoncujici znak v prave strane pravidla (neco jako '\0' pro retezce)
 * T_ACT je pomocny znak '<' ktery se bude objevovat na zasobniku (pouze precedencni SA)
 * T_EPS je epsilon pravidlo
 */
typedef struct {
	// Typ polozky
	enum {
		T_TERM,
		T_NONTERM,
		T_RULE,
		T_ACT,
		T_EPS,
		T_END
	} type;

	// Data polozky
	union {
		int rule_number;
		Ttoken term;
		TNonTermData non_term;
		enum prec_actions action;
	} data;
} TItem;

/* Maximalni delka prave strany pravidla */
#define MAX_RULE_SIZE 7

/*
 * Prava strana pravidla (+1 pro ukoncujici znak)
 */
typedef TItem TRightSide[MAX_RULE_SIZE + 1];

/*
 * Pravidlo
 */
typedef struct {
	TNonTerm left;     // Leva strana pravidla
	TRightSide right;  // Prava strana pravidla
} TRule;

/* Celkovy pocet tokenu v LL tabulce a v precedencni tabulce) */
#define TOKEN_COUNT 33

/*
 * Faze behem syntakticke/semanticke analyzy
 * (hlavni je deklaracni a definicni - ostatni jsou pomocne)
 */
typedef enum {
	PHASE_UNKNOWN,
	PHASE_DECLAR,
	PHASE_GLOBAL_VAR,
	PHASE_LOCAL_VAR,
	PHASE_BODY
} TPhases;

#endif /* #ifndef PARSER_DEFINE_H_ */

/* Konec souboru parser_define.h */
