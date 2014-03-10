<?php
// Encoding: utf8
/**
 * Validation functions.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Validates selected md5 hash.
 *
 * @param string $md5hash MD5 hash.
 * @return bool True, if the seleced md5hash is valid, false otherwise
 */
function is_valid_md5hash($md5hash)
{
	return preg_match('/^[A-Za-z0-9]*$/', $md5hash) &&
		(strlen($md5hash) == 32);
}

/**
 * Validates selected email address format.
 *
 * @param string $email Email address.
 * @return bool True, if the seleced seleced email address has valid format, false otherwise.
 */
function is_valid_email_format($email)
{
	return (!empty($email) &&
		preg_match('/^[a-z0-9&\'\.\-_\+]+@[a-z0-9\-]+\.([a-z0-9\-]+\.)*?[a-z]+$/is', $email));
}

/**
 * Checks whether the selected language is available or not.
 *
 * @param string $short_name Short name of the language
 * @return bool True, if the seleced language is available, false otherwise.
 */
function is_valid_language($short_name)
{
	$sql = "SELECT short_name
		FROM " . LANGUAGES_TABLE . "
		WHERE short_name = '$short_name'";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot check language name.',
			$sql, mysql_error());
	}

	$is_valid_language = mysql_num_rows($result) == 1;
	mysql_free_result($result);
	return $is_valid_language;
}

/**
 * Checks whether the selected module name is valid (exists) or not.
 *
 * @param string $module_name Name of the module.
 * @return bool True, if the selected module name is valid, false otherwise.
 */
function is_valid_module_name($module_name)
{
	global $modules_path;

	foreach (scandir($modules_path) as $entry) {
		if ($entry != '.' && $entry != '..') {
			if (!is_dir($entry) && ($entry == 'm_' . $module_name . '.php')) {
				return true;
			}
		}
	}

	return false;
}

/**
 * Checks whether the selected table can be ordered by the selected column.
 *
 * @param string $table Name of the table.
 * @param string $order_by Name of the column in the table.
 * @return bool True, if the selected table can be ordered by the selected
 * column, false otherwise.
 */
function is_valid_order_by($table, $order_by)
{
	// Note: This method cannot be used when a user wants to order some data by
	// a column which is an ID, so I need to use the 2nd variant
	//
	// 	$table_columns = get_table_columns($table);
	// 	return in_array($order_by, $table_columns);

	if (empty($order_by)) {
		return false;
	}

	switch ($table) {
		case USERS_TABLE:
			return in_array($order_by, array(
				'username', 'name', 'group_id', 'company',
				));
			break;

		case COMPANIES_TABLE:
			return in_array($order_by, array(
				'name', 'address', 'IC',
				));
			break;

		case EXHIBITIONS_TABLE:
			return in_array($order_by, array(
				'name', 'start', 'end',
				));
			break;

		case BRANCHES_TABLE:
			return in_array($order_by, array(
				'name',
				));
			break;

		case PRICE_CATEGORIES_TABLE:
			return in_array($order_by, array(
				'name', 'price_for_m2',
				));
			break;

		case PAVILIONS_TABLE:
			return in_array($order_by, array(
				'name', 'number_of_floors',
				));
			break;

		case PLACES_TABLE:
			return in_array($order_by, array(
				'number', 'pavilion', 'price_category',
				));
			break;

		case CONTRACTS_TABLE:
			return in_array($order_by, array(
				'contract_id', 'company', 'conclusion_date',
				));
			break;

		case EXPOSITIONS_TABLE:
			return in_array($order_by, array(
				'contract_id', 'exhibition', 'place_number',
				));
			break;

		default:
			return false;
	}
}

/**
 * Checks whether the selected ID is valid in the selected table
 * (and column) or not.
 *
 * @param int $id ID.
 * @param string $id_column Name of the column with IDs.
 * @param string $table Name of the table.
 * @return bool True, if the seleced ID is valid, false otherwise.
 */
function is_valid_id($id, $id_column, $table)
{
	$sql = 'SELECT *
		FROM ' . $table . "
		WHERE $id_column = $id
		LIMIT 1";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot check ID validity.',
			$sql, mysql_error());
	}

	$is_valid_id = mysql_num_rows($result) == 1;
	mysql_free_result($result);

	return $is_valid_id;
}

/**
 * Checks whether the selected IC is valid or not.
 *
 * @param string $ic IC.
 * @return bool True, if the selected IC is valid, false otherwise.
 */
function is_valid_IC($ic)
{
	return !empty($ic) && preg_match('#^\d{8}$#', $ic);
}

/**
 * Checks whether the selected DIC is valid or not.
 *
 * @param string $dic DIC.
 * @return bool True, if the selected DIC is valid, false otherwise.
 */
function is_valid_DIC($dic)
{
	return !empty($dic) && preg_match('#^[A-Z][A-Z]\d{8}$#', $dic);
}

/**
 * Checks whether the selected price is valid or not.
 *
 * @param string $price Price.
 * @return bool True, if the selected price is valid, false otherwise.
 */
function is_valid_price($price, $less_than_zero = false, $equal_to_zero = true)
{
	if ($price < 0.0 && !$less_than_zero) {
		return false;
	} else if ($price == 0.0 && !$equal_to_zero) {
		return false;
	}

	return true;
}

/**
 * Checks whether the selected date format is valid or not.
 * Valid formats are YYYY-MM-DD-HH-MM and YYYY-MM-DD-HH-MM-SS
 *
 * @param string $price Price.
 * @return bool True if the selected date format is valid, false otherwise.
 */
function is_valid_date_format($date_format)
{
	return preg_match('/^[0-9]{4}-[0-9]+-[0-9]+-[0-9]+-[0-9]+(-[0-9]+)?$/', $date_format);
}
?>