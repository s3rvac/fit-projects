/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		precedence_sa.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Precedencni syntakticka analyza - cte vyrazy ze standardniho
			vstupu a vyhodnocuje je - pokud je nalezen derivacni strom,
			pak se vypise OK a pravy rozbor, jinak CHYBA a nazev tokenu + cislo radku,
			na kterem SA skoncila.
			Implementace.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "predictive_sa.h"
#include "parser_define.h"
#include "parser_semantic.h"
#include "error.h"
#include "constants.h"
#include "data_type.h"
#include "instructions.h"
#include "generator.h"
#include "debug.h"
#include "code_list.h"
#include "token.h"
#include "token_debug.h"
#include "scanner.h"

/*
 * Pravidla
 * 0 - doslo k chybe (znamena to, ze v precedencni tabulce je na danem pruseciku prazdne policko)
 */
static const TRule RULES[] = {
	[0] = {.left = ERR,  .right = {{.type = T_END}}},
	[1] = {.left = E,    .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_PLUS}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[2] = {.left = E,    .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_MINUS}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[3] = {.left = E,    .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_MULTIPLE}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[4] = {.left = E,    .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_DIVDOUBLE}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[5] = {.left = E,    .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_DIVINT}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[6] = {.left = E,    .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_POWER}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[7] = {.left = E,    .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_LESS}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[8] = {.left = E,    .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_GREATER}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[9] = {.left = E,    .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_LESSEQUAL}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[10] = {.left = E,   .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_GREATEREQUAL}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[11] = {.left = E,   .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_EQUAL}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[12] = {.left = E,   .right = {{.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_NOTEQUAL}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_END}}},
	[13] = {.left = E,   .right = {{.type = T_TERM, .data.term = {.type = TOK_IDENTIFIER}}, {.type = T_END}}},
	[14] = {.left = E,   .right = {{.type = T_TERM, .data.term = {.type = TOK_CONSTINT}}, {.type = T_END}}},
	[15] = {.left = E,   .right = {{.type = T_TERM, .data.term = {.type = TOK_CONSTDOUBLE}}, {.type = T_END}}},
	[16] = {.left = E,   .right = {{.type = T_TERM, .data.term = {.type = TOK_CONSTSTRING}}, {.type = T_END}}},
	[17] = {.left = E,   .right = {{.type = T_TERM, .data.term = {.type = TOK_LBRACKET}}, {.type = T_NONTERM, .data.non_term = {.non_term = E}}, {.type = T_TERM, .data.term = {.type = TOK_RBRACKET}}, {.type = T_END}}},
};

/*
 * Precedencni tabulka pravidel (na pruseciku je vzdy akce, kterou je treba vykonat
 * nebo NA - znamena prazdne policko - chyba).
 * Musi zde byt vsechny non-terminaly, protoze se indexuje "hodnotou vyctoveho typu terminalu".
 */
static const int PREC_TABLE[][TOKEN_COUNT] = {
	               /*  int  double  string  void  var  goto  if  return  div  id   ,    ;    :   :=  ^   +   -   *   /   <   <=  >   >=  ==  !=  (   )    {    }     conststring  constint  constdouble  EOF*/
	/* int         */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* double      */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* string      */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* void        */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* var         */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* goto        */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* if          */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* return      */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* div         */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     RE,  SH,  RE,  RE,  NA, NA, SH, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, SH, RE,  NA,  NA,   NA,         SH,        NA,          RE},
	/* id          */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     RE,  NA,  RE,  RE,  NA, NA, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, FU, RE,  NA,  NA,   NA,         NA,        NA,          RE},
	/* ,           */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* ;           */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* :           */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* :=          */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* ^           */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     RE,  SH,  RE,  RE,  NA, NA, SH, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, SH, RE,  NA,  NA,   NA,         SH,        SH,          RE},
	/* +           */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     SH,  SH,  RE,  RE,  NA, NA, SH, RE, RE, SH, SH, RE, RE, RE, RE, RE, RE, SH, RE,  NA,  NA,   SH,         SH,        SH,          RE},
	/* -           */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     SH,  SH,  RE,  RE,  NA, NA, SH, RE, RE, SH, SH, RE, RE, RE, RE, RE, RE, SH, RE,  NA,  NA,   NA,         SH,        SH,          RE},
	/* *           */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     RE,  SH,  RE,  RE,  NA, NA, SH, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, SH, RE,  NA,  NA,   NA,         SH,        SH,          RE},
	/* /           */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     RE,  SH,  RE,  RE,  NA, NA, SH, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, SH, RE,  NA,  NA,   NA,         SH,        SH,          RE},
	/* <           */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     SH,  SH,  RE,  RE,  NA, NA, SH, SH, SH, SH, SH, RE, RE, RE, RE, RE, RE, SH, RE,  NA,  NA,   SH,         SH,        SH,          RE},
	/* <=          */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     SH,  SH,  RE,  RE,  NA, NA, SH, SH, SH, SH, SH, RE, RE, RE, RE, RE, RE, SH, RE,  NA,  NA,   SH,         SH,        SH,          RE},
	/* >           */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     SH,  SH,  RE,  RE,  NA, NA, SH, SH, SH, SH, SH, RE, RE, RE, RE, RE, RE, SH, RE,  NA,  NA,   SH,         SH,        SH,          RE},
	/* >=          */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     SH,  SH,  RE,  RE,  NA, NA, SH, SH, SH, SH, SH, RE, RE, RE, RE, RE, RE, SH, RE,  NA,  NA,   SH,         SH,        SH,          RE},
	/* ==          */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     SH,  SH,  RE,  RE,  NA, NA, SH, SH, SH, SH, SH, SH, SH, SH, SH, RE, RE, SH, RE,  NA,  NA,   SH,         SH,        SH,          RE},
	/* !=          */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     SH,  SH,  RE,  RE,  NA, NA, SH, SH, SH, SH, SH, SH, SH, SH, SH, RE, RE, SH, RE,  NA,  NA,   SH,         SH,        SH,          RE},
	/* (           */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     SH,  SH,  NA,  NA,  NA, NA, SH, SH, SH, SH, SH, SH, SH, SH, SH, SH, SH, SH, EQ,  NA,  NA,   SH,         SH,        SH,          NA},
	/* )           */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     RE,  NA,  RE,  RE,  NA, NA, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, NA, RE,  NA,  NA,   NA,         NA,        NA,          RE},
	/* {           */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* }           */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     NA,  NA,  NA,  NA,  NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,  NA,  NA,   NA,         NA,        NA,          NA},
	/* conststring */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     NA,  NA,  RE,  RE,  NA, NA, NA, RE, NA, NA, NA, RE, RE, RE, RE, RE, RE, NA, RE,  NA,  NA,   NA,         NA,        NA,          RE},
	/* constint    */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     RE,  NA,  RE,  RE,  NA, NA, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, NA, RE,  NA,  NA,   NA,         NA,        NA,          RE},
	/* constdouble */ {NA,  NA,     NA,     NA,   NA,  RE,   NA, NA,     NA,  NA,  RE,  RE,  NA, NA, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, NA, RE,  NA,  NA,   NA,         NA,        NA,          RE},
	/* EOF         */ {NA,  NA,     NA,     NA,   NA,  NA,   NA, NA,     SH,  SH,  NA,  NA,  NA, NA, SH, SH, SH, SH, SH, SH, SH, SH, SH, SH, SH, SH, NA,  NA,  NA,   SH,         SH,        SH,          NA}
};

