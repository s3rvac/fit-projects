/*
 * Encoding: UTF-8
 * File:     tail.c
 * Project:  IJC - DU2 1.a)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: C implementation of standard unix 'tail' utility 
 *              Only basic operations (depends on parameters):
 *                  -5          Print last 5 lines
 *                  +5          Print from 5th line
 *                  file_name   Print from file 'file_name'
 *              Default: Print last 10 lines and read from standnard input.
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* Debugging information */
#define NDEBUG
#include <assert.h>

/* 
 *If this symbol is enabled and program recieve parameter +x,
 * then MAX_LINE_LENGTH won't make any difference
 * and every line will be printed (but it will be a bit slower, 
 * becouse it will print by fgetc/fputc)
 */
//#define LINE_LIMITATION

/*
 * Constants and symbols
 */
#define MAX_PARAM 3                     // max parameters count
#define MAX_LINE_LENGTH 1024            // maximum line length
const int DEF_LINE_COUNT = 10;          // default line count 
const char *DEF_STREAM = "STDIN";       // default stream

/*
 * Modes of printing:
 * P_NOTHING:  Ilegal mode / not specified
 * P_BEGIN:    Print from 'x'th line
 * P_END:      Print last 'x' lines
 * where x is program parameter (default is P_END)
 */
enum modus { P_NOTHING, P_BEGIN, P_END };

/*
 * Structure containing command line parameters
 */
typedef struct {
    char const *file_name;              // input file name 
    enum modus mode;                    // mode of printing
    int line_count;                     // line count    
} params_t;
 
/*
 * Print format string 'fmt' to standard error output. 
 * Function has variable argument count (like printf).
 */
void error(const char *fmt, ...) {  
    va_list args;   
  
    va_start(args, fmt);  
    fprintf(stderr, "Tail: ");    
    vfprintf(stderr, fmt, args);  
    va_end(args);
   
    exit(1); 
}
 
/*
 * Check mode of printing and return result (mode).
 */
inline enum modus check_mode(char sign) {    
    if (sign == '+')
        return P_BEGIN;
    else if (sign == '-')
        return P_END;
        
    return P_NOTHING;
}    
    
/*
 * Process command line arguments, read them and 
 * returning them in params_t structure.
 */
params_t get_params(int argc, char *argv[]) {    
    // structure initialization
    params_t result = {
        .file_name = DEF_STREAM,
        .mode = P_END,   
        .line_count = DEF_LINE_COUNT
    };
     
    // check parameters count  
    if (argc > MAX_PARAM) {
        error("Maximum parameters count exceeded!\n");        
    }
    
    // file name or mode loaded
    int file_loaded = 0, mode_loaded = 0;
    // mode of printing
    int mod_res;    
    // strtol variables
    const int base = 10;
    char *endptr;
    
    // getting command line parameters
    for (int i = 1; i < argc; i++) {
        // mode
        if (!mode_loaded) {            
            // is 1st sign +/- ?
            mod_res = check_mode(argv[i][0]);            
            switch (mod_res) {
                case P_BEGIN:                                        
                case P_END:
                    // converting string -> number
                    result.line_count = (int) strtol(&argv[i][1], &endptr, base);                    
                    if (*endptr != '\0' || result.line_count < 0 ||
                        check_mode(argv[i][1]) != P_NOTHING) {
                        // wrong format
                        error("Wrong parameter: Mode format!\n");                                              
                    }
                    mode_loaded++;
                    result.mode = mod_res;
                    
                    // parameter processed
                    continue;  
                                      
                default:
                    break;
            }         
        }         
        // file
        if (!file_loaded) {
            // duplicite parameter ?
            if (check_mode(argv[i][0]) != P_NOTHING)
                error("Duplicite parameter: Mode format!\n");    
            result.file_name = argv[i];
            file_loaded++;
            continue;
        }
                
        // duplicite parameter - file
        error("Duplicite parameter: File name!\n");   
                
    } 
  
    return result; 
}

/*
 * Allocate memory for buffer that has 'size' lines
 * and return pointer to it.
 */
char **alloc_buffer(int size) {
    assert (size > 0);
     
    // first allocation for pointer to array of pointers 
    char **buffer = malloc(size*sizeof(char *));
    if (buffer == NULL) 
        return NULL;
    
    // then allocation for lines (+1 for ending '\0')
    for (int i = 0; i < size; i++) {
        buffer[i] = malloc((MAX_LINE_LENGTH + 1)*sizeof(char));
        if (buffer[i] == NULL) {
            // not enough memory 
            for (int j = 0; j < i; j++)
                free(buffer[j]);
        return buffer;
        }
    }
  
  return buffer;
}

