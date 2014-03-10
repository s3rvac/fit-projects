/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Data of a drawable object from a db.
 *
 */

package zoo;

import java.awt.Color;
import java.awt.image.BufferedImage;

/**
 * @brief Data of a drawable object from a db.
 */
public class ObjectData implements Cloneable {

	/// ID of an object which does not have any ID
	public static final int NO_OBJECT_ID = -1;

	/// Object type of an object which does not have any object type set
	public static final int NO_OBJECT_TYPE = -1;
	public static final int POINT_TYPE = 0;
	public static final int POLYGON_TYPE = 1;
	public static final int LINE_TYPE = 2;
	public static final int RECTANGLE_TYPE = 3;
	public static final int CIRCLE_TYPE = 4;

	/// Object ID.
	public int id = NO_OBJECT_ID;

	/// Table name.
	public String tableName = null;

	/// Object name.
	public String name = "";

	/// Object description.
	public String description = "";

	/// Object type (point, rectangle, ...)
	public int objectType = NO_OBJECT_TYPE;

	/// Object 2D coordinates.
	public int[] coordinates = null;

	/// Object color.
	public Color color = null;

	/// Object photo.
	BufferedImage photo = null;

	/// Object deepClone.
	public Object deepClone() {
		try {
			ObjectData copy = (ObjectData)super.clone();
			copy.coordinates = (int[])coordinates.clone();
			return copy;
		} catch (CloneNotSupportedException ex) {
			throw new Error("public Object deepClone()");
		}
	}
}
