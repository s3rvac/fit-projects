/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    serial_config.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   SerialConfig class - declarations.
 *
 */

#ifndef SERIAL_CONFIG_H
#define SERIAL_CONFIG_H

#include <string>
#include <unistd.h>
#include <termios.h>

#include "base_exception.h"
#include "config.h"

/**
 * @brief Configuration for a TCP/IP server.
 *
 * This class provides a transformation of raw config values into verified
 * and typed configuration values for a serial connection.
 *
 * It provides value semantics (object copying and comparison is allowed).
 */
class SerialConfig: public Config {
	public:
		/// Raw configuration option -> values mapping.
		typedef std::map<std::string, std::string> RawConfigValues;

		/// Parity bit generation types.
		enum Parity {
			NONE_PARITY,
			EVEN_PARITY,
			ODD_PARITY,
			MARK_PARITY,
			SPACE_PARITY
		};

		/// Number of stop bits
		enum StopBits {
			STOP_BITS_1,
			STOP_BITS_1_5,
			STOP_BITS_2
		};

		/// Flow control types.
		enum FlowControl {
			NONE_FLOW_CONTROL,
			HARDWARE_FLOW_CONTROL,
			SOFTWARE_FLOW_CONTROL
		};

	public:
		/**
		 * @brief Creates a SerialConfig object from raw configuration values.
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
		static SerialConfig fromRawConfigValues(RawConfigValues rawConfigValues);

		/**
		 * @brief Destructor.
		 *
		 * Destroys a SerialConfig object.
		 */
		~SerialConfig();

		/**
		 * @brief Copy constructor.
		 *
		 * @param other Object that will be used as a model in the object creation.
		 *
		 * Constructs a copy of the @a other object.
		 */
		SerialConfig(const SerialConfig &other);

		/**
		 * @brief Assignment operator.
		 *
		 * @param other Object that will be assigned to this object.
		 *
		 * @return Reference to this object.
		 *
		 * Guarantees the strong exception safety.
		 */
		SerialConfig & operator=(const SerialConfig &other);

		/**
		 * @brief Equality comparison.
		 *
		 * @param other Object that will be compared with this object.
		 *
		 * @return True if this object is equal to the @a other object.
		 */
		bool operator==(const SerialConfig &other) const;

		/**
		 * @brief Non-equality comparison.
		 *
		 * @param other Object that will be compared with this object.
		 *
		 * @return True if this object is not equal to the @a other object.
		 */
		bool operator!=(const SerialConfig &other) const;

		/**
		 * @brief Swap the contents of this object with the other object.
		 *
		 * @param other Object that will be swapped with this object.
		 *
		 * Guarantees nothrow exception safety.
		 */
		void swap(SerialConfig &other);

		/// @brief Returns the serial port device.
		std::string getDevice() const { return _device; }

		/// @brief Returns the serial port speed.
		speed_t getSpeed() const { return _speed; }

		/// @brief Returns the number of data bits.
		tcflag_t getDataBits() const { return _dataBits; }

		/// @brief Returns the parity bit generation type.
		Parity getParity() const { return _parity; }

		/// @brief Returns the number of stop bits.
		StopBits getStopBits() const { return _stopBits; }

		/// @brief Returns the flow control type.
		FlowControl getFlowControl() const { return _flowControl; }

	private:
		/**
		 * @brief Default constructor.
		 *
		 * Constructs an empty SerialConfig object.
		 */
		SerialConfig();

		/**
		 * @brief Constructor.
		 *
		 * @param rawConfigValues Raw configuration option -> values mappings.
		 *
		 * Constructs a TCPIPConfig object from the selected values.
		 */
		SerialConfig(Config::RawConfigValues rawConfigValues);

		// All parsig functions have the same behaviour - they parses the
		// selected value and returns it. They throw InvalidValueError if the
		// value is invalid.
		static std::string parseDevice(const std::string &rawDevice);
		static speed_t parseSpeed(const std::string &rawSpeed);
		static tcflag_t parseDataBits(const std::string &rawDataBits);
		static Parity parseParity(const std::string &rawParity);
		static StopBits parseStopBits(const std::string &rawStopBits);
		static FlowControl parseFlowControl(const std::string &rawFlowControl);

	private:
		/// Serial port device.
		std::string _device;
		/// Serial port speed (in baudes).
		speed_t _speed;
		/// Number of data bits.
		tcflag_t _dataBits;
		/// Parity bit generation type.
		Parity _parity;
		/// Number of stop bits.
		StopBits _stopBits;
		/// Flow control type.
		FlowControl _flowControl;

		/// Default interface.
		static const std::string DEFAULT_DEVICE;
		/// Default speed.
		static const speed_t DEFAULT_SPEED;
		/// Default number of data bits.
		static const tcflag_t DEFAULT_DATA_BITS;
		/// Default parity bit generation type.
		static const Parity DEFAULT_PARITY;
		/// Default number of stop bits.
		static const StopBits DEFAULT_STOP_BITS;
		/// Default flow control.
		static const FlowControl DEFAULT_FLOW_CONTROL;
};

#endif // #ifndef SERIAL_CONFIG_H

// End of file serial_config.h
