/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    connection.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Connection class - definitions.
 *
 */

#include "debug.h"
#include "connection.h"
#include "auto_array.h"

using namespace std;
using namespace std::tr1;

Connection::Connection(const Config &config, time_t timeout):
	_config(config), _timeout(timeout) {}

Connection::~Connection() {}

string Connection::tryReceiveMessage() {
	if (_config.getLengthField() > 0) {
		// Use length field
		return tryReceiveMessageViaLengthField();
	} else {
		// Use a buffer
		return tryReceiveMessageViaBuffer();
	}
}

size_t Connection::trySendMessage(const string &message) {
	if (_config.getLengthField() > 0) {
		// Use length field
		return trySendMessageViaLengthField(message);
	} else {
		// Normal message sending
		logDebugMessage(string("normal sending"));
		return doSendMessage(message);
	}
}

string Connection::tryReceiveMessageViaBuffer() {
	auto_array<char> buffer(new char[_config.getBufferSize()]);


	string receivedMessage = doReceiveMessage(_config.getBufferSize());
	if (receivedMessage.size() > 0) {
		// Part of the message was successfully received
		logDebugMessage(string("receiving via buffer_size"));

		// Receive the rest of the message
		try {
			return receivedMessage + receiveFixedLengthMessage(
				_config.getBufferSize() - receivedMessage.size());
		} catch (const ConnectionError &e) {
			// Some data were received, so send the received part of the message
			return receivedMessage;
		}
	} else {
		// Nothing to be received
		return string("");
	}
}

string Connection::tryReceiveMessageViaLengthField() {
	string lengthFieldMessage = doReceiveMessage(_config.getLengthField());
	if (lengthFieldMessage.size() > 0) {
		// Length of the message was successfully received
		logDebugMessage(string("receiving via length_field"));

		// Transform the binary number into a Config::LengthField
		Config::LengthFieldMessageLength messageLength = Config::binaryFieldLengthToNumber(
			lengthFieldMessage);
		logDebugMessage(string("message length: ") + toString(messageLength));
		if (messageLength == 0) {
			// Nothing to be received
			return string("");
		}

		// Receive the message
		return receiveFixedLengthMessage(messageLength);
	} else {
		// Nothing to be received
		return string("");
	}
}

size_t Connection::trySendMessageViaLengthField(const string &message) {
	logDebugMessage(string("sending via length_field"));

	// Check whether the message can be sent at once; if not, send only
	// a part of it
	Config::LengthFieldMessageLength maxMessageSize = Config::computeMaximalMessageLength(
		_config.getLengthField());
	string messageToSent = message.substr(0, maxMessageSize);

	// Generate and send the message size (length_field)
	string lengthField = Config::messageLengthToBinaryLengthField(
		messageToSent.size(), _config.getLengthField());
	sendMessageAtOnce(lengthField);

	// Send the whole message
	sendMessageAtOnce(messageToSent);

	return messageToSent.size();
}

string Connection::receiveFixedLengthMessage(Config::LengthFieldMessageLength messageLength) {
	if (messageLength == 0) {
		return string("");
	}

	// Set the last connection action time (so we can check timeout expiration)
	time_t connectionLastActionTime = time(0);

	// Create a receiving buffer
	size_t currentBufferSize = messageLength;
	auto_array<char> buffer(new char[currentBufferSize]);

	// Receive the message
	string receivedMessage;
	while (receivedMessage.size() < messageLength) {
		// Check timeout
		if (_timeout > 0) {
			if ((time(0) - connectionLastActionTime) > _timeout) {
				if (receivedMessage.size() > 0) {
					// Some data were received, so send the received part
					// of the message
					return receivedMessage;
				} else {
					throw Connection::ConnectionClosed(
						string("connection timeout expired"));
				}
			}
		}

		string messagePart;
		try {
			messagePart = doReceiveMessage(currentBufferSize);
		} catch (const ConnectionError &e) {
			if (receivedMessage.size() > 0) {
				// Some data were received, so send the received part of the message
				return receivedMessage;
			} else {
				throw;
			}
		}
		if (messagePart.size() > 0) {
			// Part of the message was successfully received
			receivedMessage += messagePart;

			logDebugMessage(string("received: ") + toString(receivedMessage.size()) +
				string(" out of ") + toString(messageLength));

			// Change the buffer size so we won't receive more bytes than
			// we're supposed to receive
			Config::LengthFieldMessageLength nextSupposedMessagePartSize =
				messageLength - receivedMessage.size();
			if (nextSupposedMessagePartSize < currentBufferSize) {
				currentBufferSize = nextSupposedMessagePartSize;
			}

			// Update the last action time
			connectionLastActionTime = time(0);
		}
	}

	return receivedMessage;
}

void Connection::sendMessageAtOnce(const string &message) {
	// We'll be modyfing the message (shortening its length), so copy it
	string messageToSent = message;

	// Set the last connection action time (so we can check timeout expiration)
	time_t connectionLastActionTime = time(0);

	while (messageToSent.size() > 0) {
		// Check timeout
		if (_timeout > 0) {
			if ((time(0) - connectionLastActionTime) > _timeout) {
				throw Connection::ConnectionClosed(
					string("connection timeout expired"));
			}
		}

		logDebugMessage(string("still to be sent: ") + toString(messageToSent.size()));

		ssize_t sentBytes = doSendMessage(messageToSent);
		if (sentBytes > 0) {
			// Shorten the message
			messageToSent = messageToSent.substr(sentBytes);

			// Update the last action time
			connectionLastActionTime = time(0);
		}
	}
}

// End of file connection.cc
