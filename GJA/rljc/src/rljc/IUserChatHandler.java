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
 * Interface for a user chat handling class.
 */
public interface IUserChatHandler {

	/**
	 * Called when the chat is created.
	 * 
	 * @param chat Chat that was created.
	 */
	public void chatCreated(UserChat chat);

	/**
	 * Called when there is a chat error.
	 * 
	 * @param chat       Chat that caused the error.
	 * @param errMessage Error mesage.
	 */
	public void chatError(UserChat chat, String errMessage);

	/**
	 * Called when a message was received.
	 * 
	 * @param chat    Chat in which the message was received.
	 * @param message Received message.
	 */
	public void messageReceived(UserChat chat, String message);
	
	/**
	 * Called when the chat is closed.
	 * 
	 * @param chat Chat that was closed.
	 */
	public void chatClosed(UserChat chat);
}
