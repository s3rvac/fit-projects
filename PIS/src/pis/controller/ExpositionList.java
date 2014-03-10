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
import javax.persistence.EntityManager;
import javax.faces.context.FacesContext;
import org.richfaces.component.html.HtmlDataTable;

import pis.model.*;
import pis.lang.Lang;

/**
 * @brief Managed bean that contains all expositions
 *
 */
public class ExpositionList {
	private Exposition exposition;
	private HtmlDataTable listTable;
	private EntityManager em;
	private String lastError;
	private String searchString;

	public ExpositionList()
	{
		setExposition(new Exposition());
		setLastError("");
	}

	@PostConstruct
	public void init()
	{
		em = Db.getEntityManager();
	}

	/**
	 * @param exposition the exposition to set
	 */
	public void setExposition(Exposition exposition) {
		this.exposition = exposition;
	}

	/**
	 * @return the exposition
	 */
	public Exposition getExposition() {
		// HACK to properly reflect changes in the db
		if (!CurrentPage.getInstance().isOnEditPage()) {
			try {
				em.refresh(exposition);
			} catch (IllegalArgumentException e) {
				// Entity not managed
			}
		}
		return exposition;
	}

	/**
	 * @return list of expositions
	 */
	public Collection<Exposition> getExpositions()
	{
		return em.createQuery("SELECT e FROM Exposition e").getResultList();
	}

	/**
	 * @return true if there are some expositions matching the search string,
	 *         false otherwise
	 */
	public boolean isEmptyExpositionsMatchingSearchString() {
		return getExpositionsMatchingSearchString().isEmpty();
	}

	/**
	 * @return list of expositions matching the search string
	 */
	public Collection<Exposition> getExpositionsMatchingSearchString()
	{
		if (!searchString.isEmpty()) {
			String query = "SELECT e FROM Exposition e " +
				"WHERE name LIKE '%" + searchString + "%'";
			return em.createQuery(query).getResultList();
		} else {
			return new ArrayList<Exposition>();
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
	 * Sets actual price for current contract. Expects active database transaction
	 */
	public void fixCurrentContractPrice() {
		ContractList contractList = getContractList();
		contractList.fixCurrentContractPrice();
	}
	
	/**
	 * Checks if there is enough space for the exposition in the set exhibition
	 */
	private boolean isEnoughSpace() {
		Collection<Exposition> expositions = getExpositions();
		int reserved = exposition.getArea();
		for (Exposition e : expositions) {
			if (e.getPavilion().equals(exposition.getPavilion()) &&
					e.getExhibition().equals(exposition.getExhibition()) &&
					e.getId() != exposition.getId()) {
				reserved += e.getArea();
			}
		}
		return reserved <= exposition.getPavilion().getTotalArea();
	}

	/**
	 * @brief Inserts new exposition
	 */
	public String actionInsertNew()
	{
		if (!isEnoughSpace()) {
			setLastError(Lang.getLocalizedString("expositionNotEnoughSpace"));
			return "error";
		}
		try {
			em.getTransaction().begin();
			em.persist(exposition);
			fixCurrentContractPrice();
			em.getTransaction().commit();
			reloadCurrentContract();
			return "insert";
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("expositionInsertFailedError"));
			return "error";
		}
	}

	/**
	 * @brief Prepares managed bean to receive new exposition
	 */
	public String prepareAddingExposition()
	{
		exposition = new Exposition();
		FacesContext context = FacesContext.getCurrentInstance();
		ContractList cl = (ContractList) context.getApplication().evaluateExpressionGet(context,
				"#{contractList}", ContractList.class);
		exposition.setContract(cl.getContract());
		return "addNew";
	}

	/**
	 * @brief Updates the exposition
	 */
	public String actionUpdate()
	{
		if (!isEnoughSpace()) {
			setLastError(Lang.getLocalizedString("expositionNotEnoughSpace"));
			return "error";
		}
		try {
			em.getTransaction().begin();
			em.merge(exposition);
			fixCurrentContractPrice();
			em.getTransaction().commit();
			reloadCurrentContract();
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("expositionUpdateFailedError"));
			return "error";
		}
		return "updateExposition";
	}

	/**
	 * @brief Edits exposition
	 */
	public String actionEdit()
	{
		setExposition((Exposition) listTable.getRowData());
		return "editExposition";
	}

	/**
	 * @brief Shows exposition
	 */
	public String actionView()
	{
		setExposition((Exposition) listTable.getRowData());
		return "viewExposition";
	}

	/**
	 * @brief Removes exposition
	 */
	public String actionDelete()
	{
		String ret = "deleteExposition";
		Exposition selected = (Exposition) listTable.getRowData();

		if (selected.getContract().getEmployee() != null) {
			setLastError(Lang.getLocalizedString("expositionDeleteFailedAlreadySigned"));
			return "error";
		}

		try
		{
			em.getTransaction().begin();
			em.remove(em.merge(selected));
			//fixCurrentContractPrice();
			Contract contract = getContractList().getContract();
			contract.setPrice(contract.getPrice().subtract(selected.getPrice()));
			em.merge(contract);
			em.getTransaction().commit();
			reloadCurrentContract();
		} catch (Exception e) {
			if (em.getTransaction().isActive())
			{
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("expositionDeleteFailedError"));
			return "error";
		}
		return ret;
	}

	/**
	 * @brief Sets contract to be shown
	 */
	public String showContract() {
		ContractList contractList = getContractList();
		contractList.setContract(exposition.getContract());
		return "showContract";
	}

	/**
	 * @brief Sets company to be shown
	 */
	public String showCompany() {
		FacesContext context = FacesContext.getCurrentInstance();
		CompanyList companyList = (CompanyList) context.getApplication().evaluateExpressionGet(context,
				"#{companyList}", CompanyList.class);
		companyList.setCompany(exposition.getContract().getCompany());
		return "showCompany";
	}

	/**
	 * @brief Sets exhibition to be shown
	 */
	public String showExhibition() {
		FacesContext context = FacesContext.getCurrentInstance();
		ExhibitionList exhibitionList = (ExhibitionList) context.getApplication().evaluateExpressionGet(context,
				"#{exhibitionList}", ExhibitionList.class);
		exhibitionList.setExhibition(exposition.getExhibition());
		return "showExhibition";
	}

	/**
	 * @brief Sets pavilion to be shown
	 */
	public String showPavilion() {
		FacesContext context = FacesContext.getCurrentInstance();
		PavilionList pavilionList = (PavilionList) context.getApplication().evaluateExpressionGet(context,
				"#{pavilionList}", PavilionList.class);
		pavilionList.setPavilion(exposition.getPavilion());
		return "showPavilion";
	}

	/**
	 * @brief Returns a reference to the contractList managed bean
	 */
	private ContractList getContractList() {
		FacesContext context = FacesContext.getCurrentInstance();
		return (ContractList) context.getApplication().evaluateExpressionGet(context,
				"#{contractList}", ContractList.class);
	}

	/**
	 * @brief Reloads the current contract.
	 *
	 * Reloads the current contract so the change into the list
	 * of expositions registered on it is reflected.
	 */
	private void reloadCurrentContract() {
		ContractList contractList = getContractList();
		contractList.reloadCurrentContract();
	}
}