/*
 * "Zasobnik" pro ukladani tokenu, non-terminalu a akci (u akci se bude ukladat jen '<' neboli SH)
 */
typedef struct {
	struct prec_stack_item {
		TItem item;                   // Vlastni data polozky
		struct prec_stack_item *next; // Ukazatel na dalsi prvek na zasobniku
		struct prec_stack_item *prev; // Ukazatel na predchozi prvek na zasobniku
	} *top, *term;                    // Ukazatel na vrchol zasobniku a na nejvrchnejsi terminal
} TPrecStack;

/*
 * Inicializace zasobniku
 */
static inline void prec_stack_init(TPrecStack *s)
{
	s->top = s->term = NULL;
}

/*
 * Vraci true, jestli je zasobnik prazdny
 */
static inline bool prec_stack_empty(const TPrecStack *s)
{
	return (s->top == NULL);
}

/*
 * Pomocna funkce pro vytvoreni dalsi polozky
 * (alokuje pamet a vraci ukazatel)
 */
static struct prec_stack_item *prec_stack_create_item(TItem item)
{
	struct prec_stack_item *new_item = malloc(sizeof(*new_item));
	if (!new_item)
	{
		error(RAW, "Precedencni SA: Nedostatek pameti pro vytvoreni zasobniku.\n");
		exit(ERR_FAILURE);
	}

	new_item->item = item;
	return new_item;
}

/*
 * Vlozi hodnotu na zasobnik
 */
static void prec_stack_push(TPrecStack *s, TItem item)
{
	if (s->top == NULL) {
		// Zasobnik je prazdny
		s->top = prec_stack_create_item(item);
		s->top->next = s->top->prev = NULL;
	}
	else {
		// Na vrcholu zasobniku se nachazi polozka
		struct prec_stack_item *new_item = prec_stack_create_item(item);
		new_item->next = s->top;
		new_item->prev = NULL;
		s->top->prev = new_item;
		s->top = new_item;
	}

	// Byl vlozen terminal ?
	if (item.type == T_TERM) {
		s->term = s->top;
	}
}

/*
 * Aktualizuje ukazatel na terminal, jez je nejblize vrcholu zasobniku
 */
static void prec_stack_actualize_term(TPrecStack *s)
{
	// Nastavim ukazatel na terminal pro pripad,
	// ze se v zasobniku nevyskytuje dalsi terminal
	s->term = NULL;

	struct prec_stack_item *cur_item = s->top;
	while ((cur_item != NULL)) {
		if (cur_item->item.type == T_TERM) {
			s->term = cur_item;
			return;
		}
		cur_item = cur_item->next;
	}
}

/*
 * Vraci (!) terminal (!) nejblize vrcholu zasobniku
 */
