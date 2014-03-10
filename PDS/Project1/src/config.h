/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    config.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Config class - declarations.
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <tr1/cstdint>

#include "base_exception.h"
#include "conversions.h"

/**
 * @brief Base class for all configuration classes.
 *
 * Subclasses should provide value semantics (object copying and comparison
 * should be allowed).
 */
class Config {
	public:
		/**
		 * @brief This exception should be thrown if some configuration value is invalid.
		 */
		class InvalidValueError: public BaseException {
			public:
				explicit InvalidValueError(const std::string &errorMessage):
					BaseException(errorMessage) {}
		};

		/// Raw configuration option -> values mapping.
		typedef std::map<std::string, std::string> RawConfigValues;

		/// Buffer size type.
		typedef size_t BufferSize;

		/// Length field type.
		typedef unsigned LengthField;

		/// Message length type when using length field.
		typedef std::tr1::uint32_t LengthFieldMessageLength;

	public:
		/**
		 * @brief Creates a Config object from raw configuration values.
		 *
		 * @param rawConfigValues Raw configuration option -> values mappings.
		 *
		 * If some passed value is invalid, InvalidValueError will be thrown.
		 *
		 * Preconditions:
		 *  - each option name and value must be without beginning and trailing
		 *    white spaces
		 *
		 * Meets the strong guarantee of exception safety.
		 */
		static Config fromRawConfigValues(RawConfigValues rawConfigValues);

		/**
		 * @brief Destructor.
		 *
		 * Destroys a Config object.
		 */
		~Config();

		/**
		 * @brief Copy constructor.
		 *
		 * @param other Object that will be used as a model in the object creation.
		 *
		 * Constructs a copy of the @a other object.
		 */
		Config(const Config &other);

		/**
		 * @brief Assignment operator.
		 *
		 * @param other Object that will be assigned to this object.
		 *
		 * @return Reference to this object.
		 *
		 * Guarantees the strong exception safety.
		 */
		Config & operator=(const Config &other);

		/**
		 * @brief Equality comparison.
		 *
		 * @param other Object that will be compared with this object.
		 *
		 * @return True if this object is equal to the @a other object.
		 */
		bool operator==(const Config &other) const;

		/**
		 * @brief Non-equality comparison.
		 *
		 * @param other Object that will be compared with this object.
		 *
		 * @return True if this object is not equal to the @a other object.
		 */
		bool operator!=(const Config &other) const;

		/**
		 * @brief Swap the contents of this object with the other object.
		 *
		 * @param other Object that will be swapped with this object.
		 *
		 * Guarantees nothrow exception safety.
		 */
		void swap(Config &other);

		/// @brief Returns the buffer size (in bytes).
		size_t getBufferSize() const { return _bufferSize; }

		/// @brief Returns the size of a field containing the length of a message (in bytes).
		LengthField getLengthField() const { return _lengthField; }

		/**
		 * @brief Transforms the selected binary string into a number.
		 *
		 * @param binaryNumber Binary field length value in a string.
		 *
		 * @return Numeral representation of @a binaryNumber (host byte order).
		 *
		 * Preconditions:
		 *  - 1 <= binaryNumber.size() <= sizeof(LengthFieldMessageLength)
		 *  - binaryNumber must be in network byte order (i.e. read from a socket)
		 */
		static LengthFieldMessageLength binaryFieldLengthToNumber(const std::string &binaryNumber);

		/**
		 * @brief Returns the maximal message size that can be stored in lengthField bytes.
		 *
		 * @param lengthField Size of the length field.
		 *
		 * Preconditions:
		 *  - lengthField must have been returned by getLengthField()
		 */
		static LengthFieldMessageLength computeMaximalMessageLength(unsigned lengthField);

		/**
		 * @brief Transforms the selected message length into a binary length field string.
		 *
		 * @param messageLength Length of the message.
		 * @param lengthField Length field value (returned from getLengthField()).
		 *
		 * @return Binary representation of @a messageLength in a string (network byte order).
		 *
		 * Precodnitions:
		 *  - messageLength must fit into lengthField bytes
		 *  - lengthField must have been returned by getLengthField()
		 *
		 * Postconditions:
		 *  - size of returned string will be @a lengthField.
		 */
		static std::string messageLengthToBinaryLengthField(LengthFieldMessageLength messageLength,
			unsigned lengthField);

	protected:
		/**
		 * @brief Default constructor.
		 *
		 * Constructs an empty Config object.
		 */
		Config();

		/**
		 * @brief Constructor.
		 *
		 * @param rawConfigValues Raw configuration option -> values mappings.
		 *
		 * Constructs a Config object from the selected values.
		 */
		Config(RawConfigValues rawConfigValues);

	protected:

		/**
		 * @brief Parses the selected number from the selected string.
		 *
		 * @param optionName Name of the option.
		 * @param rawValue Number value in a string.
		 * @param minValue Minimal valid number value.
		 * @param maxValue Maximal valid number value.
		 *
		 * @return Number that corresponds to the selected string.
		 *
		 * If there is some error, InvalidValueError will be thrown.
		 */
		template <typename T>
		static T parseNumber(const std::string &optionName,
				const std::string &rawValue, T minValue, T maxValue) {
			T value = 0;

			// Check whether the rawValue is a valid unsigned value
			// (I can use the trick toString(value) != rawValues because of
			// the precondition of the fromRawConfigValues() function)
			if (!strToNum(rawValue, value) || toString(value) != rawValue) {
				throw InvalidValueError(std::string("TCP/IP configuration parsing error: invalid ") +
					optionName + " (" + rawValue + ")");
			}

			// Check bounds
			if (value < minValue || value > maxValue) {
				throw InvalidValueError(std::string("TCP/IP configuration parsing error: invalid ") +
					optionName + " (" + rawValue + ") - " + optionName + " must be between " +
					toString(minValue) + " and " + toString(maxValue));
			}

			return value;
		}

	private:
		// All parsig functions have the same behaviour - they parses the
		// selected value and returns it. They throw InvalidValueError if the
		// value is invalid.
		static BufferSize parseBufferSize(const std::string &rawBufferSize);
		static LengthField parseLengthField(const std::string &rawLengthField);

	private:
		/// Buffer size (in bytes).
		BufferSize _bufferSize;
		/// Size of a field containing the length of a message (in bytes).
		LengthField _lengthField;

		/// Default buffer size (in bytes).
		static const BufferSize DEFAULT_BUFFER_SIZE;
		/// Default length field (in bytes).
		static const LengthField DEFAULT_LENGTH_FIELD;
};

#endif // #ifndef CONFIG_H

// End of file config.h
