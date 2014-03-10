/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		token_debug.c
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Debugging informace/funkce pro tokeny - implementace
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "token_debug.h"

/* Maximalni delka nazvu tokenu */
static const size_t MAX_TOKEN_NAME_LENGTH = 16;

/* Retezce obsahujici nazvy tokenu pro debugging ucely. */
static const char *token_name[] = {
	"TOK_INT",
	"TOK_DOUBLE",
	"TOK_STRING",
	"TOK_VOID",
	"TOK_VAR",
	"TOK_GOTO",
	"TOK_IF",
	"TOK_RETURN",
	"TOK_DIVINT",
	"TOK_IDENTIFIER",
	"TOK_COMMA",
	"TOK_SEMICOLON",
	"TOK_LABEL",
	"TOK_ASSIGNMENT",
	"TOK_POWER",
	"TOK_PLUS",
	"TOK_MINUS",
	"TOK_MULTIPLE",
	"TOK_DIVDOUBLE",
	"TOK_LESS",
	"TOK_LESSEQUAL",
	"TOK_GREATER",
	"TOK_GREATEREQUAL",
	"TOK_EQUAL",
	"TOK_NOTEQUAL",
	"TOK_LBRACKET",
	"TOK_RBRACKET",
	"TOK_LVINCULUM",
	"TOK_RVINCULUM",
	"TOK_CONSTSTRING",
	"TOK_CONSTINT",
	"TOK_CONSTDOUBLE",
	"TOK_EOF",
	"TOK_SCANNERERROR"
};

/*
 * Textove ekvivalenty tokenu jako je ;,:= atd.
 * Indexem je typ tokenu z token.h
 */
static const char *token_symbols[] = {
	// Klicova slova
	"int",        //"int"
	"double",     //"double"
	"string",     //"string"
	"void",       //"void"
	"var",        //"var"
	"goto",       //"goto"
	"if",         //"if"
	"return",     //"return"
	"div",        //"div"
	// Ostatni tokeny
	"",
	",",  //','
	";",  //';'
	":",  //':'
	":=", //':''='
	"^",  //'^'
	"+",  //'+'
	"-",  //'-'
	"*",  //'*'
	"/",  //'/'
	"<",  //'<'
	"<=", //'<''='
	">",  //'>'
	">=", //'>''='
	"==", //'=''='
	"!=", //'!''='
	"(",  //'('
	")",  //')'
	"{",  //'{'
	"}",  //'}'
	"",
	"",
	"",
	"EOF",  // Signalizuje konec souboru
	"ERROR" // Signalizuje chybu ve scanneru
};

/*
 * Tisk typu tokenu a hodnoty tokenu
 */
void print_token(Ttoken token)
{
	printf("%s", token_name[token.type]);

	// Vyplneni mezerami pro zarovnani
	for (size_t i = strlen(token_name[token.type]); i < MAX_TOKEN_NAME_LENGTH + 1; i++)
		printf(" ");

	switch(token.type) {
		case TOK_IDENTIFIER:
		case TOK_CONSTSTRING:
			printf("- %s\n", token.attribute.ptr);
			break;
		case TOK_CONSTINT:
			printf("- %d\n", token.attribute.i);
			break;
		case TOK_CONSTDOUBLE:
			printf("- %f\n", token.attribute.d);
			break;
		default:
			printf("- %s\n", token_symbols[token.type]);
			break;
	}
}

/*
 * Vraci ukazatel na (!) konstantni (!) retezec reprezentujici token.
 */
inline const char *tok2str(Ttoken token)
{
  if (token.type == TOK_CONSTINT || token.type == TOK_CONSTDOUBLE)
		return NULL;
	else if (token.type == TOK_IDENTIFIER || token.type == TOK_CONSTSTRING)
		return token.attribute.ptr;
	else
		return token_symbols[token.type];
}

/* Konec souboru token_debug.c */
