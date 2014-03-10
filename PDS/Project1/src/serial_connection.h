/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    serial_connection.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   SerialConnection class - declarations.
 *
 */

#ifndef SERIAL_CONNECTION_H
#define SERIAL_CONNECTION_H



#include "connection.h"
#include "serial_config.h"

/**
 * @brief Serial connection (via a serial port).
 *
 * It provides reference semantics (objects copying and comparison is NOT allowed).
 */
class SerialConnection: public Connection {
	public:
		/**
		 * @brief Constructor.
		 *
		 * @param config Connection configuration.
		 */
		SerialConnection(const SerialConfig &config);

		/**
		 * @brief Destructor.
		 */
		virtual ~SerialConnection();

		// Override
		virtual void close();

	private:

		// Override.
		virtual std::string doReceiveMessage(Config::LengthFieldMessageLength messageLength);

		// Override.
		virtual size_t doSendMessage(const std::string &message);

		// Disable copy ctor and assignment operator since this is
		// a reference object
		SerialConnection(const SerialConnection &);
		SerialConnection &operator=(const SerialConnection &);

	private:
		/// File descriptor to the serial port.
		int _serialFd;
};

#endif // #ifndef SERIAL_CONNECTION_H

// End of file serial_connection.h
