/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    validations.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Various validation functions for common usage - definitions.
 *
 */

#include <sstream>

#include "validations.h"

using namespace std;

namespace {
	// Returns true if the selected number is a valid IPv4 address octet
	inline bool isValidIPv4Octet(int num) {
		return num >= 0 && num <= 255;
	}
}

bool isValidIPv4Address(const string &str) {
	// Valid IP address must be in format (0-255).(0-255).(0-255).(0-255)
	char c;
	int octet;
	stringstream strStream(str);

	// Read first three octets and dots
	for (string::size_type i = 0; i < 3; ++i) {
		// I'm sorry that the !(...&&...&&...) pattern is used here, but it seems
		// to me more readable than the equivalent (!...||!...||!...) version
		if (!(strStream >> octet && isValidIPv4Octet(octet) &&
				strStream >> c && c == '.')) {
			return false;
		}
	}

	// Read the last octet and check there there are no more characters
	return strStream >> octet && isValidIPv4Octet(octet) && !(strStream >> c);
}

// End of file validations.cc
