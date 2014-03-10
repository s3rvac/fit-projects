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
 * @brief Managed bean that contains all Companies
 *
 */
public class CompanyList {
	private Company company;
	private HtmlDataTable listTable;
	private EntityManager em;
	private String lastError;

	public CompanyList()
	{
		setCompany(new Company());
		setLastError("");
	}

	@PostConstruct
	public void init()
	{
		em = Db.getEntityManager();
	}

	/**
	 * @param company the company to set
	 */
	public void setCompany(Company company) {
		this.company = company;
	}

	/**
	 * @return the company
	 */
	public Company getCompany() {
		// HACK to properly reflect changes in the db
		if (!CurrentPage.getInstance().isOnEditPage()) {
			try {
				em.refresh(company);
			} catch (IllegalArgumentException e) {
				// Entity not managed
			}
		}
		return company;
	}

	/**
	 * @return list of companies
	 */
	public Collection<Company> getCompanies()
	{
		return em.createQuery("SELECT c FROM Company c").getResultList();
	}

	/**
	 * @return list of expositions of the selected company
	 */
	public Collection<Exposition> getExpositionsOfCompany()
	{
		Collection<Exposition> expositions = new ArrayList<Exposition>();
		for (Contract contract : company.getContracts()) {
			for (Exposition exposition : contract.getExpositions()) {
				expositions.add(exposition);
			}
		}
		return expositions;
	}

	/**
	 * @return array of select items that can be inserted to the JSF selectonemenu
	 */
	public Collection<SelectItem> getSelectItemCompanies() {
		ArrayList<SelectItem> items = new ArrayList<SelectItem>();
		Collection<Company> companies = getCompanies();
		for (Company company : companies) {
			items.add(new SelectItem(company, company.getName()));
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
	 * @brief Inserts new company
	 */
	public String actionInsertNew()
	{
		try {
			em.getTransaction().begin();
			em.persist(company);
			em.getTransaction().commit();
			return "insert";
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("companyInsertFailedError"));
			return "error";
		}
	}

	/**
	 * @brief Prepares managed bean to receive new company
	 */
	public String prepareAddingCompany()
	{
		this.company = new Company();
		return "addNew";
	}

	/**
	 * @brief Updates the company
	 */
	public String actionUpdate()
	{
		try {
			em.getTransaction().begin();
			em.merge(company);
			em.getTransaction().commit();
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("companyUpdateFailedError"));
			return "error";
		}
		return "updateCompany";
	}

	/**
	 * @brief Edits company
	 */
	public String actionEdit()
	{
		setCompany((Company) listTable.getRowData());
		return "editCompany";
	}

	/**
	 * @brief Edits company
	 */
	public String actionView()
	{
		setCompany((Company) listTable.getRowData());
		return "viewCompany";
	}

	/**
	 * @brief Removes company
	 */
	public String actionDelete()
	{
		String ret = "deleteCompany";
		Company selected = (Company) listTable.getRowData();

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
			setLastError(Lang.getLocalizedString("companyDeleteFailedError"));
			return "error";
		}
		return ret;
	}

	/**
	 * @brief Returns true if the current company private details are viewable
	 *        by the current user.
	 */
	public boolean isPrivateDetailsViewable() {
		Auth auth = Auth.getInstance();

		if (auth.isLoggedAsEmployee()) {
			return true;
		} else if (auth.isLoggedAsCompany() && auth.getCompany().getId() == company.getId()) {
			return true;
		}

		return false;
	}

	/**
	 * @brief Returns navigation action when a company member is
	 *        going to edit details of his company.
	 */
	public String editOwnCompanyAction() {
		company = Auth.getInstance().getCompany();
		return "companyEditOwn";
	}
}
