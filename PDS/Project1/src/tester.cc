/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    tester.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Testing suite.
 *
 */

#include <iostream>
#include <limits>
#include <ctime>
#include <tr1/cstdint>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "debug.h"
#include "conversions.h"
#include "io.h"
#include "config_parser.h"
#include "serial_config.h"
#include "tcpip_server.h"
#include "tcpip_config.h"
#include "validations.h"

using namespace std;
using namespace std::tr1;
using namespace CppUnit;

/// ConfigParser testing suite
class ConfigParserTest: public TestFixture {
private:
	CPPUNIT_TEST_SUITE(ConfigParserTest);
	CPPUNIT_TEST(testParseEmptyInput);
	CPPUNIT_TEST(testParseOneOptionWithValueNoLeadingSpaces);
	CPPUNIT_TEST(testParseOneOptionWithValueLeadingSpaces);
	CPPUNIT_TEST(testParseNoOptionOneComment);
	CPPUNIT_TEST(testParseNoOptionOneCommentNoNewLineAtEnd);
	CPPUNIT_TEST(testParseOneOptionOneComment);
	CPPUNIT_TEST(testParseOneOptionNoCommentSeveralEmptyLines);
	CPPUNIT_TEST(testParseOneOptionMoreComments);
	CPPUNIT_TEST(testParseThreeOptionsMoreComments);
	CPPUNIT_TEST(testParseSpecialCharsInOptionNameAndValue);
	CPPUNIT_TEST(testParseValueContainsWhiteSpaces);
	CPPUNIT_TEST(testParseSeveralEmptyLines);
	CPPUNIT_TEST(testParsingErrorMissingOptionValue);
	CPPUNIT_TEST(testParsingErrorInvalidCharacter);
	CPPUNIT_TEST(testParsingErrorNoNewLineAfterOptionValue);
	CPPUNIT_TEST_SUITE_END();

public:
	void testParseEmptyInput() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse("");

		CPPUNIT_ASSERT(pc.empty());
	}

	void testParseOneOptionWithValueNoLeadingSpaces() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			"option value\n");

		CPPUNIT_ASSERT(pc["option"] == "value");
	}

	void testParseOneOptionWithValueLeadingSpaces() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			" 		option value\n");

		CPPUNIT_ASSERT(pc["option"] == "value");
	}

	void testParseNoOptionOneComment() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			"# comment \n");

		CPPUNIT_ASSERT(pc.empty());
	}

	void testParseNoOptionOneCommentNoNewLineAtEnd() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			"# comment");

		CPPUNIT_ASSERT(pc.empty());
	}

	void testParseOneOptionOneComment() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			"# comment...\n"
			"option value\n");

		CPPUNIT_ASSERT(pc["option"] == "value");
	}

	void testParseOneOptionNoCommentSeveralEmptyLines() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			"	\n"
			"\n"
			"\n"
			"  \n"
			"option value\n");

		CPPUNIT_ASSERT(pc["option"] == "value");
	}

	void testParseOneOptionMoreComments() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			"# comment...\n"
			"		# continues \n"
			"option value\n"
			"# Another comment\n");

		CPPUNIT_ASSERT(pc["option"] == "value");
	}

	void testParseThreeOptionsMoreComments() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			"# comment...\n"
			"# continues \n"
			"option1 value1\n"
			"\n"
			"# Another comment\n"
			"option2 value2\n");

		CPPUNIT_ASSERT(pc["option1"] == "value1");
		CPPUNIT_ASSERT(pc["option2"] == "value2");
	}

	void testParseSpecialCharsInOptionNameAndValue() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			"\nop-ti_on _value-\n");

		CPPUNIT_ASSERT(pc["op-ti_on"] == "_value-");
	}

	void testParseValueContainsWhiteSpaces() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			"# comment...\n"
			"option value\n");

		CPPUNIT_ASSERT(pc["option"] == "value");
	}

	void testParseSeveralEmptyLines() {
		ConfigParser::ParsedConfig pc = ConfigParser::parse(
			"	\n"
			"\n"
			"\n"
			"  \n"
			"             \n");

		CPPUNIT_ASSERT(pc.size() == 0);
	}

	void scenarioParsingError(const string &rawConfig) {
		try {
			ConfigParser::parse(rawConfig);
		} catch (ConfigParser::ParsingError &e) {
			return;
		}

		CPPUNIT_FAIL("Should have thrown a ConfigParser::ParsingError exception.");
	}

	void testParsingErrorMissingOptionValue() {
		scenarioParsingError("option\n");
		scenarioParsingError("option	\n");
	}

	void testParsingErrorInvalidCharacter() {
		scenarioParsingError("opt##ion value\n");
		scenarioParsingError("!\n");
	}

	void testParsingErrorNoNewLineAfterOptionValue() {
		scenarioParsingError("option value");
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ConfigParserTest);

