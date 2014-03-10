/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Animal operations like getting all animals from the selected
 *        pavilion etc.
 *
 */

package zoo;

import java.sql.SQLException;
import java.util.Date;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import oracle.jdbc.OracleResultSet;

/**
 * @brief Animal operations like getting all animals from the selected
 *        pavilion etc.
 *
 * Implements the "library" (aka "static helper") design pattern.
 */
public class AnimalOperations {

	/**
	 * @brief Returns animals from the selected object.
	 *
	 * @param[in] objectTableName Table from where the animals are searched.
	 * @param[in] objectId ID of the object.
	 * @return Animals in the selected object.
	 *
	 * If there is an error, it returns null.
	 */
	public static AnimalObjectRelData[] getAnimalsFromObject(
			String objectTableName, int objectId) {
		try {
			Vector<AnimalObjectRelData> animals = new Vector();

			// Get all animal IDs which are in the selected object
			OracleResultSet rs = DBConnection.getInstance().oracleQueryResponse(
					"SELECT row_id, " + objectTableName + "_id, " +
						"animal_id, date_from, date_to " +
					"FROM animal_" + objectTableName + "_rel " +
					"WHERE " + objectTableName + "_id = " +
							Integer.toString(objectId) + " " +
						"AND " + TemporalQuery.toTemporalAtDate(
							Calendar.getInstance().getDate()));

			// Store animal IDs from the result set
			while (rs.next()) {
				AnimalObjectRelData relData = new AnimalObjectRelData();
				relData.relationId = rs.getInt(1);
				relData.objectId = rs.getInt(2);
				relData.animalId = rs.getInt(3);
				relData.objectTableName = objectTableName;
				relData.validFrom = rs.getDate(4);
				relData.validTo = rs.getDate(5);
				animals.add(relData);
			}

			// Convert the storage type and return it
			AnimalObjectRelData[] result = new AnimalObjectRelData[animals.size()];
			for (int i = 0; i < animals.size(); ++i) {
				result[i] = (AnimalObjectRelData) animals.get(i);
			}
			return result;
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			return null;
		}
	}

	/**
	 * @brief Returns all existing animals (or null if there is an error).
	 */
	public static AnimalData[] getAllAnimals() {
		try {
			Vector<Integer> animals = new Vector();

			// Get all animal IDs
			OracleResultSet rs = DBConnection.getInstance().oracleQueryResponse(
				"SELECT animal_id " +
				"FROM " + Animals.ANIMALS_TABLE_NAME);

			// Transform each animal ID into AnimalData
			while (rs.next()) {
				animals.add(rs.getInt(1));
			}

			// Get data of all animals, convert the storage type and return it
			AnimalData[] result = new AnimalData[animals.size()];
			for (int i = 0; i < animals.size(); ++i) {
				result[i] = Animals.getAnimalData(animals.get(i));
			}
			return result;
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			return null;
		}
	}

	/**
	 * @brief Adds the selected animal to the selected object.
	 *
	 * @param[in] animal Animal to be moved.
	 * @param[in] objectTableNameFrom Name of the table from which the animal is about to move.
	 * @param[in] objectIdFrom ID of the object from which the animal will be added.
	 * @param[in] objectTableNameTo Name of the table to which the animal is about to move.
	 * @param[in] objectIdTo ID of the object to which the animal will be added.
	 * @return True if the operation was successful, false otherwise
	 */
	public static void moveAnimalFromObjectToObject(AnimalData animal,
			String objectTableNameFrom, int objectIdFrom, String objectTableNameTo,
			int objectIdTo)
	throws SQLException
	{
		if (objectIdFrom == objectIdTo &&
						objectTableNameFrom.equals(objectTableNameTo)) {
			// No need to move the animal, already there
			return;
		}

		Date currentDate = Calendar.getInstance().getDate();
		Date animalInFromUntil = TemporalQuery.whenAnimalIsRemovedFromObject(
						animal.id, objectTableNameFrom, objectIdFrom, currentDate);
		TemporalQuery.addAnimalToObject(animal, objectTableNameTo, objectIdTo,
						currentDate, animalInFromUntil);
		TemporalQuery.removeAnimalFromObject(animal.id, objectTableNameFrom,
						objectIdFrom, currentDate, animalInFromUntil);
	}

	// Deny creation of instances
	private AnimalOperations() {}
}
