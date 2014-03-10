/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		token_debug.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Debugging informace/funkce pro tokeny - rozhrani
******************************************************************************/

#ifndef _TOKEN_DEBUG_H_
#define _TOKEN_DEBUG_H_

#include "token.h"

/*
 * Tisk typu tokenu a hodnoty tokenu
 */
void print_token(Ttoken token);

/*
 * Vraci ukazatel na (!) konstantni (!) retezec reprezentujici token.
 */
inline const char *tok2str(Ttoken token);


#endif /* #ifndef _TOKEN_DEBUG_H_ */

/* Konec souboru token_debug.h */
