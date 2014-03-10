/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		predictive_sa.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Prediktivni syntakticka analyza - cte deklarace/definice/tela funkci (mimo vyrazy)
			z predaneho popisovace souboru a vyhodnocuje je - pokud je nalezen derivacni strom,
			pak vraci 0, jinak nenulove cislo. Implementace
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "precedence_sa.h"
#include "parser_define.h"
#include "parser_semantic.h"
#include "error.h"
#include "debug.h"
#include "code_list.h"
#include "constants.h"
#include "instructions.h"
#include "generator.h"
#include "token.h"
#include "token_debug.h"
#include "scanner.h"

/*
 * Cisla pravidel - koresponduji s cislem pravidla v tabulce pravidel a LL tabulce
 * a v dokumentaci
 */
enum {
	R1 = 1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15, R16, R17, R18, R19, R20,
	R21, R22, R23, R24, R25, R26, R27, R28, R29, R30, R31, R32, R33, R34, R35, R36, R37, R38, R39,
	R40, R41, R42, R43, R44, R45, R46
};

/*
 * Pravidla
 * 0 - doslo k chybe (znamena to, ze v LL tabulce neni na danem pruseciku pravidlo)
 */
static const TRule RULES[] = {
	[0] = {.left = ERR,                        .right = {{.type = T_END}}},
	// Deklarace a definice funkci
	[1] = {.left = NT_PROG,                    .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_VAR_DECLAR}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNCTIONS}}, {.type = T_END}}},
	[2] = {.left = NT_VAR_DECLAR,              .right = {{.type = T_TERM, .data.term = {.type = TOK_VAR}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_VAR_ITEM}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_VAR_LIST}}, {.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_END}}},
	[3] = {.left = NT_VAR_DECLAR,              .right = {{.type = T_EPS}, {.type = T_END}}},
	[4] = {.left = NT_VAR_ITEM,                .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_TYPE_SPECIFIER}}, {.type = T_TERM, .data.term = {.type = TOK_IDENTIFIER}}, {.type = T_END}}},
	[5] = {.left = NT_VAR_LIST,                .right = {{.type = T_TERM, .data.term = {.type = TOK_COMMA}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_VAR_ITEM}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_VAR_LIST}}, {.type = T_END}}},
	[6] = {.left = NT_VAR_LIST,                .right = {{.type = T_EPS}, {.type = T_END}}},
	[7] = {.left = NT_FUNCTIONS,               .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_UNIT}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNCTIONS}}, {.type = T_END}}},
	[8] = {.left = NT_FUNCTIONS,               .right = {{.type = T_EPS}, {.type = T_END}}},
	[9] = {.left = NT_FUNC_UNIT,               .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_RETURN_TYPE_SPECIFIER}}, {.type = T_TERM, .data.term = {.type = TOK_IDENTIFIER}}, {.type = T_TERM, .data.term = {.type = TOK_LBRACKET}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_UNIT2}}, {.type = T_END}}},
	[10] = {.left = NT_FUNC_UNIT2,             .right = {{.type = T_TERM, .data.term = {.type = TOK_RBRACKET}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_NON_PARAMS}}, {.type = T_END}}},
	[11] = {.left = NT_FUNC_UNIT2,             .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_TYPE_SPECIFIER}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_PARAMS}}, {.type = T_END}}},
	[12] = {.left = NT_FUNC_NON_PARAMS,        .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_VAR_DECLAR}}, {.type = T_TERM, .data.term = {.type = TOK_LVINCULUM}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_STATEMENT_LIST}}, {.type = T_TERM, .data.term = {.type = TOK_RVINCULUM}}, {.type = T_END}}},
	[13] = {.left = NT_FUNC_NON_PARAMS,        .right = {{.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_END}}},
	[14] = {.left = NT_FUNC_PARAMS,            .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_PARAM_DECLAR_LIST}}, {.type = T_TERM, .data.term = {.type = TOK_RBRACKET}}, {.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_END}}},
	[15] = {.left = NT_FUNC_PARAMS,            .right = {{.type = T_TERM, .data.term = {.type = TOK_IDENTIFIER}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_PARAM_DEF_LIST}}, {.type = T_TERM, .data.term = {.type = TOK_RBRACKET}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_VAR_DECLAR}}, {.type = T_TERM, .data.term = {.type = TOK_LVINCULUM}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_STATEMENT_LIST}}, {.type = T_TERM, .data.term = {.type = TOK_RVINCULUM}}, {.type = T_END}}},
	[16] = {.left = NT_FUNC_PARAM_DECLAR_LIST, .right = {{.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_TYPE_SPECIFIER}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_PARAM_DECLAR_LIST}}, {.type = T_END}}},
	[17] = {.left = NT_FUNC_PARAM_DECLAR_LIST, .right = {{.type = T_EPS}, {.type = T_END}}},
	[18] = {.left = NT_FUNC_PARAM_DEF_LIST,    .right = {{.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_TYPE_SPECIFIER}}, {.type = T_TERM, .data.term = {.type = TOK_IDENTIFIER}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_PARAM_DEF_LIST}}, {.type = T_END}}},
	[19] = {.left = NT_FUNC_PARAM_DEF_LIST,    .right = {{.type = T_EPS}, {.type = T_END}}},
	[20] = {.left = NT_TYPE_SPECIFIER,         .right = {{.type = T_TERM, .data.term = {.type = TOK_INT}}, {.type = T_END}}},
	[21] = {.left = NT_TYPE_SPECIFIER,         .right = {{.type = T_TERM, .data.term = {.type = TOK_DOUBLE}}, {.type = T_END}}},
	[22] = {.left = NT_TYPE_SPECIFIER,         .right = {{.type = T_TERM, .data.term = {.type = TOK_STRING}}, {.type = T_END}}},
	[23] = {.left = NT_RETURN_TYPE_SPECIFIER,  .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_TYPE_SPECIFIER}}, {.type = T_END}}},
	[24] = {.left = NT_RETURN_TYPE_SPECIFIER,  .right = {{.type = T_TERM, .data.term = {.type = TOK_VOID}}, {.type = T_END}}},
	// Telo funkci
	[25] = {.left = NT_STATEMENT_LIST,         .right = {{.type = T_EPS}, {.type = T_END}}},
	[26] = {.left = NT_STATEMENT_LIST,         .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_STATEMENT}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_STATEMENT_LIST}}, {.type = T_END}}},
	[27] = {.left = NT_STATEMENT,              .right = {{.type = T_TERM, .data.term = {.type = TOK_IDENTIFIER}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_ST_BEGIN_ID}}, {.type = T_END}}},
	[28] = {.left = NT_STATEMENT,              .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_ST_NOT_BEGIN_ID}}, {.type = T_END}}},
	[29] = {.left = NT_ST_NOT_BEGIN_ID,        .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_CONDITION_STATEMENT}}, {.type = T_END}}},
	[30] = {.left = NT_ST_NOT_BEGIN_ID,        .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_JUMP_STATEMENT}}, {.type = T_END}}},
	[31] = {.left = NT_ST_NOT_BEGIN_ID,        .right = {{.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_END}}},
	[32] = {.left = NT_ST_BEGIN_ID,            .right = {{.type = T_TERM, .data.term = {.type = TOK_LABEL}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_STATEMENT_LIST}}, {.type = T_END}}},
	[33] = {.left = NT_ST_BEGIN_ID,            .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_ASSIGNMENT}}, {.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_END}}},
	[34] = {.left = NT_ST_BEGIN_ID,            .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_CALL}}, {.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_END}}},
	[35] = {.left = NT_CONDITION_STATEMENT,    .right = {{.type = T_TERM, .data.term = {.type = TOK_IF}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_EXPRESSION}}, {.type = T_TERM, .data.term = {.type = TOK_GOTO}}, {.type = T_TERM, .data.term = {.type = TOK_IDENTIFIER}}, {.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_END}}},
	[36] = {.left = NT_JUMP_STATEMENT,         .right = {{.type = T_TERM, .data.term = {.type = TOK_GOTO}}, {.type = T_TERM, .data.term = {.type = TOK_IDENTIFIER}}, {.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_END}}},
	[37] = {.left = NT_JUMP_STATEMENT,         .right = {{.type = T_TERM, .data.term = {.type = TOK_RETURN}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_RETURN_VALUE}}, {.type = T_TERM, .data.term = {.type = TOK_SEMICOLON}}, {.type = T_END}}},
	[38] = {.left = NT_RETURN_VALUE,           .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_EXPRESSION}}, {.type = T_END}}},
	[39] = {.left = NT_RETURN_VALUE,           .right = {{.type = T_EPS}, {.type = T_END}}},
	[40] = {.left = NT_ASSIGNMENT,             .right = {{.type = T_TERM, .data.term = {.type = TOK_ASSIGNMENT}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_EXPRESSION}}, {.type = T_END}}},
	[41] = {.left = NT_FUNC_CALL,              .right = {{.type = T_TERM, .data.term = {.type = TOK_LBRACKET}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_ARG_LIST}}, {.type = T_TERM, .data.term = {.type = TOK_RBRACKET}}, {.type = T_END}}},
	[42] = {.left = NT_FUNC_ARG_LIST,          .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_EXPRESSION}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_EXP_IN_FUNC}}, {.type = T_END}}},
	[43] = {.left = NT_FUNC_ARG_LIST,          .right = {{.type = T_EPS}, {.type = T_END}}},
	[44] = {.left = NT_EXP_IN_FUNC,            .right = {{.type = T_NONTERM, .data.non_term = {.non_term = NT_FUNC_ARG_LIST}}, {.type = T_END}}},
	[45] = {.left = NT_EXP_IN_FUNC,            .right = {{.type = T_TERM, .data.term = {.type = TOK_COMMA}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_EXPRESSION}}, {.type = T_NONTERM, .data.non_term = {.non_term = NT_EXP_IN_FUNC}}, {.type = T_END}}},
	[46] = {.left = NT_EXPRESSION,             .right = {{.type = T_TERM, .data.term = {.type = TOK_CONSTINT}}, {.type = T_END}}}
};

