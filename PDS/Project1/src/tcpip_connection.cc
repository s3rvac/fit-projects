/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    tcpip_connection.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   TCPIPConnection class - definitions.
 *
 */

#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "debug.h"
#include "error.h"
#include "tcpip_connection.h"
#include "auto_array.h"

using namespace std;

const size_t TCPIPConnection::DEFAULT_LENGTH_FIELD_BUFFER_SIZE;

TCPIPConnection::TCPIPConnection(int socket, sockaddr_in socketInfo, const TCPIPConfig &config):
		Connection(config, config.getTimeout()), _socket(socket), _socketInfo(socketInfo) {
	// Set nonblocking behaviour for all operations
	int flags;
	errno = 0;
	if ((flags = fcntl(_socket, F_GETFL, 0)) == -1) {
		int errnoBackup = errno;
		throw ConnectionError(string("getting socket flags failed (reason: ") +
			errnoToString(errnoBackup) + ")");
	}
	flags |= O_NONBLOCK;
	errno = 0;
	if (fcntl(_socket, F_SETFL, flags) == -1) {
		int errnoBackup = errno;
		throw ConnectionError(string("setting socket flags failed (reason: ") +
			errnoToString(errnoBackup) + ")");
	}
}

TCPIPConnection::~TCPIPConnection() {
	close();
}

string TCPIPConnection::getAddress() const {
	return inet_ntoa(_socketInfo.sin_addr);
}

unsigned TCPIPConnection::getPort() const {
	return ntohs(_socketInfo.sin_port);
}

void TCPIPConnection::close() {
	::close(_socket);
}

size_t TCPIPConnection::doSendMessage(const string &message) {
	// Normal sending
	errno = 0;
	ssize_t bytesSent = send(_socket, message.c_str(), message.size(), MSG_NOSIGNAL);
	if (bytesSent == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			// Message cannot be sent at the moment
			return 0;
		} else if (errno == ECONNRESET || errno == ENOTCONN || errno == EPIPE) {
			// Connection was closed
			throw ConnectionError(string("connection is closed"));
		} else {
			int errnoBackup = errno;
			throw ConnectionError(string("message transmission failed (reason: ") +
				errnoToString(errnoBackup) + string(")"));
		}
	}

	return bytesSent;
}

string TCPIPConnection::doReceiveMessage(Config::LengthFieldMessageLength messageLength) {
	auto_array<char> buffer(new char[messageLength]);

	ssize_t receivedBytes = recv(_socket, buffer.get(), messageLength, 0);
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

// End of file tcpip_connection.cc