/// Config testing suite
class ConfigTest: public TestFixture {
private:
	CPPUNIT_TEST_SUITE(ConfigTest);
	CPPUNIT_TEST(testFromRawConfigValuesParseBufferSizeMinimalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseBufferSizeNormalValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseBufferSizeMaximalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseBufferSizeLessThanMinimalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseBufferSizeMoreThanMaximalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseBufferSizeNotAValidNumber);
	CPPUNIT_TEST(testFromRawConfigValuesParseLengthFieldMinimalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseLengthFieldMaximalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseLengthFieldNormalValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseLengthFieldMaximalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseLengthFieldLessThanMinimalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseLengthFieldMoreThanMaximalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseLengthFieldNotAValidNumber);
	CPPUNIT_TEST(testBinaryFieldLengthToNumber);
	CPPUNIT_TEST(testComputeMaximalMessageLength);
	CPPUNIT_TEST(testMessageLengthToBinaryLengthField);
	CPPUNIT_TEST(testMessageLengthBothWays);
	CPPUNIT_TEST_SUITE_END();

	// TODO - test: copy ctor, assignment operator, ==, !=

public:
	template <typename T>
	void scenarioTestFromRawConfigValuesParseValue(const string &option,
			T (Config::*getFunc)() const, const string &rawValue,
			const T &referenceValue, bool shouldSucceed) {
		try {
			Config::RawConfigValues rawConfigValues;
			rawConfigValues[option] = rawValue;
			Config config = Config::fromRawConfigValues(rawConfigValues);

			if (!shouldSucceed) {
				CPPUNIT_FAIL("Should have thrown a Config::InvalidValueError exception.");
			}

			CPPUNIT_ASSERT((config.*getFunc)() == referenceValue);
		} catch (Config::InvalidValueError &e) {
			if (shouldSucceed) {
				CPPUNIT_FAIL("Should have succeeded, InvalidValueError exception thrown instead.");
			}
		}
	}

	void testFromRawConfigValuesParseBufferSizeMinimalValid() {
		scenarioTestFromRawConfigValuesParseValue<Config::BufferSize>("buffer_size", &Config::getBufferSize,
			"0", 0, true);
	}

	void testFromRawConfigValuesParseBufferSizeNormalValues() {
		scenarioTestFromRawConfigValuesParseValue<Config::BufferSize>("buffer_size", &Config::getBufferSize,
			"556", 556, true);
		scenarioTestFromRawConfigValuesParseValue<Config::BufferSize>("buffer_size", &Config::getBufferSize,
			"130456", 130456, true);
	}

	void testFromRawConfigValuesParseBufferSizeMaximalValid() {
		scenarioTestFromRawConfigValuesParseValue<Config::BufferSize>("buffer_size", &Config::getBufferSize,
			toString(numeric_limits<Config::BufferSize>::max()),
			numeric_limits<Config::BufferSize>::max(), true);
	}

	void testFromRawConfigValuesParseBufferSizeLessThanMinimalValid() {
		scenarioTestFromRawConfigValuesParseValue<Config::BufferSize>("buffer_size", &Config::getBufferSize,
			"-100", 0, false);
	}

	void testFromRawConfigValuesParseBufferSizeMoreThanMaximalValid() {
		scenarioTestFromRawConfigValuesParseValue<Config::BufferSize>("buffer_size", &Config::getBufferSize,
			"9999999999999999999999", 0, false);
	}

