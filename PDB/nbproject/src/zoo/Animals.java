/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Information about animals from a db.
 *
 */

package zoo;

import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import oracle.jdbc.OracleResultSet;
import oracle.ord.im.OrdImage;

/**
 * @brief Information about animals from a db.
 *
 * Implements the "library" (aka "static helper") design pattern.
 */
public class Animals {

	// Animals table name.
	public static final String ANIMALS_TABLE_NAME = "animal";

	/**
	 * @brief Returns data of the selected animal.
	 *
	 * @param[in] animalId Animal ID.
	 * @return Animal data.
	 *
	 * If there is no animal with the selected ID, it returns null.
	 */
	public static AnimalData getAnimalData(int animalId) {
		try {
			AnimalData animalData = new AnimalData();

			OracleResultSet rs = DBConnection.getInstance().oracleQueryResponse(
					"SELECT * " +
					"FROM " + ANIMALS_TABLE_NAME + " " +
					"WHERE animal_id = " + animalId);
			if (rs.next()) {
				// Get animal ID
				animalData.id = rs.getInt("animal_id");

				// Get animal species
				try {
					animalData.species = rs.getString("species");
				} catch (SQLException ex) {
					animalData.species = "";
				}

				// Get animal description
				try {
					animalData.description = rs.getString("description");
				} catch (SQLException ex) {
					animalData.description = "";
				}

				// Get animal photo
				try {
					animalData.photo = Conversions.ordImageToBufferedImage(
						(OrdImage) rs.getORAData("photo", OrdImage.getORADataFactory()));
				} catch (SQLException ex) {
					animalData.photo = null;
				}
			}

			return animalData;
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		}

		return null;
	}

	// Deny creation of instances
	private Animals() {}
}
