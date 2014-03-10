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

import pis.model.Exhibition;

/**
 * @brief Converts from Exhibition to String and vice versa.
 */
public class ExhibitionStringConverter implements Converter {
	public Object getAsObject(FacesContext context, UIComponent component, String value) {
		// Convert the unique String representation of Exhibition to the actual Exhibition object

		// An alternative would be to use ExhibitionList.getExhibitions()
		// and go through this list in O(n)
		EntityManagerFactory emf = Persistence.createEntityManagerFactory("PIS");
		EntityManager em = emf.createEntityManager();
		String query = "SELECT e FROM Exhibition e " +
			"WHERE id = " + value;
		return em.createQuery(query).getSingleResult();
	}

	public String getAsString(FacesContext context, UIComponent component, Object value) {
		// Convert the Exhibition object to its unique String representation
		Exhibition exhibition = (Exhibition) value;
		return Integer.toString(exhibition.getId());
	}
}
