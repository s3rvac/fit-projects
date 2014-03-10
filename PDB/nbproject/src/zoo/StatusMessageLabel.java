/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief StatusMessageLabel singleton. Used to access StatusMessageLabel in
 *        ZooView.
 *
 */

package zoo;

/**
 * @brief This class is used to access StatusMessageLabel in ZooView.
 */
public class StatusMessageLabel {
	private final static StatusMessageLabel INSTANCE = new StatusMessageLabel();

	private javax.swing.JLabel message;

	// Private constructor suppresses generation of
	// a (public) default constructor
	private StatusMessageLabel() {
	}

	public static StatusMessageLabel getInstance() {
		return INSTANCE;
	}

	public void init(javax.swing.JLabel m) {
		message = m;
	}

	public void setText(String txt) {
		message.setText(txt);
	}
}
