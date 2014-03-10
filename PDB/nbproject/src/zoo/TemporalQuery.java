/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Static class for construction of temporal queries
 *
 */

package zoo;

import oracle.jdbc.OracleResultSet;
import java.sql.SQLException;
import java.sql.ResultSet;
import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

/**
 * @brief  Static class for construction of temporal queries
 */
public class TemporalQuery
{
	/// This value represents 'forever'; 8099 in the Date constructor is the same
	/// as 9999 A.D. ; 11th month is December (according to Java Data type)
	public static final Date FOREVER_DATE = new Date(8099, 11, 31);

	/// Textual representation of the 'forever' date
	public static final String FOREVER_DATE_TEXT_REPR = "forever";

	/// This value represents ATSQL 'forever' time value
	public static final String FOREVER = toSqlDate(FOREVER_DATE);

	/// The row ID meaning no object
	public static final int NO_OBJECT_ROW_ID = -1;


	/**
	 * @brief  A method for insertion of pavilion into database
	 *
	 * The inserted pavilion has the *to* temporal component set to 'forever'
	 *
	 * @param[in]  name          The name of the pavilion
	 * @param[in]  objectCoords  Coordinates of the pavilion
	 * @param[in]  dateFrom      Start of the temporal component
	 * @return  The ID of the inserted pavilion or NO_OBJECT_ROW_ID
	 */
	public static int insertPavilion(String name, int[] objectCoords,
		Date dateFrom) throws SQLException
	{
		// create the geometry object
		String geometry =
			"  MDSYS.SDO_GEOMETRY(" +
			"    2003, " +                                  // 2D polygon
			"    NULL, " +                                  // system of coordinates
			"    NULL, " +                                  // it is not a point
			"    MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 1), " + // polygon defined by points
			"    MDSYS.SDO_ORDINATE_ARRAY(";

		// insert coordinates
		for (int i = 0; i < objectCoords.length - 1; ++i) {
			geometry += String.valueOf(objectCoords[i]) + ", ";
		}

		// insert the last coordinate
		if (objectCoords.length > 0) {
			geometry += String.valueOf(objectCoords[objectCoords.length - 1]);
		}

		geometry += "))";

		if (isValidInsertion(geometry, dateFrom)) {
			// if the object is possible to be inserted
			// build the query
			String query = "INSERT INTO pavilion(row_id, " +
				"pavilion_id, name, location, photo, photo_signature, " +
				"date_from, date_to) VALUES ( " +
				"  seq_pavilion_row_id.NEXTVAL, " +
				"  seq_pavilion_id.NEXTVAL, " +
				"'" +  name + "', " +
				   geometry + ", " +              // geometry
				"  NULL, " +                      // no photo
				"  NULL, " +                      // no photo signature
				   toSqlDate(dateFrom) + ", " +
					 FOREVER + ")";

			// execute the query
			return DBConnection.getInstance().oracleQueryInsert(query,	"row_id");
		} else {
			return NO_OBJECT_ROW_ID;
		}
	}

	/**
	 * @brief  A method for insertion of exhibit into database
	 *
	 * The inserted exhibit has the *to* temporal component set to 'forever'
	 *
	 * @param[in]  objectCoords  Coordinates of the exhibit
	 * @param[in]  dateFrom      Start of the temporal component
	 * @return  The ID of the inserted exhibit or NO_OBJECT_ROW_ID
	 */
	public static int insertExhibit(int[] objectCoords,
		Date dateFrom) throws SQLException
	{
		// create the geometry object
		String geometry =
			"  MDSYS.SDO_GEOMETRY(" +
			"    2003, " +                                  // 2D polygon
			"    NULL, " +                                  // system of coordinates
			"    NULL, " +                                  // it is not a point
			"    MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 1), " + // polygon defined by points
			"    MDSYS.SDO_ORDINATE_ARRAY(";

		// insert coordinates
		for (int i = 0; i < objectCoords.length - 1; ++i) {
			geometry += String.valueOf(objectCoords[i]) + ", ";
		}

		// insert the last coordinate
		if (objectCoords.length > 0) {
			geometry += String.valueOf(objectCoords[objectCoords.length - 1]);
		}

		geometry += "))";

		if (isValidInsertion(geometry, dateFrom)) {
			// if the object is possible to be inserted
			// build the query
			String query = "INSERT INTO exhibit(row_id, " +
				"exhibit_id, location, photo, photo_signature, " +
				"date_from, date_to) VALUES ( " +
				"  seq_exhibit_row_id.NEXTVAL, " +
				"  seq_exhibit_id.NEXTVAL, " +
				   geometry + ", " +
				"    NULL, " +                      // no photo
				"    NULL, " +                      // no photo signature
				     toSqlDate(dateFrom) + ", " +
						 FOREVER + ")";

			// execute the query
			return DBConnection.getInstance().oracleQueryInsert(query,	"row_id");
		} else {
			return NO_OBJECT_ROW_ID;
		}
	}

	/**
	 * @brief  A method for insertion of ZOO entrance into database
	 *
	 * The inserted entrance has the *to* temporal component set to 'forever'
	 *
	 * @param[in]  coordX    The X-coordinate of the entrance location
	 * @param[in]  coordY    The Y-coordinate of the entrance location
	 * @param[in]  dateFrom  Start of the temporal component
	 * @return  The ID of the inserted entrance or NO_OBJECT_ROW_ID
	 */
	public static int insertEntrance(int coordX, int coordY,
		Date dateFrom) throws SQLException
	{
		// create the geometry object
		String geometry =
			"  MDSYS.SDO_GEOMETRY(" +
			"    2001, " +                                  // 2D point
			"    NULL, " +                                  // system of coordinates
			"    MDSYS.SDO_POINT_TYPE(" + String.valueOf(coordX) + // X coordinate
			", " + String.valueOf(coordY) + ", NULL), " +          // Y coordinate
			"    NULL, " +
			"    NULL)";

		if (isValidInsertion(geometry, dateFrom)) {
			// if the object is possible to be inserted
			// build the query
			String query = "INSERT INTO entrance(row_id, " +
				"entrance_id, location, photo, photo_signature, " +
				"date_from, date_to) VALUES ( " +
				"  seq_entrance_row_id.NEXTVAL, " +
				"  seq_entrance_id.NEXTVAL, " +
				   geometry + ", " +
				"  NULL, " +                      // no photo
				"  NULL, " +                      // no photo signature
				   toSqlDate(dateFrom) + ", " +
				   FOREVER + ")";

			// execute the query
			return DBConnection.getInstance().oracleQueryInsert(query,	"row_id");
		} else {
			return NO_OBJECT_ROW_ID;
		}
	}