/* Specialni pravidlo, ktere znaci skok do precedencni SA (nasleduje vyraz) */
#define EX -1

/*
 * LL tabulka pravidel
 */
static const int LL_TABLE[][TOKEN_COUNT] = {
	                             /*  int  double  string  void  var  goto  if  return  div  id   ,    ;    :   :=  ^   +   -   *   /   <   <=  >   >=  ==  !=  (   )    {    }     conststring  constint  constdouble  EOF*/
	/* NT_PROG                   */ {1,   1,      1,      1,    1,   0,    0,  0,      0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           1},
	/* NT_VAR_DECLAR             */ {3,   3,      3,      3,    2,   0,    0,  0,      0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   3,   0,    0,           0,        0,           3},
	/* NT_VAR_ITEM               */ {4,   4,      4,      0,    0,   0,    0,  0,      0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           0},
	/* NT_VAR_LIST               */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   0,   5,   6,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           0},
	/* NT_FUNCTIONS              */ {7,   7,      7,      7,    0,   0,    0,  0,      0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           8},
	/* NT_FUNC_UNIT              */ {9,   9,      9,      9,    0,   0,    0,  0,      0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           0},
	/* NT_FUNC_UNIT2             */ {11,  11,     11,     0,    0,   0,    0,  0,      0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  10,  0,   0,    0,           0,        0,           0},
	/* NT_FUNC_NON_PARAMS        */ {0,   0,      0,      0,    12,  0,    0,  0,      0,   0,   0,   13,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   12,  0,    0,           0,        0,           0},
	/* NT_FUNC_PARAMS            */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   15,  0,   14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  14,  0,   0,    0,           0,        0,           0},
	/* NT_FUNC_PARAM_DECLAR_LIST */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   0,   0,   16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  17,  0,   0,    0,           0,        0,           0},
	/* NT_FUNC_PARAM_DEF_LIST    */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   0,   0,   18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  19,  0,   0,    0,           0,        0,           0},
	/* NT_TYPE_SPECIFIER         */ {20,  21,     22,     0,    0,   0,    0,  0,      0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           0},
	/* NT_RETURN_TYPE_SPECIFIER  */ {23,  23,     23,     24,   0,   0,    0,  0,      0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           0},
	/* NT_STATEMENT_LIST         */ {0,   0,      0,      0,    0,   26,   26, 26,     0,   26,  26,  26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   25,   0,           0,        0,           0},
	/* NT_STATEMENT              */ {0,   0,      0,      0,    0,   28,   28, 28,     0,   27,  0,   28,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           0},
	/* NT_ST_NOT_BEGIN_ID        */ {0,   0,      0,      0,    0,   30,   29, 30,     0,   0,   0,   31,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           0},
	/* NT_ST_BEGIN_ID            */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   0,   0,   0,   32, 33, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  34, 0,   0,   0,    0,           0,        0,           0},
	/* NT_CONDITION_STATEMENT    */ {0,   0,      0,      0,    0,   0,    35, 0,      0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           0},
	/* NT_JUMP_STATEMENT         */ {0,   0,      0,      0,    0,   36,   0,  37,     0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           0},
	/* NT_RETURN_VALUE           */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   38,  0,   39,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  EX, 0,   0,   0,    38,          38,       38,          0},
	/* NT_ASSIGNMENT             */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   0,   0,   0,   0,  40, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,    0,           0,        0,           0},
	/* NT_FUNC_CALL              */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   41,  0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  41, 0,   0,   0,    0,           0,        0,           0},
	/* NT_FUNC_ARG_LIST          */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   42,  0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  42, 43,  0,   0,    42,          42,       42,          0},
	/* NT_EXP_IN_FUNC            */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   44,  45,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  44,  0,   0,    44,          44,       44,          0},
	/* NT_EXPRESSION             */ {0,   0,      0,      0,    0,   0,    0,  0,      0,   EX,  0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  EX, 0,   0,   0,    EX,          EX,       EX,          0},
};

