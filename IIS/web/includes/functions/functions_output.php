<?php
// Encoding: utf8
/**
 * Functions for output transforming to be safely viewable.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Modifies a variable to a form in which it can be safely "outputed".
 *
 * @param mixed Variable to be outputted.
 * @return mixed Variable in the from that can be safely "outputed".
 */
function safe_output_string($string)
{
	// Anything but strings is useless to "safe"
	if (is_string($string)) {
		// First we need to decode all HTML entities because this
		// function can be called on the same string (or on a string
		// that contains a string produced by this function) more than once
		$string = html_entity_decode($string, ENT_QUOTES, 'UTF-8');
		// Strip slashes (\" and \')
 		$string = stripslashes($string);
		// Finally, convert all HTML entities to their applicable characters
		$string = htmlentities($string, ENT_QUOTES, 'UTF-8');
	}

	return $string;
}

/**
 * Transforms utf8 string into unicode string. It is "a part" of the
 * cloak_email() function.
 * Copyright (C) 2007 Carlo Perassi <carlo@perassi.org>
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 *
 * Homepage: http://perassi.org/2007/09/24/an-accessible-email-cloaking-technique/
 *
 * @param string $str Utf-8 string.
 * @return string Unicode string.
 */
function cloak_email_utf8_to_unicode($str)
{
	$unicode = array();
	$values = array();
	$lookingFor = 1;
	$strlen_str = strlen($str);

	for ($i = 0; $i < $strlen_str; $i++ ) {
		$thisValue = ord( $str[ $i ] );

		if ( $thisValue < 128 ) {
			$unicode[] = $thisValue;
		} else {
			if ( count( $values ) == 0 ) {
				$lookingFor = ( $thisValue < 224 ) ? 2 : 3;
			}

			$values[] = $thisValue;

			if ( count( $values ) == $lookingFor ) {
				$number = ( $lookingFor == 3 ) ?
				( ( $values[0] % 16 ) * 4096 ) + ( ( $values[1] % 64 ) * 64 ) + ( $values[2] % 64 ):
				( ( $values[0] % 32 ) * 64 ) + ( $values[1] % 64 );

				$unicode[] = $number;
				$values = array();
				$lookingFor = 1;
			}
		}
	}

	return $unicode[0];
}

/**
 * aec - accessible email cloacking
 * Copyright (C) 2007 Carlo Perassi <carlo@perassi.org>
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 *
 * Homepage: http://perassi.org/2007/09/24/an-accessible-email-cloaking-technique/
 *
 * References:
 * http://www.csarven.ca/hiding-email-addresses
 * http://rumkin.com/tools/mailto_encoder/custom.php
 * http://www.randomchaos.com/documents/?source=php_and_unicode
 *
 * @param string $plain_email Email address.
 * @param string $mailto Result will be in the form <a href="mailto:email">email</a>
 * @param string $text Text to be shown instead of email address (only usable with $mailto == true).
 * @return string Cloaked email.
 */
function cloak_email($plain_email, $mailto = false, $text = '')
{
    $l = strlen($plain_email);

    $xemail = array();
    for ($i = 0; $i < $l; $i++) {
        $xemail[$i] = cloak_email_utf8_to_unicode($plain_email[$i]);
    }

    $oemail = '<a href="&#109;&#97;&#105;&#108;&#116;&#111;&#58;'; // "mailto:"
    if ($mailto) {
        for ($i = 0; $i < $l; $i++) {
            $oemail .= '&#' . $xemail[$i] . ';';
        }
        $oemail .= '">';
        if ($text) {
            $l = strlen($text);

            $xemail = array();
            for ($i = 0; $i < $l; $i++) {
                // I reuse xemail as a container for text in hex
                $xemail[$i] = cloak_email_utf8_to_unicode($text[$i]);
            }
        }
    } else {
        $oemail = '';
    }
    for ($i = 0; $i < $l; $i++) {
        $oemail .= '&#' . $xemail[$i] . ';';

        $oemail .= '<!-- ';
        rand(0, 1) ? $oemail .= '>' : $oemail .= '@';
        $oemail .= ' -->';
    }
    if ($mailto) $oemail .= '</a>';

    return $oemail;
}
?>