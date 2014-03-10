/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    config_parser.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   ConfigParser class - definitions.
 *
 */

#include <cctype>

#include "debug.h"
#include "conversions.h"
#include "config_parser.h"

using namespace std;

namespace {
	// Skips whitespaces (except '\n') in the selected text
	inline void skipWhitespacesExceptNewLine(const string &s, string::size_type &i) {
		while (i < s.size() && isspace(s[i])) {
			++i;
		}
	}

	// Generates an error message
	inline string generateInvalidCharErrorMessage(char c, size_t ln) {
		if (c == '\0') {
			return string("configuration parsing error: unexpected end of file");
		} else {
			return string("configuration parsing error: invalid character ('") +
				toString(c) + "') on line " + toString(ln);
		}
	}

	// Generates an error message
	inline string generateMissingOptionValueErrorMessage(const string &optionValue, size_t ln) {
		return string("configuration parsing error: missing value for option '") +
			optionValue + "' on line " + toString(ln);
	}
}

ConfigParser::ParsedConfig ConfigParser::parse(const string &rawConfig) {
	ParsedConfig pc;
	size_t lineNum = 1;

	for (string::size_type i = 0; i < rawConfig.size(); ++i) {
		skipWhitespacesExceptNewLine(rawConfig, i);

		if (i < rawConfig.size()) {
			if (rawConfig[i] == '\n') {
				// There was an empty line
				++i;
			} else if (rawConfig[i] == '#') {
				// Comment - read until the end of line
				while (i < rawConfig.size() && rawConfig[i] != '\n') {
					++i;
				}
			} else {
				// Option name
				string optionName;
				while (i < rawConfig.size() && isValidOptionChar(rawConfig[i])) {
					optionName += rawConfig[i];
					++i;
				}

				if (!isspace(rawConfig[i])) {
					// There was an invalid option name character
					throw ParsingError(generateInvalidCharErrorMessage(
						rawConfig[i], lineNum));
				}

				// At this point, optionName should not be empty
				assert(optionName.size() > 0);

				skipWhitespacesExceptNewLine(rawConfig, i);

				// Option value
				string optionValue;
				while (i < rawConfig.size() && rawConfig[i] != '\n' &&
						(isValidValueChar(rawConfig[i]) || isspace(rawConfig[i]))) {
					optionValue += rawConfig[i];
					++i;
				}

				if (rawConfig[i] != '\n') {
					// There was an invalid option value character
					throw ParsingError(generateInvalidCharErrorMessage(
						rawConfig[i], lineNum));
				}

				// Remove whitespaces from the end of the option value (if any)
				string::size_type j = optionValue.size() - 1;
				while (j > 0 && isspace(optionValue[j])) {
					--j;
				}
				optionValue = optionValue.substr(0, j + 1);

				// Check whether the option value is not empty
				if (optionValue.size() == 0) {
					throw ParsingError(generateMissingOptionValueErrorMessage(
						optionName, lineNum));
				}

				// Store this option -> value mapping
				pc[optionName] = optionValue;
			}
		}

		++lineNum;
	}

	return pc;
}

bool ConfigParser::isValidOptionChar(char c) {
	return c != '\n' && (isalnum(c) || c == '_' || c == '-');
}

bool ConfigParser::isValidValueChar(char c) {
	return c != '\n';
}

// End of file config_parser.cc
