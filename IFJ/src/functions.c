/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		functions.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Vestavene funkce, ktere jsou primo vyuzitelne v interpretu
******************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>  // isalpha, isalnum, isdigit, isspace
#include <errno.h>  // detekce preteceni
#include <math.h>
#include <limits.h>
#include "debug.h"
#include "data_type.h"
#include "functions.h"
#include "strptr_stack.h"
#include "symbol_table.h"
#include "code_list.h"
#include "scanner.h"
#include "generator.h"
#include "error.h"

/*
 * Identifikatory vlozenych funkci - indexy koresponduji s EFunction
 */
static const char *BUILT_IN_FUNCS[] = {
	[INT2STR]     = "int2str",
	[DOUBLE2STR]  = "double2str",
	[STR2DOUBLE]  = "str2double",
	[DOUBLE2INT]  = "double2int",
	[PRINTINT]    = "printint",
	[PRINTDOUBLE] = "printdouble",
	[PRINTSTRING] = "printstring",
	[READINT]     = "readint",
	[READDOUBLE]  = "readdouble",
	[READSTRING]  = "readstring",
	[STRLEN]      = "strlen",
	[SORT]        = "sort"
};

/*
 * Argument(y) predavane funkci pri "definici" - nutne pouze z duvodu
 * zachovani kompatibility s vkladanim uzivatelskych funkci - nazev nehraje roli.
 */
static const char *arg1 = "v";

/*
 * Blok dat, s kterym pracuji fce readstring, readint a readdouble
 */
const int B_INCREMENT = 16;

/* Funkce na vlozeni tela built-in funkce do kodu */
TErrCode insert_builtin_function (EFunction funcID)
{
	TInstructionParam a, b, c;
	a.IntVal = funcID;
	b.PtrVal = c.PtrVal = NULL;
	return generate_instr(RUN_STD, a, b, c);
}

/*
 * Vlozi vsechny vestavene funkce do globalni tabulky symbolu.
 */
