/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief This class is used to insert objects.
 */

package zoo;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.sql.SQLException;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @brief This class is used to insert objects.
 */
public class Insert {
	private final static Insert INSTANCE = new Insert();

	/// Implicit radius (to show center of circle)
	private static final int ZERO_CIRCLE_SIZE = 2;

	/// Inserted object
	private ObjectData _newObject = null;

	/// Table, where object is inserted
	private String _tableName = null;

	/// Incremental coordinates ...
	private Vector<Integer> _coordinatesTemp;

	/// Insert buffer.
	private BufferedImage _insertBuffer = null;

	/// Background color.
	private static final Color BACKGROUND_COLOR = new Color(238, 238, 238);

	// Private constructor suppresses generation of
	// a (public) default constructor
	private Insert() {
	}

	public static Insert getInstance() {
		return INSTANCE;
	}

	/**
	 * @brief Save current buffer function. Used to real-time redraw.
	 *
	 * @param[in] bi Back buffer to save.
	 */
	public void saveBuffer(BufferedImage bi) {
		_insertBuffer = bi;
	}

	/**
	 * @brief Initialize insert object.
	 *
	 * @param[in] objectType What object (ie. polygon/point/...).
	 * @param[in] tableName Into what table (ie. pavilion/exhibit/...)
	 */
	public void initInsert (int objectType, String tableName) {
		_tableName = tableName;
		_newObject = new ObjectData();
		_newObject.objectType = objectType;
		_coordinatesTemp = new Vector();
		_coordinatesTemp.clear();
	}

	/**
	 * @brief Transform from vector to object coordinates.
	 */
	void coordinates2object() {
		if (_newObject.objectType == ObjectData.CIRCLE_TYPE) {
			Point centre = new Point(_coordinatesTemp.get(0), _coordinatesTemp.get(1));
			if (_coordinatesTemp.size() == 2) {
				_newObject.coordinates = new int[6];
				_newObject.coordinates[0] = centre.x;
				_newObject.coordinates[1] = centre.y + ZERO_CIRCLE_SIZE;
				_newObject.coordinates[2] = centre.x + ZERO_CIRCLE_SIZE;
				_newObject.coordinates[3] = centre.y;
				_newObject.coordinates[4] = centre.x;
				_newObject.coordinates[5] = centre.y - ZERO_CIRCLE_SIZE;
			}
			else {
				Point circle = new Point(_coordinatesTemp.get(2), _coordinatesTemp.get(3));
				int r = (int) centre.distance(circle);
				_newObject.coordinates = new int[6];
				_newObject.coordinates[0] = centre.x;
				_newObject.coordinates[1] = centre.y + r;
				_newObject.coordinates[2] = centre.x + r;
				_newObject.coordinates[3] = centre.y;
				_newObject.coordinates[4] = centre.x;
				_newObject.coordinates[5] = centre.y - r;
			}
		}
		else {
			_newObject.coordinates = new int[_coordinatesTemp.size()];
			for (int i = 0; i < _coordinatesTemp.size(); ++i) {
				_newObject.coordinates[i] = (int) _coordinatesTemp.get(i);
			}
		}
	}

	/**
	 * @brief Animal dragging function.
	 *
	 * @param[in] g Graphics where we draw.
	 * @param[in] map Used to call drawObject.
	 */
	private void doDraw(Graphics g, MapPanel map) {
		g.drawImage(_insertBuffer, 0, 0, null);
		_newObject.color = (Color) Objects._mapObjectsProperties.get(_tableName);
		coordinates2object();
		map.drawObject(_newObject);
	}

