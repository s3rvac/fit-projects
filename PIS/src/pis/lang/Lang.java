/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.lang;

import java.util.Locale;
import java.util.MissingResourceException;
import java.util.ResourceBundle;
import javax.faces.context.FacesContext;

/**
 * @brief Language settings.
 */
public class Lang {
	private static final String DEFAULT_LANGUAGE = "cs";
	private String currLang = DEFAULT_LANGUAGE;

	static {
		FacesContext.getCurrentInstance().getViewRoot().setLocale(
			new Locale(DEFAULT_LANGUAGE));
	}

	/// currLang getter
	public String getCurrLang() {
		return currLang;
	}

	/// currLang setter
	public void setCurrLang(String newLang) {
		FacesContext.getCurrentInstance().getViewRoot().setLocale(
			new Locale(newLang));
		currLang = newLang;
	}

	/**
	 * @brief Returns localized string of the selected name.
	 *
	 * Returns name if the selected name does not exist.
	 */
	public static String getLocalizedString(String name) {
		ResourceBundle bundle = null;
		String bundleName = "pis.lang.messages";
		String localizedString = name;
		Locale locale = FacesContext.getCurrentInstance().getViewRoot().getLocale();
		try {
			bundle = ResourceBundle.getBundle(bundleName, locale, getCurrentLoader(bundleName));
		} catch (MissingResourceException e) {
			// Bundle with this name not found;
		}
		if (bundle == null) {
			return null;
		}
		try {
			localizedString = bundle.getString(name);
		} catch (Exception e) {
		}
		return localizedString;
	}

	/**
	 * @brief Returns current class loader.
	 */
	private static ClassLoader getCurrentLoader(Object fallbackClass) {
		ClassLoader loader = Thread.currentThread().getContextClassLoader();
		if (loader == null) {
			loader = fallbackClass.getClass().getClassLoader();
		}
		return loader;
	}
}
