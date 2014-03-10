/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.controller;

import java.util.Collection;
import java.util.ArrayList;
import javax.annotation.PostConstruct;
import javax.persistence.EntityManager;
import javax.faces.model.SelectItem;
import org.richfaces.component.html.HtmlDataTable;

import pis.model.*;
import pis.lang.Lang;

/**
 * @brief Managed bean that contains all Fields
 *
 */
public class FieldList {
	private Field field;
	private HtmlDataTable listTable;
	private EntityManager em;
	private String lastError;

	public FieldList()
	{
		setField(new Field());
		setLastError("");
	}

	@PostConstruct
	public void init()
	{
		em = Db.getEntityManager();
	}

	/**
	 * @param field the field to set
	 */
	public void setField(Field field) {
		this.field = field;
	}

	/**
	 * @return the field
	 */
	public Field getField() {
		// HACK to properly reflect changes in the db
		if (!CurrentPage.getInstance().isOnEditPage()) {
			try {
				em.refresh(field);
			} catch (IllegalArgumentException e) {
				// Entity not managed
			}
		}
		return field;
	}

	/**
	 * @return list of fields
	 */
	public Collection<Field> getFields()
	{
		return em.createQuery("SELECT b FROM Field b").getResultList();
	}

	/**
	 * @return array of select items that can be inserted to the JSF selectonemenu
	 */
	public Collection<SelectItem> getSelectItemFields() {
		ArrayList<SelectItem> items = new ArrayList<SelectItem>();
		Collection<Field> fields = getFields();
		for (Field field : fields) {
			items.add(new SelectItem(field, field.getName()));
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

	/**
	 * @brief Inserts new field
	 */
	public String actionInsertNew()
	{
		try {
			em.getTransaction().begin();
			em.persist(field);
			em.getTransaction().commit();
			return "insert";
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("fieldInsertFailedError"));
			return "error";
		}
	}

	/**
	 * @brief Prepares managed bean to receive new field
	 */
	public String prepareAddingField()
	{
		this.field = new Field();
		return "addNew";
	}

	/**
	 * @brief Updates the field
	 */
	public String actionUpdate()
	{
		try {
			em.getTransaction().begin();
			em.merge(field);
			em.getTransaction().commit();
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("fieldUpdateFailedError"));
			return "error";
		}
		return "updateField";
	}

	/**
	 * @brief Edits field
	 */
	public String actionEdit()
	{
		setField((Field) listTable.getRowData());
		return "editField";
	}

	/**
	 * @brief Shows field
	 */
	public String actionView()
	{
		setField((Field) listTable.getRowData());
		return "viewField";
	}

	/**
	 * @brief Removes field
	 */
	public String actionDelete()
	{
		String ret = "deleteField";
		Field selected = (Field) listTable.getRowData();

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
			setLastError(Lang.getLocalizedString("fieldDeleteFailedError"));
			return "error";
		}
		return ret;
	}
}
