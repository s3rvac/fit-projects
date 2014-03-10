<?php
// Encoding: utf8
/**
 * Functions for loading data from database.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Loads available languages and returns them in an associtave array.
 *
 * @return array Associative array in the form language_id => short_name.
 */
function load_available_languages()
{
	$sql = 'SELECT language_id, short_name
		FROM ' . LANGUAGES_TABLE . '
		ORDER BY language_id DESC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load available languages.',
			$sql, mysql_error());
	}

	$languages = array();
	while ($language = mysql_fetch_assoc($result))	{
		$languages[$language['language_id']] = $language['short_name'];
	}
	mysql_free_result($result);

	return $languages;
}

/**
 * Loads available groups and returns them in an associtave array.
 *
 * @return array Associative array in the form group_id => name.
 */
function load_available_groups()
{
	$sql = 'SELECT group_id, name
		FROM ' . GROUPS_TABLE . '
		ORDER BY group_id ASC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load available groups.',
			$sql, mysql_error());
	}

	$groups = array();
	while ($group = mysql_fetch_assoc($result))	{
		$groups[$group['group_id']] = $group['name'];
	}
	mysql_free_result($result);

	return $groups;
}

/**
 * Loads available users and returns them in an array.
 *
 * @return array Array of arrays of user data.
 */
function load_available_users()
{
	$sql = 'SELECT user_id, username, name, password, email, telephone, group_id, company_id
		FROM ' . USERS_TABLE . '
		ORDER BY name ASC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load available users.',
			$sql, mysql_error());
	}

	$users = array();
	while ($user = mysql_fetch_assoc($result))	{
		array_push($users, $user);
	}
	mysql_free_result($result);

	return $users;
}

/**
 * Loads available companies and returns them in an array.
 *
 * @return array Array of arrays of company data.
 */
function load_available_companies()
{
	$sql = 'SELECT company_id, name, address, email, www, telephone, IC
		FROM ' . COMPANIES_TABLE . '
		ORDER BY name ASC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load available companies.',
			$sql, mysql_error());
	}

	$companies = array();
	while ($company = mysql_fetch_assoc($result))	{
		array_push($companies, $company);
	}
	mysql_free_result($result);

	return $companies;
}

/**
 * Loads available contracts and returns them in an array.
 *
 * @return array Array of arrays of contract data.
 */
function load_available_contracts()
{
	$sql = 'SELECT contract_id, company_id, concluded_by, conclusion_date, validity_start, validity_end, maturity_date
		FROM ' . CONTRACTS_TABLE . '
		ORDER BY contract_id ASC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load available contracts.',
			$sql, mysql_error());
	}

	$contracts = array();
	while ($contract = mysql_fetch_assoc($result))	{
		array_push($contracts, $contract);
	}
	mysql_free_result($result);

	return $contracts;
}

/**
 * Loads available exhibitions and returns them in an array.
 *
 * @return array Array of arrays of exhibition data.
 */
function load_available_exhibitions()
{
	$sql = 'SELECT exhibition_id, name, start, end
		FROM ' . EXHIBITIONS_TABLE . '
		ORDER BY name ASC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load available exhibitions.',
			$sql, mysql_error());
	}

	$exhibitions = array();
	while ($exhibition = mysql_fetch_assoc($result))	{
		array_push($exhibitions, $exhibition);
	}
	mysql_free_result($result);

	return $exhibitions;
}

/**
 * Loads available branches and returns them in an array.
 *
 * @return array Array of arrays of branch data.
 */
function load_available_branches()
{
	$sql = 'SELECT branch_id, name
		FROM ' . BRANCHES_TABLE . '
		ORDER BY name ASC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load available branches.',
			$sql, mysql_error());
	}

	$branches = array();
	while ($branch = mysql_fetch_assoc($result))	{
		array_push($branches, $branch);
	}
	mysql_free_result($result);

	return $branches;
}

/**
 * Loads available price_categories and returns them in an associtave array.
 *
 * @return array Associative array in the form price_category_id => name.
 */
function load_available_price_categories()
{
	$sql = 'SELECT price_category_id, name
		FROM ' . PRICE_CATEGORIES_TABLE . '
		ORDER BY price_for_m2 ASC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load available price_categories.',
			$sql, mysql_error());
	}

	$price_categories = array();
	while ($price_category = mysql_fetch_assoc($result))	{
		$price_categories[$price_category['price_category_id']] = $price_category['name'];
	}
	mysql_free_result($result);

	return $price_categories;
}

/**
 * Loads available pavilions and returns them in an associtave array.
 *
 * @return array Associative array in the form pavilion_id => name.
 */
function load_available_pavilions()
{
	$sql = 'SELECT pavilion_id, name
		FROM ' . PAVILIONS_TABLE . '
		ORDER BY name ASC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load available pavilions.',
			$sql, mysql_error());
	}

	$pavilions = array();
	while ($pavilion = mysql_fetch_assoc($result))	{
		$pavilions[$pavilion['pavilion_id']] = $pavilion['name'];
	}
	mysql_free_result($result);

	return $pavilions;
}

/**
 * Loads available places and returns them in an array.
 *
 * @return array Array of arrays of place data.
 */
function load_available_places()
{
	$sql = 'SELECT pl.place_id, pl.number, pl.area, pl.pavilion_id, pa.name AS pavilion, pl.price_category_id
		FROM ' . PLACES_TABLE . ' pl LEFT JOIN ' . PAVILIONS_TABLE . ' pa ON pl.pavilion_id = pa.pavilion_id
		ORDER BY pavilion ASC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load available places.',
			$sql, mysql_error());
	}

	$places = array();
	while ($place = mysql_fetch_assoc($result))	{
		array_push($places, $place);
	}
	mysql_free_result($result);

	return $places;
}

/**
 * Loads a row from the selected table identified by an ID.
 *
 * @param int $row_id ID of the row.
 * @param string $row_id_column Name of the column with IDs.
 * @param string $table Name of the table.
 * @return array Array of all data in the row. If there is no row associated
 * with the selected ID, empty array will be returned.
 */
function load_row_from_table($row_id, $row_id_column, $table)
{
	$sql = 'SELECT *
		FROM ' . $table . "
		WHERE $row_id_column = $row_id
		LIMIT 1";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load row from the selected table.",
			$sql, mysql_error());
	} else if (mysql_num_rows($result) != 1) {
		return array();
	}

	$row_data = mysql_fetch_assoc($result);
	mysql_free_result($result);
	return $row_data;
}

/**
 * Loads expositions that are on the selected exhibition.
 *
 * @param int $exhibition_id Exhibition ID.
 * @return array Array of exposition IDs.
 */
function load_expositions_on_exhibition($exhibition_id)
{
	$sql = 'SELECT exposition_id
		FROM ' . EXPOSITIONS_TABLE . "
		WHERE exhibition_id = $exhibition_id
		ORDER BY exposition_id ASC";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load expositions.',
			$sql, mysql_error());
	}

	$expositions = array();
	while ($exposition = mysql_fetch_assoc($result))	{
		array_push($expositions, $exposition['exposition_id']);
	}
	mysql_free_result($result);

	return $expositions;
}
?>