/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.model;

import pis.util.MD5;

import java.io.Serializable;
import javax.persistence.*;
import java.util.List;


/**
 * The persistent class for the company database table.
 *
 */
@Entity
@Table(name="company")
public class Company implements Serializable {
	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue
	@Column(updatable=false)
	private int id;

	private String account;

	private String dic;

	private String ic;

	private String name;

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
	@OneToMany(mappedBy="company", fetch=FetchType.LAZY)
	private List<Contract> contracts;

	public Company() {
	}

	public int getId() {
		return this.id;
	}

	private void setId(int id) {
		this.id = id;
	}

	public String getAccount() {
		return this.account;
	}

	public void setAccount(String account) {
		this.account = account;
	}

	public String getDic() {
		return this.dic;
	}

	public void setDic(String dic) {
		this.dic = dic;
	}

	public String getIc() {
		return this.ic;
	}

	public void setIc(String ic) {
		this.ic = ic;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
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
		// editing a company and leaving the password empty
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

	@Override
	public boolean equals(Object other) {
		return other instanceof Company &&
			getId() == ((Company) other).getId();
	}
}
