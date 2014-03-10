/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.controller;

import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Collection;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;
import org.jfree.data.general.DefaultPieDataset;
import org.jfree.data.general.PieDataset;

import javax.faces.context.FacesContext;
import javax.persistence.EntityManager;

import pis.model.Company;
import pis.model.Contract;
import pis.model.Exhibition;
import pis.model.Exposition;
import pis.model.Pavilion;

/**
 * @brief Statistics and economics info.
 */
public class Stats {
	private static final int NUMBER_OF_MONTHS_IN_YEAR = 12;
	private static final NumberFormat CHARTS_FORMATTER = new DecimalFormat("#,##0.00");
	private static final String DOUBLE_FORMAT = "#,###,##0.00";
	private EntityManager em;

	/**
	 * @brief Default construtor.
	 */
	public Stats() {
		em = Db.getEntityManager();
	}

	/**
	 * @brief Returns the total owed price by companies.
	 */
	public BigDecimal getTotalOwedPrice() {
		BigDecimal totalOwedPrice = new BigDecimal(0);
		for (Company company : getAllCompanies()) {
			em.refresh(company);
			for (Contract contract : company.getContracts()) {
				em.refresh(contract);
				BigDecimal toAdd = contract.getPrice().subtract(contract.getSettled());
				// Do not add negative values (if the settled price
				// is higher than the owed price)
				if (toAdd.compareTo(new BigDecimal(0)) > 0) {
					totalOwedPrice = totalOwedPrice.add(toAdd);
				}
			}
		}
		return totalOwedPrice;
	}

	/**
	 * @brief Returns the total settled price by companies.
	 */
	public BigDecimal getTotalSettledPrice() {
		BigDecimal totalSettledPrice = new BigDecimal(0);
		for (Company company : getAllCompanies()) {
			em.refresh(company);
			for (Contract contract : company.getContracts()) {
				em.refresh(contract);
				totalSettledPrice = totalSettledPrice.add(contract.getSettled());
			}
		}
		return totalSettledPrice;
	}

	/**
	 * @brief Returns the total number of pavilions.
	 */
	public Long getTotalNumberOfPavilions() {
		String query = "SELECT COUNT(p) FROM Pavilion p";
		return (Long) em.createQuery(query).getSingleResult();
	}

	/**
	 * @brief Returns the total number of exhibitions.
	 */
	public Long getTotalNumberOfExhibitions() {
		String query = "SELECT COUNT(e) FROM Exhibition e";
		return (Long) em.createQuery(query).getSingleResult();
	}

	/**
	 * @brief Returns the total number of expositions.
	 */
	public Long getTotalNumberOfExpositions() {
		String query = "SELECT COUNT(e) FROM Exposition e";
		return (Long) em.createQuery(query).getSingleResult();
	}

	/**
	 * @brief Returns the total number of companies.
	 */
	public Long getTotalNumberOfCompanies() {
		String query = "SELECT COUNT(c) FROM Company c";
		return (Long) em.createQuery(query).getSingleResult();
	}

	/**
	 * @brief Returns the average number of expositions on one exhibition.
	 */
	public Double getAverageNumberOfExpositionsOnExhibition() {
		return new Double(getTotalNumberOfExpositions()) /
			getTotalNumberOfExhibitions();
	}

	/**
	 * @brief Returns the formatter for doubles.
	 */
	public String getDoubleFormat() {
		return DOUBLE_FORMAT;
	}

	/**
	 * @brief Returns the formatter for charts.
	 */
	public NumberFormat getChartsFormatter() {
		return CHARTS_FORMATTER;
	}

	/**
	 * @brief Returns the current year.
	 */
	public int getCurrentYear() {
		return Calendar.getInstance().get(Calendar.YEAR);
	}

	/**
	 * @brief Returns dataset for the revenue chart.
	 *
	 * Note: Currently, only revenue from expositions is counted in.
	 */
	public CategoryDataset getRevenueDataset() {
		DefaultCategoryDataset dataset = new DefaultCategoryDataset();
		String query = "SELECT e FROM Exhibition e";
		Collection<Exhibition> exhibitions = em.createQuery(query).getResultList();

		// Count the revenue for the current year for each month
		for (int m = 1; m <= NUMBER_OF_MONTHS_IN_YEAR; m++) {
			BigDecimal revenue = new BigDecimal(0);
			for (Exhibition exhibition : exhibitions) {
				em.refresh(exhibition);
				// We are only interested in the current year and month
				// (see http://java.sun.com/j2se/1.4.2/docs/api/java/util/Date.html#getYear%28%29
				// for the reason we add 1900 to the year)
				Date startDate = exhibition.getStartDate();
				if ((startDate.getYear() + 1900) != getCurrentYear() ||
					(startDate.getMonth()) != m) {
					continue;
				}

				for (Exposition exposition : exhibition.getExpositions()) {
					em.refresh(exposition);
					revenue = revenue.add(exposition.getPrice());
				}
			}
			dataset.addValue(revenue, "", Integer.toString(m));
		}
		return dataset;
	}

	/**
	 * @brief Returns dataset for the exposition distribution chart.
	 */
	public PieDataset getExpositionDistributionDataset() {
		DefaultPieDataset dataset  = new DefaultPieDataset();
		for (Company company : getAllCompanies()) {
			em.refresh(company);
			// Count the number of expositions for this company
			int numberOfExpositions = 0;
			for (Contract contract : company.getContracts()) {
				em.refresh(contract);
				numberOfExpositions += contract.getExpositions().size();
			}
			dataset.setValue(company.getName(), numberOfExpositions);
		}
		return dataset;
	}

	/**
	 * @brief Returns dataset for the pavilion distribution for the current exhibition.
	 */
	public PieDataset getExpositionDistributionInPavilionsInCurrentExhibitionDataset() {
		// Obtain the current exhibition
		FacesContext context = FacesContext.getCurrentInstance();
		ExhibitionList exhibitionList = (ExhibitionList) context.getApplication().evaluateExpressionGet(context,
				"#{exhibitionList}", ExhibitionList.class);
		Exhibition exhibition = exhibitionList.getExhibition();

		// Initialize a mapping between pavilion name and the number
		// of expositions in it in the current exhibition
		Map<String, Long> map = new HashMap<String, Long>();
		for (Pavilion pavilion : getAllPavilions()) {
			map.put(pavilion.getName(), new Long(0));
		}

		// Compute the number of expositions per pavilion
		for (Exposition exposition : exhibition.getExpositions()) {
			em.refresh(exposition);
			String pavilionName = exposition.getPavilion().getName();
			Long oldValue = map.get(pavilionName);
			map.put(pavilionName, oldValue + 1);
		}

		// Compute the dataset
		DefaultPieDataset dataset = new DefaultPieDataset();
		for (Map.Entry<String, Long> entry : map.entrySet()) {
			// Do not insert pavilions with no exposition
			if (entry.getValue() > 0) {
				dataset.setValue(entry.getKey(), entry.getValue());
			}
		}
		return dataset;
	}

	/**
	 * @brief Returns the collection of all companies.
	 */
	private Collection<Company> getAllCompanies() {
		String query = "SELECT c FROM Company c";
		return em.createQuery(query).getResultList();
	}

	/**
	 * @brief Returns the collection of all pavilions.
	 */
	private Collection<Pavilion> getAllPavilions() {
		String query = "SELECT c FROM Pavilion c";
		return em.createQuery(query).getResultList();
	}
}
