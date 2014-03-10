/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Package containing all classes from our PDB 2008 project.
 *
 * ^ HACK: I'm sorry that this file description doesn't make sense but I was not
 * able to figure out how to document the zoo package using Doxygen.
 */

package zoo;

import java.awt.image.BufferedImage;

/**
 * @brief Data of an animal from a db.
 */
public class AnimalData {

	/// ID of an animal which does not have any ID.
	public static final int NO_ANIMAL_ID = -1;

	/// Animal ID.
	public int id = NO_ANIMAL_ID;

	/// Animal species.
	public String species = "";

	/// Animal description.
	public String description = "";

	/// Animal photo.
	BufferedImage photo = null;

	@Override
	public String toString() {
		return species;
	}

	@Override
	public int hashCode() {
		return species.hashCode();
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == null) {
			return false;
		}
		if (getClass() != obj.getClass()) {
			return false;
		}
		final AnimalData other = (AnimalData) obj;
		if (species != other.species && (species == null || !species.equals(other.species))) {
			return false;
		}
		return true;
	}
}
