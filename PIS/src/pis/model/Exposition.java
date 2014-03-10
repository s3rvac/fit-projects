/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.model;

import java.io.Serializable;
import java.math.BigDecimal;

import javax.persistence.*;


/**
 * The persistent class for the exposition database table.
 *
 */
@Entity
@Table(name="exposition")
public class Exposition implements Serializable {
	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue
	@Column(updatable=false)
	private int id;

	private int area;

    @Lob()
	private String description;

	private String name;

	//bi-directional many-to-one association to Contract
    @ManyToOne
	@JoinColumn(name="contractNumber")
	private Contract contract;

	//bi-directional many-to-one association to Field
    @ManyToOne
	@JoinColumn(name="fieldId")
	private Field field;

	//bi-directional many-to-one association to Exhibition
    @ManyToOne
	@JoinColumn(name="exhibitionId")
	private Exhibition exhibition;

	//bi-directional many-to-one association to Pavilion
    @ManyToOne
	@JoinColumn(name="pavilionId")
	private Pavilion pavilion;

    public Exposition() {
    }

	public int getId() {
		return this.id;
	}

	private void setId(int id) {
		this.id = id;
	}

	public int getArea() {
		return this.area;
	}

	public void setArea(int area) {
		this.area = area;
	}

	public String getDescription() {
		return this.description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public Contract getContract() {
		return this.contract;
	}

	public void setContract(Contract contract) {
		this.contract = contract;
	}

	public Field getField() {
		return this.field;
	}

	public void setField(Field field) {
		this.field = field;
	}

	public Exhibition getExhibition() {
		return this.exhibition;
	}

	public void setExhibition(Exhibition exhibition) {
		this.exhibition = exhibition;
	}

	public Pavilion getPavilion() {
		return this.pavilion;
	}

	public void setPavilion(Pavilion pavilion) {
		this.pavilion = pavilion;
	}
	
	public BigDecimal getPrice() {
		return (new BigDecimal(getArea())).multiply(new BigDecimal(getPavilion().getM2Price()));
	}
}
