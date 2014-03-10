/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.controller;

import java.util.Collection;
import javax.annotation.PostConstruct;
import javax.persistence.EntityManager;
import org.richfaces.component.html.HtmlDataTable;

import pis.model.*;
import pis.lang.Lang;

/**
 * @brief Managed bean that contains all Employees
 *
 */
public class EmployeeList {
	private Employee employee;
	private HtmlDataTable listTable;
	private EntityManager em;
	private String lastError;

	public EmployeeList()
	{
		setEmployee(new Employee());
		setLastError("");
	}

	@PostConstruct
	public void init()
	{
		em = Db.getEntityManager();
	}

	/**
	 * @param employee the employee to set
	 */
	public void setEmployee(Employee employee) {
		this.employee = employee;
	}

	/**
	 * @return the employee
	 */
	public Employee getEmployee() {
		// HACK to properly reflect changes in the db
		if (!CurrentPage.getInstance().isOnEditPage()) {
			try {
				em.refresh(employee);
			} catch (IllegalArgumentException e) {
				// Entity not managed
			}
		}
		return employee;
	}

	/**
	 * @return list of employees
	 */
	public Collection<Employee> getEmployees()
	{
		return em.createQuery("SELECT e FROM Employee e").getResultList();
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
	 * @brief Inserts new employee
	 */
	public String actionInsertNew()
	{
		try {
			em.getTransaction().begin();
			em.persist(employee);
			em.getTransaction().commit();
			return "insert";
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("employeeInsertFailedError"));
			return "error";
		}
	}

	/**
	 * @brief Prepares managed bean to receive new employee
	 */
	public String prepareAddingEmployee()
	{
		this.employee = new Employee();
		return "addNew";
	}

	/**
	 * @brief Updates the employee
	 */
	public String actionUpdate()
	{
		try {
			em.getTransaction().begin();
			em.merge(employee);
			em.getTransaction().commit();
		} catch (Exception e) {
			if (em.getTransaction().isActive()) {
				em.getTransaction().rollback();
			}
			setLastError(Lang.getLocalizedString("employeeUpdateFailedError"));
			return "error";
		}
		return "updateEmployee";
	}

	/**
	 * @brief Edits employee
	 */
	public String actionEdit()
	{
		setEmployee((Employee) listTable.getRowData());
		return "editEmployee";
	}

	/**
	 * @brief Edits employee
	 */
	public String actionView()
	{
		setEmployee((Employee) listTable.getRowData());
		return "viewEmployee";
	}

	/**
	 * @brief Removes employee
	 */
	public String actionDelete()
	{
		String ret = "deleteEmployee";
		Employee selected = (Employee) listTable.getRowData();

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
			setLastError(Lang.getLocalizedString("employeeDeleteFailedError"));
			return "error";
		}
		return ret;
	}

	/**
	 * @brief Reloads the current contract from the database.
	 */
	public void reloadCurrentEmployee() {
		em.refresh(employee);
	}
}
