/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.controller;

import java.io.IOException;
import java.util.Collection;
import java.util.Date;
import javax.faces.context.FacesContext;
import javax.persistence.EntityManager;

import pis.controller.Db;
import pis.model.Company;
import pis.model.Employee;
import pis.lang.Lang;
import pis.util.MD5;

/**
 * @brief Authentication and authorization facilites.
 */
public class Auth {
	private static final String LOGIN_ACTION = "login";
	private static final String LOGOUT_ACTION = "logout";
	private static final String LOGIN_FAILED_ERROR = "loginFailed";
	private static final String LOGIN_SUCCESS_ACTION = "loginSuccess";
	private static final String LOGIN_FAILED_ACTION = "loginFailed";
	private static final String LOGOUT_SUCCESS_ACTION = "logoutSuccess";
	private static final String INACTIVITY_LOGOUT_ERROR = "inactivityLogout";
	private static final long MAX_MILISECS_OF_INACTIVITY = 600 * 1000;
	private static final String LOGIN_PAGE = "login.jsf";
	private static final String INDEX_PAGE = "index.jsf";

	EntityManager em;
	private boolean authFailed = false;
	private String login = "";
	private String password = "";
	private Company company = null;
	private Employee employee = null;
	private Date lastActionTS = new Date();
	private String inactivityLogoutMessage = "";

	public Auth() {
		em = Db.getEntityManager();
	}

	/// login getter
	public String getLogin() {
		return login;
	}

	/// login setter
	public void setLogin(String newLogin) {
		login = newLogin;
	}

	/// password getter
	public String getPassword() {
		return password;
	}

	/// password setter
	public void setPassword(String newPassword) {
		password = newPassword;
	}

	/// loginError getter
	public String getLoginError() {
		if (authFailed) {
			authFailed = false;
			return Lang.getLocalizedString(LOGIN_FAILED_ERROR);
		} else {
			return "";
		}
	}

	/// company getter
	public Company getCompany() {
		if (company != null) {
			em.refresh(company);
		}
		return company;
	}

	/// company setter
	private void setCompany(Company newCompany) {
		company = newCompany;
	}

	/// employee getter
	public Employee getEmployee() {
		if (employee != null) {
			em.refresh(employee);
		}
		return employee;
	}

	/// employee setter
	private void setEmployee(Employee newEmployee) {
		employee = newEmployee;
	}

	/// inactivityLogoutMessage getter
	public String getInactivityLogoutMessage() {
		// Uses 'modifiable getter' (anti-)pattern ;)
		String inactivityLogoutMsgBackup = inactivityLogoutMessage;
		if (CurrentPage.getInstance().isOnPageWithCommonAccess()) {
			inactivityLogoutMessage = "";
		}
		return inactivityLogoutMsgBackup;
	}

	/// inactivityLogoutMessage setter
	private void setInactivityLogoutMessage(String newInactivityLogoutMessage) {
		inactivityLogoutMessage = newInactivityLogoutMessage;
	}

	/**
	 * @brief Returns the instance of this class for the current session.
	 */
	public static Auth getInstance() {
		FacesContext context = FacesContext.getCurrentInstance();
		return (Auth) context.getApplication().evaluateExpressionGet(context,
			"#{auth}", Auth.class);
	}

	/**
	 * @brief Returns true if the user is logged, false otherwise.
	 */
	public boolean isLogged() {
		return isLoggedAsEmployee() || isLoggedAsCompany();
	}

	/**
	 * @brief Returns true if the user is logged as an employee,
	 *        false otherwise.
	 */
	public boolean isLoggedAsEmployee() {
		return employee != null;
	}

	/**
	 * @brief Returns true if the user is logged as a chief,
	 *        false otherwise.
	 */
	public boolean isLoggedAsChief() {
		return employee != null && employee.getChief();
	}

	/**
	 * @brief Returns true if the user is logged as a company member,
	 *        false otherwise.
	 */
	public boolean isLoggedAsCompany() {
		return company != null;
	}

	/**
	 * @brief Redirects the user to the login page if he is not logged.
	 * @return false if the user was logged, true otherwise.
	 */
	public boolean isRedirectedToLoginPageIfNotLogged() {
		if (!isLogged()) {
			try {
				FacesContext.getCurrentInstance().getExternalContext().redirect(
					LOGIN_PAGE);
			} catch (IOException e) {
				// This should never happen
			}
			return true;
		}
		return false;
	}

	/**
	 * @brief Redirects the user to the index page if he is logged.
	 * @return false if the user was not logged, true otherwise.
	 */
	public boolean isRedirectedToIndexPageIfLogged() {
		if (isLogged()) {
			try {
				FacesContext.getCurrentInstance().getExternalContext().redirect(
					INDEX_PAGE);
			} catch (IOException e) {
				// This should never happen
			}
			return true;
		}
		return false;
	}


	/**
	 * @brief Returns current auth action name (localized).
	 *
	 * Logouts user after certain ammount of inactivity.
	 */
	public String getAuthActionMessage() {
		// Logout user after certain time of inactivity?
		if (isLogged()) {
			if (((new Date()).getTime() - lastActionTS.getTime())
					> MAX_MILISECS_OF_INACTIVITY) {
				logout();
				inactivityLogoutMessage = Lang.getLocalizedString(
						INACTIVITY_LOGOUT_ERROR);
			}
		}

		lastActionTS = new Date();

		// Generate the auth message
		String actionName = LOGIN_ACTION;
		String suffix = "";
		if (employee != null) {
			actionName = LOGOUT_ACTION;
			suffix = " (" + employee.getLogin() + ")";
		} else if (company != null) {
			actionName = LOGOUT_ACTION;
			suffix = " (" + company.getLogin() + ")";
		}
		return Lang.getLocalizedString(actionName) + suffix;
	}

	/**
	 * @brief Returns the action name after an auth request.
	 */
	public String doAuthAction() {
		if (isLogged()) {
			logout();
			return LOGOUT_SUCCESS_ACTION;
		} else {
			// Login
			return LOGIN_ACTION;
		}
	}

	/**
	 * @brief Tries to login and returns action name.
	 */
	public String doLogin() {
		lastActionTS = new Date();

		if (userLoggedSuccessfully()) {
			return LOGIN_SUCCESS_ACTION;
		} else {
			return LOGIN_FAILED_ACTION;
		}
	}

	/**
	 * @brief Does a login attempt and if it is successfull,
	 *        it returns true and initializes employee/company,
	 *        and it returns false otherwise.
	 */
	private boolean userLoggedSuccessfully() {
		String hashedPassword = MD5.hash(password);

		// Check whether the selected user is in the db
		// It is an employee?
		String query = "SELECT e " +
			"FROM Employee e " +
			"WHERE login = '" + login + "' AND " +
				"password = '" + hashedPassword + "' AND " +
				"active = 1";
		Collection<Employee> resEmployee = em.createQuery(query).getResultList();
		for (Employee e : resEmployee) {
			employee = e;
			authFailed = false;
			return true;
		}

		// It is not an employee. Is it a company member?
		query = "SELECT c " +
			"FROM Company c " +
			"WHERE login = '" + login + "' AND " +
				"password = '" + hashedPassword + "'";
		Collection<Company> resCompany = em.createQuery(query).getResultList();
		for (Company c: resCompany) {
			company = c;
			authFailed = false;
			return true;
		}

		// There is no user with the selected credentials
		authFailed = true;
		return false;
	}

	/**
	 * @brief Logouts the user.
	 */
	private void logout() {
		company = null;
		employee = null;
		login = "";
		password = "";
		lastActionTS = new Date();
	}
}
