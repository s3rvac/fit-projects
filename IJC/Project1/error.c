// Encoding: ISO-8859-2 
/*
 * Soubor:  error.c - prelozeno gcc
 * Datum:   7.3.2006
 * Autor:   Petr Zemek, FIT, xzemek02@stud.fit.vutbr.cz
 * Projekt: IJC - DU1
 * Popis:   Modul pro vypis chybovych hlasek
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"  /* Tisk chybovych zprav */

/**
 * Vypis chybove hlasky na standardni chybovy vystup (promenny pocet parametru).
 */
void Error(const char *fmt, ...) {  
  va_list args;   
 
  // vypis podle formatovaciho retezce fmt
  va_start(args, fmt);  
  fprintf(stderr, "CHYBA: ");    
  vfprintf(stderr, fmt, args);  
  va_end(args);
 
  // konec programu s chybovym kodem 1
  exit(1); 
}

/* Konec souboru error.c */
