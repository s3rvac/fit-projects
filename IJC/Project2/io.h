/*
 * Encoding: UTF-8
 * File:     io.h
 * Project:  IJC - DU2 - 2)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Header file for module io.c   
 *
 */
 
#ifndef IO_H
    #define IO_H

/*
 * Read one word of maximum length 'max' from file 'f'
 * into 's' and return count of successfully readed chars.
 * If there are no chars to read (EOF), function returns 0. 
 */
int read_word(char *s, int max, FILE *f);

#endif

/* End of file io.h */