	/**
	 * @brief  A method for insertion of path into database
	 *
	 * The inserted path has the *to* temporal component set to 'forever'
	 *
	 * @param[in]  coordX1   The X-coordinate of the first path point
	 * @param[in]  coordY1   The Y-coordinate of the first path point
	 * @param[in]  coordX2   The X-coordinate of the second path point
	 * @param[in]  coordY2   The Y-coordinate of the second path point
	 * @param[in]  dateFrom  Start of the temporal component
	 * @return  The ID of the inserted path or NO_OBJECT_ROW_ID
	 */
	public static int insertPath(int coordX1, int coordY1,
		int coordX2, int coordY2, Date dateFrom) throws SQLException
	{
		// create the geometry object
		String geometry =
			"  MDSYS.SDO_GEOMETRY(" +
			"    2002, " +                               // 2D line/spline
			"    NULL, " +                               // system of coordinates
			"    NULL, " +                               // it is not a point
			"    MDSYS.SDO_ELEM_INFO_ARRAY(1, 2, 1), " + // line
			"    MDSYS.SDO_ORDINATE_ARRAY(" +
			       String.valueOf(coordX1) + ", " +      // X1 coordinate
			       String.valueOf(coordY1) + ", " +      // Y1 coordinate
						 String.valueOf(coordX2) + ", " +      // X2 coordinate
						 String.valueOf(coordY2) + "))";       // Y2 coordinate

		if (isValidInsertion(geometry, dateFrom)) {
			// if the object is possible to be inserted
			// build the query
			String query = "INSERT INTO path(row_id, " +
				"path_id, location, date_from, date_to) VALUES ( " +
				"  seq_path_row_id.NEXTVAL, " +
				"  seq_path_id.NEXTVAL, " +
				   geometry + ", " +
				   toSqlDate(dateFrom) + ", " +
				   FOREVER + ")";

			// execute the query
			return DBConnection.getInstance().oracleQueryInsert(query,	"row_id");
		} else {
			return NO_OBJECT_ROW_ID;
		}
	}

	/**
	 * @brief  A method for insertion of refreshment into database
	 *
	 * The inserted refreshment has the *to* temporal component set to 'forever'
	 *
	 * @param[in]  description  Description of the inserted refreshment
	 * @param[in]  coordX1   The X-coordinate of the first circle point
	 * @param[in]  coordY1   The Y-coordinate of the first circle point
	 * @param[in]  coordX2   The X-coordinate of the second circle point
	 * @param[in]  coordY2   The Y-coordinate of the second circle point
	 * @param[in]  coordX3   The X-coordinate of the third circle point
	 * @param[in]  coordY3   The Y-coordinate of the third circle point
	 * @param[in]  dateFrom  Start of the temporal component
	 * @return  The ID of the inserted pavilion or NO_OBJECT_ROW_ID
	 */
	public static int insertRefreshment(String description,
		int coordX1, int coordY1, int coordX2, int coordY2,
		int coordX3, int coordY3, Date dateFrom) throws SQLException
	{
		// create the geometry object
		String geometry =
			"  MDSYS.SDO_GEOMETRY(" +
			"    2003, " +                                  // 2D polygon
			"    NULL, " +                                  // system of coordinates
			"    NULL, " +                                  // it is not a point
			"    MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 4), " + // circle defined by points
			"    MDSYS.SDO_ORDINATE_ARRAY(" +
			       String.valueOf(coordX1) + ", " +      // X1 coordinate
			       String.valueOf(coordY1) + ", " +      // Y1 coordinate
						 String.valueOf(coordX2) + ", " +      // X2 coordinate
						 String.valueOf(coordY2) + ", " +      // Y2 coordinate
						 String.valueOf(coordX3) + ", " +      // X3 coordinate
						 String.valueOf(coordY3) + "))";       // Y3 coordinate

		if (isValidInsertion(geometry, dateFrom)) {
			// if the object is possible to be inserted
			// build the query
			String query = "INSERT INTO refreshment(row_id, refreshment_id, " +
				"description, location, photo, photo_signature, date_from, date_to) " +
				"VALUES (" +
				"  seq_refreshment_row_id.NEXTVAL, " +
				"  seq_refreshment_id.NEXTVAL, " +
				"'" + description + "', " +
			     geometry + ", " +
				"  NULL, " +
				"  NULL, " +
			     toSqlDate(dateFrom) + ", " +
					 FOREVER + ")";

			// execute the query
			return DBConnection.getInstance().oracleQueryInsert(query,	"row_id");
		} else {
			return NO_OBJECT_ROW_ID;
		}
	}

	/**
	 * @brief  Converion method to SQL date
	 *
	 * This method converts Java Date data type to SQL string representation of
	 * the date.
	 *
	 * @param[in]  date  The input Java Date
	 * @return  SQL string representation of the date
	 */
	protected static String toSqlDate(Date date)
	{
		DateFormat dateFormat = new SimpleDateFormat(Conversions.DATE_FORMAT_PATTERN);
		String strDate = "TO_DATE('" + dateFormat.format(date) + "', 'YYYY-MM-DD')";
		return strDate;
	}

	/**
	 * @brief  The method that generates temporal SQL string for a date
	 *
	 * This method generates a string that represents temporal data query. The
	 * string is to be appended at the end of a query string WHERE clausule.
	 * This only selects one point at time.
	 *
	 * @param[in]  date  The date (temporal component)
	 * @return  String representing the temporal component query
	 */
	public static String toTemporalAtDate(Date date) {
		return " (date_from <= " + toSqlDate(date) +
				" AND date_to > " + toSqlDate(date) + ") ";
	}

	/**
	 * @brief  The method that generates temporal SQL string for a date
	 *
	 * This method generates a string that represents temporal data query. The
	 * string is to be appended at the end of a query string WHERE clausule.
	 * This only selects one point at time.
	 *
	 * @param[in]  date	       The date (temporal component)
	 * @param[in]  tableAlias  The alias of the table
	 * @return  String representing the temporal component query
	 */
	public static String toTemporalAtDate(Date date, String tableAlias) {
		return " (" + tableAlias + ".date_from <= " + toSqlDate(date) +
				" AND " + tableAlias + ".date_to > " + toSqlDate(date) + ") ";
	}

