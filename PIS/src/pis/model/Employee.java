/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.model;

import java.io.Serializable;
import javax.persistence.*;
import java.util.List;

import pis.util.MD5;

/**
 * The persistent class for the employee database table.
 *
 */
@Entity
@Table(name="employee")
public class Employee implements Serializable {
	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue
	@Column(updatable=false)
	private int id;

	private boolean active;

	private boolean chief;

	private String name;

	private String rc;

	private int salary;

	private String surname;

	private String email;

	private String login;

	private String mobile;

	private String password;

	private String phone;

	private String city;

	private int number;

	private String psc;

	private String street;

	//bi-directional many-to-one association to Contract
	@OneToMany(mappedBy="employee", fetch=FetchType.LAZY)
	private List<Contract> contracts;

	public Employee() {
	}

	public int getId() {
		return this.id;
	}

	private void setId(int id) {
		this.id = id;
	}

	public boolean getActive() {
		return this.active;
	}

	public void setActive(boolean active) {
		this.active = active;
	}

	public boolean getChief() {
		return this.chief;
	}

	public void setChief(boolean chief) {
		this.chief = chief;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getRc() {
		return this.rc;
	}

	public void setRc(String rc) {
		this.rc = rc;
	}

	public int getSalary() {
		return this.salary;
	}

	public void setSalary(int salary) {
		this.salary = salary;
	}

	public String getSurname() {
		return this.surname;
	}

	public void setSurname(String surname) {
		this.surname = surname;
	}

	public List<Contract> getContracts() {
		return this.contracts;
	}

	public void setContracts(List<Contract> contracts) {
		this.contracts = contracts;
	}

	public String getEmail() {
		return this.email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getLogin() {
		return this.login;
	}

	public void setLogin(String login) {
		this.login = login;
	}

	public String getMobile() {
		return this.mobile;
	}

	public void setMobile(String mobile) {
		this.mobile = mobile;
	}

	public String getPassword() {
		return this.password;
	}

	public void setPassword(String password) {
		// Prevent setting the password to md5("") when
		// editing an employee and leaving the password empty
		if (!password.equals("")) {
			this.password = MD5.hash(password);
		}
	}

	public String getPhone() {
		return this.phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

	public String getCity() {
		return this.city;
	}

	public void setCity(String city) {
		this.city = city;
	}

	public int getNumber() {
		return this.number;
	}

	public void setNumber(int number) {
		this.number = number;
	}

	public String getPsc() {
		return this.psc;
	}

	public void setPsc(String psc) {
		this.psc = psc;
	}

	public String getStreet() {
		return this.street;
	}

	public void setStreet(String street) {
		this.street = street;
	}
}
