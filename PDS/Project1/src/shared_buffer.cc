/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    shared_buffer.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   SharedBuffer class - definitions.
 *
 */

#include <limits>
#include <algorithm>
#include <pthread.h>

#include "debug.h"
#include "error.h"
#include "shared_buffer.h"

using namespace std;

namespace {
	const SharedBuffer::MessageId MIN_MESSAGE_ID = 1;
	const SharedBuffer::MessageId MAX_MESSAGE_ID = numeric_limits<SharedBuffer::MessageId>::max();
}

const SharedBuffer::MessageWithId SharedBuffer::INVALID_MESSAGE = MessageWithId(
	string(""), 0);

SharedBuffer::SharedBuffer(): _buffer(), _lastMessageId(MIN_MESSAGE_ID),
		_bufferMutex() {
	initializeBufferMutex();
}

SharedBuffer::~SharedBuffer() {
	pthread_mutex_destroy(&_bufferMutex);
}

SharedBuffer::SharedBuffer(const SharedBuffer::SharedBuffer &other):
		_buffer(other._buffer), _lastMessageId(other._lastMessageId),
		_bufferMutex() {
	initializeBufferMutex();
}

SharedBuffer::SharedBuffer & SharedBuffer::operator=(const SharedBuffer::SharedBuffer &other) {
	lockBufferMutex();

	SharedBuffer temp(other);
	swap(temp);

	unlockBufferMutex();

	return *this;
}

bool SharedBuffer::operator==(const SharedBuffer::SharedBuffer &other) {
	lockBufferMutex();

	bool result = _buffer == other._buffer && _lastMessageId == other._lastMessageId;

	unlockBufferMutex();

	return result;
}

bool SharedBuffer::operator==(const SharedBuffer::SharedBuffer &other) const {
	// Since this method is const, I don't have to lock the mutex
	return _buffer == other._buffer && _lastMessageId == other._lastMessageId;
}

bool SharedBuffer::operator!=(const SharedBuffer::SharedBuffer &other) {
	lockBufferMutex();

	bool result = !(*this == other);

	unlockBufferMutex();

	return result;
}

bool SharedBuffer::operator!=(const SharedBuffer::SharedBuffer &other) const {
	// Since this method is const, I don't have to lock the mutex
	return !(*this == other);
}

SharedBuffer::Buffer::size_type SharedBuffer::size() {
	lockBufferMutex();

	Buffer::size_type result = _buffer.size();

	unlockBufferMutex();

	return result;
}

SharedBuffer::Buffer::size_type SharedBuffer::size() const {
	// Since this method is const, I don't have to lock the mutex
	return _buffer.size();
}

SharedBuffer::MessageWithId SharedBuffer::getMessage() {
	lockBufferMutex();

	MessageWithId messageWithId = _buffer.size() > 0 ? _buffer.front() : INVALID_MESSAGE;

	unlockBufferMutex();

	return messageWithId;
}

void SharedBuffer::addMessage(const string &message) {
	lockBufferMutex();

	_buffer.push_back(MessageWithId(message, generateNewMessageId()));

	unlockBufferMutex();
}

void SharedBuffer::removeMessage(SharedBuffer::MessageId messageId) {
	lockBufferMutex();

	for (Buffer::iterator bufferIter = _buffer.begin(); bufferIter != _buffer.end();
			++bufferIter) {
		if (bufferIter->second == messageId) {
			_buffer.erase(bufferIter);
			break;
		}
	}

	unlockBufferMutex();
}

void SharedBuffer::changeMessage(MessageId messageId, const string &newMessage) {
	lockBufferMutex();

	for (Buffer::iterator bufferIter = _buffer.begin(); bufferIter != _buffer.end();
			++bufferIter) {
		if (bufferIter->second == messageId) {
			bufferIter->first = newMessage;
			break;
		}
	}

	unlockBufferMutex();
}

void SharedBuffer::lockBufferMutex() {
	int rc = pthread_mutex_lock(&_bufferMutex);
	if (rc != 0) {
		throw ThreadingError(string("buffer mutex lock failed (reason: ") +
			errnoToString(rc) + ")");
	}
}


void SharedBuffer::unlockBufferMutex() {
	int rc = pthread_mutex_unlock(&_bufferMutex);
	if (rc != 0) {
		throw ThreadingError(string("buffer mutex unlock failed (reason: ") +
			errnoToString(rc) + ")");
	}
}

SharedBuffer::MessageId SharedBuffer::generateNewMessageId() {
	if (_lastMessageId == MAX_MESSAGE_ID) {
		_lastMessageId = MIN_MESSAGE_ID;
		return _lastMessageId;
	} else {
		return ++_lastMessageId;
	}
}

void SharedBuffer::initializeBufferMutex() {
	int rc = pthread_mutex_init(&_bufferMutex, 0);
	if (rc != 0) {
		throw ThreadingError(string("buffer mutex initialization failed (reason: ") +
			errnoToString(rc) + ")");
	}
}

void SharedBuffer::swap(SharedBuffer &other) {
	_buffer.swap(other._buffer);
	::swap(_lastMessageId, other._lastMessageId);
	::swap(_bufferMutex, other._bufferMutex);
}

// End of file shared_buffer.cc
