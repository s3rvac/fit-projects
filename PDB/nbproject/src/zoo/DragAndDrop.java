/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Provides the "drag and drop" of animals functionality
 *
 */

package zoo;

import java.awt.Graphics;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @brief Provides the "drag and drop" of animals functionality.
 */
public class DragAndDrop {
	private final static DragAndDrop INSTANCE = new DragAndDrop();

	/// Pressed object (pavilion/exhibit).
	private ObjectData _pressedObject = null;

	/// Dragged animal from pressed object.
	private AnimalData _pressedAnimal = null;

	/// Released object (pavilion/exhibit)
	private ObjectData _releasedObject = null;

	/// Drag and drop back buffer.
	private BufferedImage _dragAndDropBuffer = null;

	/// Dragged image size.
	private static final int DRAGGED_IMAGE_SIZE = 60;

	// Private constructor suppresses generation of
	// a (public) default constructor
	private DragAndDrop() {
	}

	public static DragAndDrop getInstance() {
		return INSTANCE;
	}

	/**
	 * @brief Save current buffer function. Used to real-time redraw.
	 *
	 * @param[in] bi Back buffer to save.
	 */
	public void saveBuffer(BufferedImage bi) {
		_dragAndDropBuffer = bi;
	}

	/**
	 * @brief Animal chooser function.
	 *
	 * @param[in] mapPosition Mouse position in a map.
	 */
	public void mousePressed(Point mapPosition) {
		_pressedObject = ObjectOperations.getObjectOnPosition(
				View.getInstance().map2db(mapPosition),
				Calendar.getInstance().getDate());
		if (_pressedObject != null && Objects.canContainAnimal(
				_pressedObject.tableName)) {
			Envelope envelope = new Envelope(_pressedObject);
			_pressedAnimal = envelope.getAnimal(mapPosition);
		}
	}

	/**
	 * @brief Animal dragging function.
	 *
	 * @param[in] mapPosition Mouse position in a map.
	 * @param[in] g Graphics where we draw photo of dragged animal.
	 */
	public void mouseDragged(Point mapPosition, Graphics g) {
		if (g != null && _dragAndDropBuffer != null) {
			g.drawImage(_dragAndDropBuffer, 0, 0, null);
			if (_pressedAnimal != null) {
				StatusMessageLabel.getInstance().setText("You are moving: " +
						_pressedAnimal.species);
				g.drawImage(_pressedAnimal.photo,
						mapPosition.x, mapPosition.y,
						mapPosition.x + DRAGGED_IMAGE_SIZE,
						mapPosition.y + DRAGGED_IMAGE_SIZE,
						0, 0,
						_pressedAnimal.photo.getWidth(),
						_pressedAnimal.photo.getHeight(), null);
			}
		}
	}

	/**
	 * @brief Animal dropper function.
	 *
	 * @param[in] mapPosition Mouse position in a map.
	 * @param[in] g Graphics where we draw photo of dragged animal.
	 */
	public void mouseReleased(Point mapPosition, Graphics g) {
		if (g != null && _dragAndDropBuffer != null) {
			g.drawImage(_dragAndDropBuffer, 0, 0, null);
			if (_pressedAnimal != null) {
				_releasedObject = ObjectOperations.getObjectOnPosition(
						View.getInstance().map2db(mapPosition),
						Calendar.getInstance().getDate());
				if (_releasedObject != null && Objects.canContainAnimal(
						_releasedObject.tableName)) {
					try {
						StatusMessageLabel.getInstance().setText(
								"You have just moved: " + _pressedAnimal.species +
								" from " + _pressedObject.name + " to " +
								_releasedObject.name);
						AnimalOperations.moveAnimalFromObjectToObject(
								_pressedAnimal,
								_pressedObject.tableName,
								_pressedObject.id,
								_releasedObject.tableName,
								_releasedObject.id);
					} catch (SQLException ex) {
						Logger.getLogger("zoo").log(Level.SEVERE,
								"Exception caught (" + ex.getClass() + "): " + ex.getMessage());
						Dialogs.displayErrorMessage(null,
								"There was a database error during the last operation.",
								"Database error");
					}
				}
			}
		}
		_pressedObject = null;
		_pressedAnimal = null;
		_releasedObject = null;
	}
}
