/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.controller;

/**
 * @brief Searching facilites.
 */
public class Search {
	private static final String ACTION_SEARCH = "search";

	private String searchString = "";
	private String searchStringBackup = "";

	/// searchString getter
	public String getSearchString() {
		return searchString;
	}

	/// searchString setter
	public void setSearchString(String newSearchString) {
		searchString = newSearchString;
	}

	/// searchStringBackup getter
	public String getSearchStringBackup() {
		return searchStringBackup;
	}

	/// searchStringBackup setter
	public void setSearchStringBackup(String newSearchStringBackup) {
		searchStringBackup = newSearchStringBackup;
	}

	/**
	 * @brief Does a search according to the current search string.
	 * @return Action to be performed.
	 */
	public String doSearch() {
		searchStringBackup = searchString;
		searchString = "";
		return ACTION_SEARCH;
	}
}
