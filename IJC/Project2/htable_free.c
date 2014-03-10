/*
 * Encoding: UTF-8
 * File:     htable_free.c
 * Project:  IJC - DU2 - 2)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Cancel all items in hash table 't' 
 *              and free allocated memory for that table. 
 *
 */
 
#include <stdlib.h> 
#include "htable.h"  /* Operations with hash table */
 
void htable_free(htable_t *t) {
    // cancel all items in hash table
    htable_clear(t);
    
    // free allocated memory for hash table
    free(t->data);
    free(t); 
}

/* End of file htable_free.c */