	/**
	 * @brief  A method for deletion of temporal data item at given time from the
	 *         database
	 *
	 * This method deletes temporal data item from the database. The piece of data
	 * is identified by id, table name and date. The date must be _in_ the time
	 * period for which the data item is valid.
	 *
	 * @param[in]  id         Pavilion ID
	 * @param[in]  tableName  Name of the table from which the item is to be
	 *                        deleted
	 * @param[in]  date       Time which is in the record that is to be deleted
	 */
	public static void deleteQuery(int id, String tableName, Date date)
		throws SQLException
	{
		String query =
		 	"SELECT row_id, date_from " +
			"FROM " + tableName + " " +
			"WHERE " + tableName + "_id = " + Integer.toString(id) + " AND " +
				toTemporalAtDate(date);

		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		if (!ors.next()) {
			// if the first row of the result does not exist, fail gracefully
			throw new RuntimeException("Marvin says: Invalid date!");
		}

		int rowID = ors.getInt(1);
		Date rowFromDate = ors.getDate(2);

		if ((date.getYear() == rowFromDate.getYear()) &&
				(date.getMonth() == rowFromDate.getMonth()) &&
				(date.getDay() == rowFromDate.getDay())) {
			// if the beginning of the interval coincides with the date
			query =
				"DELETE FROM " + tableName + " " +
				"WHERE row_id = " + Integer.toString(rowID);

			DBConnection.getInstance().oracleQueryVoid(query);
		} else {
			// if the beginning of the interval does not coincide with the date
			query =
				"UPDATE " + tableName + " " +
				"SET date_to = " + toSqlDate(date) +
				"WHERE row_id = " + Integer.toString(rowID);

			DBConnection.getInstance().oracleQueryVoid(query);
		}

		query =
			"DELETE FROM " + tableName + " " +
			"WHERE " + tableName + "_id = " + Integer.toString(id) + " AND " +
				"date_from > " + toSqlDate(date);

		DBConnection.getInstance().oracleQueryVoid(query);
	}

	/**
	 * @brief  A method that checks whether a geometry object does not collide
	 *         with other objects in given time period
	 *
	 * This method checks whether a geometry object collides or not with another
	 * object of the database, therefore, whether the geometry object can be
	 * inserted. The query is temporal.
	 *
	 * @param[in]  geometryObject  A SQL string representation of the geometry object
	 * @param[in]  fromDate        Start date of the period
	 * @return  Is it possible to insert the geometry object?
	 */
	private static boolean isValidInsertion(String geometryObject,
		Date fromDate) throws SQLException
	{
		// return true in case that area of intersection of any object with this is
		// zero

		String query =
			getNotDisjointQueryForTable(Objects.PAVILION_TABLE_NAME.toUpperCase(),
				geometryObject, fromDate);
		query += " UNION " +
			getNotDisjointQueryForTable(Objects.EXHIBIT_TABLE_NAME.toUpperCase(),
				geometryObject, fromDate);
		query += " UNION " +
			getNotDisjointQueryForTable(Objects.ENTRANCE_TABLE_NAME.toUpperCase(),
				geometryObject, fromDate);
		query += " UNION " +
			getNotDisjointQueryForTable(Objects.PATH_TABLE_NAME.toUpperCase(),
				geometryObject, fromDate);
		query += " UNION " +
			getNotDisjointQueryForTable(Objects.REFRESHMENT_TABLE_NAME.toUpperCase(),
				geometryObject, fromDate);

		ResultSet rs = DBConnection.getInstance().oracleQueryResponse(query);
		if (rs.next()) {
			return false;  // the insertion is invalid
		} else {
			return true;	 // the insertion is valid
		}
	}

	/**
	 * @brief  A method that constructs a query string for detection of objects
	 *         which are not disjoint with the given object
	 *
	 * This method creates a query string that checks whether a given object is
	 * not disjoint to some other objects in the given table in given time
	 * interval (fromDate to FOREVER_DATE).
	 *
	 * @param[in]  tableName       The name of the table that is to be checked
	 * @param[in]  geometryObject  A SQL string representation of the geometry object
	 * @param[in]  fromDate        Start date of the period
	 * @return  Is it possible to insert the geometry object?
	 */
	private static String getNotDisjointQueryForTable(String tableName,
		String geometryObject, Date fromDate)
	{
		String query =
			"SELECT row_id " +
			"FROM " + tableName +" " +
			"WHERE (SDO_GEOM.RELATE(" +
				geometryObject + ", " +
				"(SELECT diminfo " +
				 "FROM user_sdo_geom_metadata " +
				 "WHERE table_name = '" + tableName + "'), " +
				"'anyinteract', " +
				"location, " +
				"(SELECT diminfo " +
				 "FROM user_sdo_geom_metadata " +
				 "WHERE table_name = '" + tableName + "')) = 'TRUE' OR " +
			 "SDO_GEOM.RELATE(" +
				geometryObject + ", " +
				"(SELECT diminfo " +
				 "FROM user_sdo_geom_metadata " +
				 "WHERE table_name = '" + tableName + "'), " +
				"'determine', " +
				"location, " +
				"(SELECT diminfo " +
				 "FROM user_sdo_geom_metadata " +
				 "WHERE table_name = '" + tableName + "')) LIKE 'UNKNOWN MASK%') AND " +
				 "date_to > " + toSqlDate(fromDate);

			return query;
	}

	/**
	 * @brief  A method that changes the name of the pavilion from given time
	 *
	 * This method changes the name of the pavilion from given time in all time
	 * intervals when the name is the same as the name if in interval that
	 * fromDate is in.
	 *
	 * @param[in]  id        The ID of the pavilion
	 * @param[in]  name      The new name of the pavilion
	 * @param[in]  fromDate  The date from which the name is to be changed
	 */
	public static void changePavilionName(int id, String name, Date fromDate)
		throws SQLException
	{
		changeString("pavilion", id, "name", name, fromDate);
	}

	/**
	 * @brief  A method that changes the description of the refreshment from given time
	 *
	 * This method changes the description of the refreshment from given time in all time
	 * intervals when the description is the same as the description if in interval that
	 * fromDate is in.
	 *
	 * @param[in]  id           The ID of the refreshment
	 * @param[in]  description  The new description of the refreshment
	 * @param[in]  fromDate     The date from which the description is to be changed
	 */
	public static void changeRefreshmentDescription(int id, String description, Date fromDate)
		throws SQLException
	{
		changeString("refreshment", id, "description", description, fromDate);
	}

