/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.util;

import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * @brief MD5 hashing.
 */
public class MD5 {

	/**
	 * @brief Returns MD5 hash for the selected string.
	 */
	public static String hash(String s) {
		try {
			MessageDigest m = MessageDigest.getInstance("MD5");
			byte[] data = s.getBytes();
			m.update(data, 0, data.length);
			BigInteger i = new BigInteger(1, m.digest());
			return String.format("%1$032X", i);
		} catch (NoSuchAlgorithmException e) {
			// OK, lets be nasty and suppose that this will never happen...
			// I know, I'm a very bad boy...
		}
		return ""; // This should never happen (see ^^^)
	}
}
