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
import java.util.Date;
import javax.annotation.PostConstruct;
import javax.faces.model.SelectItem;
import javax.persistence.EntityManager;
import org.richfaces.component.html.HtmlDataTable;

import pis.model.*;
import pis.lang.Lang;

/**
 * @brief Managed bean that contains all Exhibitions
 *
 */
public class ExhibitionList {
	private Exhibition exhibition;
	private HtmlDataTable listTable;
	private EntityManager em;
	private String lastError;
	private String searchString;

	public ExhibitionList()
	{
		setExhibition(new Exhibition());
		setLastError("");
	}

	@PostConstruct
	public void init()
	{
		em = Db.getEntityManager();
	}

	/**
	 * @param exhibition the exhibition to set
	 */
	public void setExhibition(Exhibition exhibition) {
		this.exhibition = exhibition;
	}

	/**
	 * @return the exhibition
	 */
	public Exhibition getExhibition() {
		// HACK to properly reflect changes in the db
		if (!CurrentPage.getInstance().isOnEditPage()) {
			try {
				em.refresh(exhibition);
			} catch (IllegalArgumentException e) {
				// Entity not managed
			}
		}
		return exhibition;
	}

	/**
	 * @return list of exhibitions
	 */
	public Collection<Exhibition> getExhibitions()
	{
		return em.createQuery("SELECT e FROM Exhibition e").getResultList();
	}

	/**
	 * @return true if there are some current exhibitions, false otherwise
	 */
	public boolean isEmptyCurrentExhibitions() {
		return getCurrentExhibitions().isEmpty();
	}

	/**
	 * @return list of current exhibitions
	 */
	public Collection<Exhibition> getCurrentExhibitions()
	{
		// OK, this is a bit ineffective, but what...
		// (feel free to rewrite it into a single SELECT)
		Date currentDate = new Date();
		Collection<Exhibition> currentExhibitions = new ArrayList<Exhibition>();
		String query = "SELECT e FROM Exhibition e";
		Collection<Exhibition> allExhibitions = em.createQuery(query).getResultList();
		for (Exhibition e : allExhibitions) {
			if (e.getEndDate().getTime() > currentDate.getTime()) {
				currentExhibitions.add(e);
			}
		}
		return currentExhibitions;
	}

	/**
	 * @return true if there are some exhibitions matching the search string,
	 *         false otherwise
	 */
	public boolean isEmptyExhibitionsMatchingSearchString() {
		return getExhibitionsMatchingSearchString().isEmpty();
	}

	/**
	 * @return list of exhibitions matching the search string
	 */
	public Collection<Exhibition> getExhibitionsMatchingSearchString()
	{
		if (!searchString.isEmpty()) {
			String query = "SELECT e FROM Exhibition e " +
				"WHERE name LIKE '%" + searchString + "%'";
			return em.createQuery(query).getResultList();
		} else {
			return new ArrayList<Exhibition>();
		}
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

	/**
	 * @return array of select items that can be inserted to the JSF selectonemenu
	 */
	public Collection<SelectItem> getSelectItemExhibitions() {
		ArrayList<SelectItem> items = new ArrayList<SelectItem>();
		Collection<Exhibition> exhibitions = getExhibitions();
		for (Exhibition exhibition : exhibitions) {
			items.add(new SelectItem(exhibition, exhibition.getName()));
		}
		return items;
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
	 * @brief Inserts new exhibition
	 */
	public String actionInsertNew()
	{
		try {
			em.getTransaction().begin();
			em.persist(exhibition);
			em.getTransaction().commit();
			return "insert";
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("exhibitionInsertFailedError"));
			return "error";
		}
	}

	/**
	 * @brief Prepares managed bean to receive new exhibition
	 */
	public String prepareAddingExhibition()
	{
		this.exhibition = new Exhibition();
		return "addNew";
	}

	/**
	 * @brief Updates the exhibition
	 */
	public String actionUpdate()
	{
		try {
			em.getTransaction().begin();
			em.merge(exhibition);
			em.getTransaction().commit();
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("exhibitionUpdateFailedError"));
			return "error";
		}
		return "updateExhibition";
	}

	/**
	 * @brief Edits exhibition
	 */
	public String actionEdit()
	{
		setExhibition((Exhibition) listTable.getRowData());
		return "editExhibition";
	}

	/**
	 * @brief Edits exhibition
	 */
	public String actionView()
	{
		setExhibition((Exhibition) listTable.getRowData());
		return "viewExhibition";
	}

	/**
	 * @brief Removes exhibition
	 */
	public String actionDelete()
	{
		String ret = "deleteExhibition";
		Exhibition selected = (Exhibition) listTable.getRowData();

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
			setLastError(Lang.getLocalizedString("exhibitionDeleteFailedError"));
			return "error";
		}
		return ret;
	}
}