	void testFromRawConfigValuesParseBufferSizeNotAValidNumber() {
		scenarioTestFromRawConfigValuesParseValue<Config::BufferSize>("buffer_size", &Config::getBufferSize,
			"%^&*", 0, false);
		scenarioTestFromRawConfigValuesParseValue<Config::BufferSize>("buffer_size", &Config::getBufferSize,
			"3.14", 0, false);
		scenarioTestFromRawConfigValuesParseValue<Config::BufferSize>("buffer_size", &Config::getBufferSize,
			"1e56", 0, false);
	}

	void testFromRawConfigValuesParseLengthFieldMinimalValid() {
		scenarioTestFromRawConfigValuesParseValue<Config::LengthField>("length_field", &Config::getLengthField,
			"0", 0, true);
	}

	void testFromRawConfigValuesParseLengthFieldMaximalValid() {
		scenarioTestFromRawConfigValuesParseValue<Config::LengthField>("length_field", &Config::getLengthField,
			toString(sizeof(Config::LengthFieldMessageLength)),
			sizeof(Config::LengthFieldMessageLength), true);
	}

	void testFromRawConfigValuesParseLengthFieldNormalValues() {
		for (unsigned i = 0; i < sizeof(Config::LengthFieldMessageLength); ++i) {
			scenarioTestFromRawConfigValuesParseValue<Config::LengthField>("length_field", &Config::getLengthField,
			toString(i), i, true);
		}
	}

	void testFromRawConfigValuesParseLengthFieldLessThanMinimalValid() {
		scenarioTestFromRawConfigValuesParseValue<Config::LengthField>("length_field", &Config::getLengthField,
			"-100", 0, false);
	}

	void testFromRawConfigValuesParseLengthFieldMoreThanMaximalValid() {
		scenarioTestFromRawConfigValuesParseValue<Config::LengthField>("length_field", &Config::getLengthField,
			"9999999999999999999999", 0, false);
	}

	void testFromRawConfigValuesParseLengthFieldNotAValidNumber() {
		scenarioTestFromRawConfigValuesParseValue<Config::LengthField>("length_field", &Config::getLengthField,
			"%^&*", 0, false);
		scenarioTestFromRawConfigValuesParseValue<Config::LengthField>("length_field", &Config::getLengthField,
			"3.14", 0, false);
		scenarioTestFromRawConfigValuesParseValue<Config::LengthField>("length_field", &Config::getLengthField,
			"1e56", 0, false);
	}

	void testBinaryFieldLengthToNumber() {
		// Max
		CPPUNIT_ASSERT(Config::binaryFieldLengthToNumber(
			string(1, 255)) == 255UL);
		CPPUNIT_ASSERT(Config::binaryFieldLengthToNumber(
			string(1, 255) + string(1, 255)) == 65535UL);
		CPPUNIT_ASSERT(Config::binaryFieldLengthToNumber(
			string(1, 255) + string(1, 255) + string(1, 255)) == 16777215UL);
		CPPUNIT_ASSERT(Config::binaryFieldLengthToNumber(
			string(1, 255) + string(1, 255) + string(1, 255) + string(1, 255)) == 4294967295UL);

		// Random
		CPPUNIT_ASSERT(Config::binaryFieldLengthToNumber(
			string(1, 5)) == 5);
		CPPUNIT_ASSERT(Config::binaryFieldLengthToNumber(
			string(1, 0x01) + string(1, 0x2C)) == 300);
		CPPUNIT_ASSERT(Config::binaryFieldLengthToNumber(
			string(1, 0x0A) + string(1, 0x0B) + string(1, 0x0C) + string(1, 0x0D)) == 0x0A0B0C0D);
	}

	void testComputeMaximalMessageLength() {
		CPPUNIT_ASSERT(Config::computeMaximalMessageLength(0) == 0UL);
		CPPUNIT_ASSERT(Config::computeMaximalMessageLength(1) == 255UL);
		CPPUNIT_ASSERT(Config::computeMaximalMessageLength(2) == 65535UL);
		CPPUNIT_ASSERT(Config::computeMaximalMessageLength(3) == 16777215UL);
		CPPUNIT_ASSERT(Config::computeMaximalMessageLength(4) == 4294967295UL);
	}

