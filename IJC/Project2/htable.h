/*
 * Encoding: UTF-8
 * File:     htable.h
 * Project:  IJC - DU2 - 2)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Header file for statically linked library 'htable.a'   
 *
 */
 
#ifndef HTABLE_H
    #define HTABLE_H

/* Item of the htable_t list */
struct htable_listitem {
  char *uk;                         // word 
  unsigned int n;                   // total occurrence of word
  struct htable_listitem *next;     // pointer to next item
};

/* Hash Table */
typedef struct {
  unsigned int htable_size;         // hash table size
  struct htable_listitem **data;    // array of lists 
} htable_t;

/*
 * Definition and initialization of hash table of size 'size'.
 * If dynamic allocation fails, function returns NULL. 
 */
htable_t *htable_init(unsigned size);

/*
 * Cancel all items in hash table 't' and free allocated memory.
 * for that table.
 */
void htable_free(htable_t *t);

/*
 * Cancel all items in hash table 't'.
 */
void htable_clear(htable_t *t);

/*
 * Print all items in hash table 't' to standard input.
 */
void htable_print(htable_t *t);

/*
 * Look for item that contains string 'name' in hash table 't'.
 * If the searching is sucessfull, function returns pointer to that item.
 * If not, then function will add new item into the table and return
 * pointer to that item, too.
 * If memory insufficience occured, function returns NULL.
 */
struct htable_listitem *htable_lookup(htable_t *t, const char *name);

/*
 * Hash (mapping) function for table index calculation.
 */
unsigned int hash_function(const char *str, unsigned htable_size);

#endif

/* End of file htable.h */
