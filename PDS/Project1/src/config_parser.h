/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    config_parser.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   ConfigParser class - declarations.
 *
 */

#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <map>

#include "base_exception.h"

/**
 * @brief Configuration parser.
 *
 * This class is used to parse a configuration string that is in format:
 * <code>
 * # Comment
 * option1 value1
 *
 * # Another comment
 * option2 value2
 * </code>
 *
 * Comments are not be parsed. Option values must be separated from the option
 * name by whitespaces and are parsed until an end of line ('\n').
 * This means that each option-value pair must be ended with a new line.
 * Option names can contain only alphanumeric characters and the following
 * characters: '_', '-'. Option values can contain any characters except '\n'.
 *
 * If there are more than one option with the same name, only the last
 * value of this option will be taken into account.
 *
 * Static helper class (aka "library class").
 */
class ConfigParser {
	public:
		/**
		 * @brief This exception is thrown if there is an error during parsing.
		 */
		class ParsingError: public BaseException {
			public:
				explicit ParsingError(const std::string &errorMessage):
					BaseException(errorMessage) {}
		};

		/// Option -> values mapping
		typedef std::map<std::string, std::string> ParsedConfig;

	public:
		/**
		 * @brief Parses a configuration from the selected string.
		 *
		 * @param rawConfig The string from which the configuration will be parsed.
		 *
		 * @return Parsed configuration, i.e. option -> values mapping.
		 *
		 * See class description for more info about the configuration format.
		 * If there is an error during parsing, ParsingError will be thrown.
		 *
		 * Meets the strong guarantee of exception safety.
		 */
		static ParsedConfig parse(const std::string &rawConfig);

	private:
		/**
		 * @brief Checks whether the selected character is a valid part of an option name.
		 *
		 * @param c Character to be checked.
		 *
		 * @return True if the selected character is a valid part of an option name,
		 *         false otherwise.
		 */
		static bool isValidOptionChar(char c);

		/**
		 * @brief Checks whether the selected character is a valid part of an option value.
		 *
		 * @param c Character to be checked.
		 *
		 * @return True if the selected character is a valid part of an option value,
		 *         false otherwise.
		 */
		static bool isValidValueChar(char c);

		// Disable ctor, dtor, copy ctor and assignment operator since this
		// is a static helper class
		ConfigParser();
		ConfigParser(const ConfigParser &);
		ConfigParser &operator=(const ConfigParser &);
		~ConfigParser();
};

#endif // #ifndef CONFIG_PARSER_H

// End of file config_parser.h
