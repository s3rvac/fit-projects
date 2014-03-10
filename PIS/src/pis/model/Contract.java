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
import java.util.Date;
import java.util.List;
import java.math.*;

import pis.util.Calendar;

/**
 * The persistent class for the contract database table.
 *
 */
@Entity
@Table(name="contract")
public class Contract implements Serializable {
	private static final long serialVersionUID = 1L;
	private static final Date DEFAULT_DATE = new Date(0);

	@Id
	@GeneratedValue
	@Column(updatable=false)
	private int number;

    @Temporal(TemporalType.DATE)
	private Date conclusionDate;

    @Lob()
	private String contractText;

    @Temporal(TemporalType.DATE)
	private Date maturityDate;

    @Column(scale=2, precision=65, nullable=true)
	private BigDecimal price;

    @Column(scale=2, precision=65, nullable=true)
	private BigDecimal settled;

	@Transient
	private Calendar conclusionDateCalendar;

	@Transient
	private Calendar maturityDateCalendar;

	//bi-directional many-to-one association to Company
    @ManyToOne
	@JoinColumn(name="companyId")
	private Company company;

	//bi-directional many-to-one association to Employee
    @ManyToOne
	@JoinColumn(name="signedId")
	private Employee employee;

	//bi-directional many-to-one association to Exposition
	@OneToMany(mappedBy="contract", fetch=FetchType.EAGER)
	private List<Exposition> expositions;

    public Contract() {
		price = new BigDecimal(0);
		settled = new BigDecimal(0);
    }

	public int getNumber() {
		return this.number;
	}

	private void setNumber(int number) {
		this.number = number;
	}

	public Date getConclusionDate() {
		return this.conclusionDate;
	}

	public void setConclusionDate(Date conclusionDate) {
		this.conclusionDate = conclusionDate;
	}

	public String getContractText() {
		return this.contractText;
	}

	public void setContractText(String contractText) {
		this.contractText = contractText;
	}

	public Date getMaturityDate() {
		return this.maturityDate;
	}

	public void setMaturityDate(Date maturityDate) {
		this.maturityDate = maturityDate;
	}

	public BigDecimal getPrice() {
		return this.price;
	}

	public void setPrice(BigDecimal price) {
		this.price = price;
	}

	public BigDecimal getSettled() {
		return this.settled;
	}

	public void setSettled(BigDecimal settled) {
		this.settled = settled;
	}

	public Company getCompany() {
		return this.company;
	}

	public void setCompany(Company company) {
		this.company = company;
	}

	public Employee getEmployee() {
		return this.employee;
	}

	public void setEmployee(Employee employee) {
		this.employee = employee;
	}

	public List<Exposition> getExpositions() {
		return this.expositions;
	}

	public void setExpositions(List<Exposition> expositions) {
		this.expositions = expositions;
	}

	public Calendar getMaturityDateCalendar() {
		if (maturityDate == null) {
			maturityDate = (Date) DEFAULT_DATE.clone();
		}
		return new Calendar(maturityDate);
	}

	public void setMaturityDateCalendar(Calendar newMaturityDateCalendar) {
		this.maturityDateCalendar = newMaturityDateCalendar;
	}

	public Calendar getConclusionDateCalendar() {
		if (conclusionDate == null) {
			conclusionDate = (Date) DEFAULT_DATE.clone();
		}
		return new Calendar(conclusionDate);
	}

	public void setConclusionDateCalendar(Calendar newConclusionDateCalendar) {
		this.conclusionDateCalendar = newConclusionDateCalendar;
	}

	public boolean isConcluded() {
		return employee != null;
	}
}
