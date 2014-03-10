/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.validator;

import java.util.Collection;
import javax.faces.component.UIComponent;
import javax.faces.context.FacesContext;
import javax.faces.validator.Validator;
import javax.faces.validator.ValidatorException;
import javax.faces.application.FacesMessage;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

import pis.lang.Lang;
import pis.model.Employee;
import pis.model.Company;

/**
 * @brief Validates that the selected login is unique.
 */
public class UniqueLoginValidator implements Validator {
	private static final String VALIDATION_ERROR_NAME = "uniqueLoginValidationFailed";

	@Override
	public void validate(FacesContext arg0, UIComponent component, Object value)
			throws ValidatorException {
		String login = (String) value;
		boolean isUnique = true;
		EntityManagerFactory emf = Persistence.createEntityManagerFactory("PIS");
		EntityManager em = emf.createEntityManager();
		
		// Obtain the user ID from f:attribute.
		Integer userId = (Integer) component.getAttributes().get("userId");

		// Check whether the selected login is in the employee table
		String query = "SELECT e " +
			"FROM Employee e " +
			"WHERE login = '" + login + "' AND " +
				"id <> " + userId;
		Collection<Employee> resEmployee = em.createQuery(query).getResultList();
		for (Employee e : resEmployee) {
			isUnique = false;
		}

		// Check whether the selected login is in the company table
		query = "SELECT c " +
			"FROM Company c " +
			"WHERE login = '" + login + "' AND " +
				"id <> " + userId;
		Collection<Company> resCompany = em.createQuery(query).getResultList();
		for (Company c: resCompany) {
			isUnique = false;
		}

		if (!isUnique) {
			throw new ValidatorException(new FacesMessage(
				Lang.getLocalizedString(VALIDATION_ERROR_NAME)));
		}
	}
}
