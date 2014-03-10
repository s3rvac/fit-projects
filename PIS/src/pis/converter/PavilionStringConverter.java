/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.converter;

import javax.faces.component.UIComponent;
import javax.faces.context.FacesContext;
import javax.faces.convert.Converter;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

import pis.model.Pavilion;

/**
 * @brief Converts from Pavilion to String and vice versa.
 */
public class PavilionStringConverter implements Converter {
	public Object getAsObject(FacesContext context, UIComponent component, String value) {
		// Convert the unique String representation of Pavilion to the actual Pavilion object

		// An alternative would be to use PavilionList.getPavilions()
		// and go through this list in O(n)
		EntityManagerFactory emf = Persistence.createEntityManagerFactory("PIS");
		EntityManager em = emf.createEntityManager();
		String query = "SELECT p FROM Pavilion p " +
			"WHERE id = " + value;
		return em.createQuery(query).getSingleResult();
	}

	public String getAsString(FacesContext context, UIComponent component, Object value) {
		// Convert the Pavilion object to its unique String representation
		Pavilion pavilion = (Pavilion) value;
		return Integer.toString(pavilion.getId());
	}
}
