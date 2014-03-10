/*
 * Project: RLJC - Really Lightweight Jabber Client
 * Author:  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * 
 * Copyright (C) 2009 Petr Zemek
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

package rljc;

import java.util.logging.Level;

/**
 * Keeps one static instance of the Logger class for the whole application.
 */
public class Logger {

	/** Standard Java logger. */
	private static java.util.logging.Logger logger =
			java.util.logging.Logger.getLogger("rljc");

	static {
		Logger.enableLogging();
	}

	/**
	 * Returns the logger.
	 */
	public static java.util.logging.Logger getLogger() {
		return logger;
	}
	
	/**
	 * Returns true if logging is currently enabled, false otherwise.
	 */
	public static boolean isLoggingEnabled() {
		return logger.getLevel().equals(Level.ALL);
	}

	/**
	 * Enables logging.
	 */
	public static void enableLogging() {
		logger.setLevel(Level.ALL);
	}

	/**
	 * Disables logging.
	 */
	public static void disableLogging() {
		logger.setLevel(Level.OFF);
	}
	
	/**
	 * Disable construction of this object.
	 */
	private Logger() {
	}
}
