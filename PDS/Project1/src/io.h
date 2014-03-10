/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    io.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   I/O functions declarations.
 *
 */

#ifndef IO_H
#define IO_H

#include <string>
#include <istream>

/**
 * @brief Reads all content from the selected stream into a string.
 *
 * @param stream Stream from which the data will be read.
 *
 * @return All content read from the selected file.
 */
std::string readAllStreamContent(std::istream &stream);

/**
 * @brief Prints selected error message to standard error.
 *
 * @param errorMessage Error string that will be printed.
 */
void printErrorMessage(const char *errorMessage);

/**
 * @brief Prints selected error message to standard error.
 *
 * @param errorMessage Error string that will be printed.
 */
void printErrorMessage(const std::string &errorMessage);

/**
 * @brief Prints program help to the selected stream.
 *
 * @param progName Program name.
 * @param stream   Stream into which the help text will be sent.
 */
void printHelp(const std::string &progName, std::ostream &stream);

#endif // #ifndef IO_H

// End of file io.h
