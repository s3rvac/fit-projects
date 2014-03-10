/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Operations with photos.
 *
 */

package zoo;

import java.io.IOException;
import java.sql.SQLException;
import java.util.Hashtable;
import java.util.logging.Level;
import java.util.logging.Logger;
import oracle.jdbc.OraclePreparedStatement;
import oracle.jdbc.OracleResultSet;
import oracle.ord.im.OrdImage;
import oracle.ord.im.OrdImageSignature;

/**
 * @brief Operations with photos.
 *
 * This class implements the "singleton" design pattern.
 */
public class PhotoOperations {

	/// Table name -> primary key mapping.
	private static Hashtable _primaryKeys = new Hashtable();

	static {
		// insert information: table name -> primary key
		_primaryKeys.put("pavilion", "row_id");
		_primaryKeys.put("exhibit", "row_id");
		_primaryKeys.put("refreshment", "row_id");
		_primaryKeys.put("animal", "animal_id");
		_primaryKeys.put("uploaded_photo", "uploaded_photo_id");
	}

	/// Uploaded photo attributes.
	private static final String PHOTO_ATTRIBUTES = "maxScale=300 300";

	// Name of the table with temporary photos.
	public static final String TEMPORARY_UPLOADED_PHOTO_TABLE_NAME = "uploaded_photo";

	// Photo ID that means that no photo was uploaded.
	public static final int NO_UPLOADED_PHOTO_ID = -1;

	/**
	 * @brief Uploads selected photo to the table with temporary pohotos
	 *        and returns it's ID.
	 *
	 * @param photoFile Select file with the photo to be uploaded.
	 * @return ID (primary key) of the uploaded photo.
	 */
	public static int uploadTemporaryPhotoToDb(String photoFile)
			throws SQLException {
		// Insert a new (empty) row to the uploaded photos table
		String query = "INSERT INTO " + TEMPORARY_UPLOADED_PHOTO_TABLE_NAME + " " +
					"(uploaded_photo_id, photo, photo_signature) " +
				"VALUES (seq_uploaded_photo_id.NEXTVAL, NULL, NULL)";
		int photoId = DBConnection.getInstance().oracleQueryInsert(query,
				"uploaded_photo_id");

		// Update this row with the photo
		uploadPhotoToDb(photoFile, TEMPORARY_UPLOADED_PHOTO_TABLE_NAME, photoId);

		return photoId;
	}

	/**
	 * @brief Uploads the selected image to the database.
	 *
	 * @param[in] photoFile Photo image to upload.
	 * @param[in] tableName Name of the table into which the image will be uploaded.
	 * @param[in] pkId Primary key value for the table.
	 * @return Uploaded image in the oracle class format if the upload went ok,
	 *         null otherwise.
	 */
	public static OrdImage uploadPhotoToDb(String photoFile, String tableName, int pkId)
			throws SQLException {
		try {
			Logger.getLogger("zoo").log(Level.INFO,
					"Uploading image: " + photoFile + " to " + tableName +
					" [primary key: " + pkId + "]\n");

			// Check whether there already is some photo in the selected row
			String query = "SELECT photo " +
					"FROM " + tableName + " " +
					"WHERE " + _primaryKeys.get(tableName) + " = " +
						Integer.toString(pkId);
			OracleResultSet rs = DBConnection.getInstance().oracleQueryResponse(query, false);
			rs.next();
			if (rs.getORAData("photo", OrdImage.getORADataFactory()) == null) {
				// There is no photo, so initialize the "photo" and "photo_signature"
				// column values so it will be possible to upload the photo correctly
				query = "UPDATE " + tableName + " " +
						"SET photo = ordsys.ordimage.init(), " +
							"photo_signature = ordsys.ordimagesignature.init() " +
						"WHERE " + _primaryKeys.get(tableName) + " = " + pkId;
				DBConnection.getInstance().oracleQueryResponse(query);
			}
			rs.close();

			// The "Select For Update" statement allows to lock the records in
			// the cursor result set (since we will be uploading data into the
			// ORDImage's underlying BLOB)
			query = "SELECT photo, photo_signature " +
					"FROM " + tableName + " " +
					"WHERE " + _primaryKeys.get(tableName) + " = " + pkId + " " +
					"FOR UPDATE";
			rs = DBConnection.getInstance().oracleQueryResponse(query, false);
			rs.next();
			OrdImage imageProxy = (OrdImage) rs.getORAData("photo",
					OrdImage.getORADataFactory());
			OrdImageSignature signatureProxy = (OrdImageSignature) rs.getORAData(
					"photo_signature", OrdImageSignature.getORADataFactory());
			rs.close();

			// Load the new ORDImage into a java proxy OrdImage object
			imageProxy.loadDataFromFile(photoFile);

			// Process the photo (compression etc.)
			imageProxy.process(PHOTO_ATTRIBUTES);

			// Automatically detect the image's height, width, file format etc.
			// Note: the proxy object forwards the request to the database
			// to execute ORDImage.setProperties() on the server
			imageProxy.setProperties();

			// Generate signature for the photo (it will contain specific image
			// information for image comparison)
			signatureProxy.generateSignature(imageProxy);

			// Update the selected row in the db
			query = "UPDATE " + tableName + " " +
					"SET photo=?, photo_signature=? " +
					"WHERE " + _primaryKeys.get(tableName) + " = " + pkId;
			Logger.getLogger("zoo").log(Level.INFO, query);
			OraclePreparedStatement pstmt = DBConnection.getInstance(
					).oraclePrepareStatement(query);
			pstmt.setORAData(1, imageProxy);
			pstmt.setORAData(2, signatureProxy);
			DBConnection.getInstance().oracleExecutePreparedStatement(pstmt, true);

			return imageProxy;
		} catch (IOException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		}

		return null;
	}
}
