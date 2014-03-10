<?php
// Encoding: utf8
/**
 * Functions for input checking/transforming.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Escapes quotes, removes white-spaces from the beginning
 * and from the end of the $input_string and strips html tags.
 *
 * @param string $input_string Input string.
 * @param bool $trim Trim input string?
 * @param bool $strip_html_tags Strip all html tags?
 * @return string Modified $input_string according to the arguments.
 */
function handle_input_string($input_string, $trim = true, $strip_html_tags = false)
{
	global $dbc;

	// Check for earlier quote strip
	if (get_magic_quotes_gpc()) {
		$input_string = stripslashes($input_string);
	}

	if ($trim) {
		$input_string = trim($input_string);
	}

	if ($strip_html_tags) {
		$input_string = strip_tags($input_string);
	}

	return mysql_real_escape_string($input_string, $dbc);
}

/**
 * Handles input int and returns modified version of it (if necessary).
 * If the input number is invalid, it returns 0;
 *
 * @param mixed $number Number to handle.
 * @param int $min_value Minimal number value.
 * @param int $max_value Maximal number value.
 * @return int "Handled" number.
 */
function handle_input_int($number, $min_value = PHP_INT_MIN,
	$max_value = PHP_INT_MAX)
{
	$int_number = (int) $number;
	if ($number < $min_value) {
		return 0;
	} else if ($number > $max_value) {
		return 0;
	}

	return $number;
}

/**
 * Handles input double and returns modified version of it (if necessary).
 * If the input number is invalid, it returns 0.0;
 *
 * @param mixed $number Number to handle.
 * @return double "Handled" number.
 */
function handle_input_double($number)
{
	$normalized_number = trim(str_replace(',', '.', $number));

	$float_number_regexp = "-?(([0-9]+)|([0-9]+e-?[0-9]+)|((([0-9]+\.[0-9]*)|(\.[0-9]+))(e-?[0-9]+)?))";
	$regs = array();
	if (eregi($float_number_regexp, $normalized_number, $regs) != strlen($normalized_number)) {
		return 0.0;
	}
	$double_number = (float) $normalized_number;

	return $double_number;
}

/**
 * Handles input string information and returns modified version of it.
 * If there is some problem (like information length etc.), IIS_Input_Exception
 * exception will be thrown.
 *
 * @param string $information Information to be handled.
 * @param string $information_type Type of the information (like 'username').
 * @param int $min_information_length Minimal length of the information.
 * @param int $max_information_length Maximal length of the information.
 * @param bool $safe_string Will the input string be stored in the database?
 * @return string "Handled" information.
 */
function handle_input_string_information($information, $information_type,
	$min_information_length = 1, $max_information_length = 255, $safe_string = true)
{
	global $lang;

	$handled_information = $safe_string ? handle_input_string($information) : $information;
	if (empty($handled_information) && $min_information_length > 0) {
		throw new IIS_Input_Exception($lang['Error_Empty_' . ucfirst($information_type)]);
	} else if (strlen($handled_information) < $min_information_length) {
		throw new IIS_Input_Exception($lang['Error_Too_Short_' . ucfirst($information_type)]);
	} else if (strlen($handled_information) > $max_information_length) {
		throw new IIS_Input_Exception($lang['Error_Too_Long_' . ucfirst($information_type)]);
	}

	return $handled_information;
}

/**
 * Handles input email address and returns modified version of it. If there is some
 * problem (like password length etc.), IIS_Input_Exception exception will
 * be thrown.
 *
 * @param string $email Email address to be handled.
 * @return string "Handled" email.
 */
function handle_input_email($email)
{
	global $lang;

	if (!is_valid_email_format($email)) {
		throw new IIS_Input_Exception($lang['Error_Invalid_Email_Format']);
	}

	return $email;
}

/**
 * Handles input date and returns it in the UNIX timestamp format. If there is
 * some problem (like password length etc.), IIS_Input_Exception exception will
 * be thrown.
 *
 * @param string $date Date in the format YYYY-MM-DD-HH-MM.
 * @return int Date in the UNIX timestamp format.
 */
function handle_input_date($date)
{
	global $lang;

	$date = trim($date);
	if (empty($date)) {
		throw new IIS_Input_Exception($lang['Error_Empty_Date']);
	}

	if (!is_valid_date_format($date)) {
		throw new IIS_Input_Exception($lang['Error_Invalid_Date_Format']);
	}

	list($year, $month, $day, $hour, $minute) = explode('-', $date, 5);
	if (!checkdate($month, $day, $year)) {
		throw new IIS_Input_Exception($lang['Error_Invalid_Date']);
	}

	return mktime($hour, $minute, 0, $month, $day, $year);
}

/**
 * Handles input file and returns it's content. If there is some input problem
 * (like no input file was passed to this function etc.),
 * IIS_Input_Exception exception will be thrown.
 *
 * @param array $file Input file array ($_FILE['input_name']).
 * @return string File content.
 */
function handle_input_file($file)
{
	global $lang;

	if (empty($file['tmp_name'])) {
		throw new IIS_Input_Exception($lang['Error_No_File_Selected']);
	} else if ($file['size'] == 0) {
		throw new IIS_Input_Exception( $lang['Error_Empty_File']);
	} else if (!empty($file['error'])) {
		throw new IIS_Input_Exception($lang['Error_File_Transfer_Error']);
	}

	$fd = fopen($file['tmp_name'], 'r');
	if (!$fd) {
		throw new IIS_IO_Exception('Cannot open: ' . $file['tmp_name'] . '.');
	}

	$file_content = fread($fd, $file['size']);
	fclose($fd);

	return $file_content;
}
?>