/*
 * Encoding: UTF-8
 * File:     htable_clear.c
 * Project:  IJC - DU2 - 2)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Cancel all items in hash table 't'    
 *
 */
 
#include <stdlib.h> 
#include "htable.h"  /* Operations with hash table */

void htable_clear(htable_t *t) {
    // temporary pointer to list item
    struct htable_listitem *tmp;
    
    for (unsigned int i = 0; i < t->htable_size; i++) {
        while (t->data[i] != NULL) {
            tmp = t->data[i];
            t->data[i] = t->data[i]->next; 
            
            // free allocated memory for word
            free(tmp->uk);
            // free allocated memory for item 
            free(tmp);  
        }
    } 
}

/* End of file htable_clear.c */
