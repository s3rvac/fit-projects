/*
 * Encoding: UTF-8
 * File:     io.c
 * Project:  IJC - DU2 - 2)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: I/O operations   
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "io.h"     /* I/O operations */ 

int read_word(char *s, int max, FILE *f) {
    // maximum chars that can be read 
    const int MAX_READ = max - 1;    
    int i, c;
  
    // skip white spaces
    while ((c = getc(f)) != EOF && isspace(c))
        ; 
    
    // read one word from file 'f'
    for (i = 0; i < MAX_READ && !isspace(c) && c != EOF; i++) {        
        s[i] = c;
        c = getc(f);        
    }
    s[i] = '\0';  
      
    // nothing was readed
    if (i == 0)
        return 0;
    // word is longer than 'max', skip next chars till non-white space
    else if (i == MAX_READ) {       
       if (!isspace(c))
           while ((c = getc(f)) != EOF && !isspace(c))
               ;
       return -1;
    }                 
    
    return i;    
}

/* End of file io.c */
