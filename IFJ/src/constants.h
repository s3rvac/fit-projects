/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		constants.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Konstanty potřebné pro inetrpret
******************************************************************************/

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

/*
 * Konstanta určuje místo na zásobníku, které je potřeba vyhradit při volání funkce
 */
#define STACK_BOTTOM_SIZE 0

/*
 * Index, na kterem se bude nachazet navratova hodnota po volani funkce
 */
#define RETURN_VALUE_INDEX -1

/*
 * Konstanta urcujici posunuti globalnich promennych oproti normalu
 * (souvisi s RETURN_VALUE_INDEX)
 */
#define GLOBAL_STACK_SHIFT (-RETURN_VALUE_INDEX)

/*
 * Konstanta urcujici, o kolik bude posunuty index
 * u globalni promenne oproti lokalni
 */
#define GLOBAL_CONST_SHIFT ((STACK_BOTTOM_SIZE - 1) * (-1) + (GLOBAL_STACK_SHIFT))

/*
 * Velikost hashovaci tabulky
 * Doporučené hodnoty tady: http://planetmath.org/?op=getobj&from=objects&id=3327
 */
#define HASH_TABLE_SIZE 1543

#endif /* #ifndef _CONSTANTS_H_ */

/* Konec souboru constants.h */