	/**
	 * @brief Adds next point. If enought, stop and save entity.
	 *
	 * @param[in] mapPoint Next point.
	 * @param[in] g Graphics where we draw.
	 * @param[in] map Used to pass doDraw (and call drawObject).
	 */
	public void addPoint(Point mapPoint, Graphics g, MapPanel map) {
		Point dbPoint = View.getInstance().map2db(mapPoint);
		_coordinatesTemp.add(dbPoint.x);
		_coordinatesTemp.add(dbPoint.y);
		int numOfPoints = _coordinatesTemp.size()/2;
		try {
		switch (_newObject.objectType) {
			case ObjectData.POINT_TYPE:
				if (numOfPoints == 1 && _tableName.equals(Objects.ENTRANCE_TABLE_NAME)) {
					map.setMouseMode(MapPanel.MouseMode.DEFAULT);
					StatusMessageLabel.getInstance().setText("");
					if (TemporalQuery.insertEntrance(_coordinatesTemp.get(0), _coordinatesTemp.get(1), Calendar.getInstance().getDate()) == TemporalQuery.NO_OBJECT_ROW_ID) {
						Dialogs.displayErrorMessage(null,
						"You cannot insert the entrance on this position.",
						"Object insertion error");
						g.drawImage(_insertBuffer, 0, 0, null);
						return;
					}
				}
				break;

			case ObjectData.POLYGON_TYPE:
					if (numOfPoints < 2) {
						StatusMessageLabel.getInstance().setText("Inserting pavilion: left click to add point number " + (numOfPoints+1) + ", right click to cancel");
					} else {
						StatusMessageLabel.getInstance().setText("Inserting pavilion: left click to add point number " + (numOfPoints+1) + ", right click to confirm");
					}

				break;

			case ObjectData.LINE_TYPE:
				if (numOfPoints == 1 && _tableName.equals(Objects.PATH_TABLE_NAME)) {
					StatusMessageLabel.getInstance().setText("Inserting path: left click to add end of the path, right click to cancel");
				} else if (numOfPoints == 2 && _tableName.equals(Objects.PATH_TABLE_NAME)) {
					map.setMouseMode(MapPanel.MouseMode.DEFAULT);
					StatusMessageLabel.getInstance().setText("");
					if (TemporalQuery.insertPath(_coordinatesTemp.get(0), _coordinatesTemp.get(1), _coordinatesTemp.get(2), _coordinatesTemp.get(3), Calendar.getInstance().getDate()) == TemporalQuery.NO_OBJECT_ROW_ID) {
						Dialogs.displayErrorMessage(null,
						"You cannot insert the path on this position.",
						"Object insertion error");
						g.drawImage(_insertBuffer, 0, 0, null);
						return;
					}
				}
				break;

			case ObjectData.RECTANGLE_TYPE:
				break;

			case ObjectData.CIRCLE_TYPE:
				if (numOfPoints == 1 && _tableName.equals(Objects.REFRESHMENT_TABLE_NAME)) {
					StatusMessageLabel.getInstance().setText("Inserting refreshment: left click to add bound of refreshment, right click to cancel");
				} else if (numOfPoints == 2 && _tableName.equals(Objects.REFRESHMENT_TABLE_NAME)) {
					map.setMouseMode(MapPanel.MouseMode.DEFAULT);
					StatusMessageLabel.getInstance().setText("");
					coordinates2object();
					if (TemporalQuery.insertRefreshment("New Refreshment", _newObject.coordinates[0], _newObject.coordinates[1], _newObject.coordinates[2], _newObject.coordinates[3], _newObject.coordinates[4], _newObject.coordinates[5], Calendar.getInstance().getDate()) == TemporalQuery.NO_OBJECT_ROW_ID) {
						Dialogs.displayErrorMessage(null,
						"You cannot insert the refreshment on this position.",
						"Object insertion error");
						g.drawImage(_insertBuffer, 0, 0, null);
						return;
					}
				}
				break;

			default:
				Logger.getLogger("zoo").log(Level.WARNING,
						"Unknown object type in switch.");
				break;
		}
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			Dialogs.displayErrorMessage(null,
					"There was a database error during the last operation.",
					"Database error");
		}

		doDraw(g, map);
	}

	/**
	 * @brief Real time drawing. Add point, draw, drop point.
	 *
	 * @param[in] mapPoint Next point.
	 * @param[in] g Graphics where we draw.
	 * @param[in] map Used to pass doDraw (and call drawObject).
	 */
	public void movePoint(Point mapPoint, Graphics g, MapPanel map) {
		Point dbPoint = View.getInstance().map2db(mapPoint);
		_coordinatesTemp.add(dbPoint.x);
		_coordinatesTemp.add(dbPoint.y);
		doDraw(g, map);
		_coordinatesTemp.remove(_coordinatesTemp.size()-1);
		_coordinatesTemp.remove(_coordinatesTemp.size()-1);
	}

	/**
	 * @brief Adds last point and save entity. Only for polygons.
	 *
	 * @param[in] lastPoint Last point.
	 * @param[in] g Graphics where we draw.
	 * @param[in] map Used to pass doDraw (and call drawObject).
	 */
	public void lastPoint(Point lastPoint, Graphics g, MapPanel map) {
		if (_newObject.objectType == ObjectData.POLYGON_TYPE && _coordinatesTemp.size() >= 4) {
			addPoint(lastPoint, g, map);
			try {
				if (_tableName.equals(Objects.PAVILION_TABLE_NAME)) {
					map.setMouseMode(MapPanel.MouseMode.DEFAULT);
					StatusMessageLabel.getInstance().setText("");
					if (TemporalQuery.insertPavilion("New Pavilion", _newObject.coordinates, Calendar.getInstance().getDate()) == TemporalQuery.NO_OBJECT_ROW_ID) {
						Dialogs.displayErrorMessage(null,
						"You cannot insert the pavilion on this position.",
						"Object insertion error");
						_newObject.color = BACKGROUND_COLOR;
						map.drawObject(_newObject);
						g.drawImage(_insertBuffer, 0, 0, null);
						return;
					}
				}
				if (_tableName.equals(Objects.EXHIBIT_TABLE_NAME)) {
					map.setMouseMode(MapPanel.MouseMode.DEFAULT);
					StatusMessageLabel.getInstance().setText("");
					if (TemporalQuery.insertExhibit(_newObject.coordinates, Calendar.getInstance().getDate()) == TemporalQuery.NO_OBJECT_ROW_ID) {
						Dialogs.displayErrorMessage(null,
						"You cannot insert the exhibit on this position.",
						"Object insertion error");
						_newObject.color = BACKGROUND_COLOR;
						map.drawObject(_newObject);
						g.drawImage(_insertBuffer, 0, 0, null);
						return;
					}
				}
			} catch (SQLException ex) {
				Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
						ex.getClass() + "): " + ex.getMessage());
				Dialogs.displayErrorMessage(null,
						"There was a database error during the last operation.",
						"Database error");
			}
		}
		else {
			//cancel drawing
			g.drawImage(_insertBuffer, 0, 0, null);
		}
		map.setMouseMode(MapPanel.MouseMode.DEFAULT);
		StatusMessageLabel.getInstance().setText("");
	}
}