	void testMessageLengthToBinaryLengthField() {
		// Max
		CPPUNIT_ASSERT(Config::messageLengthToBinaryLengthField(
			255UL, 1) == string(1, 255));
		CPPUNIT_ASSERT(Config::messageLengthToBinaryLengthField(
			65535UL, 2) == string(1, 255) + string(1, 255));
		CPPUNIT_ASSERT(Config::messageLengthToBinaryLengthField(
			16777215UL, 3) == string(1, 255) + string(1, 255) + string(1, 255));
		CPPUNIT_ASSERT(Config::messageLengthToBinaryLengthField(
			4294967295UL, 4) == string(1, 255) + string(1, 255) + string(1, 255) + string(1, 255));

		// Random
		CPPUNIT_ASSERT(Config::messageLengthToBinaryLengthField(
			100, 1) == string(1, 100));
		CPPUNIT_ASSERT(Config::messageLengthToBinaryLengthField(
			300, 2) == string(1, 0x01) + string(1, 0x2C));
	}

	void testMessageLengthBothWays() {
		CPPUNIT_ASSERT(Config::binaryFieldLengthToNumber(Config::messageLengthToBinaryLengthField(
			300, 2)) == 300);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ConfigTest);

/// TCPIPConfig testing suite
class TCPIPConfigTest: public TestFixture {
private:
	CPPUNIT_TEST_SUITE(TCPIPConfigTest);
	CPPUNIT_TEST(testFromRawConfigValuesParseInterfacesAnyInterface);
	CPPUNIT_TEST(testFromRawConfigValuesParseInterfacesOneInterface);
	CPPUNIT_TEST(testFromRawConfigValuesParseInterfacesThreeInterfaces);
	CPPUNIT_TEST(testFromRawConfigValuesParseInterfacesThreeInterfacesVariousWhiteSpace);
	CPPUNIT_TEST(testFromRawConfigValuesParsePortMinimalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParsePortNormalValues);
	CPPUNIT_TEST(testFromRawConfigValuesParsePortMaximalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParsePortLessThanMinimalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParsePortMoreThanMaximalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParsePortNotAValidNumber);
	CPPUNIT_TEST(testFromRawConfigValuesParseTimeoutMinimalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseTimeoutNormalValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseTimeoutMaximalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseTimeoutLessThanMinimalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseTimeoutMoreThanMaximalValid);
	CPPUNIT_TEST(testFromRawConfigValuesParseTimeoutNotAValidNumber);
	CPPUNIT_TEST_SUITE_END();

	// TODO - test: copy ctor, assignment operator, ==, !=

public:
	template <typename T>
	void scenarioTestFromRawConfigValuesParseValue(const string &option,
			T (TCPIPConfig::*getFunc)() const, const string &rawValue,
			const T &referenceValue, bool shouldSucceed) {
		try {
			TCPIPConfig::RawConfigValues rawConfigValues;
			rawConfigValues[option] = rawValue;
			TCPIPConfig config = TCPIPConfig::fromRawConfigValues(rawConfigValues);

			if (!shouldSucceed) {
				CPPUNIT_FAIL("Should have thrown a InvalidValueError exception.");
			}

			CPPUNIT_ASSERT((config.*getFunc)() == referenceValue);
		} catch (Config::InvalidValueError &e) {
			if (shouldSucceed) {
				CPPUNIT_FAIL("Should have succeeded, InvalidValueError exception thrown instead.");
			}
		}
	}

	void testFromRawConfigValuesParseInterfacesAnyInterface() {
 		TCPIPConfig::Interfaces referenceInterfaces;
 		referenceInterfaces.push_back("ANY");
 		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Interfaces>(string("interface"), &TCPIPConfig::getInterfaces,
 			string("ANY"), referenceInterfaces, true);
	}

	void testFromRawConfigValuesParseInterfacesOneInterface() {
 		TCPIPConfig::Interfaces referenceInterfaces;
 		referenceInterfaces.push_back("lo");
 		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Interfaces>(string("interface"), &TCPIPConfig::getInterfaces,
 			string("lo"), referenceInterfaces, true);
	}

