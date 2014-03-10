/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Calendar that keeps information about the currently set date.
 *
 */

package zoo;

import java.util.Date;
import java.util.Observable;

/**
 * @brief Calendar that keeps information about the currently set date.
 *
 * Implements "singleton" design pattern.
 */
public class Calendar extends Observable {

	/// Singleton instance.
	private final static Calendar INSTANCE = new Calendar();

	/// Currently set date.
	private Date _currentDate = new Date();

	/**
	 * @brief Returns the only singleton instance of this class.
	 *
	 * This method is the only way how to get an instance of this class.
	 */
	public static Calendar getInstance() {
		return INSTANCE;
	}

	/**
	 * @brief Returns the currently set date.
	 */
	public Date getDate() {
		return _currentDate;
	}

	/**
	 * @brief Sets a new date.
	 *
	 * @param date Date to be set.
	 */
	public void setDate(Date date) {
		_currentDate = (Date) date.clone();
		setChanged();
		notifyObservers(date);
	}

	/**
	 * @brief Default constructor.
	 */
	private Calendar() {
	}
}
