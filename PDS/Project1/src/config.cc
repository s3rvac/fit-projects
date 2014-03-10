/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    config.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Config class - definitions.
 *
 */

#include <algorithm>
#include <limits>
#include <climits>
#include <sstream>
#include <arpa/inet.h>

#include "debug.h"
#include "config.h"

using namespace std;

namespace {
	const Config::BufferSize MIN_BUFFER_SIZE = 0;
	const Config::BufferSize MAX_BUFFER_SIZE = numeric_limits<Config::BufferSize>::max();
	const Config::LengthField MIN_LENGTH_FIELD = 0;
	const Config::LengthField MAX_LENGTH_FIELD = sizeof(Config::LengthFieldMessageLength);
}

// Static member variables initialization
const Config::BufferSize Config::DEFAULT_BUFFER_SIZE = 4096;
const Config::LengthField Config::DEFAULT_LENGTH_FIELD = 0;

Config::Config Config::fromRawConfigValues(
		Config::RawConfigValues rawConfigValues) {
	return Config(rawConfigValues);
}

Config::Config():
	_bufferSize(DEFAULT_BUFFER_SIZE),
	_lengthField(DEFAULT_LENGTH_FIELD) { }

Config::Config(Config::RawConfigValues rawConfigValues) {
	_bufferSize = (rawConfigValues.find("buffer_size") != rawConfigValues.end()) ?
		parseBufferSize(rawConfigValues["buffer_size"]) : DEFAULT_BUFFER_SIZE;
	_lengthField = (rawConfigValues.find("length_field") != rawConfigValues.end()) ?
		parseLengthField(rawConfigValues["length_field"]) : DEFAULT_LENGTH_FIELD;
}

Config::~Config() { }

Config::Config(const Config::Config &other):
	_bufferSize(other._bufferSize),
	_lengthField(other._lengthField) { }

Config::Config & Config::operator=(const Config::Config &other) {
	Config temp(other);
	swap(temp);
	return *this;
}

bool Config::operator==(const Config::Config &other) const {
	return _bufferSize == other._bufferSize &&
		_lengthField == other._lengthField;
}

bool Config::operator!=(const Config::Config &other) const {
	return !(*this == other);
}

void Config::swap(Config &other) {
	::swap(_bufferSize, other._bufferSize);
	::swap(_lengthField, other._lengthField);
}

Config::LengthFieldMessageLength Config::binaryFieldLengthToNumber(
		const string &binaryNumber) {
	Config::LengthFieldMessageLength number = 0;

	// Reverse the bytes in the binary string to get the MSB
	// to the first position
	string reversedBinaryNumber = binaryNumber;
	reverse(reversedBinaryNumber.begin(), reversedBinaryNumber.end());

	// Now prepare for some magic... :)
	// We have to transform a binary number in a string into a number.
	// We do this by sequentially ORing the number (initially 0) with the bytes
	// in the string and shifting them 8 bits to the left.
	// The ORing is done only if there is something to OR.
	number |= reversedBinaryNumber[0];
	for (unsigned i = 1; i < sizeof(Config::LengthFieldMessageLength); ++i) {
		number <<= 8;
		if (reversedBinaryNumber.size() >= i) {
			number |= reversedBinaryNumber[i];
		}
	}

	// The number is now in the network order, so transform it to the host order
	return ntohl(number);
}

Config::LengthFieldMessageLength Config::computeMaximalMessageLength(unsigned lengthField) {
	LengthFieldMessageLength maxMessageLength = 1;

	// 2^numberOfBits(lengthField), computed in a loop
	// (I don't use pow() because it is for floats/doubles)
	for (unsigned i = 0; i < lengthField * CHAR_BIT; ++i) {
		maxMessageLength *= 2;
	}

	// We have to substract 1 to get the correct size
	return maxMessageLength - 1;
}

string Config::messageLengthToBinaryLengthField(
		Config::LengthFieldMessageLength messageLength, unsigned lengthField) {
	string binaryLengthField(lengthField, '\0');

	// Transform the message length into network byte order
	Config::LengthFieldMessageLength nboMessageLength = htonl(messageLength);

	// Store the number (byte per byte) into the result binary string
	for (unsigned i = 0; i < lengthField; ++i) {
		unsigned bitShift = (sizeof(Config::LengthFieldMessageLength) - i - 1) * CHAR_BIT;
		binaryLengthField[i] = (nboMessageLength & (255 << bitShift)) >> bitShift;
	}

	// Reverse the number
	reverse(binaryLengthField.begin(), binaryLengthField.end());

	return binaryLengthField;
}

Config::BufferSize Config::parseBufferSize(const string &rawBufferSize) {
	return parseNumber("buffer_size", rawBufferSize, MIN_BUFFER_SIZE, MAX_BUFFER_SIZE);
}

Config::LengthField Config::parseLengthField(const string &rawLengthField) {
	return parseNumber("length_field", rawLengthField, MIN_LENGTH_FIELD, MAX_LENGTH_FIELD);
}

// End of file config.cc