TErrCode insert_all_builtin_functions(TSymtable *global)
{

	TSymtable *tmp = symtable_init();

	// int2str
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[INT2STR], TYPE_STRING, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_insert_arg(tmp, arg1, TYPE_INT) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(INT2STR) != ERR_SUCCESS)
		return ERR_FAILURE;


	// double2str
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[DOUBLE2STR], TYPE_STRING, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_insert_arg(tmp, arg1, TYPE_DOUBLE) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(DOUBLE2STR) != ERR_SUCCESS)
		return ERR_FAILURE;

	// str2double
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[STR2DOUBLE], TYPE_DOUBLE, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_insert_arg(tmp, arg1, TYPE_STRING) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(STR2DOUBLE) != ERR_SUCCESS)
		return ERR_FAILURE;

	// double2int
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[DOUBLE2INT], TYPE_INT, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_insert_arg(tmp, arg1, TYPE_DOUBLE) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(DOUBLE2INT) != ERR_SUCCESS)
		return ERR_FAILURE;

	// printint
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[PRINTINT], TYPE_VOID, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_insert_arg(tmp, arg1, TYPE_INT) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(PRINTINT) != ERR_SUCCESS)
		return ERR_FAILURE;

	// printdouble
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[PRINTDOUBLE], TYPE_VOID, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_insert_arg(tmp, arg1, TYPE_DOUBLE) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(PRINTDOUBLE) != ERR_SUCCESS)
		return ERR_FAILURE;

	// printstring
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[PRINTSTRING], TYPE_VOID, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_insert_arg(tmp, arg1, TYPE_STRING) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(PRINTSTRING) != ERR_SUCCESS)
		return ERR_FAILURE;

	// readint
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[READINT], TYPE_INT, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(READINT) != ERR_SUCCESS)
		return ERR_FAILURE;

	// readdouble
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[READDOUBLE], TYPE_DOUBLE, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(READDOUBLE) != ERR_SUCCESS)
		return ERR_FAILURE;

	// readstring
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[READSTRING], TYPE_STRING, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(READSTRING) != ERR_SUCCESS)
		return ERR_FAILURE;

	// strlen
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[STRLEN], TYPE_INT, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_insert_arg(tmp, arg1, TYPE_STRING) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(STRLEN) != ERR_SUCCESS)
		return ERR_FAILURE;

	// sort
	if (symtable_start_insert_func(global, tmp, BUILT_IN_FUNCS[SORT], TYPE_STRING, true) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_insert_arg(tmp, arg1, TYPE_STRING) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_func_declar(tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	if (symtable_stop_insert_func(global, tmp) != ERR_SUCCESS)
		return ERR_FAILURE;
	// vlozeni tela do kodu
	if (insert_builtin_function(SORT) != ERR_SUCCESS)
		return ERR_FAILURE;

 	return symtable_free(tmp);
}

/*
 * Prevede hodnotu typu int na string.
 */
char *int2str(int num)
{
	// Zjistim si potrebnou delku retezce postupnym delenim cisla 10
	int tmp_num = num;
	int digits_count = (num < 0) ? 2 : 1; // (2 je kvuli znamenku)
	while (tmp_num != 0) {
		tmp_num /= 10;
		digits_count++;
	}

	// Alokace pameti pro vysledny retezec
	char *str = strmalloc(sizeof(char) * (digits_count + 1));
	if (str == NULL) {
		error(RAW, "int2str: Nedostatek pameti\n");
		return NULL;
	}

	if (num != 0) {
		snprintf(str, digits_count, "%d", num);
	}
	else {
		// Nedokonalost snprintf() - "nevytiskne" 0
		str[0] = '0';
		str[1] = '\0';
	}
	return str;
}

/*
 * Prevede hodnotu typu double na string.
 */
char *double2str(double num)
{
	// Maximalni delka retezce - omezime na 30 znaku
	const int MAX_LENGTH = 30;

	// Alokace pameti pro vysledny retezec
	char *str = strmalloc(sizeof(char) * (MAX_LENGTH + 1));
	if (str == NULL) {
		error(RAW, "double2str: Nedostatek pameti\n");
		return NULL;
	}

	snprintf(str, MAX_LENGTH, "%g", num);
	return str;
}

/*
 * Prevede retezec na double.
 */
TErrCode str2double(const char * str, double *ret)
{
	// Ochrana proti "neinicializovanemu" retezci
	if (str == NULL)
		return 0.0;

	errno = 0;
	*ret = atof(str);

	// Test na preteceni
	if (errno == ERANGE) {
		error(RAW, "str2double: Preteceni [%s]\n", str);
		return ERR_RUNTIME;
	}
	else {
		return ERR_SUCCESS;
	}
}

/*
 * Prevede hodnotu typu double na int s vyuzitim funkce trunc.
 */
TErrCode double2int(double num, int *result)
{
	assert(result != NULL);

	// Test na preteceni - podteceni
	if (num > (double) INT_MAX || num < (double) INT_MIN)
	{
		error(RAW, "double2int: Nelze prevest %g na int - hodnota je mimo rozsah\n", num);
		return ERR_RUNTIME;
	}

	*result = trunc(num);
	return ERR_SUCCESS;
}

/*
 * Vypise hodnotu typu int na vystup.
 */
void printint(int num)
{
	printf("%d", num);
}


/*
 * Vypise hodnotu typu double na vystup.
 */
void printdouble(double num)
{
	printf("%g", num);
}

/*
 * Vypise hodnotu typu string na vystup.
 * Pravidla:
 * Rozpoznava escape sekvence '\n', '\\', '\"', '\$'
 * Ignoruje '$', ostatni escape sekvence
 * Jiny znak tiskne
 */
TErrCode printstring(const char *str)
{
	// Ochrana proti "neinicializovanemu" retezci
	if (str == NULL)
		return ERR_SUCCESS;

	while (*str != '\0') {
		switch(*str)
		{
			case '\\': /* escape sekvence. */
				/* nyni se muzeme podivat na dalsi znak, nebot scanner */
				/* nelegalni konstrukci typu "retezec\" nepropusti a */
				/* dalsi znak muze byt jen 'n', '\' '"' nebo '$' */
				str++;
				switch(*str)
				{
					case 'n':
						putchar('\n');
						break;
					case '\\':
						putchar('\\');
						break;
					case '\"':
						putchar('\"');
						break;
					case '$':
						putchar('$');
						break;
					default:
						/* neznamy escape znak - chyba */
						error(RAW, "printstring: Neznama escape sekvence - \\%c\n", *str);
						return ERR_RUNTIME;
						break;
				}
				break;
			case '$':  /* netiskuntelny znak. */
				break;
			default:   /* ostatni tiskuntelne znaky. */
				putchar(*str);
				break;
		}

		str++; /* posunuti na dalsi znak. */
	}

	return ERR_SUCCESS;
}

/*
 * Funkce zaradi znak do retezce - hlida meze a zvetsuje.
 * Muze skoncit chybou pri realokaci - !pozor ukazatel na ukazatel!
 */
static inline TErrCode pushchar(char **str, int *i, int *block_size, char ch)
{
	// ulozi znak do retezce
	(*str)[(*i)] = ch;
	(*i)++;

	if (!((*i) < (*block_size))) {
		// REALLOC - Jsme na konci bloku, musime ho rozsirit
		(*block_size) += B_INCREMENT;
		*str = realloc(*str, (*block_size));
		if (!(*str)) {
			error(RAW, "readint/readdouble: Nedostatek pameti\n");
			return ERR_FAILURE;
		}
	}
	(*str)[(*i)] = '\0';

	return ERR_SUCCESS;
}

/*
 * Nacte ze vstupu hodnotu typu int.
 */
TErrCode readint(int *num)
{
	int block_size = B_INCREMENT;
	int c, i = 0; // pomocny znak a index v retezci

	// zahodime bile znaky na zacatku
	while ( isspace(c = getchar()) );

	// vrat nebily znak
	ungetc(c, stdin);

	// Alokujeme si pocatecni mnozstvi pameti pro retezec
	// retezec musi byt pri navartu z fce uvolnen
	char *str = malloc(block_size * sizeof(char));
	if (str == NULL) {
		error(RAW, "readint: Nedostatek pameti\n");
		return ERR_RUNTIME;
	}

	// pamatuj si posledni index, tim zjistime, zda jsem neco nacetli
	int oldi = i;
	while((c = getchar()) != EOF && ismydigit(c)) {
		// ulozi cislici do retezce
		if (pushchar(&str, &i, &block_size, c) == ERR_FAILURE)
			return ERR_RUNTIME; //chyba realloc=uvolneno
	}
	// vrat naposledny nacteny znak
	ungetc(c,stdin);


	//nacetli jsme alespon jednu cislici?
	if (oldi == i) { //zadna cislice
		error(RAW, "readint: Neplatne cislo int - nezacina cislici\n");
		free(str);
		return ERR_RUNTIME;
	}
	//musime provest prevod, jedina chyba muze byt preteceni
	errno = 0;
	long l = strtol(str, NULL, 10);

	// Test na preteceni
	if (errno == ERANGE || l > INT_MAX || l < INT_MIN) {
		error(RAW, "readint: Preteceni [%s]\n", str);
		free(str);
		return ERR_RUNTIME;
	}
	else {
		*num = (int) l;
		free(str);
		return ERR_SUCCESS;
	}
}

/*
 * Bezpecna funkce atof, kontroluje pretecni.
 */
static inline TErrCode atof_check_free(double *d, char *str)
{
	errno = 0;

	(*d) = strtod(str, (char **)NULL);

	// Test na preteceni
	if (errno == ERANGE) {
		error(RAW, "readdouble: Preteceni [%s]\n", str);
		free(str);
		return ERR_RUNTIME;
	}
	else {
		free(str);
		return ERR_SUCCESS;
	}
}

/*
 * Nacte ze vstupu hodnotu typu double.
 */
TErrCode readdouble(double *num)
{
	int block_size = B_INCREMENT;
	int c, i = 0; // pomocny znak a index v retezci

	// zahodime bile znaky na zacatku
	while ( isspace(c = getchar()) );

	//vrat nebily znak
	ungetc(c, stdin);

	// Alokujeme si pocatecni mnozstvi pameti pro retezec
	// retezec musi byt pri navartu z fce uvolnen
	char *str = malloc(block_size * sizeof(char));
	if (str == NULL) {
		error(RAW, "readdouble: Nedostatek pameti\n");
		return ERR_RUNTIME;
	}

	//pamatuj si posledni index, tim zjistime, zda jsem neco nacetli
	int oldi = i;
	while((c = getchar()) != EOF && ismydigit(c)) {
		// ulozi cislici do retezce
		if (pushchar(&str, &i, &block_size,c) == ERR_FAILURE)
			return ERR_FAILURE; //chyba realloc=uvolneno
	}
	// vrat naposledny nacteny znak
	ungetc(c,stdin);

	//nacetli jsme alespon jednu cislici?
	if (oldi == i) { //zadna cislice
		error(RAW, "readdouble: Neplatne cislo double - nezacina cislici\n");
		free(str);
		return ERR_RUNTIME;
	}
	//[0-9]+
	//nyni musi prijit tecka
	c = getchar();
	if (c != '.') {
		error(RAW, "readdouble: Neplatne cislo double - za cislicemi neni '.'\n");
		free(str);
		return ERR_RUNTIME;
	}
	if (pushchar(&str, &i, &block_size, c) == ERR_FAILURE)
		return ERR_FAILURE; //str je jiz uvolneno
	//[0-9]+\.
	//nyni _muze_ prijit [0-9]+ volitelna cast
	oldi = i;
	while((c = getchar()) != EOF && ismydigit(c)) {
		// ulozi cislici do retezce
		if (pushchar(&str, &i, &block_size,c) == ERR_FAILURE)
			return ERR_FAILURE; //chyba realloc=uvolneno
	}
	// vrat naposledny nacteny znak
	ungetc(c,stdin);

	//nacetli jsme nejakou volitelnou cislici?
	if (oldi == i) //ne - cislo [0-9]+\. je legalni -> preved
		return atof_check_free(num, str);

	//[0-9]+\.[0-9]+
	//nyni _muze_ prijit 'e'
	c = getchar();
	if (c != 'e') {
		//neni -> vrat a preved
		ungetc(c, stdin);
		return atof_check_free(num, str);
	}
	if (pushchar(&str, &i, &block_size, c) == ERR_FAILURE)
		return ERR_FAILURE; //str je jiz uvolneno
	//[0-9]+\.[0-9]+e
	//nyni jiz _musi_ prijit [0-9] nebo +-[0-9]
	//zkusime nacist nejdrive +-
	c = getchar();
	if (c == '+' || c == '-') {
		//volitelna cast, zarad
		if (pushchar(&str, &i, &block_size, c) == ERR_FAILURE)
			return ERR_FAILURE; //str je jiz uvolneno
	}
	else {
		//+- to nebylo -> vrat
		ungetc(c, stdin);
	}
	//[0-9]+\.[0-9]+e[+-]?
	//nyni _musi_ prijit [0-9]+
	oldi = i;
	while((c = getchar()) != EOF && ismydigit(c)) {
		// ulozi cislici do retezce
		if (pushchar(&str, &i, &block_size,c) == ERR_FAILURE)
			return ERR_FAILURE; //chyba realloc=uvolneno
	}
	// vrat naposledny nacteny znak
	ungetc(c,stdin);

	//nacetli jsme alespon jednu cislici?
	if (oldi == i) { //zadna cislice
		error(RAW, "readdouble: Neplatne cislo double - za exponentem neni cislice\n");
		free(str);
		return ERR_RUNTIME;
	}
	//[0-9]+\.[0-9]+e[+-]?[0-9]+ je legalni, zkus prevest
	return atof_check_free(num, str);
}

/*
 * Nacte ze vstupu hodnotu typu string.
 */
char *readstring(void)
{
	int block_size = B_INCREMENT;
	int c, i = 0; // pomocny znak a index v retezci

	// zahodime bile znaky na zacatku
	while ( isspace(c = getchar()) );

	// kontrola zda zaciname uvozovkami
	int inString = (c  == '\"');
	if (inString == false) { // nenacteny uvozovky -> neplatny vstup
		// pripadne ungetc()
		error(RAW, "readstring: Neplatny retezec - nezacina znakem \"\n");
		return NULL;
	}


	// Alokujeme si pocatecni mnozstvi pameti pro retezec
	// retezec zatim nedavam do zasobniku retezcu - muze byt jeste reallokovan
	char *str = malloc(block_size * sizeof(char));
	if (str == NULL) {
		error(RAW, "readstring: Nedostatek pameti\n");
		return NULL;
	}

	while ((c = getchar()) != EOF)	{
		// zkontroluje zda nezacina escape sekvence
		if (c == '\\')
		{ 	/* escape sekvence. ulozi znak '\\' */
			/* pak nacte a zkontroluje dalsi znak a pokud jde o platnou escape sekvenci,
			 * pokracuje dale (a ulozi znak escape sekvence) */
			str[i] = '\\';
			i++;
			c = getchar();
			switch(c)
			{
				case 'n':
				case '\\':
				case '\"':
				case '$':
					// definovane escape sekvence nechame projit dale (a ulozit)
					break;
				default:
					/* neznamy escape znak -chyba */
					free(str);
					error(RAW, "readstring: Retezec obsahuje nepovolenou escape sekvenci (\\%c)\n",c);
					return NULL;
					break;
			}
		}
		else // kontrola zda nacetl uvozovky - konec retezce
		{
			if (c == '\"' && str[i-1] != '\\') {
				// nacteny konecne uvozovky - vyskoci z cyklu
				inString = false;
				break;
			}
		}

		// kontrola ASCII hodnoty nacteneho znaku (musi byt vetsi nebo rovna 32,
		// tomu odpovida znak ' ' - mezera)
		if (c < ' ') {
			free(str);
			error(RAW, "readstring: Retezec obsahuje nepovoleny znak (ASCII %d)\n",c);
			return NULL;
		}

		// ulozi znak do retezce
		str[i] = c;
		i++;

		if (!(i < block_size)) {
			// REALLOC - Jsme na konci bloku, musime ho rozsirit
			block_size += B_INCREMENT;
			str = realloc(str, block_size);
			if (!str) {
				error(RAW, "readstring: Nedostatek pameti\n");
				return NULL;
				// pozn.: osetreni mozneho memory-leaku mi prijde zbytecne
			}
		}
	}

	// zkontrolujeme zda byl retezec ukoncen
	if (inString == true) { // stale jsme v retezci - neukoncen '"'
		free(str);
		error(RAW, "readstring: Neukonceny retezec - nekonci znakem \"\n");
		return NULL;
	}

	// Retezec je treba radne ukoncit a pridat do zasobniku retezcu
	str[i] = '\0';
	strptr_stack_push(str);

 	return str;
}

/*
 * Vraci delku zadaneho retezce.
 */
int bstrlen(const char *str)
{
	// Ochrana proti "neinicializovanemu" retezci
	if (str == NULL)
		return 0;

	int str_length = 0;
	bool inEscape = false; // zda jsem v escape sekvenci
	while (*str) // dokud nenarazi na '\0'
	{
		if (inEscape) { // pokud jsem v escape sekvenci, zapocitam ji jako znak
			str_length++;
			inEscape = false;
		}
		else if (*str == '\\') { // pri \ pouze vejde do escape sekvence, nepocita znak
			inEscape = true;
		}
		else if (*str == '$') {
			// znak $ mimo escape sekvenci nepocitam
		}
		else { // jinak pouze pricti dalsi znak
			str_length++;
		}
		str++;
	}

	return str_length;
}

/* Konec souboru functions.c */