	/**
	 * @brief  A method that changes a string component of given table
	 *
	 * This method changes the string component of a given table's row from
	 * given time in all time intervals when the component is the same as the
	 * name if in interval that fromDate is in.
	 *
	 * @param[in]  tableName   The name of the table
	 * @param[in]  id          The ID of the object
	 * @param[in]  columnName  The name of the column the component is in
	 * @param[in]  value       The new value of the component
	 * @param[in]  fromDate    The date from which the name is to be changed
	 */
	public static void changeString(String tableName, int id, String columnName,
		String value, Date fromDate) throws SQLException
	{
		String objectID = Integer.toString(id);

		String query =
			"SELECT row_id, " + columnName + ", date_from, date_to " +
			"FROM " + tableName + " " +
			"WHERE " + tableName + "_id = " + objectID + " AND " + toTemporalAtDate(fromDate);

		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		if (!ors.next()) {
			// if the first row of the result does not exist, fail gracefully
			throw new RuntimeException("Marvin says: Invalid date!");
		}

		// read the data from the result set
		int oldRowID = ors.getInt(1);
		String oldValue = ors.getString(2);
		Date oldFromDate = ors.getDate(3);
		Date oldToDate = ors.getDate(4);

		if (value.equals(oldValue)) {
			// if the user is a moron
			return;
		}

		if ((oldFromDate.getDay() == fromDate.getDay()) &&
		    (oldFromDate.getMonth() == fromDate.getMonth()) &&
			  (oldFromDate.getYear() == fromDate.getYear())) {
			// if the beginning of the new interval equals the beginning of the olden one
			query =
				"UPDATE " + tableName + " " +
				"SET " + columnName + " = '" + value + "' " +
				"WHERE row_id = " + Integer.toString(oldRowID);

			// reuse the old interval
			DBConnection.getInstance().oracleQueryVoid(query);

			// merge with the previous interval if they are the same
			mergeIfEquals(tableName, id, fromDate);
		} else {
			// if the beginning of the new interval is behind the beginning of the olden one
			query =
				"UPDATE " + tableName + " " +
				"SET date_to = " + toSqlDate(fromDate) + " " +
				"WHERE row_id = " + Integer.toString(oldRowID);

			// shorten the old interval
			DBConnection.getInstance().oracleQueryVoid(query);

			query = "INSERT INTO " + tableName + " (row_id, " +
				tableName + "_id, " + columnName + ", location, photo, photo_signature, " +
				"date_from, date_to) " +
				"SELECT " +
					"seq_" + tableName + "_row_id.NEXTVAL, " +
					"tab." + tableName + "_id, " +
					"'" + value + "', " +
					"tab.location, " +
					"tab.photo, " +
					"tab.photo_signature, " +
					toSqlDate(fromDate) + ", " +
					toSqlDate(oldToDate) + " " +
				"FROM " + tableName + " tab " +
				"WHERE tab.row_id = " + Integer.toString(oldRowID);

			// fill the rest of the olden interval with the new data
			DBConnection.getInstance().oracleQueryVoid(query);
		}

		query =
		 	"SELECT row_id, " + columnName + ", date_from, date_to " +
			"FROM " + tableName + " " +
			"WHERE " + tableName + "_id = " + objectID + "  AND date_from = " + toSqlDate(oldToDate);

		ors = DBConnection.getInstance().oracleQueryResponse(query);
		while (ors.next()) {
			// while there are some following intervals
			int contRowID = ors.getInt(1);
			String contValue = ors.getString(2);
			Date contFromDate = ors.getDate(3);
			Date contToDate = ors.getDate(4);

			if (contValue.equals(oldValue)) {
				// if the name in the following interval is the same as it was in the previous one
				query =
					"UPDATE " + tableName + " " +
					"SET " + columnName + " = '" + value + "' " +
					"WHERE row_id = " + Integer.toString(contRowID);

				// change the name in the following interval
				DBConnection.getInstance().oracleQueryVoid(query);

				query =
					"SELECT row_id, " + columnName + ", date_from, date_to " +
					"FROM " + tableName + " " +
					"WHERE " + tableName + "_id = " + objectID + "  AND date_from = " + toSqlDate(contToDate);

				ors = DBConnection.getInstance().oracleQueryResponse(query);
			} else if (contValue.equals(value)) {
				mergeIfEquals(tableName, id, contFromDate);
				break;
			}
		}
	}

