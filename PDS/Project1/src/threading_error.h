/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    threading_error.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   ThreadingError class - declarations.
 *
 */

#ifndef THREADING_ERROR_H
#define THREADING_ERROR_H

#include "base_exception.h"

/**
 * @brief Exception for threading errors reporting.
 */
class ThreadingError: public BaseException {
	public:
		explicit ThreadingError(const std::string &errorMessage):
			BaseException(errorMessage) {}
};

#endif // #ifndef THREADING_ERROR_H

// End of file threading_error.h
