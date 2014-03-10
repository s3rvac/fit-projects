/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    shared_buffer.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   SharedBuffer class - declarations.
 *
 */

#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#include <string>
#include <utility>
#include <list>
#include <pthread.h>

#include "threading_error.h"

/**
 * @brief Message buffer that can be used in separate threads.
 *
 * All of the class operations are thread-safe. Each member function
 * can throw ThreadingError in case of some threading-related problem.
 *
 * It provides value semantics (objects copying and comparison is allowed).
 */
class SharedBuffer {
	public:
		/// Message ID.
		typedef unsigned long MessageId;

		/// Message with identification.
		typedef std::pair<std::string, MessageId> MessageWithId;

	private:
		/// Buffer type.
		typedef std::list<MessageWithId> Buffer;

	public:
		/**
		 * @brief Default constructor.
		 */
		SharedBuffer();

		/**
		 * @brief Destructor.
		 */
		~SharedBuffer();

		/**
		 * @brief Copy constructor.
		 *
		 * @param other Object that will be used as a model in the object creation.
		 *
		 * Constructs a copy of the @a other object.
		 */
		SharedBuffer(const SharedBuffer &other);

		/**
		 * @brief Assignment operator.
		 *
		 * @param other Object that will be assigned to this object.
		 *
		 * @return Reference to this object.
		 *
		 * Guarantees the strong exception safety.
		 */
		SharedBuffer & operator=(const SharedBuffer &other);

		/**
		 * @brief Equality comparison.
		 *
		 * @param other Object that will be compared with this object.
		 *
		 * @return True if this object is equal to the @a other object.
		 */
		bool operator==(const SharedBuffer &other);

		/**
		 * @brief Equality comparison.
		 *
		 * @param other Object that will be compared with this object.
		 *
		 * @return True if this object is equal to the @a other object.
		 */
		bool operator==(const SharedBuffer &other) const;

		/**
		 * @brief Non-equality comparison.
		 *
		 * @param other Object that will be compared with this object.
		 *
		 * @return True if this object is not equal to the @a other object.
		 */
		bool operator!=(const SharedBuffer &other);

		/**
		 * @brief Non-equality comparison.
		 *
		 * @param other Object that will be compared with this object.
		 *
		 * @return True if this object is not equal to the @a other object.
		 */
		bool operator!=(const SharedBuffer &other) const;

		/**
		 * @brief Returns the size of the buffer.
		 */
		Buffer::size_type size();

		/**
		 * @brief Returns the size of the buffer.
		 */
		Buffer::size_type size() const;

		/**
		 * @brief Returns the first message from the buffer.
		 *
		 * If there are no messages in the buffer, it returns INVALID_MESSAGE.
		 */
		MessageWithId getMessage();

		/**
		 * @brief Adds the selected message to the buffer.
		 *
		 * @param message The message to be added.
		 */
		void addMessage(const std::string &message);

		/**
		 * @brief Removes the selected message from the buffer.
		 *
		 * @param messageId The ID of the message to be removed.
		 *
		 * If there is no message with the selected ID, then this function
		 * does nothing.
		 */
		void removeMessage(MessageId messageId);

		/**
		 * @brief Changes the selected message with a new one.
		 *
		 * @param messageId The ID of the message to be changed.
		 * @param newMessage The new message.
		 *
		 * If there is no message with the selected ID, then this function
		 * does nothing.
		 */
		void changeMessage(MessageId messageId, const std::string &newMessage);

	public:
		/**
		 * @brief Invalid message.
		 *
		 * Can be returned by getMessage() if there are no messages in the buffer.
		 */
		static const MessageWithId INVALID_MESSAGE;

	private:
		/**
		 * @brief Locks the buffer mutex.
		 */
		void lockBufferMutex();

		/**
		 * @brief Locks the buffer mutex.
		 */
		void unlockBufferMutex();

		/**
		 * @brief Generates and returns a new message ID.
		 *
		 * @return Generated message ID.
		 */
		MessageId generateNewMessageId();

		/**
		 * @brief Initializes the buffer mutex.
		 */
		void initializeBufferMutex();

		/**
		 * @brief Swap the contents of this object with the other object.
		 *
		 * @param other Object that will be swapped with this object.
		 *
		 * Guarantees nothrow exception safety.
		 *
		 * Preconditions:
		 *  - mutex has to be locked
		 */
		void swap(SharedBuffer &other);

	private:
		/// Buffer.
		Buffer _buffer;
		/// Last message ID.
		MessageId _lastMessageId;
		/// Buffer mutex.
		pthread_mutex_t _bufferMutex;
};

#endif // #ifndef SHARED_BUFFER_H

// End of file shared_buffer.h
