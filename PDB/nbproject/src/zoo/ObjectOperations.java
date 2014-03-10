/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Object operations like creating a new pavilion, moving an existing
 *        pavilion etc.
 *
 */

package zoo;

import java.awt.Point;
import java.sql.SQLException;
import java.util.Date;
import java.util.Enumeration;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import oracle.jdbc.OracleResultSet;

/**
 * @brief Object operations like creating a new pavilion, moving an existing
 *        pavilion etc.
 *
 * Implements the "library" (aka "static helper") design pattern.
 */
public class ObjectOperations {

	/// Maximal object distance when searching for an object that is on some
	/// position. If some object is further than this distance, it won't be
	/// marked as the one on the selected position.
	private static final int MAX_DISTANCE_ON_GET_OBJECT_ON_POS = 5;

	/// Maximal object distance when searching for a nearest object to some
	/// position. If some object is further than this distance, it won't be
	/// marked as the nearest one.
	private static final int MAX_DISTANCE_ON_GET_NEAREST_OBJECT = Integer.MAX_VALUE;

	/**
	 * @brief Returns the nearest object from given tables that is closest to
	 *        the selected position.
	 *
	 * @param[in] position Position on the map.
	 * @param[in] tableNames Names of tables in which the nearest object
	 *                       is searched.
	 * @param[in] maxDistance Maximal distance (if some object is further than
	 *                        this distance, it won't be marked as the nearest one).
	 * @param[in] date The date.
	 * @return Object on the selected position.
	 *
	 * If there is an error or there is no nearest object, it returns null.
	 */
	private static ObjectData getNearestObject(Point position,
			Vector<String> tableNames, int maxDistance, Date date) {
		try {
			int nearestObjectDistance = Integer.MAX_VALUE;
			ObjectData nearestObject = new ObjectData();

			// Iterate over the selected tables
			for (String tableName : tableNames) {
				// Get distances from all objects from this table
				String query =
						"SELECT row_id, SDO_GEOM.SDO_DISTANCE(location, MDSYS.SDO_GEOMETRY(" +
							"2001, NULL, MDSYS.SDO_POINT_TYPE(" + position.getX() + ", " +
							position.getY() + ", NULL), NULL, NULL), 0.005) AS distance " +
						"FROM " + tableName + " " +
						"WHERE " + TemporalQuery.toTemporalAtDate(date);
				OracleResultSet distanceRS = DBConnection.getInstance(
						).oracleQueryResponse(query);
				while (distanceRS.next()) {
					int distance = distanceRS.getInt("distance");

					// Check if the distance is shorter than the currently
					// shortest distance and maximal distance
					if (distance < nearestObjectDistance &&
							distance < maxDistance) {
						nearestObjectDistance = distance;
						nearestObject.tableName = tableName;
						nearestObject.id = distanceRS.getInt("row_id");
					}
				}
			}

			// Has the nearest object been found?
			if (nearestObject.id != ObjectData.NO_OBJECT_ID) {
				return Objects.getObjectData(nearestObject.tableName,
						nearestObject.id);
			} else {
				return null;
			}
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			return null;
		}
	}

	/**
	 * @brief Returns object on the selected position.
	 *
	 * @param[in] position Position on the map.
	 * @param[in] date Only objects that existed at this date will be taken
	 *                 into account.
	 * @return Object on the selected position.
	 *
	 * If there is an error or there is no nearest object, it returns null.
	 */
	public static ObjectData getObjectOnPosition(Point position, Date date) {
		Vector<String> tableNames = new Vector<String>();

		// All tables that have spatial data will be searched for the
		// nearest object
		Enumeration e = Objects.getObjectTableNames();
		while (e.hasMoreElements()) {
			tableNames.add((String) e.nextElement());
		}

		return getNearestObject(position, tableNames,
				MAX_DISTANCE_ON_GET_OBJECT_ON_POS, date);
	}

	/**
	 * @brief Returns the nearest object from given table that is closest to
	 *        the position.
	 *
	 * @param[in] position Position on the map.
	 * @param[in] tableName The name of the table.
	 * @param[in] date The date.
	 * @return Object on the selected position.
	 *
	 * If there is an error or there is no nearest object, it returns null.
	 */
	public static ObjectData getNearestObject(Point position, String tableName,
			Date date) {
		Vector<String> tableNames = new Vector<String>();

		// Only one table will be searched for a nearest object
		tableNames.add(tableName);

		return getNearestObject(position, tableNames,
				MAX_DISTANCE_ON_GET_NEAREST_OBJECT, date);
	}

	// Deny creation of instances
	private ObjectOperations() {}
}