	void testFromRawConfigValuesParseInterfacesThreeInterfaces() {
 		TCPIPConfig::Interfaces referenceInterfaces;
 		referenceInterfaces.push_back("lo");
 		referenceInterfaces.push_back("eth0");
 		referenceInterfaces.push_back("37.66.87.222");
 		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Interfaces>(string("interface"), &TCPIPConfig::getInterfaces,
 			string("lo eth0 37.66.87.222"), referenceInterfaces, true);
	}

	void testFromRawConfigValuesParseInterfacesThreeInterfacesVariousWhiteSpace() {
 		TCPIPConfig::Interfaces referenceInterfaces;
 		referenceInterfaces.push_back("lo");
 		referenceInterfaces.push_back("eth0");
 		referenceInterfaces.push_back("wlan0");
 		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Interfaces>(string("interface"), &TCPIPConfig::getInterfaces,
 			string("lo\t\teth0           wlan0"), referenceInterfaces, true);
	}

	void testFromRawConfigValuesParsePortMinimalValid() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			"1", 1, true);
	}

	void testFromRawConfigValuesParsePortNormalValues() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			"120", 120, true);
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			"33476", 33476, true);
	}

	void testFromRawConfigValuesParsePortMaximalValid() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			toString(numeric_limits<TCPIPConfig::Port>::max()), numeric_limits<TCPIPConfig::Port>::max(), true);
	}

	void testFromRawConfigValuesParsePortLessThanMinimalValid() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			"-100", 0, false);
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			"0", 0, false);
	}

	void testFromRawConfigValuesParsePortMoreThanMaximalValid() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			"100000", 0, false);
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			"99999999999999999999", 0, false);
	}

	void testFromRawConfigValuesParsePortNotAValidNumber() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			"s#dfsds", 0, false);
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			"3.14", 0, false);
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Port>("port", &TCPIPConfig::getPort,
			"1e56", 0, false);
	}

	void testFromRawConfigValuesParseTimeoutMinimalValid() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Timeout>("timeout", &TCPIPConfig::getTimeout,
			"0", 0, true);
	}

	void testFromRawConfigValuesParseTimeoutNormalValues() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Timeout>("timeout", &TCPIPConfig::getTimeout,
			"556", 556, true);
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Timeout>("timeout", &TCPIPConfig::getTimeout,
			"130456", 130456, true);
	}

	void testFromRawConfigValuesParseTimeoutMaximalValid() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Timeout>("timeout", &TCPIPConfig::getTimeout,
			toString(numeric_limits<TCPIPConfig::Timeout>::max()),
			numeric_limits<TCPIPConfig::Timeout>::max(), true);
	}

	void testFromRawConfigValuesParseTimeoutLessThanMinimalValid() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Timeout>("timeout", &TCPIPConfig::getTimeout,
			"-100", 0, false);
	}

	void testFromRawConfigValuesParseTimeoutMoreThanMaximalValid() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Timeout>("timeout", &TCPIPConfig::getTimeout,
			"9999999999999999999999", 0, false);
	}

	void testFromRawConfigValuesParseTimeoutNotAValidNumber() {
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Timeout>("timeout", &TCPIPConfig::getTimeout,
			"%^&*", 0, false);
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Timeout>("timeout", &TCPIPConfig::getTimeout,
			"3.14", 0, false);
		scenarioTestFromRawConfigValuesParseValue<TCPIPConfig::Timeout>("timeout", &TCPIPConfig::getTimeout,
			"1e56", 0, false);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TCPIPConfigTest);

/// SerialConfig testing suite
class SerialConfigTest: public TestFixture {
private:
	CPPUNIT_TEST_SUITE(SerialConfigTest);
	CPPUNIT_TEST(testFromRawConfigValuesParseDevice);
	CPPUNIT_TEST(testFromRawConfigValuesParseSpeedValidValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseSpeedInvalidValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseDataBitsValidValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseDataBitsInvalidValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseParityValidValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseParityInvalidValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseStopBitsValidValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseStopBitsInvalidValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseFlowControlValidValues);
	CPPUNIT_TEST(testFromRawConfigValuesParseFlowControlInvalidValues);
	CPPUNIT_TEST_SUITE_END();

