/*
 * Encoding: UTF-8
 * File:     tail2.cc
 * Project:  IJC - DU2 1.b)
 * Date:     1.4.2006
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: C++ implementation of standard unix 'tail' utility 
 *              Only basic operations:
 *                  -5          Print last 5 lines
 *                  +5          Print from 5th line
 *                  file_name   Print from file 'file_name'
 *
 */
 
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <iterator>

/* For simplicity - i dont need to write std:: everywhere */
using namespace std; 

/* Constants and types */
const int MAX_PARAMS = 3;                           // max parameters count
const char *DEF_STREAM = "CIN";                     // default input stream 
const unsigned long DEF_LINE_COUNT = 10;            // default line count
enum modus { M_NOTHING, M_BEGIN, M_END };           // modes of printing

/*
 * Structure containing command line parameters
 */
typedef struct {
    char const *file_name;              // input file name 
    modus mode;                         // mode of printing
    unsigned long line_count;           // line count    
} params_t;

/*
 * Print error message and exit program with exit code 1.
 */
inline void error(const char *fmt) {
        cerr << "Tail2: " << fmt << endl;              
        exit(1);
}

/*
 * Check mode of printing and return result (mode)
 */
modus check_mode(char sign) {    
    if (sign == '+')
        return M_BEGIN;
    else if (sign == '-')
        return M_END;

    return M_NOTHING;
}

/*
 * Get and process command line parameters.
 */
params_t get_params(int argc, char **argv) {
    // initialization
    params_t result;
    result.file_name = DEF_STREAM;
    result.mode = M_END;
    result.line_count = DEF_LINE_COUNT; 
         
    // too many parameters  
    if (argc > MAX_PARAMS) {
        error("Maximum parameters count exceeded!");        
    }
    
    // file name or mode loaded
    bool file_loaded = false, mode_loaded = false;
    
    // strtoul variables
    const int base = 10;
    char *endptr;
    
    // getting command line parameters
    for (int i = 1; i < argc; i++) {
        // mode
        if (!mode_loaded) {            
            // is 1st sign +/- ?
            modus mod_res = check_mode(argv[i][0]);            
            switch (mod_res) {
                case M_BEGIN:                                        
                case M_END:
                    // converting string -> number
                    result.line_count = strtoul(&argv[i][1], &endptr, base);                    
                    if (*endptr != '\0' || check_mode(argv[i][1]) != M_NOTHING) {
                        // wrong format
                        error("Wrong parameter: Mode format!");                                              
                    }
                    mode_loaded = true;
                    result.mode = (mod_res == M_BEGIN) ? M_BEGIN : M_END;
                    
                    // parameter processed
                    continue;  
                                      
                default:
                    break;
            }         
        }         
        // file
        if (!file_loaded) {
            // duplicite parameter ?
            if (check_mode(argv[i][0]) != M_NOTHING)
                error("Duplicite parameter: Mode format!");
                
            // save filename    
            result.file_name = argv[i];
            file_loaded = true;
            continue;
        } 
          
        // duplicite parameter - file
        error("Duplicite parameter: File name!");   
    }
      
    return result;
}


/*
 * Print last 'line_count' count from stream 'stream'
 */
void print_last_lines(istream & stream, unsigned long line_count) {    
    // temporary string to save line from stream
    string s;
    // buffer
    deque<string> buffer;
    // counter of readed lines
    unsigned long counter = 0;
               
    // go through input stream and read lines
    while (getline(stream, s)) {
        // save line to buffer
        buffer.push_back(s);
        
        // already readed 'line_count' lines - then remove last one
        if (counter == line_count)
            buffer.pop_front();
        else
            counter++;
    }
    
    // print lines
    copy(buffer.begin(), buffer.end(), ostream_iterator<string>(cout,"\n"));   
}

/*
 * Print every line from stream 'stream' that is after 'line'.
 */
void print_from_line(istream & stream, unsigned long line) {
    // temporary string to save line from stream
    string s;
      
    // go on 'line' position
    if (line != 0)
        while (--line > 0 && getline(stream, s))
            ;
         
    // print lines until end of file
    while (stream >> s)
        cout << s << flush;
}

/*
 * Main
 */
int main(int argc, char *argv[]) {           
    // get program parameters      
    params_t params = get_params(argc, argv);         
      
    // input stream (default: cin)
    istream *stream = &cin;
    // temp file stream
    fstream fs;
              
    // open input file
    if (strcmp(params.file_name, DEF_STREAM)) {
        // open input file
        fs.open(params.file_name, ios::in);
        
        // if file was sucessfully opened, save adress to 'stream'
        if (!fs.is_open())
            error("Unable to open file!"); 
        else       
            stream = &fs;
    }      
              
    // do printing according to program parameter     
    if (params.mode == M_BEGIN)    
        print_from_line(*stream, params.line_count);  
    else 
        print_last_lines(*stream, params.line_count);
             
    // close input file
    if (fs.is_open())      
          fs.close();       
}

/* End of file tail2.cc */
