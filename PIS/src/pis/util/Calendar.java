/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.util;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import javax.faces.context.FacesContext;

/**
 * @brief Bean for the calendar facelet.
 */
public class Calendar {
	private static final String DATE_FORMAT = "yyyy-MM-dd";

	private String dateFormat;
	private Date selectedDate;

	/**
	 * @brief Constructs a new Calendar object.
	 * @param[in] assocDate Associated date which is
	 *            changed when the calendar date change.
	 */
	public Calendar(Date assocDate) {
		dateFormat = DATE_FORMAT;
		selectedDate = assocDate;
	}

	public Locale getLocale() {
		return FacesContext.getCurrentInstance().getViewRoot().getLocale();
	}

	public String getPattern() {
		return dateFormat;
	}

	public void setPattern(String dateFormat) {
		this.dateFormat = dateFormat;
	}

	public Date getSelectedDate() {
		// Hack to produce the empty string when
		// there is no date set
		if (selectedDate.getTime() == 0) {
			return null;
		} else {
			return selectedDate;
		}
	}

	public void setSelectedDate(Date selectedDate) {
		this.selectedDate.setTime(selectedDate.getTime());
	}

	/**
	 * @brief Returns the selected date formatted according to the
	 *        curent pattern
	 */
	public String getSelectedDateFormatted() {
		SimpleDateFormat sdf = new SimpleDateFormat(dateFormat);
		return sdf.format(selectedDate);
	}
}
