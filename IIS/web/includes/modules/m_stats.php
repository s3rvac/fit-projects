<?php
// Encoding: utf8
/**
 * Stats module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * @return int Number of all exhibitions so far.
 */
function get_stat_number_of_all_exhibitions_so_far()
{
	return get_number_of_rows(EXHIBITIONS_TABLE, 'WHERE start < ' . time());
}

/**
 * @return int Number of all forthcoming exhibitions.
 */
function get_stat_number_of_all_forthcoming_exhibitions_so_far()
{
	return get_number_of_rows(EXHIBITIONS_TABLE, 'WHERE start > ' . time());
}

/**
 * @return int Number of all exhibitions so far.
 */
function get_stat_number_of_all_expositions_on_forthcoming_exhibition()
{
	return get_number_of_rows(EXPOSITIONS_TABLE, 'WHERE exhibition_id = ' .
		get_forthcoming_exhibition_id());
}

/**
 * @return int Average number of expositions on one exhibition.
 */
function get_stat_average_number_of_expositions_on_exhibition()
{
	$number_of_expositions = 0;
	$number_of_exhibitions = 0;
	$current_time = time();
	foreach (load_available_exhibitions() as $exhibition) {
		if ($exhibition['start'] <= $current_time) {
			$number_of_expositions += count(load_expositions_on_exhibition(
				$exhibition['exhibition_id']));
			$number_of_exhibitions++;
		}
	}

	if ($number_of_exhibitions != 0) {
		return $number_of_expositions / $number_of_exhibitions;
	} else {
		return 0;
	}
}

/**
 * @return int Average number of companies that have expositions
 * on one exhibition.
 */
function get_stat_average_number_of_companies_on_exhibition()
{
	$companies = array();
	$number_of_exhibitions = 0;
	$current_time = time();
	foreach (load_available_exhibitions() as $exhibition) {
		if ($exhibition['start'] <= $current_time) {
			foreach (load_expositions_on_exhibition(
				$exhibition['exhibition_id']) as $exposition) {
				$sql = 'SELECT con.company_id
					FROM ' . CONTRACTS_TABLE . ' con LEFT JOIN ' . EXPOSITIONS_TABLE . " ex
						ON con.company_id = ex.contract_id
					WHERE ex.exposition_id = {$exposition['exposition_id']}";
				$result = mysql_query($sql);
				if (!$result) {
					throw new IIS_DB_Exception('Cannot load contracts.',
						$sql, mysql_error());
				}

				$row = mysql_fetch_assoc($result);
				array_push($companies, $row['company_id']);
				mysql_free_result($result);
			}
			$number_of_exhibitions++;
		}
	}

	if ($number_of_exhibitions != 0) {
		$number_of_companies = count(array_unique($companies));
		return $number_of_companies / $number_of_exhibitions;
	} else {
		return 0;
	}
}

/**
 * Generates standard statistics (that are viewable by everyone).
 *
 * @return array Array of stats in the form 'name' => 'value'.
 */
function generate_standard_stats()
{
	global $lang;

	$standard_stats = array();
	$standard_stats[$lang['Stat_Number_Of_All_Exhibitions_So_Far']] =
		get_stat_number_of_all_exhibitions_so_far();
	$standard_stats[$lang['Stat_Number_Of_All_Forthcoming_Exhibitions']] =
		get_stat_number_of_all_forthcoming_exhibitions_so_far();
	$standard_stats[$lang['Stat_Number_Of_All_Expositions_On_Forthcoming_Exhibition']] =
		get_stat_number_of_all_expositions_on_forthcoming_exhibition();
	$standard_stats[''] = ''; // Delimiter
	$standard_stats[$lang['Stat_Average_Number_Of_Expositions_On_Exhibition']] =
		get_stat_average_number_of_expositions_on_exhibition();
	$standard_stats[$lang['Stat_Average_Number_Of_Companies_On_Exhibition']] =
		get_stat_average_number_of_companies_on_exhibition();
	$standard_stats[' '] = ''; // Delimiter
	$standard_stats[$lang['Stat_Number_Of_All_Pavilions']] =
		get_number_of_rows(PAVILIONS_TABLE);
	$standard_stats[$lang['Stat_Number_Of_All_Places']] =
		get_number_of_rows(PLACES_TABLE);

	return $standard_stats;
}