/*
 * Zasobnik na terminaly a nonterminaly
 */
typedef struct {
	struct stack_item {
		TItem item;              // Vlastni data polozky
		struct stack_item *next; // Ukazatel na dalsi polozku
	} *top;                      // Ukazatel na vrchol zasobniku
} TStack;

/*
 * Inicializace zasobniku
 */
static inline void stack_init(TStack *s)
{
	s->top = NULL;
}

/*
 * Pomocna funkce pro vytvoreni dalsi polozky
 * (alokuje pamet a vraci ukazatel)
 */
static struct stack_item *stack_create_item(TItem item)
{
 	struct stack_item *new_item = malloc(sizeof(*new_item));
	if (!new_item)
	{
		error(RAW, "Prediktivni SA: Nedostatek pameti pro vytvoreni zasobniku.\n");
		exit(ERR_FAILURE);
	}

	new_item->item = item;
	return new_item;
}


/*
 * Vlozi hodnotu na zasobnik
 */
static void stack_push(TStack *s, TItem item)
{
	if (s->top == NULL) {
		// Zasobnik je prazdny
		s->top = stack_create_item(item);
		s->top->next = NULL;
	}
	else {
		// Na vrcholu zasobniku se nachazi polozka
		struct stack_item *new_item = stack_create_item(item);
		new_item->next = s->top;
		s->top = new_item;
	}
}

