<?php
// Encoding: utf8
/**
 * Search module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Performs a search operation on the users table and returns search results.
 *
 * @param string Search query.
 * @return array Search results.
 */
function search_company_members($query)
{
	global $lang;

	$search_results = array();

	$search_results['columns'] = array(
		$lang['Name'],
		$lang['Company'],
		$lang['Email'],
		);

	$search_results['data'] = array();
	$sql = 'SELECT u.user_id, u.group_id, u.name, u.email, u.company_id, c.name AS company
		FROM ' . COMPANIES_TABLE . ' c RIGHT JOIN ' . USERS_TABLE . ' u ON c.company_id = u.company_id
		WHERE u.group_id = ' . COMPANY_MEMBER . "
			AND (u.name LIKE '$query'
			OR u.email LIKE '$query'
			OR c.name LIKE '$query')
		ORDER BY u.name";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load company members.",
			$sql, mysql_error());
	}

	while ($row = mysql_fetch_assoc($result)) {
		$details_user_url = create_module_action_url('users', 'details');
		$details_user_url = add_query_to_url($details_user_url, 'user_id', $row['user_id']);
		$details_company_url = create_module_action_url('companies', 'details');
		$details_company_url = add_query_to_url($details_company_url, 'company_id', $row['company_id']);

		array_push($search_results['data'], array(
			array('url' => $details_user_url, 'value' => $row['name']),
			array('url' => $details_company_url, 'value' => $row['company']),
			array('url' => '', 'value' => $row['email']),
			));
	}
	mysql_free_result($result);

	return $search_results;
}

/**
 * Performs a search operation on the companies table and returns search results.
 *
 * @param string Search query.
 * @return array Search results.
 */
function search_companies($query)
{
	global $lang;

	$search_results = array();

	$search_results['columns'] = array(
		$lang['Company_Name'],
		$lang['Address'],
		$lang['Email'],
		$lang['IC'],
		);

	$search_results['data'] = array();
	$sql = 'SELECT company_id, name, address, email, IC
		FROM ' . COMPANIES_TABLE . "
		WHERE name LIKE '$query'
			OR address LIKE '$query'
			OR email LIKE '$query'
			OR IC LIKE '$query'
		ORDER BY name";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load companies.",
			$sql, mysql_error());
	}

	while ($row = mysql_fetch_assoc($result)) {
		$details_company_url = create_module_action_url('companies', 'details');
		$details_company_url = add_query_to_url($details_company_url, 'company_id', $row['company_id']);

		array_push($search_results['data'], array(
			array('url' => $details_company_url, 'value' => $row['name']),
			array('url' => '', 'value' => $row['address']),
			array('url' => '', 'value' => $row['email']),
			array('url' => '', 'value' => $row['IC']),
			));
	}
	mysql_free_result($result);

	return $search_results;
}

/**
 * Performs a search operation on the exhibitions table and returns search results.
 *
 * @param string Search query.
 * @return array Search results.
 */
function search_exhibitions($query)
{
	global $lang;

	$search_results = array();

	$search_results['columns'] = array(
		$lang['Exhibition_Name'],
		$lang['Exhibition_Start'],
		$lang['Exhibition_End'],
		);

	$search_results['data'] = array();
	$sql = 'SELECT exhibition_id, name, start, end
		FROM ' . EXHIBITIONS_TABLE . "
		WHERE name LIKE '$query'
		ORDER BY name";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load companies.",
			$sql, mysql_error());
	}

	while ($row = mysql_fetch_assoc($result)) {
		$details_exhibition_url = create_module_action_url('exhibitions', 'details');
		$details_exhibition_url = add_query_to_url($details_exhibition_url, 'exhibition_id', $row['exhibition_id']);

		array_push($search_results['data'], array(
			array('url' => $details_exhibition_url, 'value' => $row['name']),
			array('url' => '', 'value' => format_date($row['start'])),
			array('url' => '', 'value' => format_date($row['end'])),
			));
	}
	mysql_free_result($result);

	return $search_results;
}

/**
 * Performs a search operation on the expositions table and returns search results.
 *
 * @param string Search query.
 * @return array Search results.
 */
