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

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Application configuration in a file.
 */
public class FileConfig implements IConfig {

	/** Option name for the host name. */
	private static final String HOST_NAME_CONFIG_NAME = "host-name";
	/** Option name for the port number. */
	private static final String PORT_CONFIG_NAME = "port";
	/** Option name for the user name. */
	private static final String USER_NAME_CONFIG_NAME = "user-name";
	/** Option name for the user password. */
	private static final String USER_PASSWORD_CONFIG_NAME = "user-password";
	/** Option name for the encryption usage. */
	private static final String USE_ENCRYPTION_CONFIG_NAME = "use-encryption";
	/** Option name for the multiple chats in one window. */
	private static final String MULTIPLE_CHATS_CONFIG_NAME = "multiple-chats";
	/** Option name for the maximal number of multiple chats in one window. */
	private static final String MAX_MULTIPLE_CHATS_CONFIG_NAME = "max-multiple-chats";
	/** Option name for the main appliction window width. */
	private static final String APP_WIDTH_CONFIG_NAME = "app-width";
	/** Option name for the main appliction window height. */
	private static final String APP_HEIGHT_CONFIG_NAME = "app-height";
	/** Option name for the position on the x-axis of the main appliction window. */
	private static final String APP_X_POS_CONFIG_NAME = "app-x-pos";
	/** Option name for the position on the y-axis of the main appliction window. */
	private static final String APP_Y_POS_CONFIG_NAME = "app-y-pos";
	
	/** Host name. */
	private String hostName = DEFAULT_HOST_NAME;
	/** Port number. */
	private int port = DEFAULT_PORT;
	/** User name. */
	private String userName = DEFAULT_USER_NAME;
	/** User password. */
	private String userPassword = DEFAULT_USER_PASSWORD;
	/** Use encryption or not. */
	private boolean useEncryption = DEFAULT_USE_ENCRYPTION;
	/** Multiple chats in one window. */
	private boolean multipleChats = DEFAULT_MULTIPLE_CHATS;
	/** Maximal number of multiple chats in one window. */
	private int	maxMultipleChats = DEFAULT_MAX_MULTIPLE_CHATS;
	/** Width of the main application window. */
	private int appWidth = DEFAULT_APP_WIDTH;
	/** Height of the main application window. */
	private int appHeight = DEFAULT_APP_HEIGHT;
	/** Position on the x-axis of the main application window. */
	private int appXPos = DEFAULT_APP_X_POS;
	/** Position on the y-axis of the main application window. */
	private int appYPos = DEFAULT_APP_Y_POS;
	
	/** Configuration file. */
	private File configFile = null;

	/**
	 * Constructs an empty FileConfig Object;
	 */
	private FileConfig() {
	}
	
	/**
	 * Construct a FileConfig object from a file path.
	 * 
	 * @param configFilePath Path to the configuration file.
	 * @return FileConfig object if the file was parsed correctly, null otherwise.
	 */
	public static FileConfig fromFile(File configFilePath) throws IOException {
		FileConfig fc = new FileConfig();
		fc.configFile = configFilePath;
		
		BufferedReader reader = new BufferedReader(new FileReader(fc.configFile));
		
		// Parse the file contents, line by line
		Pattern linePattern = Pattern.compile("^\\s*(\\S+)\\s*=\\s*(\\S+)\\s*$");
		String fileLine = reader.readLine();
		while (fileLine != null) {
			Matcher matched = linePattern.matcher(fileLine);
			if (matched.find()) {
				if (matched.group(1).equals(HOST_NAME_CONFIG_NAME)) {
					fc.hostName = matched.group(2);
				} else if (matched.group(1).equals(PORT_CONFIG_NAME)) {
					fc.port = Integer.valueOf(matched.group(2)).intValue();
					if (fc.port < MIN_PORT || fc.port > MAX_PORT) {
						// Invalid port number
						return null;
					}
				} else if (matched.group(1).equals(USER_NAME_CONFIG_NAME)) {
					fc.userName = matched.group(2);
				} else if (matched.group(1).equals(USER_PASSWORD_CONFIG_NAME)) {
					fc.userPassword = matched.group(2);
				} else if (matched.group(1).equals(USE_ENCRYPTION_CONFIG_NAME)) {
					if (matched.group(2).equals("true")) {
						fc.useEncryption = true;
					} else if (matched.group(2).equals("false")) {
						fc.useEncryption = false;
					} else {
						// Invalid value
						return null;
					}
				} else if (matched.group(1).equals(MULTIPLE_CHATS_CONFIG_NAME)) {
					if (matched.group(2).equals("true")) {
						fc.multipleChats = true;
					} else if (matched.group(2).equals("false")) {
						fc.multipleChats = false;
					} else {
						// Invalid value
						return null;
					}
				} else if (matched.group(1).equals(MAX_MULTIPLE_CHATS_CONFIG_NAME)) {
					fc.maxMultipleChats = Integer.valueOf(matched.group(2)).intValue();
					if (fc.maxMultipleChats < MIN_MAX_MULTIPLE_CHATS ||
							fc.maxMultipleChats > MAX_MAX_MULTIPLE_CHATS) {
						// Invalid number of maximal number of multiple chats
						return null;
					}
				} else if (matched.group(1).equals(APP_WIDTH_CONFIG_NAME)) {
					fc.appWidth = Integer.valueOf(matched.group(2)).intValue();
					if (fc.appWidth < 0) {
						// Invalid value
						return null;
					}
				} else if (matched.group(1).equals(APP_HEIGHT_CONFIG_NAME)) {
					fc.appHeight = Integer.valueOf(matched.group(2)).intValue();
					if (fc.appHeight < 0) {
						// Invalid value
						return null;
					}
				} else if (matched.group(1).equals(APP_X_POS_CONFIG_NAME)) {
					fc.appXPos = Integer.valueOf(matched.group(2)).intValue();
					if (fc.appXPos < 0) {
						// Invalid value
						return null;
					}
				} else if (matched.group(1).equals(APP_Y_POS_CONFIG_NAME)) {
					fc.appYPos = Integer.valueOf(matched.group(2)).intValue();
					if (fc.appYPos < 0) {
						// Invalid value
						return null;
					}
				} else {
					Logger.getLogger().info("Unknown option in the configuration file: " +
							matched.group(1));
				}
			}
			fileLine = reader.readLine();
		}
		
		return fc;
	}

