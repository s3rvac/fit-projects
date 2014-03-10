/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    bridge.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Serial-TCP/IP bridge - main() definition.
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <csignal>

#include "debug.h"
#include "error.h"
#include "io.h"
#include "config_parser.h"
#include "conversions.h"
#include "serial_config.h"
#include "serial_connection.h"
#include "tcpip_config.h"
#include "tcpip_connection.h"
#include "tcpip_server.h"
#include "connection.h"
#include "shared_buffer.h"
#include "threading_error.h"

using namespace std;

namespace {
	/**
	 * @brief Data of a single connection.
	 */
	class ConnectionData {
		public:
			/**
			 * @brief Constructor.
			 */
			ConnectionData(Connection *conn, time_t connTimeout):
				connection(conn), connectionTimeout(connTimeout),
				connectionLastActionTime(time(0)), closeConnection(false),
				inputBuffer(), outputBuffer(), removeThisData(false) { }

			/**
			 * @brief Destructor.
			 */
			~ConnectionData() {
				delete connection;
				connection = 0;
			}

		public:
			/// Connection.
			Connection * volatile connection;
			/// Connection timeout.
			volatile time_t connectionTimeout;
			/// Last action time of the connection.
			volatile time_t connectionLastActionTime;
			/// Should the connection be closed?
			volatile bool closeConnection;
			/// Input buffer.
			SharedBuffer inputBuffer;
			/// Output buffer.
			SharedBuffer outputBuffer;
			/// Connection was closed - remove this data.
			volatile bool removeThisData;
			/// Connection handler.
			pthread_t connectionHandler;
	};

	/**
	 * @brief Connections storage.
	 *
	 * This class is a wrapper around a standard container and provides
	 * automatical memory deallocation for stored pointers in the destructor.
	 */
	class ConnectionsDataStorage {
		public:
			/// Underling container type.
			typedef list<ConnectionData *> StorageContainer;

			/// Iterator.
			typedef StorageContainer::iterator iterator;

			/// Pointer to connection data.
			/// (declared because of the push_back() signature)
			typedef ConnectionData *ConnectionDataPtr;

		public:
			/**
			 * @brief Constructor.
			 */
			ConnectionsDataStorage() { }

			/**
			 * @brief Destructor.
			 *
			 * All stored items will be deleted using operator delete.
			 */
			~ConnectionsDataStorage() {
				for (StorageContainer::iterator iter = _storage.begin();
						iter != _storage.end(); ++iter) {
					delete *iter;
					*iter = 0;
				}
			}

			/**
			 * @brief Returns the number of items in the container.
			 */
			StorageContainer::size_type size() const {
				return _storage.size();
			}

			/**
			 * @brief Adds a new item at the end of the container.
			 *
			 * @param data Item to be added.
			 */
			void push_back(const ConnectionDataPtr &data) {
				_storage.push_back(data);
			}

			/**
			 * @brief Returns an iterator referring to the first item in the container.
			 */
			iterator begin() {
				return _storage.begin();
			}

			/**
			 * @brief Returns an iterator referring to the past-the-end item in the container.
			 */
			iterator end() {
				return _storage.end();
			}

			/**
			 * @brief Removes a single item from the container.
			 *
			 * Calls a delete operator on that item.
			 */
			iterator erase(iterator position) {
				delete *position;
				*position = 0;
				return _storage.erase(position);
			}

		private:
			/// Underlying container.
			StorageContainer _storage;
	};

	// This variable is set to false when the program receives a terminating
	// signal - after that, the server will stop and all connections
	// will be properly closed - then the program stops
	volatile bool programShouldContinue = true;

	// Signal handler
	void signalHandler(int signalNumber) {
		// Program should be stopped only if some of these signals is received
		switch (signalNumber) {
			case SIGABRT:
			case SIGHUP:
			case SIGINT:
			case SIGTERM:
			case SIGQUIT:
				programShouldContinue = false;
				break;

			default:
				break;
		}
	}

