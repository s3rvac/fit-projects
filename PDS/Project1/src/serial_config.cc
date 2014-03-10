/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    serial_config.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   SerialConfig class - definitions.
 *
 */

#include "debug.h"
#include "conversions.h"
#include "serial_config.h"

using namespace std;

// Static member variables initialization
const string SerialConfig::DEFAULT_DEVICE = "/dev/ttyS0";
const speed_t SerialConfig::DEFAULT_SPEED = B9600;
const tcflag_t SerialConfig::DEFAULT_DATA_BITS = CS8;
const SerialConfig::Parity SerialConfig::DEFAULT_PARITY = SerialConfig::NONE_PARITY;
const SerialConfig::FlowControl SerialConfig::DEFAULT_FLOW_CONTROL = SerialConfig::NONE_FLOW_CONTROL;
const SerialConfig::StopBits SerialConfig::DEFAULT_STOP_BITS = SerialConfig::STOP_BITS_1;

SerialConfig::SerialConfig SerialConfig::fromRawConfigValues(
		SerialConfig::RawConfigValues rawConfigValues) {
	return SerialConfig(rawConfigValues);
}

SerialConfig::~SerialConfig() { }

SerialConfig::SerialConfig(const SerialConfig::SerialConfig &other):
	Config(other), _device(other._device), _speed(other._speed), _dataBits(other._dataBits),
	_parity(other._parity), _stopBits(other._stopBits), _flowControl(other._flowControl) { }

SerialConfig::SerialConfig & SerialConfig::operator=(const SerialConfig::SerialConfig &other) {
	SerialConfig temp(other);
	swap(temp);
	return *this;
}

bool SerialConfig::operator==(const SerialConfig::SerialConfig &other) const {
	return Config::operator==(other) && _device == other._device &&
		_speed == other._speed &&
		_dataBits == other._dataBits &&
		_parity == other._parity &&
		_stopBits == other._stopBits &&
		_flowControl == other._flowControl;
}

bool SerialConfig::operator!=(const SerialConfig::SerialConfig &other) const {
	return !(*this == other);
}

void SerialConfig::swap(SerialConfig &other) {
	Config::swap(other);
	_device.swap(other._device);
	::swap(_speed, other._speed);
	::swap(_dataBits, other._dataBits);
	::swap(_parity, other._parity);
	::swap(_stopBits, other._stopBits);
	::swap(_flowControl, other._flowControl);
}

SerialConfig::SerialConfig():
	Config(), _device(DEFAULT_DEVICE), _speed(DEFAULT_SPEED),
	_dataBits(DEFAULT_DATA_BITS), _parity(DEFAULT_PARITY),
	_stopBits(DEFAULT_STOP_BITS), _flowControl(DEFAULT_FLOW_CONTROL) { }

SerialConfig::SerialConfig(Config::RawConfigValues rawConfigValues):
		Config(rawConfigValues), _device(DEFAULT_DEVICE), _speed(DEFAULT_SPEED),
		_dataBits(DEFAULT_DATA_BITS), _parity(DEFAULT_PARITY), _stopBits(DEFAULT_STOP_BITS),
		_flowControl(DEFAULT_FLOW_CONTROL) {
	_device = (rawConfigValues.find("device") != rawConfigValues.end()) ?
		parseDevice(rawConfigValues["device"]) : DEFAULT_DEVICE;
	_speed = (rawConfigValues.find("speed") != rawConfigValues.end()) ?
		parseSpeed(rawConfigValues["speed"]) : DEFAULT_SPEED;
	_dataBits = (rawConfigValues.find("data_bits") != rawConfigValues.end()) ?
		parseDataBits(rawConfigValues["data_bits"]) : DEFAULT_DATA_BITS;
	_parity = (rawConfigValues.find("parity") != rawConfigValues.end()) ?
		parseParity(rawConfigValues["parity"]) : DEFAULT_PARITY;
	_stopBits = (rawConfigValues.find("stop_bits") != rawConfigValues.end()) ?
		parseStopBits(rawConfigValues["stop_bits"]) : DEFAULT_STOP_BITS;
	_flowControl = (rawConfigValues.find("flow_control") != rawConfigValues.end()) ?
		parseFlowControl(rawConfigValues["flow_control"]) : DEFAULT_FLOW_CONTROL;
}

