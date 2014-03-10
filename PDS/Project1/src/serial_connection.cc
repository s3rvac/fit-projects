/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    serial_connection.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   SerialConnection class - definitions.
 *
 * The implementation and comments are based on the following documents:
 *  - http://www.c-program.com/pdf/serialPort_Programming_c.pdf
 *  - http://slackware.osuosl.org/slackware-3.3/docs/mini/Serial-Port-Programming
 *  - http://tldp.org/HOWTO/Serial-HOWTO.html
 *  - http://www.lothosoft.ch/thomas/libmip/markspaceparity.php
 */

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "debug.h"
#include "error.h"
#include "serial_connection.h"
#include "auto_array.h"

using namespace std;

SerialConnection::SerialConnection(const SerialConfig &config):
		Connection(config, 0), _serialFd(0) {
	// Open the serial device
	// (O_RDWR means that we open the port for reading and writing,
	// O_NOCTTY specifies that the program won't be the controlling
	// entity for the port (most user programs don't want this feature) and
	// O_NDELAY means that your program ignores the DCD line (if it didn't,
	// the program will be put to sleep until DCD is set to 'space' voltage)).
	errno = 0;
	_serialFd = open(config.getDevice().c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (_serialFd == -1) {
		int errnoBackup = errno;
		throw ConnectionError(string("unable to open ") + config.getDevice() +
			" (reason: " + errnoToString(errnoBackup) + ")");
	}

	// Get the current options for the port
	termios options;
	errno = 0;
	if (tcgetattr(_serialFd, &options) == -1) {
		int errnoBackup = errno;
		throw ConnectionError(string("getting parameters for " +
			config.getDevice() + " failed ") + "(reason: " +
			errnoToString(errnoBackup) + ")");
	}

	// Set the baud rates
	cfsetispeed(&options, config.getSpeed());
	cfsetospeed(&options, config.getSpeed());

	// Set the number of data bits
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= config.getDataBits();

	// Set the number of stop bits
	switch (config.getStopBits()) {
		// 1
		case SerialConfig::STOP_BITS_1:
			// Set the CSTOPB bit to 0
			options.c_cflag &= ~CSTOPB;
			break;

		// 1.5
		case SerialConfig::STOP_BITS_1_5:
			// POSIX does not support 1.5 stop bits
			throw ConnectionError("this system does not support 1.5 stop bits)");
			break;

		// 2
		case SerialConfig::STOP_BITS_2:
			options.c_cflag |= CSTOPB;
			break;

		default:
			// Program should never get there
			assert(false);
	}

	// Set parity bit generation type
	// HACK: Space and mark parity is set using an undocumennted flag CMSPAR;
	// see http://www.lothosoft.ch/thomas/libmip/markspaceparity.php
	switch (config.getParity()) {
		// No parity
		case SerialConfig::NONE_PARITY:
			options.c_cflag &= ~PARENB;
			break;

		// Even parity
		case SerialConfig::EVEN_PARITY:
			options.c_cflag |= PARENB;
			options.c_cflag &= ~PARODD;
			break;

		// Odd parity
		case SerialConfig::ODD_PARITY:
			options.c_cflag |= PARENB | PARODD;
			break;

		// Mark parity
		case SerialConfig::MARK_PARITY:
			options.c_cflag |= PARENB | CMSPAR | PARODD;
			break;

		// Space parity
		case SerialConfig::SPACE_PARITY:
			options.c_cflag |= PARENB | CMSPAR;
			options.c_cflag &= ~PARODD;
			break;

		default:
			// Program should never get there
			assert(false);
	}

	// Set the flow control type
	switch (config.getFlowControl()) {
		// No flow control
		case SerialConfig::NONE_FLOW_CONTROL:
			options.c_cflag &= ~CRTSCTS;
			break;

		// Hardware flow control
		case SerialConfig::HARDWARE_FLOW_CONTROL:
			options.c_cflag |= CRTSCTS;
			break;

		// Software flow control
		case SerialConfig::SOFTWARE_FLOW_CONTROL:
			options.c_iflag |= (IXON | IXOFF | IXANY);
			break;

		default:
			// Program should never get there
			assert(false);
	}

	// Enable the receiver and set local mode
	// (these will ensure that the program does not become the 'owner' of the port
	// subject to sporatic job control and hangup signals, and also that
	// the serial interface driver will read incoming data bytes)
	options.c_cflag |= (CLOCAL | CREAD);

	// Set the new options for the port
	// (the TCSANOW constant specifies that all changes should occur immediately
	// without waiting for output data to finish sending or input data to finish
	// receiving)
	errno = 0;
	if (tcsetattr(_serialFd, TCSANOW, &options) == -1) {
		int errnoBackup = errno;
		throw ConnectionError(string("setting parameters for " +
			config.getDevice() + " failed ") + "(reason: " +
			errnoToString(errnoBackup) + ")");
	}

	// Set nonblocking behaviour for all operations
	errno = 0;
	if (fcntl(_serialFd, F_SETFL, O_NONBLOCK) == -1) {
		int errnoBackup = errno;
		throw ConnectionError(string("setting nonblocking socket behaviour "
			"failed (reason: ") + errnoToString(errnoBackup) + ")");
	}
}

SerialConnection::~SerialConnection() {
	close();
}

void SerialConnection::close() {
	::close(_serialFd);
}

size_t SerialConnection::doSendMessage(const string &message) {
	errno = 0;
	ssize_t bytesWritten = write(_serialFd, message.c_str(), message.size());
	if (bytesWritten == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			// Message cannot be sent at the moment
			return 0;
		} else if (errno == EPIPE || errno == ECONNRESET) {
			// Connection was closed
			throw ConnectionClosed(string("connection is closed"));
		} else {
			int errnoBackup = errno;
			throw ConnectionError(string("message write failed (reason: ") +
				errnoToString(errnoBackup) + string(")"));
		}
	}

	return bytesWritten;
}

string SerialConnection::doReceiveMessage(Config::LengthFieldMessageLength messageLength) {
	auto_array<char> buffer(new char[messageLength]);

	errno = 0;
	ssize_t receivedBytes = read(_serialFd, buffer.get(), messageLength);
	if (receivedBytes > 0) {
		return string(buffer.get(), receivedBytes);
	} else if (receivedBytes == 0) {
		// Connection was closed
		throw ConnectionClosed(string("connection is closed"));
	} else if (receivedBytes == -1) {
		if (errno == EAGAIN) {
			// There is currently no message to be received
			return string("");
		} else {
			int errnoBackup = errno;
			throw ConnectionError(string("message receiving failed (reason: ") +
				errnoToString(errnoBackup) + string(")"));
		}
	}

	assert(false); // Program should never get there

	return string("");
}

// End of file serial_connection.cc