	// Pause time (in nanoseconds)
	const long PAUSE_TIME_NSECS = 200000000L; // 0.2 seconds

	// Waits for a few nanoseconds (possibly 0), depending on the
	// connections last action time
	void pauseExecution(time_t lastActionTime) {
		time_t timeDifference = time(0) - lastActionTime;
		if (timeDifference <= 1) {
			// The connection is busy, don't wait at all
			return;
		}

		timespec sleep_spec;
		sleep_spec.tv_sec = 0;
		sleep_spec.tv_nsec = PAUSE_TIME_NSECS;
		nanosleep(&sleep_spec, 0);
	}

	// Connection handler thread.
	//
	// Connection handler reads data from the connection into output buffer,
	// sends messages from the input buffer and manages connection timeout
	// and closing.
	//
	// Preconditions:
	//  - arg must be of type ConnectionData
	//  - arg must be not null (arg != 0)
	//
	// Returns null pointer on exit.
	void *connectionHandler(void *arg) {
		assert(arg != 0);

		ConnectionData *cd = static_cast<ConnectionData *>(arg);
		try {
			while (!cd->closeConnection) {
				// If timeout is enabled, close the connection if its timeout expired
				if (cd->connectionTimeout > 0) {
					if ((time(0) - cd->connectionLastActionTime) > cd->connectionTimeout) {
						logDebugMessage(string("connection timeout"));

						throw Connection::ConnectionClosed(string("connection timeout expired"));
					}
				}

				// Check the connection for incoming messages
				string receivedMessage = cd->connection->tryReceiveMessage();
				if (receivedMessage.size() > 0) {
					logDebugMessage(string("message received (" +
						toString(receivedMessage.size()) + ")"));

					// Update the last action time
					cd->connectionLastActionTime = time(0);

					cd->outputBuffer.addMessage(receivedMessage);
				}

				// Send a message from the input queue (if there is some)
				if (cd->inputBuffer.size() > 0) {
					SharedBuffer::MessageWithId messageToSend = cd->inputBuffer.getMessage();
					size_t bytesSent = cd->connection->trySendMessage(messageToSend.first);
					if (bytesSent == messageToSend.first.size()) {
						// The message was sent successfully
						logDebugMessage(string("message sent (" +
							toString(messageToSend.first.size()) + ")"));

						// Message was sent successfully, remove it from the buffer
						cd->inputBuffer.removeMessage(messageToSend.second);

						// Update the last action time
						cd->connectionLastActionTime = time(0);
					} else if (bytesSent > 0 && bytesSent < messageToSend.first.size()) {
						// Only a part of the message was sent -> we have to
						// send the rest of the message next time
						logDebugMessage(string("part of message sent (" +
							toString(bytesSent) + ")"));

						cd->inputBuffer.changeMessage(messageToSend.second,
							messageToSend.first.substr(bytesSent));

						// Update the last action time
						cd->connectionLastActionTime = time(0);
					}
				}

				// Wait a few nanoseconds to lower CPU usage
				pauseExecution(cd->connectionLastActionTime);
			}
		} catch (const Connection::ConnectionClosed &e) {
			// Do nothing
			logDebugMessage("connection closed");
		} catch (const BaseException &e) {
			if (programShouldContinue) {
				printErrorMessage(e.what());
			}
		}

		if (cd->connection != 0) {
			cd->connection->close();
		}
		cd->removeThisData = true;

		return 0;
	}

	// Creates a handler for the selected connection
	void createHandler(ConnectionData *connectionData) {
		// Create a new thread
		int rc = pthread_create(&connectionData->connectionHandler, 0,
			connectionHandler, connectionData);
		if (rc != 0) {
			throw ThreadingError(string("cannot create connection handler (reason: ") +
				errnoToString(rc) + ")");
		}

		// We won't need to wait for this thread terminations, so
		// the storage for the thread can be reclaimed when that thread terminates
		pthread_detach(connectionData->connectionHandler);
	}
}