	// TODO - test: copy ctor, assignment operator, ==, !=

public:
	template <typename T>
	void scenarioTestFromRawConfigValuesParseValue(const string &option,
		T (SerialConfig::*getFunc)() const, const string &rawValue,
		const T &referenceValue, bool shouldSucceed) {
		try {
			SerialConfig::RawConfigValues rawConfigValues;
			rawConfigValues[option] = rawValue;
			SerialConfig config = SerialConfig::fromRawConfigValues(rawConfigValues);

			if (!shouldSucceed) {
				CPPUNIT_FAIL("Should have thrown a InvalidValueError exception.");
			}

			CPPUNIT_ASSERT((config.*getFunc)() == referenceValue);
		} catch (Config::InvalidValueError &e) {
			if (shouldSucceed) {
				CPPUNIT_FAIL("Should have succeeded, InvalidValueError exception thrown instead.");
			}
		}
	}

	void testFromRawConfigValuesParseDevice() {
		scenarioTestFromRawConfigValuesParseValue<string>("device", &SerialConfig::getDevice,
			"/dev/stty2", "/dev/stty2", true);
	}

	void testFromRawConfigValuesParseSpeedValidValues() {
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"50", B50, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"75", B75, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"110", B110, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"134", B134, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"150", B150, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"200", B200, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"300", B300, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"600", B600, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"120", B1200, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"1800", B1800, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"2400", B2400, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"4800", B4800, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"9600", B9600, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"19200", B19200, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"38400", B38400, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"57600", B57600, true);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"115200", B115200, true);
	}

	void testFromRawConfigValuesParseSpeedInvalidValues() {
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"-2374", speed_t(), false);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"2342321", speed_t(), false);
		scenarioTestFromRawConfigValuesParseValue<speed_t>("speed", &SerialConfig::getSpeed,
			"cadg", speed_t(), false);
	}

	void testFromRawConfigValuesParseDataBitsValidValues() {
		scenarioTestFromRawConfigValuesParseValue<tcflag_t>("data_bits", &SerialConfig::getDataBits,
			"5", CS5, true);
		scenarioTestFromRawConfigValuesParseValue<tcflag_t>("data_bits", &SerialConfig::getDataBits,
			"6", CS6, true);
		scenarioTestFromRawConfigValuesParseValue<tcflag_t>("data_bits", &SerialConfig::getDataBits,
			"7", CS7, true);
		scenarioTestFromRawConfigValuesParseValue<tcflag_t>("data_bits", &SerialConfig::getDataBits,
			"8", CS8, true);
	}

	void testFromRawConfigValuesParseDataBitsInvalidValues() {
		scenarioTestFromRawConfigValuesParseValue<tcflag_t>("data_bits", &SerialConfig::getDataBits,
			"0", tcflag_t(), false);
		scenarioTestFromRawConfigValuesParseValue<tcflag_t>("data_bits", &SerialConfig::getDataBits,
			"222", tcflag_t(), false);
		scenarioTestFromRawConfigValuesParseValue<tcflag_t>("data_bits", &SerialConfig::getDataBits,
			"@#$%#", tcflag_t(), false);
	}

	void testFromRawConfigValuesParseParityValidValues() {
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::Parity>("parity", &SerialConfig::getParity,
			"none", SerialConfig::NONE_PARITY, true);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::Parity>("parity", &SerialConfig::getParity,
			"even", SerialConfig::EVEN_PARITY, true);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::Parity>("parity", &SerialConfig::getParity,
			"odd", SerialConfig::ODD_PARITY, true);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::Parity>("parity", &SerialConfig::getParity,
			"mark", SerialConfig::MARK_PARITY, true);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::Parity>("parity", &SerialConfig::getParity,
			"space", SerialConfig::SPACE_PARITY, true);
	}

	void testFromRawConfigValuesParseParityInvalidValues() {
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::Parity>("parity", &SerialConfig::getParity,
			"uknown", SerialConfig::Parity(), false);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::Parity>("parity", &SerialConfig::getParity,
			"@#$%#", SerialConfig::Parity(), false);
	}

	void testFromRawConfigValuesParseStopBitsValidValues() {
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::StopBits>("stop_bits", &SerialConfig::getStopBits,
			"1", SerialConfig::STOP_BITS_1, true);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::StopBits>("stop_bits", &SerialConfig::getStopBits,
			"1.5", SerialConfig::STOP_BITS_1_5, true);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::StopBits>("stop_bits", &SerialConfig::getStopBits,
			"2", SerialConfig::STOP_BITS_2, true);
	}

	void testFromRawConfigValuesParseStopBitsInvalidValues() {
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::StopBits>("stop_bits", &SerialConfig::getStopBits,
			"0", SerialConfig::StopBits(), false);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::StopBits>("stop_bits", &SerialConfig::getStopBits,
			"10.5", SerialConfig::StopBits(), false);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::StopBits>("stop_bits", &SerialConfig::getStopBits,
			"f6$##", SerialConfig::StopBits(), false);
	}

	void testFromRawConfigValuesParseFlowControlValidValues() {
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::FlowControl>("flow_control", &SerialConfig::getFlowControl,
			"none", SerialConfig::NONE_FLOW_CONTROL, true);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::FlowControl>("flow_control", &SerialConfig::getFlowControl,
			"hardware", SerialConfig::HARDWARE_FLOW_CONTROL, true);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::FlowControl>("flow_control", &SerialConfig::getFlowControl,
			"xon/off", SerialConfig::SOFTWARE_FLOW_CONTROL, true);
	}

	void testFromRawConfigValuesParseFlowControlInvalidValues() {
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::FlowControl>("flow_control", &SerialConfig::getFlowControl,
			"nothing", SerialConfig::FlowControl(), false);
		scenarioTestFromRawConfigValuesParseValue<SerialConfig::FlowControl>("flow_control", &SerialConfig::getFlowControl,
			"!@#$<>", SerialConfig::FlowControl(), false);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SerialConfigTest);