string SerialConfig::parseDevice(const std::string &rawDevice) {
	// Nothing to really parse here, so just return the raw value
	return rawDevice;
}

speed_t SerialConfig::parseSpeed(const std::string &rawSpeed) {
	// There can be more possible speed values on some systems, but these seems
	// to be defined by POSIX, so I'll only allow only these
	if (rawSpeed == "50") return B50;
	else if (rawSpeed == "75") return B75;
	else if (rawSpeed == "110") return B110;
	else if (rawSpeed == "134") return B134;
	else if (rawSpeed == "150") return B150;
	else if (rawSpeed == "200") return B200;
	else if (rawSpeed == "300") return B300;
	else if (rawSpeed == "600") return B600;
	else if (rawSpeed == "120") return B1200;
	else if (rawSpeed == "1800") return B1800;
	else if (rawSpeed == "2400") return B2400;
	else if (rawSpeed == "4800") return B4800;
	else if (rawSpeed == "9600") return B9600;
	else if (rawSpeed == "19200") return B19200;
	else if (rawSpeed == "38400") return B38400;
	else if (rawSpeed == "57600") return B57600;
	else if (rawSpeed == "115200") return B115200;
	else {
		// The speed value is invalid
		throw InvalidValueError(string("Serial configuration parsing error: ") +
			"invalid speed (" + rawSpeed + ")");
	}
}

tcflag_t SerialConfig::parseDataBits(const std::string &rawDataBits) {
	if (rawDataBits == "5") return CS5;
	else if (rawDataBits == "6") return CS6;
	else if (rawDataBits == "7") return CS7;
	else if (rawDataBits == "8") return CS8;
	else {
		// The number of data bits value is invalid
		throw InvalidValueError(string("Serial configuration parsing error: ") +
			"invalid number of data bits (" + rawDataBits + ")");
	}
}

SerialConfig::Parity SerialConfig::parseParity(const std::string &rawParity) {
	if (rawParity == "none") return NONE_PARITY;
	else if (rawParity == "even") return EVEN_PARITY;
	else if (rawParity == "odd") return ODD_PARITY;
	else if (rawParity == "mark") return MARK_PARITY;
	else if (rawParity == "space") return SPACE_PARITY;
	else {
		// The parity bit generation type is invalid
		throw InvalidValueError(string("Serial configuration parsing error: ") +
			"invalid parity (" + rawParity + ")");
	}
}

SerialConfig::StopBits SerialConfig::parseStopBits(const std::string &rawStopBits) {
	if (rawStopBits == "1") return STOP_BITS_1;
	else if (rawStopBits == "1.5") return STOP_BITS_1_5;
	else if (rawStopBits == "2") return STOP_BITS_2;
	else {
		// The stop bits value is invalid
		throw InvalidValueError(string("Serial configuration parsing error: ") +
			"invalid number of stop bits (" + rawStopBits + ")");
	}
}

SerialConfig::FlowControl SerialConfig::parseFlowControl(const std::string &rawFlowControl) {
	if (rawFlowControl == "none") return NONE_FLOW_CONTROL;
	else if (rawFlowControl == "hardware") return HARDWARE_FLOW_CONTROL;
	else if (rawFlowControl == "xon/off") return SOFTWARE_FLOW_CONTROL;
	else {
		// The flow control type is invalid
		throw InvalidValueError(string("Serial configuration parsing error: ") +
			"invalid flow control type (" + rawFlowControl + ")");
	}
}

// End of file serial_config.cc
