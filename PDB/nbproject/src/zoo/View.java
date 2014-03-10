/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief View class. Holds user view in map and transforms coordinates.
 *
 */

package zoo;

import java.awt.Point;

/**
 * @brief Holds user view in map and transforms coordinates.
 */
public class View {
	private final static View INSTANCE = new View();

	/// Left top corner position of the view
	private Point _viewLeftTopCorner = new Point(0,0);

	/// The farest point in db (x = width; y = height;)
	private Point _dbRightBottomCorner = new Point(0,0);

	/// Map zoom (only 1,2,4 are allowed)
	private double _zoom = 1.0;
	private static final double ZOOM_MIN = 1.0;
	private static final double ZOOM_MAX = 4.0;
	private static final double ZOOM_STEP = 0.2;

	/// Step.
	private static final int STEP = 20;

	/// Map border in zoomFit
	private static int BORDER = 4;

	public enum Direction {
		UP, RIGHT, DOWN, LEFT
	}

	private Direction _mouseDirection = Direction.UP;

	// Private constructor suppresses generation of
	// a (public) default constructor
	private View() {
	}

	public static View getInstance() {
		return INSTANCE;
	}

	/**
	* @brief  A method that fits zoo into map.
	*
	* @param[in] mapWidth Map width.
	* @param[in] mapHeight Map height.
	*/
	public void zoomFit(int mapWidth, int mapHeight) {
		if (!_dbRightBottomCorner.equals(new Point(0,0))) {
			double ratioX = (double)mapWidth/((double)_dbRightBottomCorner.x + BORDER);
			double ratioY = (double)mapHeight/((double)_dbRightBottomCorner.y + BORDER);
			double ratio = 1.0;

			if (ratioX < ratioY) {
				ratio = ratioX;
			}
			else {
				ratio = ratioY;
			}

			if (ratio < ZOOM_MIN) {
				ratio = ZOOM_MIN;
			}

			if (ratio > ZOOM_MAX) {
				ratio = ZOOM_MAX;
			}

			_viewLeftTopCorner.setLocation(0, 0);
			_zoom = ratio;
		}
	}

	/**
	* @brief  A method that transforms zoom ration (float) to slider ration (int).
	*/
	public int zoom2slider() {
		return (int)((_zoom - ZOOM_MIN) / (ZOOM_MAX - ZOOM_MIN) * 100.0);
	}

	/**
	* @brief  A method that transform slider ration (int) to zoom ration (float).
	*
	* @param[in] ratio Slider ratio.
	*/
	public void slider2zoom(int ratio) {
		_zoom = ZOOM_MIN + (ratio/100.0) * (ZOOM_MAX - ZOOM_MIN);
	}

	/**
	* @brief  Methods used to move map.
	*/
	public void moveRight() {
		_viewLeftTopCorner.translate(STEP, 0);
	}

	public void moveLeft() {
		if (_viewLeftTopCorner.getX() > 0) {
			_viewLeftTopCorner.translate(-STEP, 0);
	    	}
		else {
			_viewLeftTopCorner.x = 0;
		}
	}

	public void moveDown() {
		_viewLeftTopCorner.translate(0, STEP);
	}

	public void moveUp() {
		if (_viewLeftTopCorner.getY() > 0) {
			_viewLeftTopCorner.translate(0, -STEP);
		}
		else {
			_viewLeftTopCorner.y = 0;
		}
	}

	/**
	* @brief  A method that sets mouse direction (used in moveMouse).
	*
	* @param[in] md Mouse direction.
	*/
	public void setMouseDirection(Direction md) {
		_mouseDirection = md;
	}

	/**
	* @brief  A method that moves map by mouse click.
	*/
	public void moveMouse() {
		switch(_mouseDirection) {
			case UP:
				moveUp();
				break;
			case RIGHT:
				moveRight();
				break;
			case DOWN:
				moveDown();
				break;
			case LEFT:
				moveLeft();
				break;
		}
	}

	/**
	 * @brief  A method that transforms array of coordinates.
	 *
	 * @param[in] objectCoords Array of coordinates in db.
	 * @return Transformed coordinates.
	 */
	public int[] db2map(int[] objectCoords) {
		int[] transfCoords = new int[objectCoords.length];
		for (int i = 0; i < (objectCoords.length - 1); i = i + 2) {
			Point p = new Point(objectCoords[i], objectCoords[i+1]);

			if (p.x > _dbRightBottomCorner.x) {
				_dbRightBottomCorner.x = p.x;
			}

			if (p.y > _dbRightBottomCorner.y) {
				_dbRightBottomCorner.y = p.y;
			}

			p = db2map(p);
			transfCoords[i] = p.x;
			transfCoords[i + 1] = p.y;
		}

		return transfCoords;
	}

	/**
	* @brief  A method that transforms point from db to map.
	*
	* @param[in] p Point in db.
	* @return Point in map.
	*/
	public Point db2map(Point p) {
		Point mapPoint = (Point) p.clone();
		mapPoint.x -= _viewLeftTopCorner.x;
		mapPoint.x *= _zoom;
		mapPoint.y -= _viewLeftTopCorner.y;
		mapPoint.y *= _zoom;
		return mapPoint;
	}

	/**
	* @brief  A method that transforms point from map to db.
	*
	* @param[in] p Point in map.
	* @return Point in db.
	*/
	public Point map2db(Point p) {
		Point dbPoint = (Point) p.clone();
		dbPoint.x /= _zoom;
		dbPoint.x += _viewLeftTopCorner.x;
		dbPoint.y /= _zoom;
		dbPoint.y += _viewLeftTopCorner.y;
		return dbPoint;
	}
}

