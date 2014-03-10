/*
 * Encoding: utf-8
 * Project:  PIS 2010
 * Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
 *           Boris Procházka, xproch63@stud.fit.vutbr.cz
 *           Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

package pis.validator;

import javax.faces.component.UIComponent;
import javax.faces.context.FacesContext;
import javax.faces.validator.Validator;
import javax.faces.validator.ValidatorException;
import javax.faces.application.FacesMessage;

import pis.lang.Lang;

/**
 * @brief Converts RC to String and vice-versa
 *
 */
public class RCValidator implements Validator {
	private static final String RC_VALIDATION_FAILED = "rcValidationFailed";
	private static final FacesMessage ERROR_STRING = new FacesMessage(
		Lang.getLocalizedString(RC_VALIDATION_FAILED));

	@Override
	public void validate(FacesContext arg0, UIComponent component, Object value) throws ValidatorException {
		try {
			String rcString = (String) value;
			if (rcString.length() == 11 && rcString.charAt(6) == '/') {
				long year = Long.parseLong(rcString.substring(0, 2));
				long month = Long.parseLong(rcString.substring(2, 4));
				long day = Long.parseLong(rcString.substring(4, 6));
				long control = Long.parseLong(rcString.substring(7));
				long sum = year + month + day + control;
				if ((sum % 11) != 0) {
					throw new ValidatorException(ERROR_STRING);
				}
			} else {
				throw new ValidatorException(ERROR_STRING);
			}
		} catch (Exception e) {
			throw new ValidatorException(ERROR_STRING);
		}
	}
}
