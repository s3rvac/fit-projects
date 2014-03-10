/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.controller;

import java.util.Collection;
import java.util.Date;
import java.math.*;
import javax.annotation.PostConstruct;
import javax.persistence.EntityManager;
import org.richfaces.component.html.HtmlDataTable;
import javax.faces.context.FacesContext;

import pis.model.*;
import pis.lang.Lang;

/**
 * @brief Managed bean that contains all Contracts
 *
 */
public class ContractList {
	private Contract contract;
	private HtmlDataTable listTable;
	private EntityManager em;
	private String lastError;
	private String searchString;

	public ContractList()
	{
		setContract(new Contract());
		setLastError("");
	}

	@PostConstruct
	public void init()
	{
		em = Db.getEntityManager();
	}

	/**
	 * @param contract the contract to set
	 */
	public void setContract(Contract contract) {
		this.contract = contract;
	}

	/**
	 * @return the contract
	 */
	public Contract getContract() {
		// HACK to properly reflect changes in the db
		if (!CurrentPage.getInstance().isOnEditPage()) {
			try {
				em.refresh(contract);
			} catch (IllegalArgumentException e) {
				// Entity not managed
			}
		}
		return contract;
	}

	/**
	 * @return list of contracts
	 */
	public Collection<Contract> getContracts()
	{
		Auth auth = Auth.getInstance();
		Collection<Contract> contracts;
		if (auth.isLoggedAsCompany()) {
			// Display only contracts of this company
			contracts = auth.getCompany().getContracts();
		} else {
			// Display all contracts
			contracts = em.createQuery("SELECT e FROM Contract e").getResultList();
		}
		return contracts;
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
	 * @brief Inserts new contract
	 */
	public String actionInsertNew()
	{
		try {
			em.getTransaction().begin();
			em.persist(contract);
			em.getTransaction().commit();
			return "insert";
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("contractInsertFailedError"));
			return "error";
		}
	}

	/**
	 * @brief Prepares managed bean to receive new contract
	 */
	public String prepareAddingContract()
	{
		this.contract = new Contract();
		return "addNewContract";
	}

	/**
	 * @brief Updates the contract
	 */
	public String actionUpdate()
	{
		try {
			em.getTransaction().begin();
			em.merge(contract);
			em.getTransaction().commit();
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("contractUpdateFailedError"));
			return "error";
		}
		return "updateContract";
	}

	/**
	 * @brief Edits contract
	 */
	public String actionEdit()
	{
		setContract((Contract) listTable.getRowData());
		return "editContract";
	}

	/**
	 * @brief Edits contract
	 */
	public String actionView()
	{
		setContract((Contract) listTable.getRowData());
		return "viewContract";
	}

	/**
	 * @brief Removes contract
	 */
	public String actionDelete()
	{
		String ret = "deleteContract";
		Contract selected = (Contract) listTable.getRowData();

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
			setLastError(Lang.getLocalizedString("contractDeleteFailedError"));
			return "error";
		}
		return ret;
	}

	/**
	 * @brief Sets company to be shown
	 */
	public String showCompany() {
		FacesContext context = FacesContext.getCurrentInstance();
		CompanyList companyList = (CompanyList) context.getApplication().evaluateExpressionGet(context,
				"#{companyList}", CompanyList.class);
		companyList.setCompany(contract.getCompany());
		return "showCompany";
	}

	/**
	 * @brief Signs the selected contract and returns the action name
	 *        to be done.
	 */
	public String signContract() {
		// FIXME
		contract.setConclusionDate(new Date());
		return actionUpdate().equals("error") ? "error" : "contractSigned";
	}

	/**
	 * @brief Reloads the current contract from the database.
	 */
	public void reloadCurrentContract() {
		if (contract.getNumber() != 0) {
			em.refresh(contract);
		}
	}

	/**
	 * Sets actual price for current contract. Expects active database transaction
	 */
	public void fixCurrentContractPrice() {
		em.refresh(contract);
		BigDecimal price = new BigDecimal(0);
		for (Exposition e : contract.getExpositions()) {
			price = price.add(e.getPrice());
		}
		contract.setPrice(price);
		em.merge(contract);
	}

	/**
	 * @brief Returns true if the current contract can be signed,
	 *        false otherwise.
	 *
	 * This function also checks permissions.
	 */
	public boolean getCanCurrentContractBeSigned() {
		return !contract.isConcluded() &&
			Auth.getInstance().isLoggedAsChief();
	}

	/**
	 * @brief Returns true if the "Concluded with" field for the current contract
	 *        is visible for the current user, false otherwise.
	 *
	 * It is assumed that the current user is logged in.
	 */
	public boolean isConcludedWithFieldVisible() {
		if (!Auth.getInstance().isLoggedAsCompany()) {
			return true;
		}

		return Auth.getInstance().getCompany().getId() != contract.getCompany().getId();
	}
}
