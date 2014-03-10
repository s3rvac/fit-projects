/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    validations.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Various validation functions for common usage - declarations.
 *
 */

#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include <string>

/**
 * @brief Checks whether the selected string is a valid IPv4 address.
 *
 * @param str The string to be checked.
 *
 * @return True if the selected string is a valid IPv4 address, false otherwise.
 *
 * Preconditions:
 *   - @a str must be without beginning and trailing whitespaces
 */
bool isValidIPv4Address(const std::string &str);

#endif // #ifndef VALIDATIONS_H

// End of file validations.h