static Ttoken prec_stack_top_term(const TPrecStack *s)
{
	if (s->top != NULL) {
		struct prec_stack_item *cur_item = s->top;
		while ((cur_item != NULL)) {
			if (cur_item->item.type == T_TERM)
				return cur_item->item.data.term;
			else
				cur_item = cur_item->next;
		}
	}
	else {
		fprintf(stderr, "Precedencni SA: Zasobnik je prazdny - nemuze byt proveden top.\n");
		exit(ERR_FAILURE);
	}

	// Doslo k chybe - vracena hodnota je nedefinovana - to by nikdy nemelo nastat
	assert(0);
	Ttoken err_token;
	return err_token;
}

/*
 * Vraci (!) non_terminal (!) (jako TItem) nejblize vrcholu zasobniku
 */
static TItem prec_stack_top_nonterm(const TPrecStack *s)
{
	if (s->top != NULL) {
		struct prec_stack_item *cur_item = s->top;
		while ((cur_item != NULL)) {
			if (cur_item->item.type == T_NONTERM)
				return cur_item->item;
			else
				cur_item = cur_item->next;
		}
	}
	else {
		fprintf(stderr, "Precedencni SA: Zasobnik je prazdny - nemuze byt proveden top.\n");
		exit(ERR_FAILURE);
	}

	// Doslo k chybe - vracena hodnota je nedefinovana - to by nikdy nemelo nastat
	assert(0);
	TItem err_item;
	return err_item;
}

/*
 * Odstrani hodnotu z vrcholu zasobniku
 */
static void prec_stack_pop(TPrecStack *s)
{
	if (s->top != NULL) {
		// Proved pop
		struct prec_stack_item *tmp_item = s->top;
		s->top = s->top->next;
		if (s->top != NULL) {
			s->top->prev = NULL;
		}

		if (tmp_item->item.type == T_TERM) {
			// Odstranuje se terminal, musime aktualizovat ukazatel na
			// terminal nejblize vrcholu
			prec_stack_actualize_term(s);
		}

		free(tmp_item);
	}
	else {
		fprintf(stderr, "Precedencni SA: Zasobnik je prazdny - nemuze byt proveden pop.\n");
		exit(ERR_FAILURE);
	}
}

/*
 * Vlozi '<' (SH) za nejvrchnejsi terminal na zasobniku
 */
static void prec_stack_shift(TPrecStack *s)
{
	// Je vubec na zasobniku terminal?
	if (!s->term)
		return;

	TItem item = {.type = T_ACT, .data.action = SH};
	if (s->term == s->top) {
		// Bude stacit provest push
		prec_stack_push(s, item);
	}
	else {
		// Zde uz to neni tak jednoduche - vyuziju toho, ze zasobnik je
		// tvoren obousmerne vazanym seznamem
		struct prec_stack_item *new_item = prec_stack_create_item(item);
		new_item->next = s->term;
		new_item->prev = s->term->prev;
		s->term->prev->next = new_item;
		s->term->prev = new_item;
	}
}

/*
 * Funkce vraci true, jestlize je dana polozka "konec" prave strany pravidla
 */
static inline bool is_rule_end(TItem item)
{
	return (item.type == T_END);
}

/*
 * Funkce vraci true, jestlize jsou oba tokeny shodneho typu
 */
static inline bool are_same_tokens(Ttoken tok1, Ttoken tok2)
{
	return (tok1.type == tok2.type);
}

/*
 * Funkce vraci true, jestlize jsou obe polozky shodne
 */
static bool are_same_items(TItem item1, TItem item2)
{
	if (item1.type == item2.type) {
		if ((item1.type == T_NONTERM) && (item1.data.non_term.non_term == item2.data.non_term.non_term))
			return true;
		else if ((item1.type == T_TERM) && are_same_tokens(item1.data.term, item2.data.term))
			return true;
		else
			return (item1.type == T_END);
	}

	// Nejsou shodneho typu
	return false;
}

/*
 * Prevrati pravidlo
 */
static void reverse_rule(TRule *rule)
{
	// Zjisti pocet prvku (mimo koncoveho)
	int i = 0;
	while (rule->right[i].type != T_END)
		i++;

	// Pokud je v pravidle 0 nebo jen 1 prvek, neni treba prevracet
	if (i <= 1)
		return;

	// Prevrat ho
	for (int j = 0; j <= i; j++, i--) {
		TItem tmp = rule->right[j];
		rule->right[j] = rule->right[i - 1];
		rule->right[i - 1] = tmp;
	}
}

/*
 * Vyhleda dane pravidlo v poli pravidel (RULES) a vrati jeho index
 * nebo 0, pokud dane pravidlo neexistuje.
 */
static int get_rule_index(const TRule *rule)
{
	const int RULES_COUNT = sizeof(RULES) / sizeof(TRule);
	for (int i = 0; i < RULES_COUNT; i++) {
		for (int j = 0; j <= MAX_RULE_SIZE; j++) {
			if (are_same_items(rule->right[j], RULES[i].right[j])) {
				if (is_rule_end(rule->right[j])) {
					// Konec pravidla - nalezena shoda
					return i;
				}
			}
			else {
				// Toto pravidlo to neni, prejdi na test dalsiho
				break;
			}
		}
	}

	// Nenalezeno
	return 0;
}

