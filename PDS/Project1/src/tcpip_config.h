/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    tcpip_config.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   TCPIPConfig class - declarations.
 *
 */

#ifndef TCPIP_CONFIG_H
#define TCPIP_CONFIG_H

#include <string>
#include <vector>
#include <tr1/cstdint>

#include "base_exception.h"
#include "config.h"

/**
 * @brief Configuration for a TCP/IP server.
 *
 * This class provides a transformation of raw config values into verified
 * and typed configuration values for a TCP/IP server and connections.
 *
 * It provides value semantics (object copying and comparison is allowed).
 */
class TCPIPConfig: public Config {
	public:
		/// List of network iterfaces.
		typedef std::vector<std::string> Interfaces;

		/// Port number type.
		typedef std::tr1::uint16_t Port;

		/// Timeout type.
		typedef time_t Timeout;

	public:
		/**
		 * @brief Creates a TCPIPConfig object from raw configuration values.
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
		static TCPIPConfig fromRawConfigValues(RawConfigValues rawConfigValues);

		/**
		 * @brief Destructor.
		 *
		 * Destroys a TCPIPConfig object.
		 */
		~TCPIPConfig();

		/**
		 * @brief Copy constructor.
		 *
		 * @param other Object that will be used as a model in the object creation.
		 *
		 * Constructs a copy of the @a other object.
		 */
		TCPIPConfig(const TCPIPConfig &other);

		/**
		 * @brief Assignment operator.
		 *
		 * @param other Object that will be assigned to this object.
		 *
		 * @return Reference to this object.
		 *
		 * Guarantees the strong exception safety.
		 */
		TCPIPConfig & operator=(const TCPIPConfig &other);

		/**
		 * @brief Equality comparison.
		 *
		 * @param other Object that will be compared with this object.
		 *
		 * @return True if this object is equal to the @a other object.
		 */
		bool operator==(const TCPIPConfig &other) const;

		/**
		 * @brief Non-equality comparison.
		 *
		 * @param other Object that will be compared with this object.
		 *
		 * @return True if this object is not equal to the @a other object.
		 */
		bool operator!=(const TCPIPConfig &other) const;

		/**
		 * @brief Swap the contents of this object with the other object.
		 *
		 * @param other Object that will be swapped with this object.
		 *
		 * Guarantees nothrow exception safety.
		 */
		void swap(TCPIPConfig &other);

		/**
		 * @brief Returns the list of interfaces on which the TCP/IP server should listen.
		 *
		 * If the resulting list contains only one interface named "ANY", then
		 * it means that the server should listen on all available interfaces.
		 * Iterface could be either a network iterface (like "eth0") or
		 * an IPv4 address (like 212.45.97.22).
		 */
		Interfaces getInterfaces() const { return _interfaces; }

		/// @brief Returns the port number on which the TCP/IP server should listen.
		Port getPort() const { return _port; }

		/// @brief Returns the connection timout (in seconds).
		time_t getTimeout() const { return _timeout; }

	public:
		/// This value that can be returned in an Interfaces container and
		/// it means that the server should listen on every interface.
		static const std::string ANY_INTERFACE;

	private:
		/**
		 * @brief Default constructor.
		 *
		 * Constructs an empty TCPIPConfig object.
		 */
		TCPIPConfig();

		/**
		 * @brief Constructor.
		 *
		 * @param rawConfigValues Raw configuration option -> values mappings.
		 *
		 * Constructs a TCPIPConfig object from the selected values.
		 */
		TCPIPConfig(Config::RawConfigValues rawConfigValues);

		// All parsig functions have the same behaviour - they parses the
		// selected value and returns it. They throw InvalidValueError if the
		// value is invalid.
		static Interfaces parseInterfaces(const std::string &rawInterfaces);
		static Port parsePort(const std::string &rawPort);
		static Timeout parseTimeout(const std::string &rawTimeout);

	private:
		/// Interfaces on which the TCP/IP server should listen.
		Interfaces _interfaces;
		/// Port number on which the TCP/IP server should listen.
		Port _port;
		/// Connection timeout (in seconds).
		Timeout _timeout;

		/// Default port number.
		static const Port DEFAULT_PORT;
		/// Default connection timeout (in seconds).
		static const Timeout DEFAULT_TIMEOUT;
};

#endif // #ifndef TCPIP_CONFIG_H

// End of file tcpip_config.h