/*
 * Odstrani hodnotu z vrcholu zasobniku
 */
static void stack_pop(TStack *s)
{
	if (s->top != NULL) {
		struct stack_item *tmp_item = s->top;
		s->top = s->top->next;
 		free(tmp_item);
	}
	else {
		fprintf(stderr, "Prediktivni SA: Zasobnik je prazdny - nemuze byt proveden pop.\n");
		exit(ERR_FAILURE);
	}
}

/*
 * Vraci hodnotu z vrcholu zasobniku
 */
static TItem stack_top(const TStack *s)
{
	if (s->top != NULL)
		return s->top->item;
	else {
		fprintf(stderr, "Prediktivni SA: Zasobnik je prazdny - nemuze byt proveden top.\n");
		exit(ERR_FAILURE);
	}

	// Doslo k chybe - vracena hodnota je nedefinovana - nemelo by nikdy nastat
	assert(0);
	TItem error;
	return error;
}

/*
 * Vraci true, jestli je zasobnik prazdny
 */
static inline bool stack_empty(const TStack *s)
{
	return (s->top == NULL);
}

/*
 * Zrusi vsechny polozky v zasobniku
 */
static inline void stack_clear(TStack *s)
{
	while (!stack_empty(s))
		stack_pop(s);
}

/*
 * Vola prediktivni syntaktickou analyzu a vraci 0, pokud vse probehlo v poradku a nenulove cislo v pripade chyby.
 * Popis parametru:
 *  stream - popisovac souboru se zdrojovym kodem
 *  phase - faze, ve ktere se SA zrovna nachazi
 *  global_st - globalni tabulka symbolu
 *  local_st - lokalni tabulka symbolu
 *  cur_token - aktualni token
 *  end_token - ukoncujici token
 *  last_token - posledni nacteny token
 *  starting_nonterm - pocatecni non-terminal, od ktereho bude SA zacinat
 *  return_item - navratova hodnota z analyzy (muze byt NULL)
 *  last_called_func - nazev posledne volane funkce
 */