	/**
	 * @brief  A method that changes a photo component of given table
	 *
	 * This method changes the photo component of a given table's row from
	 * given time in all time intervals when the component is the same as the
	 * photo if in interval that fromDate is in.
	 *
	 * @param[in]  tableName   The name of the table
	 * @param[in]  id          The ID of the object
	 * @param[in]  fileName    The file name of the new photo
	 * @param[in]  fromDate    The date from which the photo is to be changed
	 */
	public static void changePhoto(String tableName, int id,
		String fileName, Date fromDate) throws SQLException
	{
		String objectID = Integer.toString(id);

		int photoID = PhotoOperations.uploadTemporaryPhotoToDb(fileName);

		String query =
			"SELECT row_id, date_from, date_to " +
			"FROM " + tableName + " " +
			"WHERE " + tableName + "_id = " + objectID + " AND " + toTemporalAtDate(fromDate);

		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		if (!ors.next()) {
			// if the first row of the result does not exist, fail gracefully
			throw new RuntimeException("Marvin says: Invalid date!");
		}

		// read the data from the result set
		int oldRowID = ors.getInt(1);
		Date oldFromDate = ors.getDate(2);
		Date oldToDate = ors.getDate(3);

		query =
			"SELECT * " +
			"FROM " + tableName + " a, uploaded_photo b " +
			"WHERE a." + tableName + "_id = " + objectID + " AND " +
				"b.uploaded_photo_id = " + Integer.toString(photoID) + " AND " +
				toTemporalAtDate(fromDate, "a") + " AND " +
				"ORDSYS.IMGSimilar(a.photo_signature, b.photo_signature, " +
					"'color=\"0.3\" texture=\"0.3\" shape=\"0.3\" location=\"0.1\"', " +
					" 100, 123) < 100 AND " +
				"ORDSYS.IMGScore(123) = 0";

		ors = DBConnection.getInstance().oracleQueryResponse(query);
		if (ors.next()) {
			// if the user is a moron
			return;
		}

		// backup the previous photo
		query =
			"INSERT INTO uploaded_photo " +
				"(uploaded_photo_id, photo, photo_signature) " +
			"VALUES (seq_uploaded_photo_id.NEXTVAL, " +
				"(SELECT photo " +
				"FROM " + tableName + " " +
				"WHERE " + tableName + "_id = " + Integer.toString(oldRowID) + "), " +
				"(SELECT photo_signature " +
				"FROM " + tableName + " " +
				"WHERE " + tableName + "_id = " + Integer.toString(oldRowID) + "))";

		int oldenPhotoID =
			DBConnection.getInstance().oracleQueryInsert(query, "uploaded_photo_id");

		if ((oldFromDate.getDay() == fromDate.getDay()) &&
		    (oldFromDate.getMonth() == fromDate.getMonth()) &&
			  (oldFromDate.getYear() == fromDate.getYear())) {
			// if the beginning of the new interval equals the beginning of the olden one
			query =
				"UPDATE " + tableName + " " +
				"SET photo = " +
					"(SELECT photo " +
					"FROM uploaded_photo " +
					"WHERE uploaded_photo_id = " + Integer.toString(photoID) + "), " +
					" photo_signature = " +
					"(SELECT photo_signature " +
					"FROM uploaded_photo " +
					"WHERE uploaded_photo_id = " + Integer.toString(photoID) + ") " +
				"WHERE row_id = " + Integer.toString(oldRowID);

			// reuse the old interval
			DBConnection.getInstance().oracleQueryVoid(query);

			// merge with the previous interval if they are the same
			mergeIfEquals(tableName, id, fromDate);
		} else {
			// if the beginning of the new interval is behind the beginning of the olden one
			query =
				"UPDATE " + tableName + " " +
				"SET date_to = " + toSqlDate(fromDate) + " " +
				"WHERE row_id = " + Integer.toString(oldRowID);

			// shorten the old interval
			DBConnection.getInstance().oracleQueryVoid(query);

			String columnName = "";
			if (tableName.equals("pavilion")) {
				columnName = "name, ";
			} else if (tableName.equals("refreshment")) {
				columnName = "description, ";
			}

			String columnValue = "";
			if (tableName.equals("pavilion")) {
				columnValue = "tab.name, ";
			} else if (tableName.equals("refreshment")) {
				columnValue = "tab.description, ";
			}

			query = "INSERT INTO " + tableName + " (row_id, " +
				tableName + "_id, " + columnName + "location, photo, photo_signature, " +
				"date_from, date_to) " +
				"SELECT " +
					"seq_" + tableName + "_row_id.NEXTVAL, " +
					"tab." + tableName + "_id, " +
					columnValue +
					"tab.location, " +
					"(SELECT photo " +
					"FROM uploaded_photo " +
					"WHERE uploaded_photo_id = " + Integer.toString(photoID) + "), " +
					"(SELECT photo_signature " +
					"FROM uploaded_photo " +
					"WHERE uploaded_photo_id = " + Integer.toString(photoID) + "), " +
					toSqlDate(fromDate) + ", " +
					toSqlDate(oldToDate) + " " +
				"FROM " + tableName + " tab " +
				"WHERE tab.row_id = " + Integer.toString(oldRowID);

			// fill the rest of the olden interval with the new data
			DBConnection.getInstance().oracleQueryVoid(query);
		}

		query =
		 	"SELECT row_id, date_from, date_to " +
			"FROM " + tableName + " " +
			"WHERE " + tableName + "_id = " + objectID + "  AND date_from = " + toSqlDate(oldToDate);

		ors = DBConnection.getInstance().oracleQueryResponse(query);
		while (ors.next()) {
			// while there are some following intervals
			int contRowID = ors.getInt(1);
			Date contFromDate = ors.getDate(2);
			Date contToDate = ors.getDate(3);

			query =
				"SELECT * " +
				"FROM " + tableName + " a, uploaded_photo b " +
				"WHERE a.row_id = " + contRowID + " AND " +
					"b.uploaded_photo_id = " + Integer.toString(oldenPhotoID) + " AND " +
					"ORDSYS.IMGSimilar(a.photo_signature, b.photo_signature, " +
						"'color=\"0.3\" texture=\"0.3\" shape=\"0.3\" location=\"0.1\"', " +
						" 100, 123) < 100 AND " +
					"ORDSYS.IMGScore(123) = 0";

			ors = DBConnection.getInstance().oracleQueryResponse(query);
			if (ors.next()) {
				// if the photo in the following interval is the same as it was in the previous one
				query =
					"UPDATE " + tableName + " " +
					"SET photo = " +
						"(SELECT photo " +
						"FROM uploaded_photo " +
						"WHERE uploaded_photo_id = " + Integer.toString(photoID) + "), " +
						" photo_signature = " +
						"(SELECT photo_signature " +
						"FROM uploadedPhoto " +
						"WHERE uploaded_photo_id = " + Integer.toString(photoID) + ") " +
					"WHERE row_id = " + Integer.toString(contRowID);

				// change the name in the following interval
				DBConnection.getInstance().oracleQueryVoid(query);

				query =
					"SELECT row_id, date_from, date_to " +
					"FROM " + tableName + " " +
					"WHERE " + tableName + "_id = " + objectID + "  AND " +
						"date_from = " + toSqlDate(oldToDate);

				ors = DBConnection.getInstance().oracleQueryResponse(query);
			} else {

				query =
					"SELECT * " +
					"FROM " + tableName + " a, uploaded_photo b " +
					"WHERE a.row_id = " + contRowID + " AND " +
						"b.uploaded_photo_id = " + Integer.toString(photoID) + " AND " +
						"ORDSYS.IMGSimilar(a.photo_signature, b.photo_signature, " +
							"'color=\"0.3\" texture=\"0.3\" shape=\"0.3\" location=\"0.1\"', " +
							" 100, 123) < 100 AND " +
						"ORDSYS.IMGScore(123) = 0";

					ors = DBConnection.getInstance().oracleQueryResponse(query);
				if (ors.next()) {
					mergeIfEquals(tableName, id, contFromDate);
					break;
				}
			}
		}
	}

