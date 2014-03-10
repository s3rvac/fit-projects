/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    connection.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Connection class - declarations.
 *
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <string>

#include "base_exception.h"
#include "config.h"

/**
 * @brief Abstract class for all connection types.
 *
 * Each connection class should derive from this class.
 *
 * It provides reference semantics (objects copying and comparison is NOT allowed).
 */
class Connection {
	public:
		/**
		 * @brief This exception is thrown in case of a connection related problem.
		 */
		class ConnectionError: public BaseException {
			public:
				explicit ConnectionError(const std::string &errorMessage):
					BaseException(errorMessage) {}
		};

		/**
		 * @brief This exception is thrown when trying to read or send a message
		 *        while the connection is closed.
		 */
		class ConnectionClosed: public ConnectionError {
			public:
				explicit ConnectionClosed(const std::string &errorMessage):
					ConnectionError(errorMessage) {}
		};

	public:
		/**
		 * @brief Constructor.
		 *
		 * @param config Connection configuration.
		 * @param timeout Connection timeout (in seconds, if 0 then the timeout
		 *                is disabled).
		 */
		Connection(const Config &config, time_t timeout);

		/**
		 * @brief Destructor.
		 */
		virtual ~Connection();

		/**
		 * @brief Closes an established TCP connection.
		 *
		 * If the connection is not established, nothing will happen.
		 */
		virtual void close() = 0;

		/**
		 * @brief Tries to receive a message from a client.
		 *
		 * @return Received message or an empty string if there was no
		 *         message to be received.
		 *
		 * If there is some error, ConnectionError will be thrown.
		 * If the connection is closed, ConnectionClosed will be thrown.
		 */
		std::string tryReceiveMessage();

		/**
		 * @brief Tries to send a message to the client.
		 *
		 * @param message Message to be sent.
		 *
		 * @return Number of bytes that were actually sent (if < message.size(),
		 *         then the message did not fit into the sending buffer, so try
		 *         again later - you have to send only the part that wan't sent
		 *         yet!).
		 *
		 * If there is some error, ConnectionError will be thrown.
		 * If the connection is closed, ConnectionClosed will be thrown.
		 */
		size_t trySendMessage(const std::string &message);

	private:
		/**
		 * @brief Receives a message.
		 *
		 * @param messageLength Length of the message.
		 *
		 * @return Received message.
		 *
		 * This operation should be nonblocking.
		 *
		 * If there is some error, ConnectionError will be thrown.
		 * If the connection is closed, ConnectionClosed will be thrown.
		 */
		virtual std::string doReceiveMessage(Config::LengthFieldMessageLength messageLength) = 0;

		/**
		 * @brief Sends the selected message.
		 *
		 * @param message Message to be sent.
		 *
		 * @return Number of sent bytes.
		 *
		 * This operation should be nonblocking.
		 *
		 * If there is some error, ConnectionError will be thrown.
		 * If the connection is closed, ConnectionClosed will be thrown.
		 */
		virtual size_t doSendMessage(const std::string &message) = 0;

		/**
		 * @brief Tries to receive a message via fixed-length buffer.
		 *
		 * @return Received message or an empty string if there was no
		 *         message to be received.
		 *
		 * If there is some error, ConnectionError will be thrown.
		 * If the connection is closed, ConnectionClosed will be thrown.
		 */
		std::string tryReceiveMessageViaBuffer();

		/**
		 * @brief Tries to receive a message via length field.
		 *
		 * @return Received message or an empty string if there was no
		 *         message to be received.
		 *
		 * If there is some error, ConnectionError will be thrown.
		 * If the connection is closed, ConnectionClosed will be thrown.
		 */
		std::string tryReceiveMessageViaLengthField();

		/**
		 * @brief Receives a message of the selected length.
		 *
		 * @param messageLength Requested message length.
		 *
		 * @return Received message.
		 *
		 * If there is some error, ConnectionError will be thrown.
		 * If the connection is closed, ConnectionClosed will be thrown.
		 */
		std::string receiveFixedLengthMessage(Config::LengthFieldMessageLength messageLength);

		/**
		 * @brief Tries to send a message via length field.
		 *
		 * @param message Message to be sent.
		 *
		 * @return Number of sent bytes.
		 *
		 * If the whole message length does not fit into the length field,
		 * only the part of the message that can be send will be sent.
		 *
		 * This method checks timeout and if it expires, it throws
		 * ConnectionClosed.
		 *
		 * If there is some error, ConnectionError will be thrown.
		 * If the connection is closed, ConnectionClosed will be thrown.
		 */
		size_t trySendMessageViaLengthField(const std::string &message);

		/**
		 * @brief Sends the selected message at once.
		 *
		 * @param message Message to be sent.
		 *
		 * This method checks timeout and if it expires, it throws
		 * ConnectionClosed.
		 *
		 * If there is some error, ConnectionError will be thrown.
		 * If the connection is closed, ConnectionClosed will be thrown.
		 */
		void sendMessageAtOnce(const std::string &message);

		// Disable copy ctor and assignment operator since this is
		// a reference object
		Connection(const Connection &);
		Connection &operator=(const Connection &);

	private:
		/// Configuration.
		const Config _config;
		/// Connection timeout.
		time_t _timeout;
};

#endif // #ifndef CONNECTION_H

// End of file connection.h
