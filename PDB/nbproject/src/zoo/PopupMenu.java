/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief popup menu for the map class.
 *
 */

package zoo;

import java.awt.Point;
import java.awt.event.ActionEvent;
import java.sql.SQLException;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

/**
 * @brief Creates popup menu and performs actions after the user select menu item
 */
public class PopupMenu {

	/// String for the insert submenu
	private static final String INSERT_SUBMENU = "Insert";

	/// String for the insert -> pavilion menu
	private static final String INSERT_PAVILION = "Pavilion";

	/// String for the insert -> exhibit menu
	private static final String INSERT_EXHIBIT = "Exhibit";

	/// String for the insert -> refreshment menu
	private static final String INSERT_REFRESHMENT = "Refreshment";

	/// String for the insert -> entrance menu
	private static final String INSERT_ENTRANCE = "Entrance";

	/// String for the insert -> path menu
	private static final String INSERT_PATH = "Path";

	/// Items in the string submenu
	private static final String[] INSERT_SUBMENU_ITEMS = {
		INSERT_PAVILION, INSERT_EXHIBIT, INSERT_REFRESHMENT,
		INSERT_ENTRANCE, INSERT_PATH};

	/// String for the show nearest submenu
	private static final String SHOW_NEAREST_SUBMENU = "Show";

	/// String for the insert -> exhibit menu
	private static final String SHOW_NEAREST_PAVILION = "Nearest pavilion";

	/// String for the insert -> exhibit menu
	private static final String SHOW_NEAREST_EXHIBIT = "Nearest exhibit";

	/// String for the insert -> refreshment menu
	private static final String SHOW_NEAREST_REFRESHMENT = "Nearest refreshment";

	/// String for the insert -> entrance menu
	private static final String SHOW_NEAREST_ENTRANCE = "Nearest entrance";

	/// Items in the string submenu
	private static final String[] SHOW_NEAREST_SUBMENU_ITEMS = {
		SHOW_NEAREST_PAVILION, SHOW_NEAREST_EXHIBIT, SHOW_NEAREST_REFRESHMENT,
		SHOW_NEAREST_ENTRANCE};

	/// String for the deletation of the object
	private static final String DELETE_OBJECT = "Delete";

	/// String for the animals management
	private static final String ANIMALS_MANAGEMENT = "Animals management";

	/// String for searching for number of genera in the pavilion
	private static final String PAVILION_GENERA = "Number of genera in the pavilion";

	/// String for searching for number of genera in the pavilion
	private static final String PAVILION_CLASSES = "Number of classes in the pavilion";

	/// Coordinates where the menu was created for the last time
	private static Point _lastClicked = null;

	public static void DisplayMenu(java.awt.event.MouseEvent evt, ZooView zv,
					Date date, ObjectData object) {
		_lastClicked = evt.getPoint();

		// Create the popup menu
		JPopupMenu popupMenu = new JPopupMenu();

		// Add items that will be present every time popup menu is shown
		JMenuItem menuItem;
		JMenu subMenu = new JMenu(SHOW_NEAREST_SUBMENU);
		popupMenu.add(subMenu);
		for (String label : SHOW_NEAREST_SUBMENU_ITEMS) {
			menuItem = new JMenuItem(label);
			menuItem.addActionListener(zv);
			subMenu.add(menuItem);
		}

		Point clicked = View.getInstance().map2db(evt.getPoint());

		// Inserting new objects
		subMenu = new JMenu(INSERT_SUBMENU);
		popupMenu.add(subMenu);
		for (String label : INSERT_SUBMENU_ITEMS) {
			menuItem = new JMenuItem(label);
			menuItem.addActionListener(zv);
			subMenu.add(menuItem);
		}

		ObjectData clickedObject = ObjectOperations.getObjectOnPosition(clicked, date);
		if (clickedObject != null) {
				// Clicked on the existing object
				if (clickedObject.tableName.equals(Objects.PAVILION_TABLE_NAME) ||
						clickedObject.tableName.equals(Objects.EXHIBIT_TABLE_NAME)) {
					// Clicked on an object where animals do live
					menuItem = new JMenuItem(ANIMALS_MANAGEMENT);
					menuItem.addActionListener(zv);
					popupMenu.add(menuItem);
				}

				if (clickedObject.tableName.equals(Objects.PAVILION_TABLE_NAME)) {
					// Clicked on a pavilion
					menuItem = new JMenuItem(PAVILION_GENERA);
					menuItem.addActionListener(zv);
					popupMenu.add(menuItem);

					menuItem = new JMenuItem(PAVILION_CLASSES);
					menuItem.addActionListener(zv);
					popupMenu.add(menuItem);
				}

			try {
				if (!Objects.canContainAnimal(object.tableName) ||
						!TemporalQuery.objectContainsAnimals(object.id, object.tableName,
						Calendar.getInstance().getDate())) {
					// Insert delete object only when it doesn't contain animal
					menuItem = new JMenuItem(DELETE_OBJECT);
					menuItem.addActionListener(zv);
					popupMenu.add(menuItem);
				}
			} catch (SQLException ex) {
				Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
						ex.getClass() + "): " + ex.getMessage());
				// No action because, just don't add delete to the menu
			}
		}

