/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    tcpip_server.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   TCPIPServer class - definitions.
 *
 */

#include <vector>
#include <algorithm>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "debug.h"
#include "error.h"
#include "validations.h"
#include "tcpip_server.h"

using namespace std;

// Static member variables initialization
const int TCPIPServer::MAX_CONNECTION_COUNT = 32;

TCPIPServer::TCPIPServer(const TCPIPConfig &config):
		_config(config), _sockets() {
	IPAddresses ipAddresses = getAddressesToListenOn(config.getInterfaces());

	// We have to create a single socket for each IP address
	for (IPAddresses::iterator ipAddrIter = ipAddresses.begin();
			ipAddrIter != ipAddresses.end(); ++ipAddrIter) {
		// Create a socket
		int sock;
		errno = 0;
		if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
			int errnoBackup = errno;
			throw TCPIPServerError(string("socket creation failed (reason: ") +
				errnoToString(errnoBackup) + ")");
		}

		// Set SO_REUSEADDR on the socket to true (1)
		// This prevents errors when reusing the port number (for example when
		// restarting this the server on the same port in a very short time interval)
		int reuseAddrOption = 1;
		errno = 0;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseAddrOption,
				sizeof(reuseAddrOption)) == -1) {
			int errnoBackup = errno;
			throw TCPIPServerError(string("setting port number reuse option failed (reason: ") +
				errnoToString(errnoBackup) + ")");
		}

		// Set nonblocking behaviour for all operations
		int flags;
		errno = 0;
		if ((flags = fcntl(sock, F_GETFL, 0)) < 0) {
			int errnoBackup = errno;
			throw TCPIPServerError(string("getting socket flags failed (reason: ") +
				errnoToString(errnoBackup) + ")");
		}
		flags |= O_NONBLOCK;
		errno = 0;
		if (fcntl(sock, F_SETFL, flags) < 0) {
			int errnoBackup = errno;
			throw TCPIPServerError(string("setting socket flags failed (reason: ") +
				errnoToString(errnoBackup) + ")");
		}

		// TCP/IP connection settings
		sockaddr_in socketInfo;
		// Protocol family
		socketInfo.sin_family = AF_INET;
		// Port
		socketInfo.sin_port = htons(_config.getPort());
		// IP addresses, on which the server will listen
		if (*ipAddrIter == TCPIPConfig::ANY_INTERFACE) {
			// Listen on every interface
			socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
		} else {
			// Listen on this specific IP address
			socketInfo.sin_addr.s_addr = inet_addr(ipAddrIter->c_str());
		}

		// Bind the "name" (address) to the socket (reinterpret_cast<> is necessary here)
		errno = 0;
		if (bind(sock, reinterpret_cast<sockaddr *>(&socketInfo),
				sizeof(socketInfo)) == -1) {
			int errnoBackup = errno;
			close(sock);
			throw TCPIPServerError(string("name bind to the socket failed (reason: ") +
				errnoToString(errnoBackup) + ")");
		}

		// Create a client connection queue
		errno = 0;
		if (listen(sock, MAX_CONNECTION_COUNT) == -1) {
			int errnoBackup = errno;
			close(sock);
			throw TCPIPServerError(string("client connection queue creation failed (reason: ") +
				errnoToString(errnoBackup) + ")");
		}

		// Add this socket to the sockets list
		_sockets.push_back(sock);
	}
}

TCPIPServer::~TCPIPServer() {
	stopListening();
}

auto_ptr<TCPIPConnection> TCPIPServer::tryAcceptConnection() {
	for (Sockets::iterator iter = _sockets.begin(); iter != _sockets.end(); ++iter) {
		sockaddr_in socketInfo;
		socklen_t socketInfoSize = sizeof(socketInfo);

		// (reinterpret_cast<> is necessary here)
		errno = 0;
		int sock = accept(*iter, reinterpret_cast<sockaddr *>(&socketInfo),
			&socketInfoSize);
		if (errno == EAGAIN) {
			// There is currently no incoming connection for this socket
			// - try the next socket
			continue;
		} else if (sock == -1) {
			int errnoBackup = errno;
			throw Connection::ConnectionError(string(
				"client connection acceptance failed (reason: ") +
				errnoToString(errnoBackup) + ")");
		}

		return auto_ptr<TCPIPConnection>(new TCPIPConnection(sock, socketInfo, _config));
	}

	// No incomming connection
	return auto_ptr<TCPIPConnection>(0);
}

void TCPIPServer::stopListening() {
	for (Sockets::iterator iter = _sockets.begin(); iter != _sockets.end(); ++iter) {
		close(*iter);
	}
}

TCPIPServer::IPAddresses TCPIPServer::getAddressesToListenOn(const TCPIPConfig::Interfaces &interfaces) {
	IPAddresses ipAddresses;
	for (TCPIPConfig::Interfaces::const_iterator ifaceIter = interfaces.begin();
			ifaceIter != interfaces.end(); ++ifaceIter) {
		if (*ifaceIter == TCPIPConfig::ANY_INTERFACE) {
			// Server should listen on every possible interface
			ipAddresses.clear();
			ipAddresses.push_back(TCPIPConfig::ANY_INTERFACE);
			return ipAddresses;
		}

		// Check whether the selected interface is an IP address
		// (in this case just append the IP addres to ipAddresses)
		if (isValidIPv4Address(*ifaceIter)) {
			ipAddresses.push_back(*ifaceIter);
		} else {
			// Get the list of IP address associated with this interface...
			IPAddresses ifaceAddresses = getInterfaceAddresses(*ifaceIter);

			// ...and add them to the address container
			copy(ifaceAddresses.begin(), ifaceAddresses.end(), back_inserter(ipAddresses));
		}
	}

	return ipAddresses;
}

TCPIPServer::IPAddresses TCPIPServer::getInterfaceAddresses(const string &interface) {
	// To get the IP adress, a temporary socket has to be created
	int fd;
	errno = 0;
	if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		int errnoBackup = errno;
		throw TCPIPServerError(string("temporary socket creation failed (reason: ") +
			errnoToString(errnoBackup) + ")");
	}

	// Fill the ifreq structure
	ifreq ifr;
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, interface.c_str(), IF_NAMESIZE - 1);

	// Obtain the IP address
	errno = 0;
	if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
		int errnoBackup = errno;
		throw TCPIPServerError(string("IP address of the interface " +
			interface + " could not be obtained (reason: ") +
			errnoToString(errnoBackup) + ")");
	}

	close(fd);

	IPAddresses addresses;
	// (reinterpret_cast<> is necessary here)
	addresses.push_back(string(inet_ntoa(reinterpret_cast<struct sockaddr_in *>(
		&ifr.ifr_addr)->sin_addr)));
	return addresses;
}

// End of file tcpip_server.cc
