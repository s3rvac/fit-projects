/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Keeps one static instance of the Logger class for the whole application.
 *
 */

package zoo;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @brief Keeps one static instance of the Logger class for the whole application.
 *
 * Use Logger.getLogger("zoo") to get the underlying logger (only one instance
 * of a logger called "zoo" is created).
 */
public class ZooLogger {

	// Standard Java logger.
	private static Logger _logger = Logger.getLogger("zoo");

	static {
		ZooLogger.enableLogging();
	}

	/**
	 * @brief Returns true if logging is currently enabled, false otherwise.
	 */
	public static boolean isLoggingEnabled() {
		return _logger.getLevel().equals(Level.ALL);
	}

	/**
	 * @brief Enables logging.
	 */
	public static void enableLogging() {
		_logger.setLevel(Level.ALL);
	}

	/**
	 * @brief Disables logging.
	 */
	public static void disableLogging() {
		_logger.setLevel(Level.OFF);
	}
}
