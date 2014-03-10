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

import pis.model.Field;

/**
 * @brief Converts from Field to String and vice versa.
 */
public class FieldStringConverter implements Converter {
	public Object getAsObject(FacesContext context, UIComponent component, String value) {
		// Convert the unique String representation of Field to the actual Field object

		// An alternative would be to use FieldList.getFields()
		// and go through this list in O(n)
		EntityManagerFactory emf = Persistence.createEntityManagerFactory("PIS");
		EntityManager em = emf.createEntityManager();
		String query = "SELECT f FROM Field f " +
			"WHERE id = " + value;
		return em.createQuery(query).getSingleResult();
	}

	public String getAsString(FacesContext context, UIComponent component, Object value) {
		// Convert the Field object to its unique String representation
		Field field = (Field) value;
		return Integer.toString(field.getId());
	}
}
