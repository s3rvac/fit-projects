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

import java.awt.Component;
import java.io.File;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

/**
 * Various dialogs and messages for interaction with the application user.
 *
 * Implements the "library" (aka "static helper") design pattern.
 */
public class Dialogs {

	/**
	 * Brings up a dialog that displays the selected message.
	 *
	 * @param parentComponent Determines the Frame in which the dialog is
	 *                        displayed; if null, or if the parentComponent has
	 *                        no Frame, a default Frame is used.
	 * @param message Message text to display.
	 * @param title Title for the dialog.
	 * @param messageType Type of message to be displayed:
	 *                    ERROR_MESSAGE, INFORMATION_MESSAGE, WARNING_MESSAGE,
	 *                    QUESTION_MESSAGE, or PLAIN_MESSAGE
	 */
	private static void displayMessage(Component parentComponent,
			String message, String title, int messageType) {
		JOptionPane.showMessageDialog(parentComponent, message, title, messageType);
	}

	/**
	 * Brings up a dialog that displays the selected information message.
	 *
	 * @param message Message text to display.
	 * @param title Title for the dialog.
	 */
	public static void displayInformationMessage(String message, String title) {
		displayInformationMessage(null, message, title);
	}

	/**
	 * Brings up a dialog that displays the selected information message.
	 *
	 * @param parentComponent Determines the Frame in which the dialog is
	 *                        displayed; if null, or if the parentComponent has
	 *                        no Frame, a default Frame is used.
	 * @param message Message text to display.
	 * @param title Title for the dialog.
	 */
	public static void displayInformationMessage(Component parentComponent,
			String message, String title) {
		displayMessage(parentComponent, message, title, JOptionPane.INFORMATION_MESSAGE);
	}

	/**
	 * Brings up a dialog that displays the selected warning message.
	 *
	 * @param message Message text to display.
	 * @param title Title for the dialog.
	 */
	public static void displayWarningMessage(String message, String title) {
		displayWarningMessage(null, message, title);
	}

	/**
	 * Brings up a dialog that displays the selected warning message.
	 *
	 * @param parentComponent Determines the Frame in which the dialog is
	 *                        displayed; if null, or if the parentComponent has
	 *                        no Frame, a default Frame is used.
	 * @param message Message text to display.
	 * @param title Title for the dialog.
	 */
	public static void displayWarningMessage(Component parentComponent,
			String message, String title) {
		displayMessage(parentComponent, message, title, JOptionPane.WARNING_MESSAGE);
	}

	/**
	 * Brings up a dialog that displays the selected error message.
	 *
	 * @param message Message text to display.
	 * @param title Title for the dialog.
	 */
	public static void displayErrorMessage(String message, String title) {
		displayErrorMessage(null, message, title);
	}

	/**
	 * Brings up a dialog that displays the selected error message.
	 *
	 * @param parentComponent Determines the Frame in which the dialog is
	 *                        displayed; if null, or if the parentComponent has
	 *                        no Frame, a default Frame is used.
	 * @param message Message text to display.
	 * @param title Title for the dialog.
	 */
	public static void displayErrorMessage(Component parentComponent,
			String message, String title) {
		displayMessage(parentComponent, message, title, JOptionPane.ERROR_MESSAGE);
	}

	/**
	 * Gets a string input from the user by displaying a simple dialog.
	 *
	 * @param message Message text to display.
	 * @param title Title for the dialog.
	 * @param initialInputValue Initial value of the input field.
	 * @return Input string set by the user (or null if the "Cancel" button was
	 *         pressed).
	 */
	public static String getInputStringFromUser(String message, String title,
			String initialInputValue) {
		return getInputStringFromUser(null, message, title, initialInputValue);
	}

	/**
	 * Gets a string input from the user by displaying a simple dialog.
	 *
	 * @param parentComponent Determines the Frame in which the dialog is
	 *                        displayed; if null, or if the parentComponent has
	 *                        no Frame, a default Frame is used.
	 * @param message Message text to display.
	 * @param title Title for the dialog.
	 * @param initialInputValue Initial value of the input field.
	 * @return Input string set by the user (or null if the "Cancel" button was
	 *         pressed).
	 */
	public static String getInputStringFromUser(Component parentComponent,
			String message, String title, String initialInputValue) {
		return (String) JOptionPane.showInputDialog(
				parentComponent, message, title, JOptionPane.INFORMATION_MESSAGE,
				null, null, initialInputValue);
	}

	/**
	 * Gets a file from the user by displaying a file dialog.
	 *
	 * @return Selected file by user (if some), otherwise null.
	 */
	public static File getFileFromUser() {
		return getFileFromUser(null);
	}

	/**
	 * Gets a file from the user by displaying a file dialog.
	 *
	 * @param parentComponent Parent component of the dialog, can be null.
	 * @return Selected file by user (if some), otherwise null.
	 */
	public static File getFileFromUser(Component parentComponent) {
		JFileChooser fc = new JFileChooser();
		fc.showOpenDialog(parentComponent);

		return fc.getSelectedFile();
	}

	/**
	 * Deny creation of instances.
	 */
	private Dialogs() {
	}
}