	/**
	 * @brief  A method that checks if the intervals that meets at the given
	 *         date are the same and in a possitive case merges them together
	 *
	 * A method that checks if the intervals that meets at the given
	 * date are the same and in a possitive case merges them together
	 *
	 * @param[in]  tableName  Table name
	 * @param[in]  id         The ID of the pavilion
	 * @param[in]  date       The date at which the intervals should meet
	 */
	private static void mergeIfEquals(String tableName, int id, Date date)
		throws SQLException
	{
		String query =
			"SELECT a.row_id, b.row_id, b.date_to " +
			"FROM " + tableName + " a, " + tableName + " b " +
			"WHERE " +
				"a." + tableName + "_id = " + Integer.toString(id) + " AND " +
				"a.date_to = " + toSqlDate(date) + " AND " +
				"b." + tableName + "_id = " + Integer.toString(id) + " AND " +
				"b.date_from = " + toSqlDate(date) + " AND " +
				"ORDSYS.IMGSimilar(a.photo_signature, b.photo_signature, " +
					"'color=\"0.3\" texture=\"0.3\" shape=\"0.3\" location=\"0.1\"', " +
					" 100, 123) < 100 AND " +
				"ORDSYS.IMGScore(123) = 0";

		if (tableName.equals(Objects.PAVILION_TABLE_NAME)) {
			query += " AND a.name = b.name";
		} else if (tableName.equals(Objects.EXHIBIT_TABLE_NAME)) {
			// nothing
		} else if (tableName.equals(Objects.PATH_TABLE_NAME)) {
			// nothing
		} else if (tableName.equals(Objects.ENTRANCE_TABLE_NAME)) {
			// nothing
		} else if (tableName.equals(Objects.REFRESHMENT_TABLE_NAME)) {
			query += " AND a.description = b.description";
		}

		// are there such intervals?
		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		if (ors.next()) {
			int formerRowID = ors.getInt(1);
			int laterRowID = ors.getInt(2);
			Date laterDateTo = ors.getDate(3);

			// first, stretch the end of the former interval to the end of the
			// later one
			query =
				"UPDATE " + tableName + " " +
				"SET date_to = " + toSqlDate(laterDateTo) + " " +
				"WHERE row_id = " + Integer.toString(formerRowID);

			DBConnection.getInstance().oracleQueryVoid(query);

			// second, delete the later interval
			query =
				"DELETE FROM " + tableName + " " +
				"WHERE row_id = " + Integer.toString(laterRowID);

			DBConnection.getInstance().oracleQueryVoid(query);
		}
	}

	/**
	 * @brief Adds the selected animal to the selected object.
	 *
	 * @param[in] animal Animal to be added.
	 * @param[in] objectTableName Name of the table to which the animal will be added.
	 * @param[in] objectId ID of the object to which the animal will be added.
	 * @param[in] from Begining of the interval for which the animal should be added.
	 * @param[in] to End of the interval for which the animal should be added.
	 */
	public static void addAnimalToObject(AnimalData animal,
			String objectTableName, int objectId, Date from, Date to)
	throws SQLException
	{
		String tableName = "animal_" + objectTableName + "_rel";
		String objectIdName = objectTableName + "_id";
		String fromDateString = toSqlDate(from);
		String toDateString = toSqlDate(to);
		String query;
		OracleResultSet ors;

		// Delete information about the animal being in the object during the interval
		query = "DELETE FROM " + tableName + " WHERE " + objectIdName + " = " +
						objectId + " AND animal_id = " + animal.id + " AND date_from >= " +
						fromDateString + " AND date_to <= " + toDateString;
		DBConnection.getInstance().oracleQueryVoid(query);

		// Search if the animals has been in the object before given date
		query = "SELECT row_id, date_from FROM " + tableName +
						" WHERE " + objectIdName + " = " + objectId +
						" AND animal_id = " + animal.id + " AND " +
						" date_from < " + fromDateString + " AND date_to >= " +
						fromDateString;

		ors = DBConnection.getInstance().oracleQueryResponse(query);
		if (ors.next()) {
			// if the first row of the result does exist
			fromDateString = toSqlDate(ors.getDate(2)); // set new start date for row to be inserted
			int rowId = ors.getInt(1);

			// Delete existing row
			query = "DELETE FROM " + tableName + " WHERE row_id = " + rowId;
			DBConnection.getInstance().oracleQueryVoid(query);
		}

		// Search if the animals has been in the object after given date
		query = "SELECT row_id, date_to FROM " + tableName +
						" WHERE " + objectIdName + " = " + objectId +
						" AND animal_id = " + animal.id + " AND " +
						" date_from <= " + toDateString + " AND date_to > " +
						toDateString;

		ors = DBConnection.getInstance().oracleQueryResponse(query);
		if (ors.next()) {
			// if the first row of the result does exist
			toDateString = toSqlDate(ors.getDate(2)); // set new start date for row to be inserted
			int rowId = ors.getInt(1);

			// Delete existing row
			query = "DELETE FROM " + tableName + " WHERE row_id = " + rowId;
			DBConnection.getInstance().oracleQueryVoid(query);
		}

		query = "INSERT INTO " + tableName + " ( " +
						"	row_id, " + objectIdName + ", animal_id, date_from, date_to" +
						") VALUES (" +
						"seq_animal_" + objectTableName + "_rel_row_id.NEXTVAL, " +
						objectId + ", " + animal.id + ", " + fromDateString + ", " +
						toDateString + ")";
		DBConnection.getInstance().oracleQueryVoid(query);
	}

