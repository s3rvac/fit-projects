/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.controller;

import javax.faces.context.FacesContext;

/**
 * @brief Information about the current page.
 */
public class CurrentPage {
	/**
	 * @brief Returns the instance of this class for the current session.
	 */
	public static CurrentPage getInstance() {
		FacesContext context = FacesContext.getCurrentInstance();
		return (CurrentPage) context.getApplication().evaluateExpressionGet(context,
			"#{currentPage}", CurrentPage.class);
	}

	/**
	 * @return true if the user is on the index page, false otherwise.
	 */
	public boolean isOnIndexPage() {
		return getCurrentPageName().equals("index");
	}

	/**
	 * @return true if the user is on the login page, false otherwise.
	 */
	public boolean isOnLoginPage() {
		return getCurrentPageName().equals("login");
	}

	/**
	 * @return true if the user is on some page where every one has access,
	 *         false otherwise.
	 */
	public boolean isOnPageWithCommonAccess() {
		return isOnIndexPage() || isOnFairsPage() ||
			isOnServicesPage() || isOnContactPage() ||
			isOnLoginPage() || isOnSearchPage();
	}

	/**
	 * @return true if the user is on the search page, false otherwise.
	 */
	public boolean isOnSearchPage() {
		return getCurrentPageName().equals("search");
	}

	/**
	 * @return true if the user is on the fairs page, false otherwise.
	 */
	public boolean isOnFairsPage() {
		return getCurrentPageName().equals("fairs");
	}

	/**
	 * @return true if the user is on the services page, false otherwise.
	 */
	public boolean isOnServicesPage() {
		return getCurrentPageName().equals("services");
	}

	/**
	 * @return true if the user is on the contact page, false otherwise.
	 */
	public boolean isOnContactPage() {
		return getCurrentPageName().equals("contact");
	}

	/**
	 * @return true if the user is on some page related to exhibitions
	 *         (list/view/add/edit), false otherwise.
	 */
	public boolean isOnExhibitionsPage() {
		return getCurrentPageName().startsWith("exhibition");
	}

	/**
	 * @return true if the user is on some page related to pavilions
	 *         (list/view/add/edit), false otherwise.
	 */
	public boolean isOnPavilionsPage() {
		return getCurrentPageName().startsWith("pavilion");
	}

	/**
	 * @return true if the user is on some page related to expositions
	 *         (list/view/add/edit), false otherwise.
	 */
	public boolean isOnExpositionsPage() {
		return getCurrentPageName().startsWith("exposition");
	}

	/**
	 * @return true if the user is on some page related to companies
	 *         (list/view/add/edit), false otherwise.
	 */
	public boolean isOnCompaniesPage() {
		return getCurrentPageName().startsWith("company");
	}

	/**
	 * @return true if the user is on some page related to employees
	 *         (list/view/add/edit), false otherwise.
	 */
	public boolean isOnEmployeesPage() {
		return getCurrentPageName().startsWith("employee");
	}

	/**
	 * @return true if the user is on the new contract page, false otherwise.
	 */
	public boolean isOnNewContractPage() {
		return getCurrentPageName().equals("contractAdd");
	}

	/**
	 * @return true if the user is on some page related to contracts
	 *         other than new contract (list/view/edit), false otherwise.
	 */
	public boolean isOnContractsPage() {
		String currentPageName = getCurrentPageName();
		return currentPageName.startsWith("contract") &&
			!currentPageName.equals("contractAdd");
	}

	/**
	 * @return true if the user is on some page related to fields
	 *         (list/view/add/edit), false otherwise.
	 */
	public boolean isOnFieldsPage() {
		return getCurrentPageName().startsWith("field");
	}

	/**
	 * @return true if the user is on the economics page, false otherwise.
	 */
	public boolean isOnEconomicsPage() {
		return getCurrentPageName().equals("economics");
	}

	/**
	 * @return true if the user is on the some edit page, false otherwise.
	 */
	public boolean isOnEditPage() {
		return getCurrentPageName().contains("Edit");
	}

	/**
	 * @return Current page name (without the initial '/' and extension).
	 */
	public String getCurrentPageName() {
		// Obtain the current page name (including '/' and extension)
		FacesContext fc = FacesContext.getCurrentInstance();
		String pageName = fc.getViewRoot().getViewId();

		// Strip the initial '/'
		pageName = pageName.substring(1, pageName.length());

		// Strip the extension
		return pageName.substring(0, pageName.lastIndexOf('.'));
	}
}
