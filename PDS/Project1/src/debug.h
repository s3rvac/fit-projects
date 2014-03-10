/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    debug.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Debugging declarations.
 *
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

/*
 * NOTE: Debugging can be enabled/disabled in the Makefile.
 */

// Standard debugging
#ifdef DEBUG
	#if DEBUG == 0
		// Disable debugging
		#ifndef NDEBUG
			#define NDEBUG
		#endif
	#endif
#endif
#include <cassert>

/**
 * @brief Logs the selected debugging message.
 *
 * @param debugMessage Debug string to be logged.
 */
inline void logDebugMessage(const std::string &debugMessage) {
	#if DEBUG
		std::cerr << "[debug]: " << debugMessage << std::endl;
	#else
		(void) debugMessage; // To get rid of the "unused variable" warning
	#endif
}

#endif // #ifndef DEBUG_H

// End of file debug.h
