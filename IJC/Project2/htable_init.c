/*
 * Encoding: UTF-8
 * File:     htable_init.c
 * Project:  IJC - DU2 - 2)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description:  Initialization of hash table  
 *
 */
 
#include <stdlib.h>
#include "htable.h"  /* Operations with hash table */
 
htable_t *htable_init(unsigned size) {
    htable_t *t;
  
    // memory allocation for hash table
    if ((t = malloc(sizeof(htable_t))) == NULL)
        return NULL; 
  
    // table initialization
    t->htable_size = size;
    t->data = calloc(t->htable_size, sizeof(struct htable_listitem *));
    if (t->data == NULL) {
        free(t);
        return NULL;
    }  
  
    return t;
}

/* End of file htable_init.c */
