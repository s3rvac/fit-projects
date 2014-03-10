/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.controller;

import java.util.ArrayList;
import java.util.Collection;
import javax.annotation.PostConstruct;
import javax.faces.model.SelectItem;
import javax.persistence.EntityManager;
import org.richfaces.component.html.HtmlDataTable;

import pis.model.*;
import pis.lang.Lang;

/**
 * @brief Managed bean that contains all Pavilions
 *
 */
public class PavilionList {
	private Pavilion pavilion;
	private HtmlDataTable listTable;
	private EntityManager em;
	private String lastError;
	private String searchString;

	public PavilionList()
	{
		setPavilion(new Pavilion());
		setLastError("");
	}

	@PostConstruct
	public void init()
	{
		em = Db.getEntityManager();
	}

	/**
	 * @param pavilion the pavilion to set
	 */
	public void setPavilion(Pavilion pavilion) {
		this.pavilion = pavilion;
	}

	/**
	 * @return the pavilion
	 */
	public Pavilion getPavilion() {
		// HACK to properly reflect changes in the db
		if (!CurrentPage.getInstance().isOnEditPage()) {
			try {
				em.refresh(pavilion);
			} catch (IllegalArgumentException e) {
				// Entity not managed
			}
		}
		return pavilion;
	}

	/**
	 * @return list of pavilions
	 */
	public Collection<Pavilion> getPavilions()
	{
		return em.createQuery("SELECT p FROM Pavilion p").getResultList();
	}

	/**
	 * @return array of select items that can be inserted to the JSF selectonemenu
	 */
	public Collection<SelectItem> getSelectItemPavilions() {
		ArrayList<SelectItem> items = new ArrayList<SelectItem>();
		Collection<Pavilion> pavilions = getPavilions();
		for (Pavilion pavilion : pavilions) {
			items.add(new SelectItem(pavilion, pavilion.getName()));
		}
		return items;
	}

	/**
	 * @param listTable the listTable to set
	 */
	public void setListTable(HtmlDataTable listTable) {
		this.listTable = listTable;
	}

	/**
	 * @return the listTable
	 */
	public HtmlDataTable getListTable() {
		// HACK to enable reuse of this listTable
		return null;
		// return listTable;
	}

	public String getLastError() {
		String le = lastError;
		lastError = "";
		return le;
	}

	private void setLastError(String lastError) {
		this.lastError = lastError;
	}

	public String getSearchString() {
		return searchString;
	}

	public void setSearchString(String newSearchString) {
		searchString = newSearchString;
	}

	/**
	 * @brief Inserts new pavilion
	 */
	public String actionInsertNew()
	{
		try {
			em.getTransaction().begin();
			em.persist(pavilion);
			em.getTransaction().commit();
			return "insert";
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("pavilionInsertFailedError"));
			return "error";
		}
	}

	/**
	 * @brief Prepares managed bean to receive new pavilion
	 */
	public String prepareAddingPavilion()
	{
		this.pavilion = new Pavilion();
		return "addNew";
	}

	/**
	 * @brief Updates the pavilion
	 */
	public String actionUpdate()
	{
		try {
			em.getTransaction().begin();
			em.merge(pavilion);
			em.getTransaction().commit();
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("pavilionUpdateFailedError"));
			return "error";
		}
		return "updatePavilion";
	}

	/**
	 * @brief Edits pavilion
	 */
	public String actionEdit()
	{
		setPavilion((Pavilion) listTable.getRowData());
		return "editPavilion";
	}

	/**
	 * @brief Edits pavilion
	 */
	public String actionView()
	{
		setPavilion((Pavilion) listTable.getRowData());
		return "viewPavilion";
	}

	/**
	 * @brief Removes pavilion
	 */
	public String actionDelete()
	{
		String ret = "deletePavilion";
		Pavilion selected = (Pavilion) listTable.getRowData();

		try
		{
			em.getTransaction().begin();
			em.remove(em.merge(selected));
			em.getTransaction().commit();
		} catch (Exception e) {
			if (em.getTransaction().isActive())
			{
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("pavilionDeleteFailedError"));
			return "error";
		}
		return ret;
	}
}
