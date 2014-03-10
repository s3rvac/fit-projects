/*
 * Encoding: UTF-8
 * File:     htable_hash.c
 * Project:  IJC - DU2 - 2)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Hash (mapping) function (taken from project assignment)     
 *
 */
 
unsigned int hash_function(const char *str, unsigned htable_size) { 
    unsigned int h = 0; 
    unsigned char *p; 
    for (p = (unsigned char*)str; *p != '\0'; p++) 
        h = 31*h + *p; 
    return h % htable_size; 
}

/* End of file htable_hash.c */
