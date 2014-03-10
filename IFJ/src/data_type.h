/*****************************************************************************
	Kodovani:	UTF-8
	Soubor:		data_type.h
	Projekt:	IFJ06
	Datum:		17.12.2006
	Autori:
 		Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
		Libor Polcak, xpolca03@stud.fit.vutbr.cz
		Boris Prochazka, xproch63@stud.fit.vutbr.cz
		Martin Tomec, xtomec05@stud.fit.vutbr.cz
		Petr Zemek, xzemek02@stud.fit.vutbr.cz
	Popis:	Datové typy používané v různých modulech
******************************************************************************/

#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

/*
 * Mozne datove typy (jejich hodnota je rovna ordinalni hodnote pocatecniho
 * pismene datoveho typu - pro kontrolu deklaraci)
 */
typedef enum {
	TYPE_VOID = 'v',
	TYPE_INT = 'i',
	TYPE_DOUBLE = 'd',
	TYPE_STRING = 's',
} TDataType;

#endif /* #ifndef _DATA_TYPE_H_ */

/* Konec souboru data_type.h */
