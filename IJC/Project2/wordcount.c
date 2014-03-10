/*
 * Encoding: UTF-8
 * File:     wordcount.c
 * Project:  IJC - DU2 - 2)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Program 'Wordcount' is reading words from standard input
 *              until EOF is detected. 
 *              Then program print each unique word on a new line
 *              and total occurrence of that word.    
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "io.h"     /* I/O operations */ 
#include "htable.h" /* Operations with hash table */

/* Hash table size. 
 * Size was get by experimenting and testing dependancy between htable size
 * and input file character. Final size is the best ratio between rate 
 * and total size of htable.
 * Legend:  Htable size    [sizeof(int)]
 *          Average ratio  [seconds]
 *
 * 1st test - File: /usr/share/dict/words (cca all different 500.000 words)
 * +-------------------------------------------------------------+
 * | 1000 | 5000 | 10000 | 15000 | 20000 | 25000 | 30000 | 35000 |
 * |-------------------------------------------------------------|
 * | 17.0 | 4.1  |  2.6  |  2.1  |  2.0  |  1.9  |  1.7  |  1.5  |
 * +-------------------------------------------------------------+
 *
 * 2nd test - File: n2794.txt - c99 standard (cca 156.000 words, some are the same)
 * +----------------------------------------------------------------+
 * | 1000  | 5000  | 10000 | 15000 | 20000 | 25000 | 30000 | 35.000 |
 * |----------------------------------------------------------------|
 * | 0.108 | 0.106 | 0.104 | 0.103 | 0.101 | 0.102 | 0.103 |  0.105 |
 * +----------------------------------------------------------------+
 * 
 * 3rd test - File: my own (cca 500000 words, all the same (20 letters))
 * Avarage ratio is 0.5 sec for all sizes - bacause only 1 item is used
 *
 * 4th test - File: some very small files (cca 10-50 words)
 * Avarage ratio is 0.00x, that is only time to allocate htable
 *
 * And then, according to some hints on http://en.wikipedia.org/wiki/Hash_table
 * I have chosen table size: 21107
 */ 
const unsigned int HTABLE_INIT_SIZE = 21107;
/* Maximum word length */
const int MAX_WORD_LENGTH = 128;

/*
 * Print format string 'fmt' to standard error output. 
 * Function has variable argument count (like printf).
 */
void error(const char *fmt, ...) {  
    va_list args;   
  
    va_start(args, fmt);  
    fprintf(stderr, "Wordcount: ");    
    vfprintf(stderr, fmt, args);  
    va_end(args);
   
    return; 
}

/*
 * Main
 */
int main(void) {     
    // hash table definition/initialization
    htable_t *t = htable_init(HTABLE_INIT_SIZE);
    if (t == NULL) {
        // memory insufficience for hash table creation
        error("Insufficient memory, cannot create hash table!\n"); 
        exit(1);
    }
  
    // string for saving readed words 
    char str[MAX_WORD_LENGTH];
    // pointer to hash table item
    struct htable_listitem *new;
    // signalize, that there was too long word on input
    int long_word_err = 0;
    // return code
    int rc;
   
    // loop for reading words from standard input
    // if rc == 0, end of file was detected  
    while ((rc = read_word(str, MAX_WORD_LENGTH, stdin)) != 0) {
        // line was too long - print error message and continue    
        if (rc < 0 && !long_word_err) {
            error("Too long word found! Max. word size limit is %u chars.\n", MAX_WORD_LENGTH-1);
            long_word_err = 1;
        }
           
        // save readed word into hash table
        new = htable_lookup(t, str);
        if (new == NULL) {
            // memory insufficience
            htable_free(t);
            error("Insufficient memory, cannot create new item!\n");
            exit(1);      
        }
        else
            // increase total occurrence of readed word
            new->n++; 
    }
          
    // print all items in hash table
    htable_print(t);  
    
    // free allocated memory for whole hash table
    htable_free(t);       
         
    return 0;
}

/* End of file wordcount.c */
