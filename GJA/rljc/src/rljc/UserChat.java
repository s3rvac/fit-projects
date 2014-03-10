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

import org.jivesoftware.smack.Chat;
import org.jivesoftware.smack.ChatManager;
import org.jivesoftware.smack.MessageListener;
import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smack.packet.Message;

/**
 * Chat with a user.
 */
public class UserChat {
	
	/** Underlying chat. */
	Chat chat = null;
	/** User. */
	String user = null;
	/** Chat handler. */
	IUserChatHandler chatHandler = null;
			
	/**
	 * Constructs a new UserChar object and starts a chat with
	 * the selected user.
	 * 
	 * @param handler Chat handler.
	 * @param conn    Established connection to the Jabber server
	 * @param user    Username of the user to chat with.
	 */
	public UserChat(IUserChatHandler handler, XMPPConnection conn, String user) {
		chatHandler = handler;
		this.user = user;
		final UserChat userChat = this;
		
		ChatManager chatManager = conn.getChatManager();
		chat = chatManager.createChat(user, new MessageListener() {
			public void processMessage(Chat chat, Message message) {
				chatHandler.messageReceived(userChat, message.getBody());
			}
		});
		
		chatHandler.chatCreated(this);
	}
	 
	/**
	 * Returns the user in this chat.
	 */
	public String getUser() {
		return user;
	}
	
	/**
	 * "Receives" the selected message. This method can be used in a siuation
	 * where you want to explicitly send some message to the user of the
	 * application as if it was sent by the user with which the application
	 * user communicates.
	 * 
	 * @param message Message that was "received".
	 */
	public void receivedMessage(String message) {
		chatHandler.messageReceived(this, message);
	}
	
	/**
	 * Sends the selected message to the user in the chat.
	 * 
	 * @param message Message to be sent.
	 * @return True if the message was sent, false otherwise.
	 */
	public boolean sendMessage(String message) {
		try {
			chat.sendMessage(message);
			return true;
		} catch (Exception e) {
			// This handler should only catch XMPPException, but the
			// sendMessage() method throws java.lang.IllegalStateException
			// when not connected to the server, so I'm catching all
			// the exceptions
			chatHandler.chatError(this, e.getMessage());
			return false;
		}
	}
	 
	/**
	 * Closes the chat.
	 */
	public void closeChat() {
		chatHandler.chatClosed(this);
	}
}
