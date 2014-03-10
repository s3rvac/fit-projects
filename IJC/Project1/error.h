// Encoding: ISO-8859-2 
/*
 * Soubor:  error.h - prelozeno gcc
 * Datum:   7.3.2006
 * Autor:   Petr Zemek, FIT, xzemek02@stud.fit.vutbr.cz
 * Projekt: IJC - DU1
 * Popis:   Hlavickovy soubor pro modul error.c
 */

#ifndef ERROR_H
  #define ERROR_H

/**
 * Vypis chybove hlasky na standardni chybovy vystup.
 * Funkce ma promenny pocet parametru.
 * @param fmt Formatovy retezec obsahujici informace 
 *            o parametrech (jako u printf)
 */
void Error(const char *fmt, ...);

#endif

/* Konec souboru error.h */
