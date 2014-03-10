/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Data for the relation between animals and objects that contain them.
 *
 */

package zoo;

import java.util.Date;

/**
 * @brief Data for the relation between animals and objects that contain them.
 */
public class AnimalObjectRelData {

	/// ID of a relation which does not have any ID.
	public static final int NO_RELATION_ID = -1;

	/// ID of an animal which does not have any ID.
	public static final int NO_ANIMAL_ID = -1;

	/// ID of an object which does not have any ID.
	public static final int NO_OBJECT_ID = -1;

	/// Relation ID.
	public int relationId = NO_RELATION_ID;

	/// Animal ID.
	public int animalId = NO_ANIMAL_ID;

	/// Object containing this animal ID.
	public int objectId = NO_OBJECT_ID;

	/// Object (that contain this animal) table name.
	public String objectTableName = "";

	/// Relation validity start time.
	public Date validFrom = null;

	/// Relation validity end time.
	public Date validTo = null;
}