/*
 * Test - if line is longer then max line length, then return 1.
 * Otherwise return 0; 
 */
inline int test_line_length(char *line) {
    return (strlen(line) > 1 && strstr(line, "\n") == NULL) ? 1 : 0;
}

/*
 * Load last '1st parameter' lines from file '2nd parameter'
 * into buffer '3rd parameter' and return position of last loaded line,
 * or -1, if error appears.
 */
int load_lines(int count, FILE *fd, char **buffer) {
    assert (fd != NULL && buffer != NULL);    
    
    // position in buffer
    int pos = 0;
    
    // loading lines
    while (fgets(buffer[pos], MAX_LINE_LENGTH, fd) != NULL) {
        // line is longer then max line length
        if (test_line_length(buffer[pos]))
            return -1;
        pos++;
        
        // end of buffer, starting from beginning
        if (pos == count)
            pos = 0; 
    }

    return pos;
}

/*
 * Write all loaded lines to standard output.
 */
void print_lines(int pos, char **buffer, int max) {
    assert (pos >= 0 && buffer != NULL);
        
    for (int i = pos; i < max; i++)
        printf("%s", buffer[i]);  
    for (int i = 0; i < pos; i++)
        printf("%s", buffer[i]);
}

/*
 * Free allocated memory for buffer.
 */
void free_buffer(char **buffer, int size) {  
    for (int i = 0; i < size; i++)
        free(buffer[i]);
    free(buffer);    
}
 
/*
 * Mode: P_END
 * Print last 'line_count' from stream 'fd'
 */ 
void print_end(FILE *fd, int line_count) {      
    assert (fd != NULL);
    
    int zero_count = (line_count == 0) ? 1 : 0;
   
    // if line_count is 0, then set it to 1 and after the end
    // of loading dont print anything
    if (zero_count)
        line_count = 1;        
    
    // memory allocation for buffer
    char **buffer = alloc_buffer(line_count);    
    if (buffer == NULL) {
        if (fd == stdin)
            fclose(fd);
        error("Not enough memory!\n");        
    } 
       
    // get last line_count lines from file file_name
    int pos = load_lines(line_count, fd, buffer);
    // line too long    
    if (pos < 0) {
        if (fd == stdin)
            fclose(fd);
        free_buffer(buffer, line_count);
        error("Input line too long!\n");   
    }
      
    // write all loaded lines to standard output
    if (!zero_count)
        print_lines(pos, buffer, line_count);
    
    // free buffer and close input file
    free_buffer(buffer, line_count);    
}

/*
 * Mode: P_BEGIN
 * Print every line from stream 'fd' after 'line_pos' line
 */
void print_begin(FILE *fd, int line_pos) {    
  // line limitation enabled ?
  #ifdef LINE_LIMITATION
    // string to save line
    char line[MAX_LINE_LENGTH];
    
    // go on 'line_pos' line
    for (int i = 1; fgets(line, MAX_LINE_LENGTH, fd) != NULL && i < line_pos; i++) {
        // line is longer then max line length
        if (test_line_length(line)) {
            if (fd == stdin)
                fclose(fd);            
            error("Input line too long!\n"); 
        }       
    }
    
    // print remaining lines until end of file
    do {  
        if (test_line_length(line)) {            
            if (fd == stdin)
                fclose(fd);            
            error("Input line too long!\n"); 
        } 
        printf("%s", line);
    } while (fgets(line, MAX_LINE_LENGTH, fd) != NULL);
    
  #else 
    int c, counter = 1;
    
    // go on 'line_pos' line
    while ((c = fgetc(fd)) != EOF && counter < line_pos) {
        if (c == '\n')
            counter++;
    }
    
    // print remaining lines until end of file
    do {
        printf("%c", c);
    } while ((c = fgetc(fd)) != EOF);
        
  #endif            
} 

/*
 * Main
 */
int main(int argc, char *argv[]) {    
    // get program parameters
    params_t params = get_params(argc, argv);
  
    // external file or stdin ?
    int is_stdin = strcmp(params.file_name, DEF_STREAM) == 0 ? 1 : 0;
   
    // input file descriptor
    FILE *fd; 
   
    // test, if file exists
    if (!is_stdin) {
        if ((fd = fopen(params.file_name, "r")) == NULL) {
            error("Unable to open input file!\n");            
        }
    }
    else
        fd = stdin;  
      
    // call printing function according to mode
    switch (params.mode) {
        case P_BEGIN:
            print_begin(fd, params.line_count);
            break;
        case P_END:
            print_end(fd, params.line_count);
            break;
        default:
            break; 
    }
        
    // close input file
    if (!is_stdin)
        fclose(fd);    
        
    return 0;
}

/* End of file tail.c */