function search_expositions($query)
{
	global $lang;

	$search_results = array();

	$search_results['columns'] = array(
		$lang['Details'],
		$lang['Company'],
		$lang['Exhibition'],
		$lang['Branch'],
		$lang['Description'],
		);

	$search_results['data'] = array();
	$sql = 'SELECT ex.exposition_id, fa.name AS exhibition, fa.exhibition_id, ex.contract_id, ex.description,
				   con.company_id, com.name AS company, br.name AS branch
		FROM ' . EXPOSITIONS_TABLE . ' ex LEFT JOIN ' . EXHIBITIONS_TABLE . ' fa ON ex.exhibition_id = fa.exhibition_id
										  LEFT JOIN ' . BRANCHES_TABLE . ' br ON ex.branch_id = br.branch_id
										  LEFT JOIN ' . CONTRACTS_TABLE . ' con ON ex.contract_id = con.contract_id
										  LEFT JOIN ' . COMPANIES_TABLE . " com ON con.company_id = com.company_id
		WHERE fa.name LIKE '$query'
			OR br.name LIKE '$query'
			OR com.name LIKE '$query'
			OR ex.description LIKE '$query'
		ORDER BY fa.name";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load expositions.",
			$sql, mysql_error());
	}

	while ($row = mysql_fetch_assoc($result)) {
		$details_exposition_url = create_module_action_url('expositions', 'details');
		$details_exposition_url = add_query_to_url($details_exposition_url, 'exposition_id', $row['exposition_id']);
		$details_company_url = create_module_action_url('companies', 'details');
		$details_company_url = add_query_to_url($details_company_url, 'company_id', $row['company_id']);
		$details_exhibition_url = create_module_action_url('exhibitions', 'details');
		$details_exhibition_url = add_query_to_url($details_exhibition_url, 'exhibition_id', $row['exhibition_id']);

		array_push($search_results['data'], array(
			array('url' => $details_exposition_url, 'value' => $lang['Details']),
			array('url' => $details_company_url, 'value' => $row['company']),
			array('url' => $details_exhibition_url, 'value' => $row['exhibition']),
			array('url' => '', 'value' => $row['branch']),
			array('url' => '', 'value' => $row['description']),
			));
	}
	mysql_free_result($result);

	return $search_results;
}

/**
 * Performs a search operation on the pavilions table and returns search results.
 *
 * @param string Search query.
 * @return array Search results.
 */
function search_pavilions($query)
{
	global $lang;

	$search_results = array();

	$search_results['columns'] = array(
		$lang['Pavilion_Name'],
		$lang['Number_Of_Floors'],
		$lang['Number_Of_Restaurants'],
		$lang['Number_Of_Settlements'],
		);

	$search_results['data'] = array();
	$sql = 'SELECT pavilion_id, name, number_of_floors,
				number_of_restaurants, number_of_social_settlements
		FROM ' . PAVILIONS_TABLE . "
		WHERE name LIKE '$query'
		ORDER BY name";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load pavilions.",
			$sql, mysql_error());
	}

	while ($row = mysql_fetch_assoc($result)) {
		$details_pavilion_url = create_module_action_url('pavilions', 'details');
		$details_pavilion_url = add_query_to_url($details_pavilion_url, 'pavilion_id', $row['pavilion_id']);

		array_push($search_results['data'], array(
			array('url' => $details_pavilion_url, 'value' => $row['name']),
			array('url' => '', 'value' => $row['number_of_floors']),
			array('url' => '', 'value' => $row['number_of_restaurants']),
			array('url' => '', 'value' => $row['number_of_social_settlements']),
			));
	}
	mysql_free_result($result);

	return $search_results;
}

/**
 * Performs a search operation and returns search results.
 *
 * @param string Search query.
 * @return array Search results.
 */
function do_search($query)
{
	global $lang;

	// Replace '*' with '%' and '?' with '_' (because of MySQL LIKE command syntax)
	// and update query to possibly get more search results
	$query = str_replace('*', '%', $query);
	$query = str_replace('?', '_', $query);
	$query = str_replace(' ', '%', $query);
	$query = '%' . $query . '%';
	$query = ereg_replace('[%]+', '%', $query);

	// Do all available searches
	$search_results = array();
	$available_searches = array('Company_Members', 'Companies', 'Exhibitions',
		'Expositions', 'Pavilions');
	foreach ($available_searches as $search_name) {
		$searching_function = 'search_' . strtolower($search_name);
		array_push($search_results, array(
			'name' => $lang[$search_name],
			'results' => $searching_function($query)));
	}

	return $search_results;
}

// Check whether there is a search request
if (isset($_GET['query'])) {
	$query = handle_input_string($_GET['query']);
	if (!empty($query)) {
		$search_results = do_search($query);
		$results_found = false;
		foreach ($search_results as $search_result) {
			if (!empty($search_result) && !empty($search_result['results']) &&
				!empty($search_result['results']['data'])) {
				$results_found = true;
 				break;
			}
		}

		$smarty->assign(array(
			'SEARCH_DONE' => true,
			'SEARCH_RESULTS' => $search_results,
			'RESULTS_FOUND' => $results_found,

			'L_SEARCH_RESULTS' => $lang['Search_Results'],
			'L_NO_SEARCH_RESULTS' => $lang['No_Search_Results'],
			));
	}
}

// General page data
$smarty->assign(array(
	'U_SEARCH' => add_query_to_url(current_base_url(), 'module', 'search'),

	'PAGE_HEADER' => $lang['Search_Header'],
	'PAGE_DESCRIPTION' => $lang['Search_Description'],
	'QUERY' => isset($_GET['query']) ? $_GET['query'] : '',

	'L_SEARCH' => $lang['Search'],
	'L_SEARCH_QUERY' => $lang['Search_Query'],
	));

$smarty->display('search.tpl');
?>