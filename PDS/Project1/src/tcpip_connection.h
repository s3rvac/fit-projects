/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    tcpip_connection.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   TCPIPConnection class - declarations.
 *
 */

#ifndef TCPIP_CONNECTION_H
#define TCPIP_CONNECTION_H

#include <tr1/cstdint>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "connection.h"
#include "tcpip_config.h"

/**
 * @brief TCP/IP connection.
 *
 * Objects of this class can be created only by a TCPIPServer.
 *
 * It provides reference semantics (objects copying and comparison is NOT allowed).
 */
class TCPIPConnection: public Connection {
	public:
		friend class TCPIPServer;

	public:
		/**
		 * @brief Destructor.
		 */
		virtual ~TCPIPConnection();

		/**
		 * @brief Returns IPv4 address of the connected node.
		 */
		std::string getAddress() const;

		/**
		 * @brief Returns port number through which the connected node is connected.
		 */
		unsigned getPort() const;

		// Override
		virtual void close();

	private:
		/**
		 * @brief Constructor.
		 *
		 * @param socket Created and connected socket.
		 * @param socketInfo Initialized sockaddr_in structure.
		 * @param config Connection configuration.
		 */
		TCPIPConnection(int socket, sockaddr_in socketInfo, const TCPIPConfig &config);

		// Override.
		virtual std::string doReceiveMessage(Config::LengthFieldMessageLength messageLength);

		// Override.
		virtual size_t doSendMessage(const std::string &message);

		// Disable copy ctor and assignment operator since this is
		// a reference object
		TCPIPConnection(const TCPIPConnection &);
		TCPIPConnection &operator=(const TCPIPConnection &);

	private:
		/// Connection socket.
		int _socket;
		/// Socket information.
		sockaddr_in _socketInfo;

	private:
		/// Default buffer size used when receiving a message via length_field
		static const size_t DEFAULT_LENGTH_FIELD_BUFFER_SIZE = 4096;
};

#endif // #ifndef TCPIP_CONNECTION_H

// End of file tcpip_connection.h
