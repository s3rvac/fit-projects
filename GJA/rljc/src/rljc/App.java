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

import org.jdesktop.application.Application;
import org.jdesktop.application.SingleFrameApplication;

/**
 * The main class of the application.
 */
public class App extends SingleFrameApplication {

    /**
     * At startup create and show the main frame of the application.
     */
    @Override
	protected void startup() {
		View app = new View(this);
        show(app);
		// This has to be called here because it seems that Netbeans
		// changes the size of the window after the window is created
		app.resizeAndRepos();
    }

    /**
     * This method is to initialize the specified window by injecting resources.
     * Windows shown in our application come fully initialized from the GUI
     * builder, so this additional configuration is not needed.
     */
    @Override
	protected void configureWindow(java.awt.Window root) {
    }

    /**
     * A convenient static getter for the application instance.
	 * 
     * @return The instance of App.
     */
    public static App getApplication() {
        return Application.getInstance(App.class);
    }

    /**
     * Main method launching the application.
     */
    public static void main(String[] args) {
        launch(App.class, args);
    }
}
