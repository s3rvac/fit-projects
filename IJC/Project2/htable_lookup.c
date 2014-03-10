/*
 * Encoding: UTF-8
 * File:     htable_lookup.c
 * Project:  IJC - DU2 - 2)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Lookup for item that contains string 'name' in hash table 't'.   
 *
 */
 
#include <stdlib.h> 
#include <string.h> 
#include "htable.h"  /* Operations with hash table */
 
/*
 * Create new item that will containing word 'name',
 * set occurence of that word to 0 and return pointer
 * to that item.
 */
static struct htable_listitem *htable_newitem (const char *name) {
  struct htable_listitem *new;
  
  // memory allocation
  if ((new = malloc(sizeof(struct htable_listitem))) == NULL)
    return NULL;
  if ((new->uk = malloc((strlen(name)+1)*sizeof(char))) == NULL)
    return NULL;
    
  // copy word 'name' into it
  strcpy(new->uk, name);
  new->next = NULL;
  new->n = 0; // here is 0 because incrementations does 'wordcount'
    
  return new;  
} 
 
struct htable_listitem *htable_lookup(htable_t *t, const char *name) {
  // get index
  unsigned int index = hash_function(name, t->htable_size); 
   
  // check whether there already is an item on word index position
  if (t->data[index] == NULL) {
    // it isnt -> make it
    t->data[index] = htable_newitem(name);
    return t->data[index];   
  }
    
  struct htable_listitem *tmp = t->data[index];
  // check whether item that contains word 'name' already exists  
  for ( ; tmp != NULL; tmp = tmp->next) {
    if (strcmp(name, tmp->uk) == 0)
      return tmp;
    if (tmp->next == NULL)
      break;      
  }
  
  // it isnt -> make it
  tmp->next = htable_newitem(name);
  
  return tmp->next;
}

/* End of file htable_lookup.c */
