/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.validator;

import java.util.Date;
import javax.faces.component.UIComponent;
import javax.faces.component.UIInput;
import javax.faces.context.FacesContext;
import javax.faces.validator.Validator;
import javax.faces.validator.ValidatorException;
import javax.faces.application.FacesMessage;

import pis.lang.Lang;

/**
 * @brief Validates that interval specified by two dates is valid.
 */
public class DateIntervalValidator implements Validator {
	private static final String VALIDATION_ERROR_NAME = "invalidDateInterval";

	@Override
	public void validate(FacesContext arg0, UIComponent component, Object value)
			throws ValidatorException {
		Date endDate = (Date) value;

		// Obtain the ID of the start date field from f:attribute
		String startDateId = (String) component.getAttributes().get("startDateId");

		// Find the actual JSF component for the start date ID
		UIInput startDateInput = (UIInput) FacesContext.getCurrentInstance(
			).getViewRoot().findComponent(startDateId);

		// Get its value
		Date startDate = (Date) startDateInput.getValue();

		// Validate the interval
		if (startDate == null || endDate == null ||
				startDate.getTime() > endDate.getTime()) {
			throw new ValidatorException(new FacesMessage(
				Lang.getLocalizedString(VALIDATION_ERROR_NAME)));
		}
	}
}
