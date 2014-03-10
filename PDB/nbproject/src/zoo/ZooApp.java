/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Main application class.
 *
 */

package zoo;

import org.jdesktop.application.Application;
import org.jdesktop.application.SingleFrameApplication;

/**
 * @brief The main class of the application.
 */
public class ZooApp extends SingleFrameApplication {

	/**
	 * @brief At startup create and show the main frame of the application.
	 */
	@Override
	protected void startup() {
		show(new ZooView(this));
	}

	/**
	 * @brief Initializes the specified window by injecting resources.
	 *
	 * Windows shown in our application come fully initialized from the GUI
	 * builder, so this additional configuration is not needed.
	 */
	@Override
	protected void configureWindow(java.awt.Window root) {
	}

	/**
	 * @brief A convenient static getter for the application instance.
	 *
	 * @return the instance of ZooApp
	 */
	public static ZooApp getApplication() {
		return Application.getInstance(ZooApp.class);
	}

	/**
	 * @brief Main method launching the application.
	 */
	public static void main(String[] args) {
		launch(ZooApp.class, args);
	}
}
