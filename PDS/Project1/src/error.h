/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    error.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Error messages functions declarations.
 *
 */

#ifndef ERROR_H
#define ERROR_H

#include <string>

/**
 * @brief Transforms the selected errno number into a string that represents the error.
 *
 * @param errNum The errno number.
 *
 * @return String describing the passed error code.
 *
 * Thread-safe version of the standard strerror() function (it is a wrapper
 * around the thread-safe function strerror_r() to provide a more convenient
 * thread-safe way how to transform an errno into a string).
 */
std::string errnoToString(int errNum);

#endif // #ifndef ERROR_H

// End of file error.h
