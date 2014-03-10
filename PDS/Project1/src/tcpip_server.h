/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    tcpip_server.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   TCPIPServer class - declarations.
 *
 */

#ifndef TCPIP_SERVER_H
#define TCPIP_SERVER_H

#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>

#include "tcpip_config.h"
#include "tcpip_connection.h"

/**
 * @brief TCPIP server that accepts client connections.
 *
 * This class provides a server which listens on the selected interface and
 * port and accepts client connections.
 *
 * It provides reference semantics (objects copying and comparison is NOT allowed).
 */
class TCPIPServer {
	public:
		/**
		 * @brief This exception is thrown in case of some server error.
		 */
		class TCPIPServerError: public BaseException {
			public:
				explicit TCPIPServerError(const std::string &errorMessage):
					BaseException(errorMessage) {}
		};

	public:
		/**
		 * @brief Constructor.
		 *
		 * @param config TCP/IP configuration.
		 *
		 * Creates and initializes a TCPIPServer object and starts listening
		 * (accepting new connections). If there is some error,
		 * TCPIPServerError will be thrown.
		 *
		 * Server "tryX" operations will be blocking (by default). You can
		 * change this by calling setBlocking(false).
		 */
		explicit TCPIPServer(const TCPIPConfig &config);

		/**
		 * @brief Destructor.
		 */
		~TCPIPServer();

		/**
		 * @brief Tries to accept a TCP/IP connection from a client.
		 *
		 * @return Accepted connection or auto_ptr<TCPIPConnection>(0),
		 *         if there was no connection to be accepted.
		 *
		 * The behaviour of this operation is nonblocking (i.e., when there is
		 * no connection to be accepted, it returns immediately).
		 *
		 * In case of a connection error, TCPIPConnection::TCPIPConnectionError
		 * exception is thrown.
		 */
		std::auto_ptr<TCPIPConnection> tryAcceptConnection();

		/**
		 * @brief Stops listening (accepting new connections).
		 */
		void stopListening();

	private:
		/// List of sockets.
		typedef std::vector<int> Sockets;

		/// List of IPv4 addresses.
		typedef std::vector<std::string> IPAddresses;

	private:
		// Disable copy ctor and assignment operator since this is
		// a reference object
		TCPIPServer(const TCPIPServer &);
		TCPIPServer &operator=(const TCPIPServer &);

		/**
		 * @brief Returns a list of IPv4 address on which the server should listen on.
		 *
		 * @param interfaces The list of network interfaces and/or IPv4 addresses
		 *                   (or TCPIPConfig::ANY_INTERFACE).
		 *
		 * @return All IPv4 addresses on which the server should listen on.
		 *
		 * If one value in the @a interfaces list is TCPIPConfig::ANY_INTERFACE,
		 * then the server should listen on all available interfaces and a list
		 * containing one value (TCPIPConfig::ANY_INTERFACE) will be returned.
		 *
		 * If there is an error (e.g. IP addresses for some interface cannot
		 * be obtained), TCPIPServerError exception will be thrown.
		 */
		IPAddresses getAddressesToListenOn(const TCPIPConfig::Interfaces &interfaces);

		/**
		 * @brief Returns all IPv4 addresses associated with the selected interface.
		 *
		 * @param interface The network interface.
		 *
		 * @return All IPv4 address associated with @a interface.
		 *
		 * If there is an error (e.g. non-existing interface), TCPIPServerError
		 * exception will be thrown.
		 *
		 * @todo Currently only one IP will be returned. I haven't found any
		 *       way how to obtain all IP addresses of an interface yet.
		 */
		IPAddresses getInterfaceAddresses(const std::string &interface);

	private:
		/// TCP/IP configuration.
		const TCPIPConfig _config;
		/// List of sockets on which the server listens.
		Sockets _sockets;

		/// Maximal incoming connection count.
		static const int MAX_CONNECTION_COUNT;
};

#endif // #ifndef TCPIP_SERVER_H

// End of file tcpip_server.h
