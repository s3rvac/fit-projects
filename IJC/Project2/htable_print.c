/*
 * Encoding: UTF-8
 * File:     htable_print.c
 * Project:  IJC - DU2 - 2)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Print all items of hash table 't' to standard input.    
 *
 */

#include <stdio.h> 
#include <stdlib.h> 
#include "htable.h"  /* Operations with hash table */

void htable_print(htable_t *t) {
    // temporary pointer to list item
    struct htable_listitem *tmp;
    for (unsigned int i = 0; i < t->htable_size; i++) {
        tmp = t->data[i];
        for ( ; tmp != NULL; tmp = tmp->next)
            printf("%s\t%u\n", tmp->uk, tmp->n);      
    }
}

/* End of file htable_print.c */
