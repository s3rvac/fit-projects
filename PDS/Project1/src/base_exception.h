/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    base_exception.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   BaseException class - declarations.
 *
 */

#ifndef BASE_EXCEPTION_H
#define BASE_EXCEPTION_H

#include <string>
#include <exception>

/**
 * @brief Base exception class for all project exceptions.
 *
 * All project exceptions are derived from this class.
 */
class BaseException: public std::exception {
	public:
		/**
		 * @brief Constructor.
		 *
		 * @param errorMessage Error message.
		 */
		explicit BaseException(const std::string &errorMessage):
			_errorMessage(errorMessage) {}

		// Override
		virtual ~BaseException() throw() {}

		// Override
		virtual const char *what() throw() { return _errorMessage.c_str(); }

		// Override
		virtual const char *what() const throw() { return _errorMessage.c_str(); }

	private:
		/// Error message carried in the exception.
		const std::string _errorMessage;
};

#endif // #ifndef BASE_EXCEPTION_H

// End of file base_exception.h