int main(int argc, const char **argv) {
	// Check program arguments (this is so simple so I didn't create a special
	// class for that)
	if (argc > 1 && (argv[1] == string("-h") || argv[1] == string("--help"))) {
		printHelp(argv[0], cout);
		return EXIT_SUCCESS;
	} else if (argc != 3 || (argc == 3 && argv[1] != string("-f"))) {
		printHelp(argv[0], cerr);
		return EXIT_FAILURE;
	}

	// Arguments should be correct
	string configFilePath(argv[2]);

	int mainReturnCode = EXIT_SUCCESS;
	try {
		// Open the configuration file
		errno = 0;
		ifstream configFile(configFilePath.c_str());
		if (!configFile) {
			int errnoBackup = errno;
			printErrorMessage(string("file ") + configFilePath + " cannot be opened (reason: " +
				errnoToString(errnoBackup) + ")");
			return EXIT_FAILURE;
		}

		// Read all file data
		string configFileContent(readAllStreamContent(configFile));

		// Parse the configuration file content
		ConfigParser::ParsedConfig parsedConfig(ConfigParser::parse(configFileContent));

		// Parse both TCP/IP and serial configurations
		TCPIPConfig tcpipConfig(TCPIPConfig::fromRawConfigValues(parsedConfig));
		SerialConfig serialConfig(SerialConfig::fromRawConfigValues(parsedConfig));

		// Set signal handling (it is not tested whether the signal handler
		// was registered successfully because if it wasn't, there is no
		// serious problem and the program can continue - only connections
		// will not be closed by the program itself in this situation, but by the OS)
		struct sigaction signalAction;
		sigemptyset(&signalAction.sa_mask);
		signalAction.sa_handler = signalHandler;
		signalAction.sa_flags = 0;
		sigaction(SIGABRT, &signalAction, 0);
		sigaction(SIGALRM, &signalAction, 0);
		sigaction(SIGCHLD, &signalAction, 0);
		sigaction(SIGFPE, &signalAction, 0);
		sigaction(SIGHUP, &signalAction, 0);
		sigaction(SIGILL, &signalAction, 0);
		sigaction(SIGINT, &signalAction, 0);
		sigaction(SIGQUIT, &signalAction, 0);
		sigaction(SIGPIPE, &signalAction, 0);
		sigaction(SIGTERM, &signalAction, 0);
		sigaction(SIGUSR1, &signalAction, 0);
		sigaction(SIGUSR2, &signalAction, 0);

		// Create the server and start listening
		auto_ptr<TCPIPServer> server(new TCPIPServer(tcpipConfig));

		// Create a connection to the serial port, connection data
		// and a handler for this connection
		ConnectionData serialConnectionData(new SerialConnection(serialConfig), 0);
		createHandler(&serialConnectionData);
		logDebugMessage(string("serial connection created"));

		// Main server loop
		ConnectionsDataStorage tcpipConnectionsData;
		while (programShouldContinue) {
			try {
				// Check for pending incoming TCP/IP connection
				auto_ptr<TCPIPConnection> connectionFromClient(server->tryAcceptConnection());
				if (connectionFromClient.get() != 0) {
					// There is an incoming connection
					logDebugMessage(string("TCP/IP connection accepted (") +
						connectionFromClient->getAddress() + ":" +
						toString(connectionFromClient->getPort()) + ")");

					// Store this connection...
					ConnectionData *tcpipConnectionData = new ConnectionData(
						connectionFromClient.release(), tcpipConfig.getTimeout());
					tcpipConnectionsData.push_back(tcpipConnectionData);

					// ...and create a handler for it
					createHandler(tcpipConnectionData);
				}

				// Check whether there is a message in the serial connection output buffer
				// to be sent to all clients (only one message is get)
				SharedBuffer::MessageWithId serialMessage = serialConnectionData.outputBuffer.getMessage();
				if (serialMessage != SharedBuffer::INVALID_MESSAGE) {
					// There is a message -> add it into every TCP/IP connections input buffer
					for (ConnectionsDataStorage::iterator tcpipConnIter = tcpipConnectionsData.begin();
							tcpipConnIter != tcpipConnectionsData.end(); ++tcpipConnIter) {
						(*tcpipConnIter)->inputBuffer.addMessage(serialMessage.first);
					}

					serialConnectionData.outputBuffer.removeMessage(serialMessage.second);
				}

				// Check whether there are any client messages to be sent to the serial connection
				// (only one message per connection is get)
				for (ConnectionsDataStorage::iterator tcpipConnIter = tcpipConnectionsData.begin();
						tcpipConnIter != tcpipConnectionsData.end(); ++tcpipConnIter) {
					SharedBuffer::MessageWithId tcpipMessage = (*tcpipConnIter)->outputBuffer.getMessage();
					if (tcpipMessage != SharedBuffer::INVALID_MESSAGE) {
						// There is a message -> add it to the serial connections input buffer
						serialConnectionData.inputBuffer.addMessage(tcpipMessage.first);
						(*tcpipConnIter)->outputBuffer.removeMessage(tcpipMessage.second);
					}
				}

				// Check whether there are any connections to be removed
				for (ConnectionsDataStorage::iterator tcpipConnIter = tcpipConnectionsData.begin();
						tcpipConnIter != tcpipConnectionsData.end(); ++tcpipConnIter) {
					if ((*tcpipConnIter)->removeThisData) {
						tcpipConnIter = tcpipConnectionsData.erase(tcpipConnIter);
					}
				}

				// Wait a few nanoseconds to lower CPU usage
				pauseExecution(serialConnectionData.connectionLastActionTime);
			} catch (const BaseException &e) {
				printErrorMessage(e.what());
				mainReturnCode = EXIT_FAILURE;

				// Server error - program cannot continue
				// (connection errors are catched in connection handlers)
				break;
			}
		}

		// Close all connections and free allocated memory
		server->stopListening();

		// Close the serial connection
		serialConnectionData.closeConnection = true;
		// Close all TCP/IP connections
		for (ConnectionsDataStorage::iterator tcpipConnIter = tcpipConnectionsData.begin();
				tcpipConnIter != tcpipConnectionsData.end(); ++tcpipConnIter) {
			(*tcpipConnIter)->closeConnection = true;
		}

		// Wait until the serial connection gets closed
		// (but wait at most one second)
		time_t serialConnStopStartingTime = time(0);
		while (!serialConnectionData.removeThisData &&
				(time(0) - serialConnStopStartingTime) < 1)
			;

		// Wait until all TCP/IP connections gets closed
		// (but wait at most one second)
		time_t tcpipConnsStopStartingTime = time(0);
		while (tcpipConnectionsData.size() > 0 && (time(0) - tcpipConnsStopStartingTime) < 1) {
			for (ConnectionsDataStorage::iterator tcpipConnIter = tcpipConnectionsData.begin();
					tcpipConnIter != tcpipConnectionsData.end(); ++tcpipConnIter) {
				if ((*tcpipConnIter)->removeThisData) {
					tcpipConnIter = tcpipConnectionsData.erase(tcpipConnIter);
				}
			}
		}

		// If there is some connection that was not closed already, force its close
		serialConnectionData.connection->close();
		for (ConnectionsDataStorage::iterator tcpipConnIter = tcpipConnectionsData.begin();
					tcpipConnIter != tcpipConnectionsData.end(); ++tcpipConnIter) {
			(*tcpipConnIter)->connection->close();
		}
	} catch (const bad_alloc &e) {
		printErrorMessage("insufficient memory");
		return EXIT_FAILURE;
	} catch (const exception &e) {
		printErrorMessage(e.what());
		return EXIT_FAILURE;
	}

	return mainReturnCode;
}

// End of file bridge.cc
