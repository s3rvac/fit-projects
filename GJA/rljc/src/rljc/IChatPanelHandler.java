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
 * Interface that should implement class that wants to be informed
 * of some chat panel actions.
 */
public interface IChatPanelHandler {

	/**
	 * Called when a chat panel received a message.
	 * 
	 * @param chatPanel Chat panel that received a message.
	 */
	 public void chatPanelMessageReceived(ChatPanel chatPanel);
	
	/**
	 * Called when a chat panel is closed.
	 * 
	 * @param chatPanel Chat panel that was closed.
	 */
	 public void chatPanelClosed(ChatPanel chatPanel);
}
