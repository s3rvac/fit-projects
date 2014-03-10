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


/**
 * The persistent class for the businessField database table.
 *
 */
@Entity
@Table(name="businessField")
public class Field implements Serializable {
	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue
	@Column(updatable=false)
	private int id;

	private String name;

	//bi-directional many-to-one association to Exposition
	@OneToMany(mappedBy="field", fetch=FetchType.EAGER)
	private List<Exposition> expositions;

    public Field() {
    }

	public int getId() {
		return this.id;
	}

	private void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public List<Exposition> getExpositions() {
		return this.expositions;
	}

	public void setExpositions(List<Exposition> expositions) {
		this.expositions = expositions;
	}

	@Override
	public boolean equals(Object other) {
		return other instanceof Field &&
			getId() == ((Field) other).getId();
	}
}
