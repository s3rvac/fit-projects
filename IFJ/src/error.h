/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		error.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis: Chybove kody a specialni funkce vypisujici chybu. Rozhrani.
******************************************************************************/

#ifndef _ERROR_H_
#define _ERROR_H_

/*
 * Chybový výčet
 */
typedef enum {
	ERR_SUCCESS = 0,  // Operace proběhla v pořádku
	ERR_FAILURE = 1,  // Operace skončila chybou

	ERR_LOWMEM,       // Konec pro nedostatek pameti (interpret)
	ERR_RUNTIME,      // Navrat z non-void bez return (interpret)
} TErrCode;

/*
 * Vycet akci pro funkci error().
 */
typedef enum {
	RAW,      /* hola funkce fprintf */
	ROWONLY,  /* tisk jen radku */
	ROWLEX    /* tisk radku i lexemu */
} TErrEnum;

/*
 * Chybova funkce, VRACI SE
 * Zpusob zpracovani udava argument 'action'
 * Doporuceny format:
 * cast_prekladace: duvod
 * pr:
 * error("scanner: Nedostatek pameti pro lexem!\n");
 */
void error(TErrEnum action, char *format, ...);

#endif /* #ifndef _ERROR_H_ */

/* Konec souboru error.h */