int predictive_SA(FILE *stream,	TPhases *phase, TSymtable *global_st, TSymtable *local_st,
	Ttoken cur_token, Ttoken end_token, Ttoken *last_token,
	TNonTerm starting_nonterm, TItem *return_item, const char *last_called_func)
{
	Ttoken tok = cur_token; // Prave nacteny token
	TStack stack;           // Zasobnik pro uchovani terminalu a non-terminalu
	TItem item;             // Polozka (terminal nebo neterminal)
	TItem ret_item;         // Navratovy "typ" (pozice a non-terminal) bude v return_item (potreba pri krizovem volani SA)
	bool success = false;   // Uspech/neuspech SA
	TErrCode rc;            // Navratovy kod

	// Inicializace zasobniku
	stack_init(&stack);

	// Vlozeni ukoncujiciho tokenu
	item.type = T_TERM;
	item.data.term = end_token;
	stack_push(&stack, item);

	// Vlozeni pocateniho nonterminalu
	item.type = T_NONTERM;
	item.data.non_term.non_term = starting_nonterm;
	stack_push(&stack, item);

	// Predchozi pravidlo pred volanim funkce
	int before_func_rule_number = 0;
	// Posledni identifikator
	Ttoken last_identifier;
	// Index posledniho identifikatoru
	int id_index;
	// Typ posledniho identifikatoru
	TDataType id_data_type;

	// Vlastni telo prediktivni SA
	while ((!stack_empty(&stack) && tok.type != TOK_SCANNERERROR) && !success) {
		// Ziskej vrchol zasobniku
		item = stack_top(&stack);
		switch (item.type) {
			// Na vrcholu zasobniku je terminal
			case T_TERM:
				// Pokud se token shoduje s tokenem oznacenym jako konecny, tak jsme na konci SA
				if ((tok.type == end_token.type)) {
					if (item.data.term.type == end_token.type) {
						// Uspech
						success = true;
						break;
					}
					else {
						// Chyba
						*last_token = tok;
						stack_clear(&stack);
						return ERR_FAILURE;
					}
				}
				// Je nacteny token shodny s tim, co je na zasobniku ?
				else if (tok.type == item.data.term.type) {
					// Generovani instrukce
					switch (before_func_rule_number) {
						// Prazdny prikaz ;
						case R31: {
							// Nic negenerovat - zbytecne (optimalizace)
						}
						break;
						// if <expression> goto id ;
						case R35: {
							if (tok.type == TOK_IDENTIFIER) {
								TMicroInstruction *label = symtable_get_label_addr(local_st, tok.attribute.ptr);
								if (label == NULL) {
									// Chyba
									*last_token = tok;
									stack_clear(&stack);
									return ERR_FAILURE;
								}

								// Proved konverzi na typ int (pokud je treba), protoze vysledek podminky musi byt
								// cele cislo
								int index;
								switch (ret_item.data.non_term.data_type) {
									case TYPE_DOUBLE: {
										index = symtable_insert_tmp(local_st);
										if (generate_dbl2int(index, ret_item.data.non_term.index) != ERR_SUCCESS) {
											// Chyba
											*last_token = tok;
											stack_clear(&stack);
											return ERR_FAILURE;
										}
									}
									break;
									case TYPE_STRING: {
										index = symtable_insert_tmp(local_st);
										if (generate_str2bool(index, ret_item.data.non_term.index) != ERR_SUCCESS) {
											// Chyba
											*last_token = tok;
											stack_clear(&stack);
											return ERR_FAILURE;
										}
									}
									break;
									case TYPE_INT:
									default:
										index = ret_item.data.non_term.index;
										break;
								}

								// Vygenerovat instrukci
								if (generate_goto_if(index, label) != ERR_SUCCESS) {
									// Chyba
									*last_token = tok;
									stack_clear(&stack);
									return ERR_FAILURE;
								}
							}
						}
						break;
						// goto id ;
						case R36: {
							if (tok.type == TOK_IDENTIFIER) {
								TMicroInstruction *label_addr = symtable_get_label_addr(local_st, tok.attribute.ptr);
								if (generate_goto(label_addr) != ERR_SUCCESS) {
									*last_token = tok;
									stack_clear(&stack);
									return ERR_FAILURE;
								}
							}
						}
						break;
						// return <expression> ;
 						case R38: {
 							if (tok.type != TOK_RETURN && tok.type == TOK_SEMICOLON) {
								// Zkontrolovat, jestli sedi navratovy typ
								TDataType ret_type;
								if ((symtable_get_func_rettype(global_st, get_current_func_name(), &ret_type) != ERR_SUCCESS) ||
									(ret_type != ret_item.data.non_term.data_type)) {
									// Navratovy typ neni void nebo funkce neexistuje - chyba
									error(ROWONLY, "Semanticka chyba: Nesouhlasi navratovy typ funkce (%s) a typ navratove hodnoty.\n",
										get_current_func_name());
									*last_token = tok;
									stack_clear(&stack);
									return ERR_FAILURE;
								}

								if (generate_return(ret_item.data.non_term.index) != ERR_SUCCESS) {
									*last_token = tok;
									stack_clear(&stack);
									return ERR_FAILURE;
								}
							}
						}
						break;
						// <st_begin_id>
						case R27: {
							// Ulozit posledni identifikator
							if (tok.type == TOK_IDENTIFIER) {
								if (symtable_variable_exists(global_st, local_st, tok.attribute.ptr)) {
  									get_var_data(tok, &id_index, &id_data_type, global_st, local_st);
  									last_identifier = tok;
								}
							}
						}
						break;
						// Assignment :=
						case R40: {
							if (tok.type == TOK_SEMICOLON) {
								// Kontrola typu
								if (id_data_type != ret_item.data.non_term.data_type) {
									// Nesedi typy
									error(ROWONLY, "Semanticka analyza: Prirazeni nelze provest, nesouhlasi typy.\n");
									*last_token = tok;
									stack_clear(&stack);
									return ERR_FAILURE;
								}

								if (generate_mov(id_index, ret_item.data.non_term.index) != ERR_SUCCESS) {
									*last_token = tok;
									stack_clear(&stack);
									return ERR_FAILURE;
								}
							}
						}
						break;
						default:
							break;
					}

					// Ano, je - proved pop a nacti dalsi token
					stack_pop(&stack);

					rc = gnt_iist(stream, phase, global_st, local_st, &tok);
					if (rc != ERR_SUCCESS) {
						*last_token = tok;
						stack_clear(&stack);
						return ERR_FAILURE;
					}
					continue;
				}
				else {
					// Chyba
					*last_token = tok;
					stack_clear(&stack);
					return ERR_FAILURE;
				}
				break;

			// Na vrcholu zasobniku je non-terminal
			case T_NONTERM:	{
				// Najdi pravidlo v LL tabulce
				int rule_number = LL_TABLE[item.data.non_term.non_term][tok.type];

				// Pokud se jedna o volani funkce (to zjistime z cisla pravidla)
				// tak zacneme kontrolovat predavane argumenty (zda sedi typy)
				if (rule_number == R41) {
					if (symtable_start_call_func(global_st, get_last_called_func_name()) != ERR_SUCCESS) {
						// Chyba
						*last_token = tok;
						stack_clear(&stack);
						return ERR_FAILURE;
					}
				}
				else if (rule_number == R44 || rule_number == R45) {
					// Dalsi argument - pridame jej do kontroly
					// (datovy typ je ulozen v ret_item)
					if (symtable_call_arg(global_st, ret_item.data.non_term.data_type) != ERR_SUCCESS) {
						// Chyba
						*last_token = tok;
						stack_clear(&stack);
						return ERR_FAILURE;
					}

					if (generate_push(ret_item.data.non_term.index) != ERR_SUCCESS) {
						// Chyba
						*last_token = tok;
						stack_clear(&stack);
						return ERR_FAILURE;
					}
				}
				else if (rule_number == R43) {
					// Konec volani funkce - muzeme zkontrolovat, zda sedi argumenty
					// Zaroven se nam vrati ukazatel na instrukci (funkci), ktera se bude volat
					TMicroInstruction *func_address = symtable_stop_call_func(global_st);
					if (func_address == NULL) {
						// Nesedi typy - chyba
						*last_token = tok;
						stack_clear(&stack);
						return ERR_FAILURE;
					}

					if (generate_call(func_address) != ERR_SUCCESS) {
						// Chyba
						*last_token = tok;
						stack_clear(&stack);
						return ERR_FAILURE;
					}
				}

				if (rule_number == EX) {
					// Bude nasledovat vyraz, takze provedeme pop a pokracujeme precedencni SA
					stack_pop(&stack);

					#ifdef PRED_SA_DEBUG
						fprintf(stderr, "Volam precedencni SA\n");
					#endif

					// Posledni nacteny token bude ulozen v tok
					// Navratovy "typ" (pozice a non-terminal) bude v ret_item
					if (precedence_SA(stream, phase, global_st, local_st, tok, &tok, &ret_item) != 0) {
						// Chyba
						*last_token = tok;
						stack_clear(&stack);
						return ERR_FAILURE;
					}

					#ifdef PRED_SA_DEBUG
						fprintf(stderr, "Navrat z precedencni SA - aktualni token: %d\n", tok.type);
						fprintf(stderr, "                        - vracena polozka - typ: %d index: %d\n",
							ret_item.data.non_term.data_type, ret_item.data.non_term.index);
					#endif

				}
				else {
					// Nenasleduje vyraz, tak prokracuj normalne v prediktivni SA
					TRule rule = RULES[rule_number];

					#ifdef PRED_SA_DEBUG
						// Zapis pravidlo na vystup
						fprintf(stderr, "item.data.non_term.non_term: %d, tok.type: %d, LL_TABLE[item.data.non_term.non_term][tok.type]: %d\n",
							item.data.non_term.non_term, tok.type, LL_TABLE[item.data.non_term.non_term][tok.type]);
					#endif

					if (rule.left != ERR) {
						// Jestlize se jedna o epsilon pravidlo, tak provedeme pouze pop
						// a pokracujeme dal
						if (rule.right[0].type == T_EPS) {
							stack_pop(&stack);
							continue;
						}

						// Zjisti, kolik ma dana strana polozek
						int i = 0;
						while (rule.right[i].type != T_END) {
							i++;
						}
						i--;

					#ifdef PRED_SA_DEBUG
						// Zapis index posledniho symbolu na prave strane
						fprintf(stderr, "Index posledniho symbolu na prave strane: %d\n", i);
					#endif

						// Zamen vrchol zasobniku za reversal(prava_strana_pravidla)
						stack_pop(&stack);
						while (i >= 0) {
							stack_push(&stack, rule.right[i]);
							i--;
						}

						// Uloz predchozi pravidlo pred volanim funkce
						if (rule_number != R41)
							before_func_rule_number = rule_number;
					}
					else {
						// Chyba
						*last_token = tok;
						stack_clear(&stack);
						return ERR_FAILURE;
					}
				}
			}
			break;

			default:
				// Tady by se switch nemel nikdy dostat, je to jen kvuli varovanim prekladace
				assert(0);
				fprintf(stderr, "Prediktivni SA: Neocekavany typ symbolu na vrcholu zasobniku.\n");
				exit(ERR_FAILURE);
				break;
		}
	}

	if (!success) {
		// Chyba - SA neskoncila uspesne
		*last_token = tok;
		stack_clear(&stack);
		return ERR_FAILURE;
	}

	// Ulozeni indexu, kde bude ulozena vysledna hodnota (platne pouze u volani funkce ve vyrazu)
	if (return_item != NULL) {
		// Nazev funkce by mel byt ruzny od NULL
		assert(last_called_func != NULL);

		TDataType func_ret_type;
		symtable_get_func_rettype(global_st, last_called_func, &func_ret_type);
		return_item->data.non_term.data_type = func_ret_type;
		return_item->data.non_term.index = RETURN_VALUE_INDEX; // Definovan v constants.h
	}

	*last_token = tok;
	stack_clear(&stack);
    return ERR_SUCCESS;
}

/* Konec souboru predictive_sa.c */