/**
 * @return double Total owed price by companies.
 */
function get_total_owed_price_by_companies()
{
	$sql = 'SELECT (SUM(price) - SUM(settled))
		FROM ' . CONTRACTS_TABLE;
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load total owed price by companies.",
			$sql, mysql_error());
	}

	$row = mysql_fetch_row($result);
	mysql_free_result($result);

	$total_owed_price = (int) $row[0];
	return $total_owed_price > 0 ? $total_owed_price : 0;
}

/**
 * @return double Total settled price by companies.
 */
function get_total_settled_price_by_companies()
{
	$sql = 'SELECT SUM(settled)
		FROM ' . CONTRACTS_TABLE;
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load total settled price by companies.",
			$sql, mysql_error());
	}

	$row = mysql_fetch_row($result);
	mysql_free_result($result);

	$total_settled_price = (int) $row[0];
	return $total_settled_price > 0 ? $total_settled_price : 0;
}

/**
 * Generates extended statistics (that are viewable only by a exhibitionground member).
 *
 * @return array Array of stats in the form 'name' => 'value'.
 */
function generate_extended_stats()
{
	global $lang;

	$extended_stats = array();
	$extended_stats[$lang['Stat_Total_Owed_Price_By_Companies']] =
		get_total_owed_price_by_companies() . ',-';
		$extended_stats[$lang['Stat_Total_Settled_Price_By_Companies']] =
		get_total_settled_price_by_companies() . ',-';
	$extended_stats[''] = ''; // Delimiter
	$extended_stats[$lang['Stat_Number_Of_All_Users']] =
		get_number_of_rows(USERS_TABLE);
	$extended_stats[$lang['Stat_Number_Of_All_Companies']] =
		get_number_of_rows(COMPANIES_TABLE);
	$extended_stats[$lang['Stat_Number_Of_All_Contracts']] =
		get_number_of_rows(CONTRACTS_TABLE);
	$extended_stats[$lang['Stat_Number_Of_All_Exhibitions']] =
		get_number_of_rows(EXHIBITIONS_TABLE);
	$extended_stats[$lang['Stat_Number_Of_All_Expositions']] =
		get_number_of_rows(EXPOSITIONS_TABLE);
	$extended_stats[$lang['Stat_Number_Of_All_Branches']] =
		get_number_of_rows(BRANCHES_TABLE);
	$extended_stats[$lang['Stat_Number_Of_All_Price_Categories']] =
		get_number_of_rows(PRICE_CATEGORIES_TABLE);
	$extended_stats[$lang['Stat_Number_Of_All_Pavilions']] =
		get_number_of_rows(PAVILIONS_TABLE);
	$extended_stats[$lang['Stat_Number_Of_All_Places']] =
		get_number_of_rows(PLACES_TABLE);

	return $extended_stats;
}

// General page data
$smarty->assign(array(
	'PAGE_HEADER' => $lang['Stats_Header'],
	'PAGE_DESCRIPTION' => $lang['Stats_Description'],
	'STANDARD_STATS' => generate_standard_stats(),
	'EXTENDED_STATS' => generate_extended_stats(),

	'L_STANDARD_STATS' => $lang['Standard_Stats'],
	'L_EXTENDED_STATS' => $lang['Extended_Stats'],
	'L_STAT_NAME' => $lang['Stat_Name'],
	'L_STAT_VALUE' => $lang['Stat_Value'],
	'L_NO_STATS' => $lang['No_Stats'],
	));

$smarty->display('stats.tpl');
?>