	/**
	 * @brief Removes the selected animal from the selected object.
	 *
	 * @param[in] animalId ID of the animal to be removed.
	 * @param[in] objectTableName Name of the table from which the animal will be removed.
	 * @param[in] objectId ID of the object from which the animal will be removed.
	 * @param[in] from Begining of the interval for which the animal should be removed.
	 * @param[in] to End of the interval for which the animal should be removed.
	 */
	public static void removeAnimalFromObject(int animalId,
			String objectTableName, int objectId, Date from, Date to)
	throws SQLException
	{
		String tableName = "animal_" + objectTableName + "_rel";
		String objectIdName = objectTableName + "_id";
		String fromDateString = toSqlDate(from);
		String toDateString = toSqlDate(to);
		String query;
		OracleResultSet ors;

		// Delete rows where the animal is present inside the interval
		query = "DELETE FROM " + tableName + " WHERE animal_id = " + animalId +
						" AND " + objectIdName + " = " + objectId + " AND date_from >= " +
						fromDateString + " AND date_to <= " + toDateString;
		DBConnection.getInstance().oracleQueryVoid(query);

		// Alter rows which starts before the interval and ends inside the interval
		query = "UPDATE " + tableName + " SET date_to = " + fromDateString +
						" WHERE animal_id = " + animalId + " AND " + objectIdName + " = " +
						objectId + " AND date_from < " + fromDateString +
						" AND date_to > " + fromDateString + " AND date_to <= " +
						toDateString;
		DBConnection.getInstance().oracleQueryVoid(query);

		// Alter rows which starts inside the interval and ends after it
		query = "UPDATE " + tableName + " SET date_from = " + toDateString +
						" WHERE animal_id = " + animalId + " AND " + objectIdName + " = " +
						objectId + " AND date_from < " + toDateString + " AND date_from >=" +
						fromDateString + " AND date_to > " + toDateString;
		DBConnection.getInstance().oracleQueryVoid(query);

		// Split row where the given interval is inside the interval when the
		// animal is present in the object
		String whereCondition = " WHERE animal_id = " + animalId + " AND " +
						objectIdName + " = " + objectId + " AND date_from < " +
						fromDateString + " AND date_to > " + toDateString;
		query = "SELECT row_id, date_to FROM " + tableName +
						whereCondition;

		ors = DBConnection.getInstance().oracleQueryResponse(query);
		if (ors.next()) {
			// if the first row of the result does exist
			int rowId = ors.getInt(1);
			String origToDateString = toSqlDate(ors.getDate(2));

			// Alter existing row
			query = "UPDATE " + tableName + " SET date_to = " + fromDateString +
							" WHERE row_id = " + rowId;
			DBConnection.getInstance().oracleQueryVoid(query);

			// Insert new row to the end
			query = "INSERT INTO " + tableName + " ( " +
							"	row_id, " + objectIdName + ", animal_id, date_from, date_to" +
							") VALUES (" +
							"seq_animal_" + objectTableName + "_rel_row_id.NEXTVAL, " +
							objectId + ", " + animalId + ", " + toDateString + ", " +
							origToDateString + ")";
			DBConnection.getInstance().oracleQueryVoid(query);
		}
	}

	/**
	 * @brief  Method checking that pavilion or exhibit does not contain an
	 *         animal
	 *
	 * This methods checks whether pavilion or exhibit does not contain an
	 * animal
	 *
	 * @param[in]  idObject   ID of pavilion or exhibit
	 * @param[in]  tableName  The name of the target table
	 * @param[in]  date       The date in which the method asks
	 *
	 * @return  Does the object contain an animal?
	 */
	public static boolean objectContainsAnimals(int idObject, String tableName, Date date)
		throws SQLException
	{
		String query =
			"SELECT * " +
			"FROM animal_" + tableName + "_rel " +
			"WHERE "  + tableName + "_id = " + Integer.toString(idObject) + " AND " +
				"date_to > " + toSqlDate(date);

		// return if the response contains one or more rows
		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		return ors.next();
	}

	/**
	 * Returns date when the selected animal will be removed from given pavilion
	 * or exhibition after the given date. If the animal is not present in the
	 * object at the given date it returns given date.
	 *
	 * @param[in]  animalId         Id of the selected animal.
	 * @param[in]  objectTableName  Name of the table.
	 * @param[in]  objectId         Id of the object in the table.
	 * @param[in]  afterDate        Date after which we are searching.
	 * @return Date when the animal is removed from the given object.
	 */
	public static Date whenAnimalIsRemovedFromObject(int animalId,
					String objectTableName, int objectId, Date afterDate) throws SQLException
	{
		String tableName = "animal_" + objectTableName + "_rel";
		String objectIdName = objectTableName + "_id";
		String afterDateString = toSqlDate(afterDate);
		String query;
		OracleResultSet ors;

		// Search for the row of the animal in given time
		query = "SELECT date_to FROM " + tableName +
						" WHERE " + objectIdName + " = " + objectId +
						" AND animal_id = " + animalId + " AND " +
						" date_from <= " + afterDateString + " AND date_to > " +
						afterDateString;

		ors = DBConnection.getInstance().oracleQueryResponse(query);
		if (ors.next()) {
			// if the first row of the result does exist
			return ors.getDate(1);
		}
		else {
			// No result found
			return afterDate;
		}
	}

	/**
	 * @brief  A method that returns the length of all paths in the Zoo in given
	 *         time point
	 *
	 * @param[in]  date  The date in which we want to count the lengths
	 * @return  The length of all paths
	 */
	public static double getLengthOfAllPaths(Date date) throws SQLException
	{
		String query =
			"SELECT SUM(" +
				"SDO_GEOM.SDO_LENGTH(location, " +
					"(SELECT diminfo " +
					"FROM user_sdo_geom_metadata " +
					"WHERE table_name = 'PATH'))) " +
			"FROM path " +
			"WHERE " + toTemporalAtDate(date);

		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		ors.next();
		return ors.getDouble(1);
	}

	/**
	 * @brief  A method that returns the area of all expositions in the Zoo at
	 *         given time point
	 *
	 * @param[in]  date  The date in which we want to count area
	 * @return  The area of all expositions
	 */
	public static double getAreaOfAllExpositions(Date date) throws SQLException
	{
		String tolerance = "0.005";

		String query =
			"SELECT SDO_GEOM.SDO_AREA(" +
				"SDO_GEOM.SDO_UNION(" +
					"SDO_AGGR_UNION(" +
						"SDOAGGRTYPE(ex.location, " +
							"(SELECT dim.sdo_tolerance " +
							"FROM user_sdo_geom_metadata usgm, " +
								"TABLE (usgm.diminfo) dim " +
							"WHERE table_name = 'EXHIBIT' AND " +
								"ROWNUM = 1))), " +
					"SDO_AGGR_UNION(" +
						"SDOAGGRTYPE(pa.location, " +
							"(SELECT dim.sdo_tolerance " +
							"FROM user_sdo_geom_metadata usgm, " +
								"TABLE (usgm.diminfo) dim " +
							"WHERE table_name = 'PAVILION' AND " +
								"ROWNUM = 1))), " +
					tolerance + "), " +
				tolerance + ") " +
			"FROM exhibit ex, pavilion pa " +
			"WHERE " + toTemporalAtDate(date, "ex") + " AND "
		 	+ toTemporalAtDate(date, "pa");

		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		ors.next();
		return ors.getDouble(1);
	}

