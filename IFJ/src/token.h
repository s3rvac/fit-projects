/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		token.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Deklarace typu tokenu
******************************************************************************/

#ifndef _TOKEN_H_
#define _TOKEN_H_

/*
 * Vycet pojmenovanych konstant, ktere urcuji typ tokenu (Ttoken.type)
 * Prvni jsou klicova slova a _musi_ korespondovat s retezcovymi
 * konstantami v souboru scanner.c (*keywords[])
 */
typedef enum {
	// Klicova slova
	TOK_INT,        //"int"
	TOK_DOUBLE,     //"double"
	TOK_STRING,     //"string"
	TOK_VOID,       //"void"
	TOK_VAR,        //"var"
	TOK_GOTO,       //"goto"
	TOK_IF,         //"if"
	TOK_RETURN,     //"return"
	TOK_DIVINT,     //"div"
	// Ostatni tokeny
	TOK_IDENTIFIER,
	TOK_COMMA,      //','
	TOK_SEMICOLON,  //';'
	TOK_LABEL,      //':'
	TOK_ASSIGNMENT, //':''='
	TOK_POWER,      //'^'
	TOK_PLUS,       //'+'
	TOK_MINUS,      //'-'
	TOK_MULTIPLE,   //'*'
	TOK_DIVDOUBLE,  //'/'
	TOK_LESS,       //'<'
	TOK_LESSEQUAL,  //'<''='
	TOK_GREATER,    //'>'
	TOK_GREATEREQUAL, //'>''='
	TOK_EQUAL,      //'=''='
	TOK_NOTEQUAL,   //'!''='
	TOK_LBRACKET,   //'('
	TOK_RBRACKET,   //')'
	TOK_LVINCULUM,  //'{'
	TOK_RVINCULUM,  //'}'
	TOK_CONSTSTRING,
	TOK_CONSTINT,
	TOK_CONSTDOUBLE,
	TOK_EOF,         // Signalizuje konec souboru
	TOK_SCANNERERROR // Signalizuje chybu ve scanneru
} Etoken;

/* Datova cast tokenu, podle Ttoken.type pozname, jak k nemu pristupovat */
typedef union {
	int i;
	double d;
	char *ptr;
} Utoken;

/* Vlastni token = typ + data */
typedef struct {
	Etoken type;
	Utoken attribute;
} Ttoken;

#endif /* #ifndef _TOKEN_H_ */

/* Konec souboru token.h */
