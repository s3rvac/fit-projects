/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Information about drawable objects from a db.
 *
 */

package zoo;

import java.awt.Color;
import java.math.BigDecimal;
import java.sql.SQLException;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.logging.Level;
import java.util.logging.Logger;
import oracle.jdbc.OracleResultSet;
import oracle.ord.im.OrdImage;
import oracle.sql.ARRAY;
import oracle.sql.STRUCT;

/**
 * @brief Information about drawable objects from a db.
 *
 * Implements the "library" (aka "static helper") design pattern.
 */
public class Objects {

	// Pavilion table name.
	public static final String PAVILION_TABLE_NAME = "pavilion";

	// Pavilion table name.
	public static final String EXHIBIT_TABLE_NAME = "exhibit";

	// Pavilion table name.
	public static final String ENTRANCE_TABLE_NAME = "entrance";

	// Pavilion table name.
	public static final String REFRESHMENT_TABLE_NAME = "refreshment";

	// Pavilion table name.
	public static final String PATH_TABLE_NAME = "path";

	/// Table name -> object color mapping.
	public static final Hashtable _mapObjectsProperties = new Hashtable();

	static {
		// Insert information about objects
		_mapObjectsProperties.put(PAVILION_TABLE_NAME, new Color(0, 0, 255));
		_mapObjectsProperties.put(EXHIBIT_TABLE_NAME, new Color(0, 255, 0));
		_mapObjectsProperties.put(ENTRANCE_TABLE_NAME, new Color(255, 0, 0));
		_mapObjectsProperties.put(REFRESHMENT_TABLE_NAME, new Color(255, 0, 0));
		_mapObjectsProperties.put(PATH_TABLE_NAME, new Color(0, 0, 0));
	}

	/**
	 * @brief Returns names of all tables which contains drawable objects.
	 * @return Names of all tables which contains drawable objects.
	 */
	public static Enumeration getObjectTableNames() {
		return _mapObjectsProperties.keys();
	}

	/**
	 * @brief Checks whether an animal can be added to the selected object.
	 *
	 * @param objectTableName Name of the selected object's table.
	 * @return True if an animal can be added to the selected object, false otherwise.
	 */
	public static boolean canContainAnimal(String objectTableName) {
		// Animals can be added only into a pavilion or into an exhibit
		return objectTableName.equals(PAVILION_TABLE_NAME) ||
				objectTableName.equals(EXHIBIT_TABLE_NAME);
	}

	/**
	 * @brief Returns data of the selected object.
	 *
	 * @param[in] tableName Name of the table.
	 * @param[in] rowId Row ID.
	 * @return Object data.
	 *
	 * If the object does not exist, null will be returned.
	 */
	public static ObjectData getObjectData(String tableName, int rowId) {
		try {
			ObjectData objectData = new ObjectData();
			objectData.tableName = tableName;

			OracleResultSet rs = DBConnection.getInstance().oracleQueryResponse(
					"SELECT * " +
					"FROM " + tableName + " " +
					"WHERE row_id = " + rowId);
			if (rs.next()) {
				// Get object ID
				objectData.id = rs.getInt(tableName + "_id");

				// Get object name
				try {
					objectData.name = rs.getString("name");
				} catch (SQLException ex) {
					// This object doesn't have a name, so set the name
					// to the table name with the first letter in uppercase
					objectData.name = tableName.substring(0,1).toUpperCase() +
							tableName.substring(1);

					// Special name hanlding for exhibitions:
					if (tableName.equals(EXHIBIT_TABLE_NAME)) {
						objectData.name += " " + objectData.id;
					}
				}

				// Get object description
				try {
					objectData.description = rs.getString("description");
				} catch (SQLException ex) {
					objectData.description = "";
				}

				// Get object color
				objectData.color = (Color) _mapObjectsProperties.get(tableName);

				// Get object type and location
				STRUCT location = rs.getSTRUCT("location");
				Object[] locAttr = location.getAttributes();
				int gType = ((java.math.BigDecimal) locAttr[0]).intValue();
				if (Integer.toString(gType).endsWith("01")) {
					objectData.objectType = 0;
					Object[] attributes = ((STRUCT) locAttr[2]).getAttributes();
					objectData.coordinates = new int[]{
							((BigDecimal) attributes[0]).intValue(),
							((BigDecimal) attributes[1]).intValue()};
				} else {
					int[] eInfo = ((ARRAY) locAttr[3]).getIntArray();
					objectData.objectType = eInfo[2];
					objectData.coordinates = ((ARRAY) locAttr[4]).getIntArray();
				}

				// Get object photo
				try {
					objectData.photo = Conversions.ordImageToBufferedImage(
						(OrdImage) rs.getORAData("photo", OrdImage.getORADataFactory()));
				} catch (SQLException ex) {
					objectData.photo = null;
				}
			}

			return objectData;
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		}

		return null;
	}

	// Deny creation of instances
	private Objects() {}
}
