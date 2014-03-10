/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		generator.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Generator instrukci - implementace.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"
#include "generator.h"
#include "symbol_table.h"
#include "error.h"

/*
 * Vygeneruje a zaradi novou instrukci podle zadanych parametru. Pokud nema pamet, vraci ERR_FAILURE
 */
TErrCode generate_instr(EInstruction type, TInstructionParam a, TInstructionParam b, TInstructionParam c)
{
	// naalokuje novou instrukci
	TMicroInstruction *new = malloc(sizeof(TMicroInstruction));
	if (new == NULL) {
		error(RAW, "generator: Nedostatek pameti pro vytvoreni instrukce!\n");
		return ERR_FAILURE;
	}

	// urci jeji typ (enum)
	new->type = type;
	// parametry A a C jsou jen integery
	new->a.IntVal = a.IntVal;
	new->c.IntVal = c.IntVal;
	// parametr B je kompletni unie (vcetne double)
	new->b = b;
	// zaradi instrukci do kodu
	code_list_add_instr(new);

	return ERR_SUCCESS;
}

/*
 * Vygenerovani instrukci pro binarni operace + - * / ^ div <= >= < > != ==
 */
TErrCode generate_binary(EInstruction type, int dest, int src1, int src2)
{
	TInstructionParam a = {.IntVal = dest};
	TInstructionParam b = {.IntVal = src1};
	TInstructionParam c = {.IntVal = src2};

	return generate_instr(type, a, b, c);
}


/*
 * Vygenerovani instrukce: PUSH
 */
TErrCode generate_push(int variable)
{
	TInstructionParam a = {.IntVal = variable};
	TInstructionParam b = {.PtrVal = NULL};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(PUSH, a, b, c);
}

/*
 * Vygenerovani instrukce: CALL
 */
TErrCode generate_call(TMicroInstruction *address)
{
	TInstructionParam a = {.PtrVal = NULL};
	TInstructionParam b = {.PtrVal = address};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(CALL, a, b, c);
}

/*
 * Vygenerovani instrukce: ALLOC
 */
TErrCode generate_alloc(int locals, int arguments)
{
	TInstructionParam a = {.IntVal = locals};
	TInstructionParam b = {.IntVal = arguments};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(ALLOC, a, b, c);
}

/*
 * Vygenerovani instrukce: RETURN
 */
TErrCode generate_return(int value)
{
	TInstructionParam a = {.IntVal = value};
	TInstructionParam b = {.PtrVal = NULL};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(RETURN, a, b, c);
}

/*
 * Vygenerovani instrukce: ERROR
 */
TErrCode generate_error(void)
{
	TInstructionParam a = {.PtrVal = NULL};
	TInstructionParam b = {.PtrVal = NULL};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(ERROR, a, b, c);
}

/*
 * Vygenerovani instrukce: RUN_STD
 */
TErrCode generate_run_std(EFunction function)
{
	TInstructionParam a = {.IntVal = function};
	TInstructionParam b = {.PtrVal = NULL};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(RUN_STD, a, b, c);
}

/*
 * Vygenerovani instrukce: GOTO
 */
TErrCode generate_goto(TMicroInstruction *label)
{
	TInstructionParam a = {.PtrVal = NULL};
	TInstructionParam b = {.PtrVal = label};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(GOTO, a, b, c);
}

/*
 * Vygenerovani instrukce: GOTO_IF
 */
TErrCode generate_goto_if(int condition_index, TMicroInstruction *label)
{
	TInstructionParam a = {.IntVal = condition_index};
	TInstructionParam b = {.PtrVal = label};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(GOTO_IF, a, b, c);
}

/*
 * Vygenerovani instrukce: MOV
 */
TErrCode generate_mov(int dest, int src)
{
	TInstructionParam a = {.IntVal = dest};
	TInstructionParam b = {.IntVal = src};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(MOV, a, b, c);
}

/*
 * Vygenerovani instrukce: LOAD_INT
 */
TErrCode generate_load_int(int dest, int value)
{
	TInstructionParam a = {.IntVal = dest};
	TInstructionParam b = {.IntVal = value};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(LOAD_INT, a, b, c);
}

/*
 * Vygenerovani instrukce: LOAD_DBL
 */
TErrCode generate_load_dbl(int dest, double value)
{
	TInstructionParam a = {.IntVal = dest};
	TInstructionParam b = {.DblVal = value};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(LOAD_DBL, a, b, c);
}

/*
 * Vygenerovani instrukce: LOAD_STR
 */
TErrCode generate_load_str(int dest, char *value)
{
	TInstructionParam a = {.IntVal = dest};
	TInstructionParam b = {.StrVal = value};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(LOAD_STR, a, b, c);
}

/*
 * Vygenerovani instrukce: INT2DBL
 */
TErrCode generate_int2dbl(int dest, int src)
{
	TInstructionParam a = {.IntVal = dest};
	TInstructionParam b = {.IntVal = src};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(INT2DBL, a, b, c);
}

/*
 * Vygenerovani instrukce: DBL2INT
 */
TErrCode generate_dbl2int(int dest, int src)
{
	TInstructionParam a = {.IntVal = dest};
	TInstructionParam b = {.IntVal = src};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(DBL2INT, a, b, c);
}

/*
 * Vygenerovani instrukce: STR2BOOL
 */
TErrCode generate_str2bool(int dest, int src)
{
	TInstructionParam a = {.IntVal = dest};
	TInstructionParam b = {.IntVal = src};
	TInstructionParam c = {.PtrVal = NULL};

	return generate_instr(STR2BOOL, a, b, c);
}

/*
 * Vloží do kodu danou instrukci vytvořenou v synaktické analýze. (pocatek funkce)
 */
inline void generator_ins_inst(TMicroInstruction *inst)
{
	code_list_add_instr(inst);
}

/*
 * Upraví paramery instrukce startu funkce.
 */
inline void generator_edit_head_func(TMicroInstruction *inst, int local, int arg)
{
	inst->a.IntVal = local;
	inst->b.IntVal = arg;
}


/*
 * Naalokuje novou instrukci a vlozi do ni instrukci ALLOC (pocatecni instrukce funkce)
 * ostatni parametry neinicializuje!
 */
inline TMicroInstruction * generate_new_func(void)
{
	TMicroInstruction *new = malloc(sizeof(TMicroInstruction));
	if (new != NULL)
		new->type = ALLOC;
	else
		error(RAW, "generator: Nedostatek pameti!\n");
	return new;
}

/*
 * Naalokuje novou instrukci a vlozi do ni instrukci LABEL
 * ostatni parametry neinicializuje!
 */
TMicroInstruction * generate_new_label(void)
{
	TMicroInstruction *new = malloc(sizeof(TMicroInstruction));
	if (new != NULL)
		new->type = LABEL;
	else
		error(RAW, "generator: Nedostatek pameti!\n");
	
	return new;
}

/*
 * Vloží do kódu startovací mikroinstrukci.
 */
TErrCode generate_start_point(TMicroInstruction *imain, int global_var) {
	EInstruction type = INIT;
	TInstructionParam a, b, c;
 	a.IntVal = global_var;
	b.PtrVal = imain;
	c.PtrVal = NULL;
	return generate_instr (type, a, b, c);
}

/* Konec souboru generator.c */