/// Validations module testing suite
class ValidationsTest: public TestFixture {
private:
	CPPUNIT_TEST_SUITE(ValidationsTest);
	CPPUNIT_TEST(testIsValidIPv4AddressMinimal);
	CPPUNIT_TEST(testIsValidIPv4AddressMaximal);
	CPPUNIT_TEST(testIsValidIPv4AddressValidAddresses);
	CPPUNIT_TEST(testIsValidIPv4AddressInvalidAddresses);
	CPPUNIT_TEST_SUITE_END();

public:
	void scenarioTestIsValidIPv4Address(const string &rawAddress, bool shouldBeValid) {
		CPPUNIT_ASSERT(isValidIPv4Address(rawAddress) == shouldBeValid);
	}

	void testIsValidIPv4AddressMinimal() {
		scenarioTestIsValidIPv4Address("0.0.0.0", true);
	}

	void testIsValidIPv4AddressMaximal() {
		scenarioTestIsValidIPv4Address("255.255.255.255", true);
	}

	void testIsValidIPv4AddressValidAddresses() {
		scenarioTestIsValidIPv4Address("1.22.100.255", true);
		scenarioTestIsValidIPv4Address("100.99.3.7", true);
		scenarioTestIsValidIPv4Address("201.0.0.0", true);
	}

	void testIsValidIPv4AddressInvalidAddresses() {
		scenarioTestIsValidIPv4Address("....", false);
		scenarioTestIsValidIPv4Address("sdfsdfs", false);
		scenarioTestIsValidIPv4Address("256.1.2.1", false);
		scenarioTestIsValidIPv4Address("100.1.1.1.", false);
		scenarioTestIsValidIPv4Address("5.6.3.300", false);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ValidationsTest);

// Runs all registered test suites.
int main() {
	TextUi::TestRunner runner;
	TestFactoryRegistry &registry = TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());
	bool wasSuccessful = runner.run("", false);
	return !wasSuccessful;
}

// End of file tester.cc
