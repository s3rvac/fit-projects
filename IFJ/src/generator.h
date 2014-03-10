/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		generator.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Generator instrukci - rozhrani.
******************************************************************************/

#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "instructions.h"
#include "functions.h"
#include "code_list.h"
#include "error.h"

/*
 * Vygeneruje a zaradi novou instrukci podle zadanych parametru. Pokud nema pamet, vraci ERR_FAILURE
 */
TErrCode generate_instr(EInstruction type, TInstructionParam a, TInstructionParam b, TInstructionParam c);

/*
 * Vygenerovani instrukci pro binarni operace + - * / ^ div <= >= < > != ==
 */
TErrCode generate_binary(EInstruction type, int dest, int src1, int src2);

/*
 * Vygenerovani instrukce: PUSH
 */
TErrCode generate_push(int variable);

/*
 * Vygenerovani instrukce: CALL
 */
TErrCode generate_call(TMicroInstruction *address);

/*
 * Vygenerovani instrukce: ALLOC
 */
TErrCode generate_alloc(int locals, int arguments);

/*
 * Vygenerovani instrukce: RETURN
 */
TErrCode generate_return(int value);

/*
 * Vygenerovani instrukce: ERROR
 */
TErrCode generate_error(void);

/*
 * Vygenerovani instrukce: RUN_STD
 */
TErrCode generate_run_std(EFunction function);

/*
 * Vygenerovani instrukce: GOTO
 */
TErrCode generate_goto(TMicroInstruction *label);

/*
 * Vygenerovani instrukce: GOTO_IF
 */
TErrCode generate_goto_if(int condition_index, TMicroInstruction *label);

/*
 * Vygenerovani instrukce: MOV
 */
TErrCode generate_mov(int dest, int src);

/*
 * Vygenerovani instrukce: LOAD_INT
 */
TErrCode generate_load_int(int dest, int value);

/*
 * Vygenerovani instrukce: LOAD_DBL
 */
TErrCode generate_load_dbl(int dest, double value);

/*
 * Vygenerovani instrukce: LOAD_STR
 */
TErrCode generate_load_str(int dest, char *value);

/*
 * Vygenerovani instrukce: INT2DBL
 */
TErrCode generate_int2dbl(int dest, int src);

/*
 * Vygenerovani instrukce: DBL2INT
 */
TErrCode generate_dbl2int(int dest, int src);

/*
 * Vygenerovani instrukce: STR2BOOL
 */
TErrCode generate_str2bool(int dest, int src);

/*
 * Vloží do kodu danou instrukci vytvořenou v synaktické analýze. (pocatek funkce)
 */
inline void generator_ins_inst(TMicroInstruction *inst);


/*
 * Upraví paramery instrukce startu funkce.
 */
inline void generator_edit_head_func(TMicroInstruction *inst, int local, int arg);


/*
 * Naalokuje novou instrukci a vlozi do ni instrukci ALLOC (pocatecni instrukce funkce)
 * ostatni parametry neinicializuje!
 */
inline TMicroInstruction * generate_new_func(void);

/*
 * Naalokuje novou instrukci a vlozi do ni instrukci LABEL
 * ostatni parametry neinicializuje!
 */
inline TMicroInstruction * generate_new_label(void);

/*
 * Vloží do kódu startovací mikroinstrukci.
 */
TErrCode generate_start_point(TMicroInstruction *imain, int global_var);

#endif /* #ifndef _GENERATOR_H_ */

/* Konec souboru generator.h */