	/**
	 * @brief  A method that returns the area of the complex hull of the Zoo at
	 *         given time point
	 *
	 * @param[in]  date  The date in which we want to measure the area
	 * @return  The area of the convex hull of the Zoo
	 */
	public static double getZooArea(Date date) throws SQLException
	{
		String tolerance = "0.005";

		String query =
			"SELECT SDO_GEOM.SDO_AREA(" +
				"SDO_GEOM.SDO_CONVEXHULL(" +
					"SDO_GEOM.SDO_UNION(" +
						"SDO_GEOM.SDO_UNION(" +
							"SDO_GEOM.SDO_UNION(" +
								"SDO_GEOM.SDO_UNION(" +
									"SDO_AGGR_UNION(" +
										"SDOAGGRTYPE(ex.location, " +
											"(SELECT dim.sdo_tolerance " +
											"FROM user_sdo_geom_metadata usgm, " +
												"TABLE (usgm.diminfo) dim " +
											"WHERE table_name = 'EXHIBIT' AND " +
												"ROWNUM = 1))), " +
									"SDO_AGGR_UNION(" +
										"SDOAGGRTYPE(pa.location, " +
											"(SELECT dim.sdo_tolerance " +
											"FROM user_sdo_geom_metadata usgm, " +
												"TABLE (usgm.diminfo) dim " +
											"WHERE table_name = 'PAVILION' AND " +
												"ROWNUM = 1))), " +
									tolerance + "), " +
								"SDO_AGGR_UNION(" +
									"SDOAGGRTYPE(re.location, " +
										"(SELECT dim.sdo_tolerance " +
										"FROM user_sdo_geom_metadata usgm, " +
											"TABLE (usgm.diminfo) dim " +
										"WHERE table_name = 'REFRESHMENT' AND " +
											"ROWNUM = 1))), " +
								tolerance + "), " +
							"SDO_AGGR_UNION(" +
								"SDOAGGRTYPE(pt.location, " +
									"(SELECT dim.sdo_tolerance " +
									"FROM user_sdo_geom_metadata usgm, " +
										"TABLE (usgm.diminfo) dim " +
									"WHERE table_name = 'PATH' AND " +
										"ROWNUM = 1))), " +
							tolerance + "), " +
						"SDO_AGGR_UNION(" +
							"SDOAGGRTYPE(en.location, " +
								"(SELECT dim.sdo_tolerance " +
								"FROM user_sdo_geom_metadata usgm, " +
									"TABLE (usgm.diminfo) dim " +
								"WHERE table_name = 'ENTRANCE' AND " +
									"ROWNUM = 1))), " +
						tolerance + "), " +
					tolerance + "), " +
				tolerance + ") " +
			"FROM exhibit ex, pavilion pa, refreshment re, path pt, entrance en " +
			"WHERE " +
				toTemporalAtDate(date, "ex") + " AND " +
				toTemporalAtDate(date, "pa") + " AND " +
				toTemporalAtDate(date, "re") + " AND " +
				toTemporalAtDate(date, "pt") + " AND " +
				toTemporalAtDate(date, "en");

		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		ors.next();
		return ors.getDouble(1);
	}

	/**
	 * @brief  A method that counts all animals' genera in the Zoo at given time
	 *
	 * @param[in]  date  The date in which we want to be counting
	 * @return  The count of all animals' genera
	 */
	public static int getCountOfGenera(Date date) throws SQLException
	{
		String query =
			"SELECT COUNT(*) " +
			"FROM (" +
				"SELECT UNIQUE genus.genus_id " +
				"FROM animal_pavilion_rel JOIN " +
					"animal ON animal_pavilion_rel.animal_id = animal.animal_id JOIN " +
					"genus ON animal.genus_id = genus.genus_id " +
				"WHERE " + toTemporalAtDate(date, "animal_pavilion_rel") + " " +
				"UNION " +
				"SELECT UNIQUE genus.genus_id " +
				"FROM animal_exhibit_rel JOIN " +
					"animal ON animal_exhibit_rel.animal_id = animal.animal_id JOIN " +
					"genus ON animal.genus_id = genus.genus_id " +
				"WHERE " + toTemporalAtDate(date, "animal_exhibit_rel") +
				")";

		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		ors.next();
		return ors.getInt(1);
	}

	/**
	 * @brief  A method that gets the name of the pavilion with the most animals
	 *         at given time
	 *
	 * @param[in]  date  The date in which we want to be counting
	 * @return  The name of the pavilion with the most animals (the first one)
	 */
	public static String getPavilionWithMostAnimals(Date date) throws SQLException
	{
		String query =
			"SELECT name " +
			"FROM pavilion " +
			"WHERE pavilion_id = " +
				"(SELECT pavilion_id " +
				"FROM (" +
					"SELECT pavilion_id, COUNT(*) AS num " +
					"FROM animal_pavilion_rel " +
					"WHERE " + toTemporalAtDate(date) + " " +
					"GROUP BY pavilion_id " +
					"ORDER BY num DESC) " +
				"WHERE ROWNUM  = 1)";

		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		ors.next();
		return ors.getString(1);
	}

	/**
	 * @brief  A method that gets the count of genera in a pavilion with given ID
	 *         at given time
	 *
	 * @param[in]  id    The ID of the pavilion
	 * @param[in]  date  The date in which we want to be counting
	 * @return  The count of genera in the pavilion
	 */
	public static int getCountOfGeneraInPavilion(int id, Date date) throws SQLException
	{
		String query =
			"SELECT COUNT(DISTINCT gn) " +
			"FROM " +
				"(SELECT animal.genus_id AS gn " +
				"FROM animal_pavilion_rel JOIN " +
					"animal ON animal_pavilion_rel.animal_id = animal.animal_id " +
				"WHERE animal_pavilion_rel.pavilion_id = " + Integer.toString(id) + " AND " +
					toTemporalAtDate(date, "animal_pavilion_rel") + ")";

		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		ors.next();
		return ors.getInt(1);
	}

	/**
	 * @brief  A method that gets the count of classes in a pavilion with given ID
	 *         at given time
	 *
	 * @param[in]  id    The ID of the pavilion
	 * @param[in]  date  The date in which we want to be counting
	 * @return  The count of classes in the pavilion
	 */
	public static int getCountOfClassesInPavilion(int id, Date date) throws SQLException
	{
		String query =
			"SELECT COUNT(DISTINCT cs) " +
			"FROM " +
				"(SELECT genus.class_id AS cs " +
				"FROM animal_pavilion_rel JOIN " +
					"animal ON animal_pavilion_rel.animal_id = animal.animal_id JOIN " +
					"genus ON animal.genus_id = genus.genus_id " +
				"WHERE animal_pavilion_rel.pavilion_id = " + Integer.toString(id) + " AND " +
					toTemporalAtDate(date, "animal_pavilion_rel") + ")";

		OracleResultSet ors = DBConnection.getInstance().oracleQueryResponse(query);
		ors.next();
		return ors.getInt(1);
	}
}
