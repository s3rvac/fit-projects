/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Various conversions for both general and special purpose.
 *
 */

package zoo;

import java.awt.image.BufferedImage;
import java.io.InputStream;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import javax.imageio.ImageIO;
import oracle.ord.im.OrdImage;

/**
 * @brief Various conversions for both general and special purpose.
 *
 * Implements the "library" (aka "static helper") design pattern.
 */
public class Conversions {

	/// Pattern for the date format.
	public static final String DATE_FORMAT_PATTERN = "yyyy-MM-dd";

	/**
	 * @brief Converts the selected image in the oracle OrdImage format to BufferedImage.
	 *
	 * @param[in] ordImage Image in the oracle OrdImage format.
	 * @return Buffered image if the conversion went ok, null otherwise.
	 */
	public static BufferedImage ordImageToBufferedImage(OrdImage ordImage) {
		try {
			InputStream imageStream = ordImage.getDataInStream();
			return ImageIO.read(imageStream);
		} catch (Exception ex) {
			// There is no image or it cannot be read
			return null;
		}
	}

	/**
	 * Converts the selected date to string which can be shown to the user.
	 *
	 * @param date Date to be converted.
	 * @return Date represented by string.
	 */
	public static String dateToString(Date date) {
		if (date.equals(TemporalQuery.FOREVER_DATE)) {
			return TemporalQuery.FOREVER_DATE_TEXT_REPR;
		} else {
			return date.toString();
		}
	}

	/**
	 * @brief Converts selected string into date.
	 *
	 * @param date Date represented by string.
	 * @return Transformed string into date. Null if there was a conversion
	 *         problem (like invalid date format).
	 */
	public static Date stringToDate(String date) {
		if (date.equals(TemporalQuery.FOREVER_DATE_TEXT_REPR)) {
			return (Date) TemporalQuery.FOREVER_DATE.clone();
		} else {
			try {
				SimpleDateFormat sdf = new SimpleDateFormat();
				sdf.applyPattern(DATE_FORMAT_PATTERN);
				return sdf.parse(date);
			} catch (ParseException ex) {
				return null;
			}
		}
	}

	// Deny creation of instances
	private Conversions() {}
}
