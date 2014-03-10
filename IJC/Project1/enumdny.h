// Encoding: ISO-8859-2 
/*
 * Soubor:  enumdny.h - prelozeno gcc
 * Datum:   7.3.2006
 * Autor:   Petr Zemek, FIT, xzemek02@stud.fit.vutbr.cz
 * Projekt: IJC - DU1
 * Popis:   Hlavickovy soubor pro modul enumdny.c
 */

#ifndef ENUMDNY_H
  #define ENUMDNY_H

/*
 * Vyctovy typ s prvky dnu v tydnu
 */
enum dny { Po=1, Ut, St, Ct, Pa, So, Ne };

/**
 * Tiskne retezec odpovidajici zadane hodnote 
 * vyctoveho typu 'česky' ("Po", "Út", "St", "Čt" ...).
 * @param den Prvek vyctoveho typu
 */
void PrintDayShort(enum dny den);

/**
 * Tiskne retezec odpovidajici zadane hodnote 
 * vyctoveho typu 'cesky' ("Po", "Ut", "St", Ct, ...).
 * @param den Prvek vyctoveho typu
 */
void PrintDayShortASCII(enum dny den);

/**
 * Tiskne retezec odpovidajici zadane hodnote
 * vyctoveho typu 'česky' ("Pondělí", "Úterý", ...).
 * @param den Prvek vyctoveho typu
 */
void PrintDayLong(enum dny den);

/**
 * Tiskne retezec odpovidajici zadane hodnote
 * vyctoveho typu 'cesky' ("Pondeli", "Utery", ...).
 * @param den Prvek vyctoveho typu
 */
void PrintDayLongASCII(enum dny den);

/**
 * Cte hodnotu vyctoveho typu ze standardniho vstupu.
 * @return Prvek vyctoveho typu
 */
enum dny ReadDay(void);


#endif

/* Konec souboru enumdny.h */

