/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    conversions.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Various conversion functions for common usage - declarations.
 *
 */

#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <limits>

/**
 * @brief Converts the selected value into a string.
 *
 * @param value Value that should be converted into string
 */
template<typename T> inline std::string toString(const T &value) {
	std::ostringstream out;
	out << value;
	return out.str();
}

/// Specialization for bool
template<> inline std::string toString<bool>(const bool &value) {
	std::ostringstream out;
	out << std::boolalpha << value;
	return out.str();
}

/// Specialization for double
template<> inline std::string toString<double>(const double &value) {
	const int sigDigits = std::numeric_limits<double>::digits10;
	std::ostringstream out;
	out << std::setprecision(sigDigits) << value;
	return out.str();
}

/// Specialization for float
template<> inline std::string toString<float>(const float &value) {
	const int sigDigits = std::numeric_limits<float>::digits10;
	std::ostringstream out;
	out << std::setprecision(sigDigits) << value;
	return out.str();
}

/// Specialization for long double
template<> inline std::string toString<long double>(const long double &value) {
	const int sigDigits = std::numeric_limits<long double>::digits10;
	std::ostringstream out;
	out << std::setprecision(sigDigits) << value;
	return out.str();
}

// Only for optimalization reasons - if there is someone trying to
// convert std::string into std::string...
/// Specialization for std::string
template<> inline std::string toString<std::string>(const std::string &value) {
	return std::string(value);
}

// Only for optimalization reasons - if there is someone trying to
// convert char * into std::string...
typedef char * PtrToChar;
/// Specialization for char *
template<> inline std::string toString<PtrToChar>(const PtrToChar &value) {
	return std::string(value);
}

// Only for optimalization reasons - if there is someone trying to
// convert char into std::string...
/// Specialization for char
template<> inline std::string toString<char>(const char &value) {
	return std::string(1, value);
}

/**
 * @brief Converts the selected string into a number.
 *
 * @param str    String that should be converted into a number.
 * @param number Into this parameter the result number will be stored.
 * @param format Number format (e.g. std::dec, std::hex).
 * @return True if the conversion went ok, false otherwise.
 *
 * If the conversion fails, number value will be left unchanged.
 */
template<typename N> inline bool strToNum(const std::string &str,
	N &number, std::ios_base &(* format)(std::ios_base &) = std::dec) {
	return !(std::istringstream(str) >> format >> number).fail();
}

#endif // #ifndef CONVERSIONS_H

// End of file conversions.h