	public String getHostName() {
		return hostName;
	}

	public void setHostName(String newHostName) {
		hostName = newHostName;
	}

	public int getPort() {
		return port;
	}

	public void setPort(int newPort) {
		port = newPort;
	}

	public String getUserName() {
		return userName;
	}

	public void setUserName(String newUserName) {
		userName = newUserName;
	}

	public String getUserPassword() {
		return userPassword;
	}

	public void setUserPassword(String newUserPassword) {
		userPassword = newUserPassword;
	}

	public boolean getUseEncryption() {
		return useEncryption;
	}

	public void setUseEncryption(boolean newUseEncryption) {
		useEncryption = newUseEncryption;
	}

	public boolean getMultipleChats() {
		return multipleChats;
	}

	public void setMultipleChats(boolean newMultipleChats) {
		multipleChats = newMultipleChats;
	}
	
	public int getMaxMultipleChats() {
		return maxMultipleChats;
	}
	
	public void setMaxMultipleChats(int newMaxMultipleChats) {
		maxMultipleChats = newMaxMultipleChats;
	}
	
	public int getAppWidth() {
		return appWidth;
	}

	public void setAppWidth(int newWidth) {
		appWidth = newWidth;
	}

	public int getAppHeight() {
		return appHeight;
	}

	public void setAppHeight(int newHeight) {
		appHeight = newHeight;
	}

	public int getAppXPos() {
		return appXPos;
	}

	public void setAppXPos(int newXPos) {
		appXPos = newXPos;
	}

	public int getAppYPos() {
		return appYPos;
	}

	public void setAppYPos(int newYPos) {
		appYPos = newYPos;
	}
	
	public boolean saveConfiguration() {
		FileWriter fw = null;
		try {
			// Create the future file content
			String fc = "";
			fc += HOST_NAME_CONFIG_NAME + "=" + hostName + "\n";
			fc += PORT_CONFIG_NAME + "=" + port + "\n";
			fc += USER_NAME_CONFIG_NAME + "=" + userName + "\n";
			fc += USER_PASSWORD_CONFIG_NAME + "=" + userPassword + "\n";
			fc += USE_ENCRYPTION_CONFIG_NAME + "=" + useEncryption + "\n";
			fc += MULTIPLE_CHATS_CONFIG_NAME + "=" + multipleChats + "\n";
			fc += MAX_MULTIPLE_CHATS_CONFIG_NAME + "=" + maxMultipleChats + "\n";
			fc += APP_WIDTH_CONFIG_NAME + "=" + appWidth + "\n";
			fc += APP_HEIGHT_CONFIG_NAME + "=" + appHeight + "\n";
			fc += APP_X_POS_CONFIG_NAME + "=" + appXPos + "\n";
			fc += APP_Y_POS_CONFIG_NAME + "=" + appYPos + "\n";
			
			// Write the content
			fw = new FileWriter(configFile);
			fw.write(fc, 0, fc.length());
			
			return true;
		} catch (IOException ex) {
			return false;
		} finally {
			try {
				fw.close();
			} catch (IOException ex) {
				// Do nothing
			}
		}
	}
}
