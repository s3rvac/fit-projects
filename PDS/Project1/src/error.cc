/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    error.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Error messages functions implementation.
 *
 */

#include <cstdlib>
#include <cstring>

#include "error.h"
#include "debug.h"

using namespace std;

namespace {
	const size_t ERRNO_TO_STRING_BUFFER_SIZE = 128;
}

string errnoToString(int errNum) {
	char buffer[ERRNO_TO_STRING_BUFFER_SIZE + 1] = {'\0'};
	// HACK: g++ unconditionally defines _GNU_SOURCE and I haven't found
	// a way how to set g++ to not define it so I could use the XSI-compliant version
	// of strerror_r (see http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=485135)
	#ifdef _GNU_SOURCE
		// Use the GNU-specific version
		return string(strerror_r(errNum, buffer, ERRNO_TO_STRING_BUFFER_SIZE));
	#else
		// Use the XSI-compliant version
		strerror_r(errNum, buffer, ERRNO_TO_STRING_BUFFER_SIZE)
		return string(buffer);
	#endif
}

// End of file error.cc