		popupMenu.show(evt.getComponent(), evt.getX(), evt.getY());
	}

	public static void PopupMenuClicked(ActionEvent arg0, ObjectData selectedObject,
			ZooView zv) {
		Date currentDate = Calendar.getInstance().getDate();
		if (arg0.getActionCommand().compareTo(INSERT_PAVILION) == 0) {
			zv.setMouseMode(MapPanel.MouseMode.INSERTING);
			Insert.getInstance().initInsert(ObjectData.POLYGON_TYPE,
					Objects.PAVILION_TABLE_NAME);
			StatusMessageLabel.getInstance().setText("Inserting pavilion: left click to add point number 1, right click to cancel");
		}
		else if (arg0.getActionCommand().compareTo(INSERT_EXHIBIT) == 0) {
			zv.setMouseMode(MapPanel.MouseMode.INSERTING);
			Insert.getInstance().initInsert(ObjectData.POLYGON_TYPE,
					Objects.EXHIBIT_TABLE_NAME);
			StatusMessageLabel.getInstance().setText("Inserting exhibit: left click to add point number 1, right click to cancel");
		}
		else if (arg0.getActionCommand().compareTo(INSERT_REFRESHMENT) == 0) {
			zv.setMouseMode(MapPanel.MouseMode.INSERTING);
			Insert.getInstance().initInsert(ObjectData.CIRCLE_TYPE,
					Objects.REFRESHMENT_TABLE_NAME);
			StatusMessageLabel.getInstance().setText("Inserting refreshment: left click to add centre of refreshment, right click to cancel");
		}
		else if (arg0.getActionCommand().compareTo(INSERT_ENTRANCE) == 0) {
			zv.setMouseMode(MapPanel.MouseMode.INSERTING);
			Insert.getInstance().initInsert(ObjectData.POINT_TYPE,
					Objects.ENTRANCE_TABLE_NAME);
			StatusMessageLabel.getInstance().setText("Inserting entrance: left click to add entrance, right click to cancel");
		}
		else if (arg0.getActionCommand().compareTo(INSERT_PATH) == 0) {
			zv.setMouseMode(MapPanel.MouseMode.INSERTING);
			Insert.getInstance().initInsert(ObjectData.LINE_TYPE,
					Objects.PATH_TABLE_NAME);
			StatusMessageLabel.getInstance().setText("Inserting path: left click to add begin of the path, right click to cancel");
		}
		else if (arg0.getActionCommand().compareTo(SHOW_NEAREST_PAVILION) == 0) {
			zv.setSelectedObject(ObjectOperations.getNearestObject(_lastClicked,
							Objects.PAVILION_TABLE_NAME, currentDate));
		}
		else if (arg0.getActionCommand().compareTo(SHOW_NEAREST_EXHIBIT) == 0) {
			zv.setSelectedObject(ObjectOperations.getNearestObject(_lastClicked,
							Objects.EXHIBIT_TABLE_NAME, currentDate));
		}
		else if (arg0.getActionCommand().compareTo(SHOW_NEAREST_REFRESHMENT) == 0) {
			zv.setSelectedObject(ObjectOperations.getNearestObject(_lastClicked,
							Objects.REFRESHMENT_TABLE_NAME, currentDate));
		}
		else if (arg0.getActionCommand().compareTo(SHOW_NEAREST_ENTRANCE) == 0) {
			zv.setSelectedObject(ObjectOperations.getNearestObject(_lastClicked,
							Objects.ENTRANCE_TABLE_NAME, currentDate));
		}
		else if (arg0.getActionCommand().compareTo(DELETE_OBJECT) == 0) {
			try {
				TemporalQuery.deleteQuery(
						selectedObject.id,
						selectedObject.tableName,
						currentDate);
				zv.setSelectedObject(null);
			} catch (SQLException ex) {
				Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
						ex.getClass() + "): " + ex.getMessage());
				Dialogs.displayErrorMessage(null,
						"There was a database error during the last operation.",
						"Database error");
			}
		}
		else if (arg0.getActionCommand().compareTo(ANIMALS_MANAGEMENT) == 0) {
			(new AnimalsManagement(selectedObject, zv)).setVisible(true);
		}
		else if (arg0.getActionCommand().compareTo(PAVILION_GENERA) == 0) {
			try {
				int count = TemporalQuery.getCountOfGeneraInPavilion(selectedObject.id,
						Calendar.getInstance().getDate());
				Dialogs.displayInformationMessage("Genera: " + count,
						"Count of genera in the " + selectedObject.name);
			} catch (SQLException ex) {
				Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
						ex.getClass() + "): " + ex.getMessage());
				Dialogs.displayErrorMessage(null,
						"There was a database error during the last operation.",
						"Database error");
			}
		}
		else if (arg0.getActionCommand().compareTo(PAVILION_CLASSES) == 0) {
			try {
				int count = TemporalQuery.getCountOfClassesInPavilion(selectedObject.id,
						Calendar.getInstance().getDate());
				Dialogs.displayInformationMessage("Classes: " + count,
						"Count of classes in the " + selectedObject.name);
			} catch (SQLException ex) {
				Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
						ex.getClass() + "): " + ex.getMessage());
				Dialogs.displayErrorMessage(null,
						"There was a database error during the last operation.",
						"Database error");
			}
		}
	}
}
