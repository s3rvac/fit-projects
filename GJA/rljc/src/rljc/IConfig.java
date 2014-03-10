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

/**
 * Application configuration interface.
 */
public interface IConfig {
	
	/** Default server host name. */
	public static final String DEFAULT_HOST_NAME = "jabber.org";
	/** Default port number. */
	public static final int DEFAULT_PORT = 5222;
	/** Default user name. */
	public static final String DEFAULT_USER_NAME = "rljctest";
	/** Default user password. */
	public static final String DEFAULT_USER_PASSWORD = "rljctest";
	/** Default use encryption value. */
	public static final boolean DEFAULT_USE_ENCRYPTION = true;
	/** Default multiple chats in one window. */
	public static final boolean DEFAULT_MULTIPLE_CHATS = false;
	/** Default maximal number of multiple chats in one window. */
	public static final int DEFAULT_MAX_MULTIPLE_CHATS = 4;
	/** Default main application window width */
	public static final int DEFAULT_APP_WIDTH = 0;
	/** Default main application window height */
	public static final int DEFAULT_APP_HEIGHT = 0;
	/** Default main application position (x-axis) */
	public static final int DEFAULT_APP_X_POS = 0;
	/** Default main application position (y-axis) */
	public static final int DEFAULT_APP_Y_POS = 0;
	/** Minimal port number. */
	public static final int MIN_PORT = 1;
	/** Maximal port number. */
	public static final int MAX_PORT = 65535;
	/** Minimal number of maximal number of multiple chats in one window. */
	public static final int MIN_MAX_MULTIPLE_CHATS = 1;
	/** Maximal number of maximal number of multiple chats in one window. */
	public static final int MAX_MAX_MULTIPLE_CHATS = 16;
	
	/**
	 * Returns the server host name.
	 */
	public String getHostName();
	
	/**
	 * Sets a new server host name. 
	 * 
	 * @param newHostName New host name of the server to be set.
	 */
	public void setHostName(String newHostName);
	
	/**
	 * Returns the server port number.
	 */
	public int getPort();

	/**
	 * Sets a new server port number.
	 * 
	 * @param newPort New server port number to be set.
	 */
	public void setPort(int newPort);
	
	/**
	 * Returns user name.
	 */
	public String getUserName();
	
	/**
	 * Sets a new user name.
	 * 
	 * @param newUserName New user name to be set.
	 */
	public void setUserName(String newUserName); 
	 
	/**
	 * Returns user password (unencrypted).
	 */
	public String getUserPassword();
	 
	/**
	 * Sets a new user password (unencrypted).
	 * 
	 * @param newUserPassword New user password to be set (unencrypted).
	 */
	public void setUserPassword(String newUserPassword); 
	 
	/**
	 * Returns true if encryption should be used, false otherwise.
	 */
	public boolean getUseEncryption();
	
	/**
	 * Enables or disables connection encryption.
	 * 
	 * @param newUseEncryption True if encryption should be used, false otherwise.
	 */
	public void setUseEncryption(boolean newUseEncryption);

	/**
	 * Returns true if multiple chats in one window should be used, false otherwise.
	 */
	public boolean getMultipleChats();
	
	/**
	 * Enables or disables multiple chats in one window.
	 * 
	 * @param newMultipleChats True if multiple chats in one window should be
	 *                         used, false otherwise.
	 */
	public void setMultipleChats(boolean newMultipleChats);
	
	/**
	 * Returns the maximal number of multiple chats in one window.
	 */
	public int getMaxMultipleChats();
	
	/**
	 * Sets a new maximal number of multiple chats in one window.
	 * 
	 * @param newMaxMultipleChats Maximal number of multiple chats in one window.
	 */
	public void setMaxMultipleChats(int newMaxMultipleChats);
	
	/**
	 * Returns width of the main application window.
	 */
	public int getAppWidth();
	
	/**
	 * Sets a new application main window width.
	 * 
	 * @param newWidth New width to be set.
	 */
	public void setAppWidth(int newWidth);
	
	/**
	 * Returns height of the main application window.
	 */
	public int getAppHeight();
	
	/**
	 * Sets a new application main window width.
	 * 
	 * @param newHeight New height to be set.
	 */
	public void setAppHeight(int newHeight);
	
	/**
	 * Returns position on the x-axis of the main application window.
	 */
	public int getAppXPos();
	
	/**
	 * Sets a new application main window position on the x-axis.
	 * 
	 * @param newXPos New position to be set.
	 */
	public void setAppXPos(int newXPos);
	
	/**
	 * Returns position on the y-axis of the main application window.
	 */
	public int getAppYPos();
	
	/**
	 * Sets a new application main window position on the y-axis.
	 * 
	 * @param newYPos New position to be set.
	 */
	public void setAppYPos(int newYPos);
	
	/**
	 * Saves the configuration (changes will become permanent).
	 * 
	 * @return True if the configuration was saved successfully, false otherwise.
	 */
	public boolean saveConfiguration();
}
