/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    tcpip_config.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   TCPIPConfig class - definitions.
 *
 */

#include "debug.h"
#include "conversions.h"
#include "tcpip_config.h"

using namespace std;

namespace {
	const TCPIPConfig::Port MIN_PORT_NUMBER = 1;
	const TCPIPConfig::Port MAX_PORT_NUMBER = numeric_limits<TCPIPConfig::Port>::max();
	const TCPIPConfig::Timeout MIN_TIMEOUT = 0;
	const TCPIPConfig::Timeout MAX_TIMEOUT = numeric_limits<TCPIPConfig::Timeout>::max();

	// Returns the default interfaces value
	TCPIPConfig::Interfaces getDefaultInterfaces() {
		TCPIPConfig::Interfaces interfaces;
		interfaces.push_back(TCPIPConfig::ANY_INTERFACE);
		return interfaces;
	}
}

// Static member variables initialization
const string TCPIPConfig::ANY_INTERFACE = string("ANY");
const TCPIPConfig::Port TCPIPConfig::DEFAULT_PORT = 12000;
const TCPIPConfig::Timeout TCPIPConfig::DEFAULT_TIMEOUT = 60;

TCPIPConfig::TCPIPConfig TCPIPConfig::fromRawConfigValues(
		TCPIPConfig::RawConfigValues rawConfigValues) {
	return TCPIPConfig(rawConfigValues);
}

TCPIPConfig::~TCPIPConfig() { }

TCPIPConfig::TCPIPConfig(const TCPIPConfig::TCPIPConfig &other):
	Config(other), _interfaces(other._interfaces), _port(other._port),
	_timeout(other._timeout) { }

TCPIPConfig::TCPIPConfig & TCPIPConfig::operator=(const TCPIPConfig::TCPIPConfig &other) {
	TCPIPConfig temp(other);
	swap(temp);
	return *this;
}

bool TCPIPConfig::operator==(const TCPIPConfig::TCPIPConfig &other) const {
	return Config::operator==(other) && _interfaces == other._interfaces &&
		_port == other._port &&
		_timeout == other._timeout;
}

bool TCPIPConfig::operator!=(const TCPIPConfig::TCPIPConfig &other) const {
	return !(*this == other);
}

void TCPIPConfig::swap(TCPIPConfig &other) {
	Config::swap(other);
	_interfaces.swap(other._interfaces);
	::swap(_port, other._port);
	::swap(_timeout, other._timeout);
}

TCPIPConfig::TCPIPConfig():
	Config(), _interfaces(getDefaultInterfaces()), _port(DEFAULT_PORT),
	_timeout(DEFAULT_TIMEOUT) { }

TCPIPConfig::TCPIPConfig(Config::RawConfigValues rawConfigValues):
		Config(rawConfigValues), _interfaces(getDefaultInterfaces()),
		_port(DEFAULT_PORT), _timeout(DEFAULT_TIMEOUT) {
	_interfaces = (rawConfigValues.find("interface") != rawConfigValues.end()) ?
		parseInterfaces(rawConfigValues["interface"]) : getDefaultInterfaces();
	_port = (rawConfigValues.find("port") != rawConfigValues.end()) ?
		parsePort(rawConfigValues["port"]) : DEFAULT_PORT;
	_timeout = (rawConfigValues.find("timeout") != rawConfigValues.end()) ?
		parseTimeout(rawConfigValues["timeout"]) : DEFAULT_TIMEOUT;
}

TCPIPConfig::Interfaces TCPIPConfig::parseInterfaces(const std::string &rawInterfaces) {
	Interfaces interfaces;

	// Stringstream is used to "split" the rawInterfaces string into
	// "a list" of strings that were separated by whitespaces
	stringstream interfacesStream(rawInterfaces);
	string interface;
	while (interfacesStream >> interface) {
		if (interface == ANY_INTERFACE) {
			// Any interface (empty list)
			interfaces.push_back(ANY_INTERFACE);
			break;
		}

		interfaces.push_back(interface);
	}

	return interfaces;
}

TCPIPConfig::Port TCPIPConfig::parsePort(const string &rawPort) {
	return parseNumber("port", rawPort, MIN_PORT_NUMBER, MAX_PORT_NUMBER);
}

TCPIPConfig::Timeout TCPIPConfig::parseTimeout(const string &rawTimeout) {
	return parseNumber("timeout", rawTimeout, MIN_TIMEOUT, MAX_TIMEOUT);
}

// End of file tcpip_config.cc
