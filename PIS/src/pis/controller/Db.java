/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.controller;

import javax.faces.context.FacesContext;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

/**
 * @brief Manages operations with the database.
 */
public class Db {
	private EntityManagerFactory emf;
	private EntityManager em;
	private static final String PERSISTENCE_UNIT_NAME = "PIS";

	/**
	 * @brief Default constructor.
	 */
	public Db() {
		emf = Persistence.createEntityManagerFactory(PERSISTENCE_UNIT_NAME);
		em = emf.createEntityManager();
	}

	/**
	 * @return Entity manager for the current session.
	 */
	public static EntityManager getEntityManager() {
		FacesContext context = FacesContext.getCurrentInstance();
		Db db = (Db) context.getApplication().evaluateExpressionGet(context,
				"#{db}", Db.class);
		return db.em;
	}
}
