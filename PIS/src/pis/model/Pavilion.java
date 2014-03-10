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
 * The persistent class for the pavilion database table.
 *
 */
@Entity
@Table(name="pavilion")
public class Pavilion implements Serializable {
	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue
	@Column(updatable=false)
	private int id;

	private boolean electricity;

	private int m2Price;

	private String name;

	private int numberOfFloors;

	private int numberOfRestaurants;

	private int numberOfToilets;

    @Temporal( TemporalType.DATE)
	private Date openingDate;

	private int totalArea;

	private boolean water;

	//bi-directional many-to-one association to Exposition
	@OneToMany(mappedBy="pavilion", fetch=FetchType.EAGER)
	private List<Exposition> expositions;

	@Transient
	private Calendar openingDateCalendar;

    public Pavilion() {
		numberOfFloors = 1;
		openingDate = new Date();
		openingDateCalendar = new Calendar(openingDate);
    }

	public int getId() {
		return this.id;
	}

	private void setId(int id) {
		this.id = id;
	}

	public boolean getElectricity() {
		return this.electricity;
	}

	public void setElectricity(boolean electricity) {
		this.electricity = electricity;
	}

	public int getM2Price() {
		return this.m2Price;
	}

	public void setM2Price(int m2Price) {
		this.m2Price = m2Price;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getNumberOfFloors() {
		return this.numberOfFloors;
	}

	public void setNumberOfFloors(int numberOfFloors) {
		this.numberOfFloors = numberOfFloors;
	}

	public int getNumberOfRestaurants() {
		return this.numberOfRestaurants;
	}

	public void setNumberOfRestaurants(int numberOfRestaurants) {
		this.numberOfRestaurants = numberOfRestaurants;
	}

	public int getNumberOfToilets() {
		return this.numberOfToilets;
	}

	public void setNumberOfToilets(int numberOfToilets) {
		this.numberOfToilets = numberOfToilets;
	}

	public Date getOpeningDate() {
		return this.openingDate;
	}

	public void setOpeningDate(Date openingDate) {
		this.openingDate = openingDate;
	}

	public int getTotalArea() {
		return this.totalArea;
	}

	public void setTotalArea(int totalArea) {
		this.totalArea = totalArea;
	}

	public boolean getWater() {
		return this.water;
	}

	public void setWater(boolean water) {
		this.water = water;
	}

	public List<Exposition> getExpositions() {
		return this.expositions;
	}

	public void setExpositions(List<Exposition> expositions) {
		this.expositions = expositions;
	}

	public Calendar getOpeningDateCalendar() {
		return new Calendar(openingDate);
	}

	public void setOpeningDateCalendar(Calendar newOpeningDateCalendar) {
		this.openingDateCalendar = newOpeningDateCalendar;
	}
	
	@Override
	public boolean equals(Object other) {
		return other instanceof Pavilion &&
			getId() == ((Pavilion) other).getId();
	}
}
