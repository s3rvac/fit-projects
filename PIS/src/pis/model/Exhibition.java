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

import pis.util.Calendar;

/**
 * The persistent class for the exhibition database table.
 *
 */
@Entity
@Table(name="exhibition")
public class Exhibition implements Serializable {
	private static final long serialVersionUID = 1L;
	private static final Date DEFAULT_DATE = new Date(0);

	@Id
	@GeneratedValue
	@Column(updatable=false)
	private int id;

    @Temporal(TemporalType.DATE)
	private Date endDate;

	private int entranceFee;

	private String name;

    @Temporal(TemporalType.DATE)
	private Date startDate;

	//bi-directional many-to-one association to Exposition
	@OneToMany(mappedBy="exhibition", fetch=FetchType.EAGER)
	private List<Exposition> expositions;

	@Transient
	private Calendar startDateCalendar;
	@Transient
	private Calendar endDateCalendar;

	public Exhibition() {
		name = "";
		entranceFee = 0;
		startDate = (Date) DEFAULT_DATE.clone();
		endDate = (Date) DEFAULT_DATE.clone();
		startDateCalendar = new Calendar(startDate);
		endDateCalendar = new Calendar(endDate);
	}

	public int getId() {
		return this.id;
	}

	private void setId(int id) {
		this.id = id;
	}

	public Date getEndDate() {
		return endDate;
	}

	public void setEndDate(Date endDate) {
		this.endDate = endDate;
	}

	public int getEntranceFee() {
		return this.entranceFee;
	}

	public void setEntranceFee(int entranceFee) {
		this.entranceFee = entranceFee;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public Date getStartDate() {
		return startDate;
	}

	public void setStartDate(Date startDate) {
		this.startDate = startDate;
	}

	public List<Exposition> getExpositions() {
		return this.expositions;
	}

	public void setExpositions(List<Exposition> expositions) {
		this.expositions = expositions;
	}

	public Calendar getStartDateCalendar() {
		return new Calendar(startDate);
	}

	public void setStartDateCalendar(Calendar newStartDateCalendar) {
		this.startDateCalendar = newStartDateCalendar;
	}

	public Calendar getEndDateCalendar() {
		return new Calendar(endDate);
	}

	public void setEndDateCalendar(Calendar newEndDateCalendar) {
		this.endDateCalendar = newEndDateCalendar;
	}

	@Override
	public boolean equals(Object other) {
		return other instanceof Exhibition &&
			getId() == ((Exhibition) other).getId();
	}
}