/* Mozne operace s operandy */
typedef enum {
	OPER_DIVINT = TOK_DIVINT,
	OPER_POWER = TOK_POWER,
	OPER_PLUS = TOK_PLUS,
	OPER_MINUS = TOK_MINUS,
	OPER_MULTIPLE = TOK_MULTIPLE,
	OPER_DIVDOUBLE = TOK_DIVDOUBLE,
	OPER_LESS = TOK_LESS,
	OPER_LESSEQUAL = TOK_LESSEQUAL,
	OPER_GREATER = TOK_GREATER,
	OPER_GREATEREQUAL = TOK_GREATEREQUAL,
	OPER_EQUAL = TOK_EQUAL,
	OPER_NOTEQUAL = TOK_NOTEQUAL
} TOperations;

/* Prevodni tabulka mezi tokenem a operaci */
static const TOperations OPERATIONS[] = {
	[TOK_DIVINT] = OPER_DIVINT,
	[TOK_POWER] = OPER_POWER,
	[TOK_PLUS] = OPER_PLUS,
	[TOK_MINUS] = OPER_MINUS,
	[TOK_MULTIPLE] = OPER_MULTIPLE,
	[TOK_DIVDOUBLE] = OPER_DIVDOUBLE,
	[TOK_LESS] = OPER_LESS,
	[TOK_LESSEQUAL] = OPER_LESSEQUAL,
	[TOK_GREATER] = OPER_GREATER,
	[TOK_GREATEREQUAL] = OPER_GREATEREQUAL,
	[TOK_EQUAL] = OPER_EQUAL,
	[TOK_NOTEQUAL] = OPER_NOTEQUAL
};

/*
 * Vrati akci (operaci) podle operatoru
 */
static inline TOperations token2operation(Ttoken tok)
{
	return OPERATIONS[tok.type];
}

/*
 * Nastavi index a typ nonterminalu podle zadanych parametru
 */
static inline void set_index_and_type(TItem *item, TDataType data_type, int index)
{
	item->data.non_term.data_type = data_type;
	item->data.non_term.index = index;
}

/*
 * Zjisti datovy typ konstanty a vrati jej v data_type. Pokud se nejedna o konstantu,
 * vraci ERR_FAILURE a hodnota data_type je nedefinovana.
 */
static TErrCode get_const_data_type(Ttoken tok, TDataType *data_type)
{
	TErrCode rc = ERR_SUCCESS; // Navratovy kod

	switch (tok.type) {
		case TOK_CONSTINT:
			*data_type = TYPE_INT;
			break;
		case TOK_CONSTDOUBLE:
			*data_type = TYPE_DOUBLE;
			break;
		case TOK_CONSTSTRING:
			*data_type = TYPE_STRING;
			break;
		default:
			// Chyba - neni to konstanta
			rc = ERR_FAILURE;
			break;
	}

	return rc;
}

/*
 * Vytvori novou lokalni promennou a vrati non-terminal s indexem a datovym typem,
 * ktery bude odpovidat. Pokud nastane chyba, je vracena polozka typu T_END.
 */
