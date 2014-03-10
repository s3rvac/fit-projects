<?php
// Encoding: utf8
/**
 * "get ..." functions.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * @return string Name of the group according to the selected group ID.
 * @param int $group_id Group ID.
 */
function get_group_name($group_id)
{
	$sql = 'SELECT name
		FROM ' . GROUPS_TABLE . "
		WHERE group_id = $group_id
		LIMIT 1";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot get name of the group.',
			$sql, mysql_error());
	}
	$row = mysql_fetch_assoc($result);
	mysql_free_result($result);

	return $row['name'];
}

/**
 * @return array Array of all columns from the selected table.
 * @param string $table Name of the table.
 */
function get_number_of_rows($table, $where_clause = '')
{
	$sql = "SELECT COUNT(*) AS number_of_rows
		FROM $table
		$where_clause";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot get number of rows in the table.',
			$sql, mysql_error());
	}
	$row = mysql_fetch_assoc($result);
	mysql_free_result($result);

	return $row['number_of_rows'];
}

/**
 * Returns (first) value that corresponds to the another value.
 *
 * @param string $value_column Column name in which the corresponding value should exists.
 * @param mixed $other_value Other value (to which a corresponding one is searched).
 * @param string $other_value_column Column name in which the other value should exists.
 * @param string $table Name of the table.
 * @return mixed First value that corresponds to the another value. Null if the
 * corresponding value doesn't exist.
 */
function get_value_corresponding_to_other_value($value_column, $other_value,
	$other_value_column, $table)
{
	$sql = "SELECT $value_column AS value
		FROM " . $table . "
		WHERE $other_value_column = '$other_value'
		LIMIT 1";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot get corresponding value.',
			$sql, mysql_error());
	}

	if (mysql_num_rows($result) == 0) {
		mysql_free_result($result);
		return null;
	}
	$corresponding_row = mysql_fetch_assoc($result);
	$corresponding_value = $corresponding_row['value'];
	mysql_free_result($result);

	return $corresponding_value;
}

/**
 * @return array Array of all columns from the selected table.
 * @param string $table Name of the table.
 */
function get_table_columns($table)
{
	$sql = "SHOW COLUMNS FROM $table";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot get columns in the selected table.',
			$sql, mysql_error());
	}

	$table_columns = array();
	while ($row = mysql_fetch_assoc($result)) {
		array_push($table_columns, $row['Field']);
	}
	mysql_free_result($result);

	return $table_columns;
}

/**
 * @return double Total price for the selected exposition.
 * @param int $exposition_id Exposition ID.
 */
function get_price_for_exposition($exposition_id)
{
	$exposition_data = load_row_from_table($exposition_id, 'exposition_id', EXPOSITIONS_TABLE);

	$sql = 'SELECT pl.area, pc.price_for_m2
		FROM ' . PLACES_TABLE . ' pl LEFT JOIN ' . PRICE_CATEGORIES_TABLE . " pc ON pl.price_category_id = pc.price_category_id
		WHERE pl.place_id = {$exposition_data['place_id']}
		LIMIT 1";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load price for the selected exposition.',
			$sql, mysql_error());
	} else if (mysql_num_rows($result) != 1) {
		return 0;
	}

	$place_row = mysql_fetch_assoc($result);
	$total_price += $place_row['area'] * $place_row['price_for_m2'];
	mysql_free_result($result);

	return $total_price;
}

/**
 * @return int Forthcoming (current exhibition or the next one) exhibition ID.
 * Returns 0 if there is no forthcoming exhibition.
 */
function get_forthcoming_exhibition_id()
{
	$time = time();

	$forthcoming_exhibition_id = 0;
	$nearest_exhibition_time = PHP_INT_MAX;
	$exhibitions = load_available_exhibitions();
	foreach ($exhibitions as $exhibition) {
		if ($time < $exhibition['end']) {
			if ($time >= $exhibition['start']) {
				// This exhibition is currently in progress
				return $exhibition['exhibition_id'];
			}

			$exhibition_time = abs($exhibition['start'] - $time);
			if ($exhibition_time < $nearest_exhibition_time) {
				$nearest_exhibition_time = $exhibition_time;
				$forthcoming_exhibition_id = $exhibition['exhibition_id'];
			}
		}
	}

	return $forthcoming_exhibition_id;
}

/**
 * @return array Array of all debtor IDs.
 */
function get_all_debtor_ids()
{
	$sql = 'SELECT com.company_id, SUM(con.price) AS total_price, SUM(con.settled) AS settled
		FROM ' . COMPANIES_TABLE . ' com LEFT JOIN ' . CONTRACTS_TABLE . ' con ON com.company_id = con.company_id
		GROUP BY com.company_id
		HAVING settled < total_price';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load debtor IDs.",
			$sql, mysql_error());
	}

	$debtor_ids = array();
	while ($row = mysql_fetch_assoc($result)) {
		array_push($debtor_ids, $row['company_id']);
	}
	mysql_free_result($result);

	return $debtor_ids;
}

/**
 * @return int Pavilion ID for the selected place.
 * @param int $place Place ID.
 */
function get_pavilion_id_for_place($place_id)
{
	return get_value_corresponding_to_other_value('pavilion_id', $place_id,
		'place_id', PLACES_TABLE);
}
?>