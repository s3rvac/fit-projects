/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Envelope class. Determines where to draw photos inside object.
 *
 */

package zoo;

import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.awt.image.BufferedImage;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @brief Determines where to draw photos inside object.
 *
 * It finds the biggest circle inside the object (polygon) and then the
 * box inside the circle. The box is used to draw photo of the object or
 * photos of object's animals. It also can determine, which photo was
 * clicked by user (in case of animals and drag and drop).
 */
public class Envelope {

	/// Enveloped object (polygon).
	private ObjectData _envelopedObjectData = null;

	/// Center of gravity of the object (polygon).
	private Point2D.Double _centerOfGravity = new Point2D.Double(0.0 ,0.0);

	/// Radius of the circle inside object(polygon).
	private double _radius = 0.0;

	/// Box inside the circle.
	private Rectangle _box = new Rectangle(0, 0, 0 ,0 );

	/// In case, object can contain animals:
	/// Animals in a box = _animalsSidePics * _animalsSidePics
	private int _animalsSidePics = 0;

	/// Animals ids.
	private AnimalObjectRelData[] _animalsRelData = null;

	/**
	 * @brief Constructor.
	 *
	 * @param[in] od Object to envelope.
	 */
	public Envelope(ObjectData od) {
		// Copy enveloped object with map coordinates.
		_envelopedObjectData = (ObjectData) od.deepClone();
		_envelopedObjectData.coordinates = View.getInstance().db2map(_envelopedObjectData.coordinates);

		// Find center of gravity.
		int num = 0;
		for (int i = 0; i < (_envelopedObjectData.coordinates.length - 1); i = i + 2) {
			_centerOfGravity.x += _envelopedObjectData.coordinates[i];
			_centerOfGravity.y += _envelopedObjectData.coordinates[i+1];
			num++;
		}

		_centerOfGravity.x /= num;
		_centerOfGravity.y /= num;

		// Find enveloping circle.
		switch (_envelopedObjectData.objectType) {
			case 0: // point
				break;

			case 1: // polygon
				_radius = java.lang.Double.MAX_VALUE;
				for (int i = 0; i < (_envelopedObjectData.coordinates.length - 1); i = i + 2) {
					Point2D.Double a = new Point2D.Double(
							_envelopedObjectData.coordinates[(i) % _envelopedObjectData.coordinates.length],
							_envelopedObjectData.coordinates[(i + 1) % _envelopedObjectData.coordinates.length]);
					Point2D.Double b = new Point2D.Double(
							_envelopedObjectData.coordinates[(i + 2) % _envelopedObjectData.coordinates.length],
							_envelopedObjectData.coordinates[(i+3) % _envelopedObjectData.coordinates.length]);
					Line2D.Double line = new Line2D.Double(a,b);
					if (_radius > line.ptSegDist(_centerOfGravity)) {
					    _radius = line.ptSegDist(_centerOfGravity);
					}
				}
				break;

			case 2: // line
				break;

			case 3: // rectangle
				int width = _envelopedObjectData.coordinates[2] - _envelopedObjectData.coordinates[0];
				int height = _envelopedObjectData.coordinates[3] - _envelopedObjectData.coordinates[1];

				width /= 2;
				height /= 2;

				_radius = (width < height) ? width : height;
				break;

			case 4: // circle
				_centerOfGravity.x = _envelopedObjectData.coordinates[0];
				_centerOfGravity.y = _envelopedObjectData.coordinates[3];
				_radius = Math.abs(_centerOfGravity.y - _envelopedObjectData.coordinates[1]);
				break;

			default:
				Logger.getLogger("zoo").log(Level.WARNING,
						"Unknown object type in switch.");
				break;
		}

		// Find box inside circle.
		if (_radius > 0.0) {
			double a = java.lang.Math.sqrt(_radius*_radius/2);
			int x = (int)(_centerOfGravity.x - a);
			int y = (int)(_centerOfGravity.y - a);
			int width = (int)(_centerOfGravity.x + a) - x;
			int height =(int)(_centerOfGravity.y + a) - y;
			_box = new Rectangle(x, y, width, height);
		}

		// If object can contain animals, find them.
		if (_envelopedObjectData.tableName != null && Objects.canContainAnimal(_envelopedObjectData.tableName)) {
			_animalsRelData = AnimalOperations.getAnimalsFromObject(_envelopedObjectData.tableName, _envelopedObjectData.id);
			_animalsSidePics = (int)java.lang.Math.ceil(java.lang.Math.sqrt(_animalsRelData.length));
		}
	}

	/**
	 * @brief Draw all animals that contains enveloped object.
	 *
	 * @param[in] g Graphics into we draw.
	 */
	public void drawAnimals(Graphics g) {
		if (g != null && _animalsRelData != null) {
			for (int i = 0 ; i < _animalsRelData.length; i++) {
				AnimalData animalData = Animals.getAnimalData(_animalsRelData[i].animalId);
				drawPhoto(g, animalData.photo, _animalsSidePics, i);
			}
		}
	}

	/**
	 * @brief Draw inside box function.
	 *
	 * @param[in] g Graphics into we draw.
	 * @param[in] photo Photo to draw.
	 * @param[in] sidePics Number of pics in one side.
	 * @param[in] n Picture in mosaic sidePics * sidePics
	 */
	public void drawPhoto(Graphics g, BufferedImage photo, int sidePics, int n) {
		if (g != null && photo != null && n < sidePics*sidePics) {
			int col = n % sidePics;
			int row = n / sidePics;
			int newWidth = _box.width / sidePics;
			int newHeight = _box.height / sidePics;
			int x = _box.x + newWidth * col;
			int y = _box.y + newHeight * row;
			g.drawImage(photo, x, y, x + newWidth, y + newHeight, 0, 0, photo.getWidth(), photo.getHeight(), null);
		}
	}

	/**
	 * @brief Draw inside box function.
	 *
	 * @param[in] g Graphics into we draw.
	 */
	public void drawPhoto(Graphics g) {
		drawPhoto(g, _envelopedObjectData.photo, 1, 0);
	}

	/**
	 * @brief Get clicked photo.
	 *
	 * @param[in] position Mouse position.
	 */
	public AnimalData getAnimal(Point position) {
		// Are we inside the box.
		if (_box.contains(position) && _animalsSidePics > 0) {
			int x = position.x - _box.x;
			int y = position.y - _box.y;
			int col = x / (_box.width / _animalsSidePics);
			int row = y / (_box.height / _animalsSidePics);
			int n = row * _animalsSidePics + col;

			// Is it a picture = animal?
			if (n < _animalsRelData.length) {
				return Animals.getAnimalData(_animalsRelData[n].animalId);
			}
			else {
				return null;
			}
		}
		return null;
	}
}