static TItem set_nonterm_attributes(TRule *rule, TItem *return_item, TSymtable *global_st, TSymtable *local_st)
{
	TItem item = {.type = T_NONTERM};
	TItem err_item = {.type = T_END};

	if (rule->right[0].type == T_TERM) {
		// Non-terminal v zavorkach nebo pouze terminal
		if (rule->right[0].data.term.type == TOK_LBRACKET) {
			// Non-terminal v zavorce - je umisten na indexu 1 v pravidle
			// - nastane pouze "kopie" atributu mezi promennymi
			int index = symtable_insert_tmp(local_st);
			set_index_and_type(&item, rule->right[1].data.non_term.data_type, index);

			if (generate_mov(index, rule->right[1].data.non_term.index) != ERR_SUCCESS) {
				return err_item;
			}
		}
		else if (rule->right[0].data.term.type == TOK_IDENTIFIER &&
			symtable_func_exists(global_st, rule->right[0].data.term.attribute.ptr)) {

			// Terminal je "funkce" (byla volana funkce),
			// zjisti jeji navratovy typ a index, na kterem bude vysledek (ten je ulozen v return_item)
			TDataType data_type;
			if (symtable_get_func_rettype(global_st, rule->right[0].data.term.attribute.ptr, &data_type) != ERR_SUCCESS) {
				// Chyba - funkce neexistuje - nemelo by nastat...
				error(ROWONLY, "Semanticka analyza: Funkce %s nebyla deklarovana.\n", rule->right[0].data.term.attribute.ptr);
				return err_item;
			}

			// Zkontrolovat navratovy typ, musi byt ruzny od void
			if (data_type == TYPE_VOID) {
				error(ROWONLY, "Semanticka analyza: Funkci %s neni mozno volat ve vyrazu, protoze ma navratovy typ void.\n",
					rule->right[0].data.term.attribute.ptr);
				return err_item;
			}

			int index = symtable_insert_tmp(local_st);
			set_index_and_type(&item, data_type, index);

			if (generate_mov(index, return_item->data.non_term.index) != ERR_SUCCESS) {
				return err_item;
			}
		}
		else {
			// Pouze terminal (promenna nebo konstanta)
			TDataType data_type;
			int index;

			if (rule->right[0].data.term.type == TOK_IDENTIFIER) {
				// Identifikator
				// Zjisti datovy typ a index
				if (get_var_data(rule->right[0].data.term, &index, &data_type, global_st, local_st) != ERR_SUCCESS) {
					// Chyba
					return err_item;
				}

				// Uloz datovy typ a index
				set_index_and_type(&item, data_type, index);
			}
			else {
				// Konstanta
				if (get_const_data_type(rule->right[0].data.term, &data_type) != ERR_SUCCESS) {
					// Toto by nemelo nikdy nastat
					assert(0);
					exit(ERR_FAILURE);
				}

				// Ziskej index pro konstantu
				index = symtable_insert_tmp(local_st);

				// Uloz datovy typ a index
				set_index_and_type(&item, data_type, index);

				// Vygeneruj instrukci, ktera na danou pozici vlozi hodnotu, kterou konstanta mela
				switch (rule->right[0].data.term.type) {
					case TOK_CONSTINT:
						if (generate_load_int(index, rule->right[0].data.term.attribute.i) != ERR_SUCCESS) {
							return err_item;
						}
						break;
					case TOK_CONSTDOUBLE:
						if (generate_load_dbl(index, rule->right[0].data.term.attribute.d) != ERR_SUCCESS) {
							return err_item;
						}
						break;
					case TOK_CONSTSTRING:
						if (generate_load_str(index, rule->right[0].data.term.attribute.ptr) != ERR_SUCCESS) {
							return err_item;
						}
						break;
					default:
						// Toto by nemelo nikdy nastat
						assert(0);
						break;
				}
			}
		}
	}
	else {
		// Binarni operace
		// Indexy v pravidlech
		const int NONTERM1 = 0;
		const int OPERATOR = 1;
		const int NONTERM2 = 2;

		// Typova kontrola a vygenerovani instrukce, ktera provadi danou akci
		TOperations operation = token2operation(rule->right[OPERATOR].data.term);
		switch (operation) {
			// Operace definovane pouze pro int
			case OPER_DIVINT:
				if ((rule->right[NONTERM1].data.non_term.data_type == TYPE_INT) &&
					(rule->right[NONTERM2].data.non_term.data_type == TYPE_INT)) {
					// Splneno - oba operandy typu int
					int index = symtable_insert_tmp(local_st);
					set_index_and_type(&item, TYPE_INT, index);

					if (generate_binary(DIV, index, rule->right[NONTERM1].data.non_term.index,
						rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
						return err_item;
					}
				}
				else {
					// Semanticka chyba
					error(ROWONLY, "Semanticka analyza: Operace div je definovana pouze pro operandy typu int.\n");
					return err_item;
				}
				break;

			// Operace definovane pouze pro int a double
			// (pri operaci power se operandy pretypuji na double a vysledkem je double)
			case OPER_PLUS: // Toto je jedina pripustna operace pro string z tehle kategorie
			case OPER_POWER:
			case OPER_MINUS:
			case OPER_MULTIPLE:
			case OPER_DIVDOUBLE: {
				// Zjistit, jaka instrukce se bude vykonavat
				EInstruction instr_dbl;
				EInstruction instr_int;
				switch (operation) {
					default:
					case OPER_PLUS:
						instr_int = ADD;
						instr_dbl = ADD_DBL;
						break;
					case OPER_POWER:
						instr_dbl = POW_DBL;
						break;
					case OPER_MINUS:
						instr_int = SUB;
						instr_dbl = SUB_DBL;
						break;
					case OPER_MULTIPLE:
						instr_int = MUL;
						instr_dbl = MUL_DBL;
						break;
					case OPER_DIVDOUBLE:
						instr_dbl = DIV_DBL;
						break;
				}
				if ((rule->right[NONTERM1].data.non_term.data_type == TYPE_STRING) ||
					(rule->right[NONTERM2].data.non_term.data_type == TYPE_STRING)) {
					if (operation != OPER_PLUS) {
						// Semanticka chyba
						error(ROWONLY, "Semanticka analyza: Operace - * / ^ nejsou definovane pro operand(y) typu string.\n");
						return err_item;
					}
					else {
						// Konkatenace string + string
						int index = symtable_insert_tmp(local_st);
						set_index_and_type(&item, TYPE_STRING, index);

						if (generate_binary(CONCAT, index, rule->right[NONTERM1].data.non_term.index,
							rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
							return err_item;
						}
					}
				}
				else if (rule->right[NONTERM1].data.non_term.data_type == rule->right[NONTERM2].data.non_term.data_type) {
					if (rule->right[NONTERM1].data.non_term.data_type == TYPE_INT) {
						if ((operation == OPER_POWER ||	operation == OPER_DIVDOUBLE)) {
							// Mocnina nebo deleni - vysledkem je double
							// Musime prevest oba inty na double

							// Prevest 1. int na double
							int index_1 = symtable_insert_tmp(local_st);
							if (generate_int2dbl(index_1, rule->right[NONTERM1].data.non_term.index) != ERR_SUCCESS) {
								return err_item;
							}

							// Prevest 2. int na double
							int index_2 = symtable_insert_tmp(local_st);
							if (generate_int2dbl(index_2, rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
								return err_item;
							}

							int index = symtable_insert_tmp(local_st);
							set_index_and_type(&item, TYPE_DOUBLE, index);

							// Provest umocneni
							if (generate_binary(instr_dbl, index, index_1, index_2) != ERR_SUCCESS) {
								return err_item;
							}
						}
						else {
							// Oba operandy jsou typu int
							int index = symtable_insert_tmp(local_st);
							set_index_and_type(&item, rule->right[NONTERM1].data.non_term.data_type, index);

							// Jsou typu int
							if (generate_binary(instr_int, index, rule->right[NONTERM1].data.non_term.index,
								rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
								return err_item;
							}
						}
					}
					else {
						// Oba operandy jsou typu double
						int index = symtable_insert_tmp(local_st);
						set_index_and_type(&item, rule->right[NONTERM1].data.non_term.data_type, index);

						// Jsou typu double
						if (generate_binary(instr_dbl, index, rule->right[NONTERM1].data.non_term.index,
							rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
							return err_item;
						}
					}
				}
				else if (rule->right[NONTERM1].data.non_term.data_type == TYPE_INT) {
					// Prvni operand je typu int, preved ho na double
					int index_int2dbl = symtable_insert_tmp(local_st);
					if (generate_int2dbl(index_int2dbl, rule->right[NONTERM1].data.non_term.index) != ERR_SUCCESS) {
						return err_item;
					}

					int index = symtable_insert_tmp(local_st);
					set_index_and_type(&item, TYPE_DOUBLE, index);

					// Provest operaci
					if (generate_binary(instr_dbl, index, index_int2dbl,
						rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
						return err_item;
					}
				}
				else {
					// Druhy operand je typu int, preved ho na double
					int index_int2dbl = symtable_insert_tmp(local_st);
					if (generate_int2dbl(index_int2dbl, rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
						return err_item;
					}

					int index = symtable_insert_tmp(local_st);
					set_index_and_type(&item, TYPE_DOUBLE, index);

					// Provest operaci
					if (generate_binary(instr_dbl, index, rule->right[NONTERM1].data.non_term.index,
						index_int2dbl) != ERR_SUCCESS) {
						return err_item;
					}
				}
				break;
			}

			// Operace definovane pro int,double,string (operace + jako konkatenace je osetrena vyse)
			case OPER_LESS:
			case OPER_LESSEQUAL:
			case OPER_GREATER:
			case OPER_GREATEREQUAL:
			case OPER_EQUAL:
			case OPER_NOTEQUAL: {
				// Zjistit, jaka instrukce se bude vykonavat
				EInstruction instr_int;
				EInstruction instr_dbl;
				EInstruction instr_str;
				switch (operation) {
					default:
					case OPER_LESS:
						instr_int = CMP_LES;
						instr_dbl = CMP_LES_DBL;
						instr_str = CMP_LES_STR;
						break;
					case OPER_LESSEQUAL:
						instr_int = CMP_LES_EQ;
						instr_dbl = CMP_LES_EQ_DBL;
						instr_str = CMP_LES_EQ_STR;
						break;
					case OPER_GREATER:
						instr_int = CMP_GRE;
						instr_dbl = CMP_GRE_DBL;
						instr_str = CMP_GRE_STR;
						break;
					case OPER_GREATEREQUAL:
						instr_int = CMP_GRE_EQ;
						instr_dbl = CMP_GRE_EQ_DBL;
						instr_str = CMP_GRE_EQ_STR;
						break;
					case OPER_EQUAL:
						instr_int = CMP_EQU;
						instr_dbl = CMP_EQU_DBL;
						instr_str = CMP_EQU_STR;
						break;
					case OPER_NOTEQUAL:
						instr_int = CMP_NOT_EQ;
						instr_dbl = CMP_NOT_EQ_DBL;
						instr_str = CMP_NOT_EQ_STR;
						break;
				}
				// Vysledny datovy typ vsech porovnani je INT
				if (rule->right[NONTERM1].data.non_term.data_type == rule->right[NONTERM2].data.non_term.data_type) {
					// Oba operandy jsou stejneho typu

					int index = symtable_insert_tmp(local_st);
					set_index_and_type(&item, TYPE_INT, index);

					// Provest operaci
					if (rule->right[NONTERM1].data.non_term.data_type == TYPE_INT) {
						// Int
						if (generate_binary(instr_int, index, rule->right[NONTERM1].data.non_term.index,
							rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
							return err_item;
						}
					}
					else if (rule->right[NONTERM1].data.non_term.data_type == TYPE_DOUBLE) {
						// Double
						if (generate_binary(instr_dbl, index, rule->right[NONTERM1].data.non_term.index,
							rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
							return err_item;
						}
					}
					else {
						// String
						if (generate_binary(instr_str, index, rule->right[NONTERM1].data.non_term.index,
							rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
							return err_item;
						}
					}
				}
				else if ((rule->right[NONTERM1].data.non_term.data_type == TYPE_STRING) ||
						(rule->right[NONTERM2].data.non_term.data_type == TYPE_STRING)) {
						// Pouze jeden z operandu je string -> semanticka chyba
						error(ROWONLY, "Semanticka analyza: Operace + < > <= >= == != jsou definovane bud pro oba operandy typu string"
							", nebo oba ciselne.\n");
						return err_item;
				}
				else if (rule->right[NONTERM1].data.non_term.data_type == TYPE_INT) {
					// Prvni operand je typu int, preved ho na double a proved porovnani (instrukce)
					int index_int2dbl = symtable_insert_tmp(local_st);
					if (generate_int2dbl(index_int2dbl, rule->right[NONTERM1].data.non_term.index) != ERR_SUCCESS) {
						return err_item;
					}

					int index = symtable_insert_tmp(local_st);
					set_index_and_type(&item, TYPE_INT, index);

					// Provest operaci
					if (generate_binary(instr_dbl, index, index_int2dbl,
						rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
						return err_item;
					}
				}
				else {
					// Druhy operand je typu int, preved ho na double a proved porovnani (instrukce)
					int index_int2dbl = symtable_insert_tmp(local_st);
					if (generate_int2dbl(index_int2dbl, rule->right[NONTERM2].data.non_term.index) != ERR_SUCCESS) {
						return err_item;
					}

					int index = symtable_insert_tmp(local_st);
					set_index_and_type(&item, TYPE_INT, index);

					// Provest operaci
					if (generate_binary(instr_dbl, index, rule->right[NONTERM1].data.non_term.index,
						index_int2dbl) != ERR_SUCCESS) {
						return err_item;
					}
				}

			}
			break;
			default:
					// Jen pro debugging ucely, nemelo by nikdy nastat
					assert(0);
					fprintf(stderr, "Precedencni SA: Nedefinovana operace pri semanticke kontrole vyrazu.\n");
					exit(ERR_FAILURE);
		}

	}

	return item;
}

/*
 * Zameni 'handle' na zasobniku za levou stranu daneho pravidla (non-terminal).
 * Funkce vraci ERR_SUCCESS, pokud je operace proveditelna a sedi typy, jinak ERR_FAILURE.
 */
static TErrCode prec_stack_change_handle(TPrecStack *s, TItem *return_item, TSymtable *global_st, TSymtable *local_st)
{
	TRule rule = {.right = {{.type = T_END}}};
	int i = 0;

	while (i <= MAX_RULE_SIZE) {
		// Pokud je zasobnik prazdny, doslo k chybe
		if (prec_stack_empty(s))
			return ERR_FAILURE;

		if (s->top->item.type == T_ACT) {
			// Byl nalezen "znak <", ukonci pravou stranu pravidla
			TItem item = {.type = T_END};
			rule.right[i] = item;
			prec_stack_pop(s);
			break;
		}
		else {
			// Nalezen terminal/non-terminal, vloz jej do prave strany pravidla
			rule.right[i] = s->top->item;
			prec_stack_pop(s);
			i++;
		}
	}

	// Nyni je nutne prevratit pravidlo
	reverse_rule(&rule);

	int rule_number = get_rule_index(&rule);
	if (rule_number != 0) {
		// Prirad novemu non-terminalu typ a index (vyhrad novou lokalni promennou)
		// podle pravidla
		TItem item = set_nonterm_attributes(&rule, return_item, global_st, local_st);
		if (item.type != T_NONTERM) {
			// Nastala chyba
			return ERR_FAILURE;
		}

		// Uloz non-terminal na zasobnik
		prec_stack_push(s, item);

		return ERR_SUCCESS;
	}

	return ERR_FAILURE;
}

/*
 * Zrusi vsechny polozky v zasobniku
 */
static void prec_stack_clear(TPrecStack *s)
{
	while (!prec_stack_empty(s))
		prec_stack_pop(s);
}

#ifdef PREC_SA_DEBUG
/*
 * Vypise obsah zasobniku na chybovy vystup (pouze pro debugging ucely)
 */
static void prec_stack_print(const TPrecStack *s)
{
	struct prec_stack_item *cur_item = s->top;
	while ((cur_item != NULL)) {
		if (cur_item->item.type == T_TERM) {
			fprintf(stderr, " T: %d |", cur_item->item.data.term.type);
		}
		else if (cur_item->item.type == T_NONTERM) {
			fprintf(stderr, " NT: %d |", cur_item->item.data.non_term.non_term);
		}
		else {
			// Akce '<'
			fprintf(stderr, " SH |");
		}
		cur_item = cur_item->next;
	}

	fprintf(stderr, "\n");
}
#endif

/*
 * Funkce vraci true, pokud nastal uspesny konec SA
 */
static inline bool is_SA_end(Ttoken tok, Ttoken term)
{
	return ((((tok.type == TOK_EOF) ||
			(tok.type == TOK_SEMICOLON) ||
			(tok.type == TOK_GOTO) ||
			(tok.type == TOK_COMMA) ||
			(tok.type == TOK_RBRACKET))) && ((term.type == TOK_EOF)));
}

/*
 * Vola prcedencni syntaktickou analyzu a vraci 0, pokud probehlo vse v poradku, nebo nenulove cislo
 * v pripade chyby.
 * Popis parametru:
 *  stream - popisovac souboru se zdrojovym kodem
 *  phase - faze, ve ktere se SA zrovna nachazi
 *  global_st - globalni tabulka symbolu
 *  local_st - lokalni tabulka symbolu
 *  cur_token - aktualni token
 *  last_token - posledni nacteny token
 *  return_item - navratova hodnota z analyzy
 */
int precedence_SA(FILE *stream, TPhases *phase, TSymtable *global_st, TSymtable *local_st,
	Ttoken cur_token, Ttoken *last_token, TItem *return_item)
{
	Ttoken tok = cur_token; // Prave nacteny token
	Ttoken term;            // Terminal nejblize vrcholu zasobniku
	TPrecStack stack;       // Zasobnik na terminaly a nonterminaly (+ znak '<')
	TErrCode rc;            // Navratovy kod
	TItem ret_item;         // Navratovy "typ" (pozice a non-terminal) bude v return_item (potreba pri krizovem volani SA)

	// Inicializace zasobniku a vlozeni pocatecniho znaku
	prec_stack_init(&stack);
	TItem starting_item = {.type = T_TERM, .data.term = {.type = TOK_EOF}};
	prec_stack_push(&stack, starting_item);

	// Vlastni algoritmus precedencni SA
	do {
		// Ziskej terminal nejvyse na zasobniku
		term = prec_stack_top_term(&stack);

		#ifdef PREC_SA_DEBUG
			// Zapis aktualni token, terminal na zasobniku a polozku na danem indexu
			fprintf(stderr, "----------------------\n");
			fprintf(stderr, "Zasobnik: top>");
			prec_stack_print(&stack);
			fprintf(stderr, "Terminal na zasobniku:\n");
			print_token(term);
			fprintf(stderr, "Aktualni token:\n");
			print_token(tok);
			fprintf(stderr, "Akce: %d\n", PREC_TABLE[term.type][tok.type]);
			fprintf(stderr, "----------------------\n");
		#endif

		switch (PREC_TABLE[term.type][tok.type]) {
			// =
			case EQ: {
				// Vloz terminal (token) na zasobnik a proved push
				TItem tok_item = {.type = T_TERM, .data.term = tok};
				prec_stack_push(&stack, tok_item);

				rc = gnt_iist(stream, phase, global_st, local_st, &tok);
				if (rc != ERR_SUCCESS) {
					*last_token = tok;
					prec_stack_clear(&stack);
					return ERR_FAILURE;
				}
				term = prec_stack_top_term(&stack);
				break;
			}

			// < shift
			case SH:
				// Vloz specialni znak pred nejvrchenjsi terminal
				prec_stack_shift(&stack);
				TItem tok_item = {.type = T_TERM, .data.term = tok};
				prec_stack_push(&stack, tok_item);

				rc = gnt_iist(stream, phase, global_st, local_st, &tok);
				if (rc != ERR_SUCCESS) {
					*last_token = tok;
					prec_stack_clear(&stack);
					return ERR_FAILURE;
				}
				term = prec_stack_top_term(&stack);
				break;

			// > redukce
			case RE: {
				// Zamen vrchol zasobniku za handle
				rc = prec_stack_change_handle(&stack, &ret_item, global_st, local_st);
				term = prec_stack_top_term(&stack);

				if (rc != ERR_SUCCESS) {
					// Nenalezeno pravidlo
					*last_token = tok;
					prec_stack_clear(&stack);
					return ERR_FAILURE;
				}

				// Pozn.: instrukce se generuji pri zmene handelu
			}
			break;

			// Volani funkce
			case FU: {
				Ttoken end_token = {.type = TOK_RBRACKET};
				TNonTerm starting_nonterm = NT_FUNC_CALL;

				#ifdef PREC_SA_DEBUG
					fprintf(stderr, "Volam prediktivni SA:\n");
				#endif

				// Posledni nacteny token bude ulozen v tok (pro pripad chyby)
				// ret_item bude obsahovat index, kde se bude nachazet vysledek
				if (predictive_SA(stream, phase, global_st, local_st, tok, end_token, &tok, starting_nonterm,
					&ret_item, get_last_called_func_name()) != ERR_SUCCESS) {
					// Chyba
					*last_token = tok;
					prec_stack_clear(&stack);
					return ERR_FAILURE;
				}

				rc = gnt_iist(stream, phase, global_st, local_st, &tok);
				if (rc != ERR_SUCCESS) {
					*last_token = tok;
					prec_stack_clear(&stack);
					return ERR_FAILURE;
				}
				term = prec_stack_top_term(&stack);

				#ifdef PREC_SA_DEBUG
					fprintf(stderr, "Navrat z prediktivni SA - vracena polozka - typ: %d index: %d\n",
						ret_item.data.non_term.data_type, ret_item.data.non_term.index);
				#endif

				break;
			}

			// Zadna akce - chyba
			case NA:
				// Chyba
				*last_token = tok;
				prec_stack_clear(&stack);
				return ERR_FAILURE;
		}

	} while (!is_SA_end(tok, term));

	// Uspech SA - umisti konecny non-terminal na zasobniku do navratove hodnoty
	*return_item = prec_stack_top_nonterm(&stack);

	prec_stack_clear(&stack);
	*last_token = tok;
    return ERR_SUCCESS;
}

/* Konec souboru precedence_sa.c */
