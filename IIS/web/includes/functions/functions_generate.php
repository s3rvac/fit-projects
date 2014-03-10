<?php
// Encoding: utf8
/**
 * Generating functions.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Generates a sidebar.
 *
 * @return array Generated sidebar.
 */
function generate_sidebar()
{
	global $user, $lang;

	$sidebar = array();

	// Sections accessible by all users
	$sidebar[$lang['Sidebar_Main_Menu']] = array();
	array_push($sidebar[$lang['Sidebar_Main_Menu']], array(
		'module_name' => 'main',
		'name' => $lang['Sidebar_Main_Page'],
		'url' => add_query_to_url(current_base_url(), 'module', 'main'),
		));
	array_push($sidebar[$lang['Sidebar_Main_Menu']], array(
		'module_name' => 'forthcoming_exhibition',
		'name' => $lang['Sidebar_Forthcoming_Exhibition'],
		'url' => add_query_to_url(current_base_url(), 'module', 'forthcoming_exhibition'),
		));
	array_push($sidebar[$lang['Sidebar_Main_Menu']], array(
		'module_name' => 'search',
		'name' => $lang['Sidebar_Search'],
		'url' => add_query_to_url(current_base_url(), 'module', 'search'),
		));
	array_push($sidebar[$lang['Sidebar_Main_Menu']], array(
		'module_name' => 'stats',
		'name' => $lang['Sidebar_Stats'],
		'url' => add_query_to_url(current_base_url(), 'module', 'stats'),
		));


	// Sections accessible by:
	if ($user->get_group_id() == COMPANY_MEMBER) {
		$sidebar[$lang['Sidebar_Administration']] = array();
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'companies',
			'name' => $lang['Sidebar_Company_Details'],
			'url' => create_module_action_url('companies', 'details'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'contracts',
			'name' => $lang['Sidebar_Contracts'],
			'url' => create_module_action_url('contracts', 'view'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'expositions',
			'name' => $lang['Sidebar_Expositions'],
			'url' => create_module_action_url('expositions', 'view'),
			));
	} else if ($user->get_group_id() == EXHIBITIONGROUND_MEMBER) {
		$sidebar[$lang['Sidebar_Administration']] = array();
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'users',
			'name' => $lang['Sidebar_Users'],
			'url' => create_module_action_url('users', 'view'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'companies',
			'name' => $lang['Sidebar_Companies'],
			'url' => create_module_action_url('companies', 'view'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'contracts',
			'name' => $lang['Sidebar_Contracts'],
			'url' => create_module_action_url('contracts', 'view'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'exhibitions',
			'name' => $lang['Sidebar_Exhibitions'],
			'url' => create_module_action_url('exhibitions', 'view'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'expositions',
			'name' => $lang['Sidebar_Expositions'],
			'url' => create_module_action_url('expositions', 'view'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'branches',
			'name' => $lang['Sidebar_Branches'],
			'url' => create_module_action_url('branches', 'view'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'price_categories',
			'name' => $lang['Sidebar_Price_Categories'],
			'url' => create_module_action_url('price_categories', 'view'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'pavilions',
			'name' => $lang['Sidebar_Pavilions'],
			'url' => create_module_action_url('pavilions', 'view'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'places',
			'name' => $lang['Sidebar_Places'],
			'url' => create_module_action_url('places', 'view'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array()); // Separator
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'debtors',
			'name' => $lang['Sidebar_Debtors'],
			'url' => add_query_to_url(current_base_url(), 'module', 'debtors'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array()); // Separator
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'imports',
			'name' => $lang['Sidebar_Imports'],
			'url' => add_query_to_url(current_base_url(), 'module', 'imports'),
			));
		array_push($sidebar[$lang['Sidebar_Administration']], array(
			'module_name' => 'exports',
			'name' => $lang['Sidebar_Exports'],
			'url' => add_query_to_url(current_base_url(), 'module', 'exports'),
			));
	}

	return $sidebar;
}

/**
 * Generates user data according to the selected user ID and $_POST array.
 * If the specified ID is zero, default values will be used instead of the
 * data stored in the database (for example if the user doesn't exist yet).
 *
 * @param int $user_id User ID.
 * @return array Generated user data.
 */
function generate_user_data($user_id = 0)
{
	global $config;

	$user_data = $user_id != 0 ? load_row_from_table($user_id, 'user_id', USERS_TABLE) :
		array('group_id' => $config->get_value('default_group_id'),
			'company_id' => '',
			);

	$user_data['username'] = isset($_POST['username']) ? $_POST['username'] : $user_data['username'];
	$user_data['name'] = isset($_POST['name']) ? $_POST['name'] : $user_data['name'];
	$user_data['email'] = isset($_POST['email']) ? $_POST['email'] : $user_data['email'];
	$user_data['telephone'] = isset($_POST['telephone']) ? $_POST['telephone'] : $user_data['telephone'];
	$user_data['group_id'] = isset($_POST['group_id']) ? $_POST['group_id'] : $user_data['group_id'];
	$user_data['company_id'] = isset($_POST['company_id']) ? $_POST['company_id'] : $user_data['company_id'];

	if ($user_id != 0) {
		$details_company_url = create_module_action_url('companies', 'details');
		$details_company_url = add_query_to_url($details_company_url, 'company_id', $user_data['company_id']);

		$user_data['details_company_url'] = $details_company_url;
		$user_data['email_url'] = cloak_email($user_data['email']);
	}

	return $user_data;
}

/**
 * Generates user data of all users in the database.
 *
 * @param int $records_per_page How many records should be generated.
 * @param int $page Starting "page number" (1-*).
 * @param string $order_by Which column should be use for records ordering.
 * @param bool $descending If true, records will be ordered in descending order.
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 * @return array Generated users data.
 */
function generate_users_data($records_per_page, $page = 1, $order_by = 'username',
	$descending = false, $where_clause = '')
{
	global $config, $lang;

	if (!is_valid_order_by(USERS_TABLE, $order_by)) {
		$order_by = 'username';
	}

	$order_direction = $descending ? 'DESC' : 'ASC';
	$from_record = ($page - 1) * $records_per_page;
	$sql = 'SELECT u.user_id, u.username, u.name, u.email, u.telephone, u.group_id, c.name AS company
		FROM ' . COMPANIES_TABLE . ' c RIGHT JOIN ' . USERS_TABLE . " u ON c.company_id = u.company_id
		$where_clause
		ORDER BY $order_by $order_direction
		LIMIT $from_record, $records_per_page";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load users.",
			$sql, mysql_error());
	}

	$users_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_user_url = create_module_action_url('users', 'details');
		$details_user_url = add_query_to_url($details_user_url, 'user_id', $row['user_id']);
		$edit_user_url = create_module_action_url('users', 'edit');
		$edit_user_url = add_query_to_url($edit_user_url, 'user_id', $row['user_id']);

		array_push($users_data, array(
			'user_id' => $row['user_id'],
			'username' => $row['username'],
			'name' => $row['name'],
			'email' => $row['email'],
			'telephone' => $row['telephone'],
			'group' => $lang['Group_' . get_group_name($row['group_id'])],
			'company' => $row['company'],
			'details_user_url' => $details_user_url,
			'edit_user_url' => $edit_user_url,
			));
	}
	mysql_free_result($result);

	return $users_data;
}

/**
 * Generates company data according to the selected company ID and $_POST array.
 * If the specified ID is zero, default values will be used instead of the
 * data stored in the database (for example if the company doesn't exist yet).
 *
 * @param int  $company_id Company ID.
 * @return array Generated company data.
 */
function generate_company_data($company_id = 0)
{
	global $config;

	$company_data = $company_id != 0 ? load_row_from_table($company_id, 'company_id', COMPANIES_TABLE) : array();

	$company_data['name'] = isset($_POST['name']) ? $_POST['name'] : $company_data['name'];
	$company_data['address'] = isset($_POST['address']) ? $_POST['address'] : $company_data['address'];
	$company_data['email'] = isset($_POST['email']) ? $_POST['email'] : $company_data['email'];
	$company_data['www'] = isset($_POST['www']) ? $_POST['www'] : $company_data['www'];
	$company_data['telephone'] = isset($_POST['telephone']) ? $_POST['telephone'] : $company_data['telephone'];
	$company_data['IC'] = isset($_POST['IC']) ? $_POST['IC'] : $company_data['IC'];
	$company_data['DIC'] = isset($_POST['DIC']) ? $_POST['DIC'] : $company_data['DIC'];

	if ($company_id != 0) {
		$company_data['email_url'] = cloak_email($company_data['email']);
	}

	return $company_data;
}

/**
 * Generates data of all companies in the database.
 *
 * @param int  $records_per_page How many records should be generated.
 * @param int  $page Starting "page number" (1-*).
 * @param string $order_by Which column should be use for records ordering.
 * @param bool $descending If true, records will be ordered in descending order.
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 * @return array Generated companies data.
 */
function generate_companies_data($records_per_page, $page = 1, $order_by = 'name',
	$descending = false, $where_clause = '')
{
	global $config;

	if (!is_valid_order_by(COMPANIES_TABLE, $order_by)) {
		$order_by = 'name';
	}

	$order_direction = $descending ? 'DESC' : 'ASC';
	$from_record = ($page - 1) * $records_per_page;
	$sql = 'SELECT company_id, name, address, email, www, telephone, IC, DIC
		FROM ' . COMPANIES_TABLE . "
		$where_clause
		ORDER BY $order_by $order_direction
		LIMIT $from_record, $records_per_page";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load companies.",
			$sql, mysql_error());
	}

	$companies_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_company_url = create_module_action_url('companies', 'details');
		$details_company_url = add_query_to_url($details_company_url, 'company_id', $row['company_id']);
		$edit_company_url = create_module_action_url('companies', 'edit');
		$edit_company_url = add_query_to_url($edit_company_url, 'company_id', $row['company_id']);

		array_push($companies_data, array(
			'company_id' => $row['company_id'],
			'name' => $row['name'],
			'address' => $row['address'],
			'email' => $row['email'],
			'www' => $row['www'],
			'telephone' => $row['telephone'],
			'IC' => $row['IC'],
			'DIC' => $row['DIC'],
			'details_company_url' => $details_company_url,
			'edit_company_url' => $edit_company_url,
			));
	}
	mysql_free_result($result);

	return $companies_data;
}

/**
 * Generates all available groups.
 *
 * @return array Generated groups.
 */
function generate_groups()
{
	global $lang;

	$groups = array();
	foreach (load_available_groups() as $group_id => $group_name) {
		array_push($groups, array(
			'group_id' => $group_id,
			'name' => $lang['Group_' . $group_name],
			));
	}

	return $groups;
}

/**
 * Generates array of representatives of the selected company.
 *
 * @param int $company_id Company ID.
 * @return array Array of representatives of the selected company.
 */
function generate_company_representatives($company_id)
{
	$sql = 'SELECT user_id
		FROM ' . USERS_TABLE . "
		WHERE company_id = $company_id";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load company representatives.",
			$sql, mysql_error());
	}

	$representatives = array();
	while ($row = mysql_fetch_assoc($result)) {
		$user_data = load_row_from_table($row['user_id'], 'user_id', USERS_TABLE);

		$details_user_url = create_module_action_url('users', 'details');
		$details_user_url = add_query_to_url($details_user_url, 'user_id', $row['user_id']);
		$user_data['details_user_url'] = $details_user_url;

		array_push($representatives, $user_data);
	}
	mysql_free_result($result);

	return $representatives;
}

/**
 * Generates array of contracts of the selected company.
 *
 * @param int $company_id Company ID.
 * @return array Array of contracts of the selected company.
 */
function generate_company_contracts($company_id)
{
	$sql = 'SELECT contract_id
		FROM ' . CONTRACTS_TABLE . "
		WHERE company_id = $company_id";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load company contracts.",
			$sql, mysql_error());
	}

	$contracts = array();
	while ($row = mysql_fetch_assoc($result)) {
		$contract_data = load_row_from_table($row['contract_id'], 'contract_id', CONTRACTS_TABLE);

		$details_contract_url = create_module_action_url('contracts', 'details');
		$details_contract_url = add_query_to_url($details_contract_url, 'contract_id', $row['contract_id']);
		$contract_data['details_contract_url'] = $details_contract_url;

		array_push($contracts, $contract_data);
	}
	mysql_free_result($result);

	return $contracts;
}

/**
 * Generates array of expositions that belongs to the selected contract.
 *
 * @param int $contract_id Contract ID.
 * @return array Array of expositions that belongs to the selected contract.
 */
function generate_contract_expositions($contract_id)
{
	$sql = 'SELECT exposition_id
		FROM ' . EXPOSITIONS_TABLE . "
		WHERE contract_id = $contract_id";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load expositions.", $sql, mysql_error());
	}

	$expositions = array();
	while ($row = mysql_fetch_assoc($result)) {
		$exposition_data = load_row_from_table($row['exposition_id'], 'exposition_id', EXPOSITIONS_TABLE);

		$details_exposition_url = create_module_action_url('expositions', 'details');
		$details_exposition_url = add_query_to_url($details_exposition_url , 'exposition_id', $row['exposition_id']);
		$exposition_data['details_exposition_url'] = $details_exposition_url;

		array_push($expositions, $exposition_data);
	}
	mysql_free_result($result);

	return $expositions;
}

/**
 * Generates exhibition data according to the selected exhibition ID and $_POST array.
 * If the specified ID is zero, default values will be used instead of the
 * data stored in the database (for example if the exhibition doesn't exist yet).
 *
 * @param int $exhibition_id Exhibition ID.
 * @return array Generated exhibition data.
 */
function generate_exhibition_data($exhibition_id = 0)
{
	global $config;

	$exhibition_data = $exhibition_id != 0 ? load_row_from_table($exhibition_id, 'exhibition_id', EXHIBITIONS_TABLE) : array();

	$exhibition_data['name'] = isset($_POST['name']) ? $_POST['name'] : $exhibition_data['name'];
	$exhibition_data['start'] = isset($_POST['start']) ? $_POST['start'] : format_date($exhibition_data['start']);
	$exhibition_data['end'] = isset($_POST['end']) ? $_POST['end'] : format_date($exhibition_data['end']);

	if ($exhibition_id != 0) {
		$details_exhibition_url = create_module_action_url('exhibitions', 'details');
		$details_exhibition_url = add_query_to_url($details_exhibition_url, 'exhibition_id', $exhibition_id);

		$exhibition_data['details_exhibition_url'] = $details_exhibition_url;
	}

	return $exhibition_data;
}

/**
 * Generates exhibition data of all exhibitions in the database.
 *
 * @param int $records_per_page How many records should be generated.
 * @param int $page Starting "page number" (1-*).
 * @param string $order_by Which column should be use for records ordering.
 * @param bool $descending If true, records will be ordered in descending order.
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 * @return array Generated exhibitions data.
 */
function generate_exhibitions_data($records_per_page, $page = 1, $order_by = 'name',
	$descending = false, $where_clause = '')
{
	global $config, $lang;

	if (!is_valid_order_by(EXHIBITIONS_TABLE, $order_by)) {
		$order_by = 'name';
	}

	$order_direction = $descending ? 'DESC' : 'ASC';
	$from_record = ($page - 1) * $records_per_page;
	$sql = 'SELECT exhibition_id, name, start, end
		FROM ' . EXHIBITIONS_TABLE . "
		$where_clause
		ORDER BY $order_by $order_direction
		LIMIT $from_record, $records_per_page";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load exhibitions.",
			$sql, mysql_error());
	}

	$exhibitions_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_exhibition_url = create_module_action_url('exhibitions', 'details');
		$details_exhibition_url = add_query_to_url($details_exhibition_url, 'exhibition_id', $row['exhibition_id']);
		$edit_exhibition_url = create_module_action_url('exhibitions', 'edit');
		$edit_exhibition_url = add_query_to_url($edit_exhibition_url, 'exhibition_id', $row['exhibition_id']);

		array_push($exhibitions_data, array(
			'exhibition_id' => $row['exhibition_id'],
			'name' => $row['name'],
			'start' => format_date($row['start']),
			'end' => format_date($row['end']),
			'details_exhibition_url' => $details_exhibition_url,
			'edit_exhibition_url' => $edit_exhibition_url,
			));
	}
	mysql_free_result($result);

	return $exhibitions_data;
}

/**
 * Generates branch data according to the selected branch ID and $_POST array.
 * If the specified ID is zero, default values will be used instead of the
 * data stored in the database (for example if the branch doesn't exist yet).
 *
 * @param int $branch_id Branch ID.
 * @return array Generated branch data.
 */
function generate_branch_data($branch_id = 0)
{
	global $config;

	$branch_data = $branch_id != 0 ? load_row_from_table($branch_id, 'branch_id', BRANCHES_TABLE) : array();

	$branch_data['name'] = isset($_POST['name']) ? $_POST['name'] : $branch_data['name'];

	return $branch_data;
}

/**
 * Generates branch data of all branches in the database.
 *
 * @param int $records_per_page How many records should be generated.
 * @param int $page Starting "page number" (1-*).
 * @param string $order_by Which column should be use for records ordering.
 * @param bool $descending If true, records will be ordered in descending order.
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 * @return array Generated branches data.
 */
function generate_branches_data($records_per_page, $page = 1, $order_by = 'name',
	$descending = false, $where_clause = '')
{
	global $config, $lang;

	if (!is_valid_order_by(BRANCHES_TABLE, $order_by)) {
		$order_by = 'name';
	}

	$order_direction = $descending ? 'DESC' : 'ASC';
	$from_record = ($page - 1) * $records_per_page;
	$sql = 'SELECT branch_id, name
		FROM ' . BRANCHES_TABLE . "
		$where
		ORDER BY $order_by $order_direction
		LIMIT $from_record, $records_per_page";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load branches.",
			$sql, mysql_error());
	}

	$branches_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_branch_url = create_module_action_url('branches', 'details');
		$details_branch_url = add_query_to_url($details_branch_url, 'branch_id', $row['branch_id']);
		$edit_branch_url = create_module_action_url('branches', 'edit');
		$edit_branch_url = add_query_to_url($edit_branch_url, 'branch_id', $row['branch_id']);

		array_push($branches_data, array(
			'branch_id' => $row['branch_id'],
			'name' => $row['name'],
			'details_branch_url' => $details_branch_url,
			'edit_branch_url' => $edit_branch_url,
			));
	}
	mysql_free_result($result);

	return $branches_data;
}

/**
 * Generates price category data according to the selected price category ID and $_POST array.
 * If the specified ID is zero, default values will be used instead of the
 * data stored in the database (for example if the price category doesn't exist yet).
 *
 * @param int $price_category_id Price category ID.
 * @return array Generated price category data.
 */
function generate_price_category_data($price_category_id = 0)
{
	global $config;

	$price_category_data = $price_category_id != 0 ?
		load_row_from_table($price_category_id, 'price_category_id', PRICE_CATEGORIES_TABLE) : array();

	$price_category_data['name'] = isset($_POST['name']) ? $_POST['name'] : $price_category_data['name'];
	$price_category_data['price_for_m2'] = isset($_POST['price_for_m2']) ? $_POST['price_for_m2'] : $price_category_data['price_for_m2'];

	return $price_category_data;
}

/**
 * Generates price_category data of all price categories in the database.
 *
 * @param int $records_per_page How many records should be generated.
 * @param int $page Starting "page number" (1-*).
 * @param string $order_by Which column should be use for records ordering.
 * @param bool $descending If true, records will be ordered in descending order.
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 * @return array Generated price categories data.
 */
function generate_price_categories_data($records_per_page, $page = 1, $order_by = 'price_for_m2',
	$descending = false, $where_clause)
{
	global $config, $lang;

	if (!is_valid_order_by(PRICE_CATEGORIES_TABLE, $order_by)) {
		$order_by = 'price_for_m2';
	}

	$order_direction = $descending ? 'DESC' : 'ASC';
	$from_record = ($page - 1) * $records_per_page;
	$sql = 'SELECT price_category_id, name, price_for_m2
		FROM ' . PRICE_CATEGORIES_TABLE . "
		$where_clause
		ORDER BY $order_by $order_direction
		LIMIT $from_record, $records_per_page";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load price categories.",
			$sql, mysql_error());
	}

	$price_categories_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_price_category_url = create_module_action_url('price_categories', 'details');
		$details_price_category_url = add_query_to_url($details_price_category_url, 'price_category_id', $row['price_category_id']);
		$edit_price_category_url = create_module_action_url('price_categories', 'edit');
		$edit_price_category_url = add_query_to_url($edit_price_category_url, 'price_category_id', $row['price_category_id']);

		array_push($price_categories_data, array(
			'price_category_id' => $row['price_category_id'],
			'name' => $row['name'],
			'price_for_m2' => $row['price_for_m2'],
			'details_price_category_url' => $details_price_category_url,
			'edit_price_category_url' => $edit_price_category_url,
			));
	}
	mysql_free_result($result);

	return $price_categories_data;
}

/**
 * Generates pavilion data according to the selected pavilion ID and $_POST array.
 * If the specified ID is zero, default values will be used instead of the
 * data stored in the database (for example if the pavilion doesn't exist yet).
 *
 * @param int $pavilion_id Pavilion ID.
 * @return array Generated pavilion data.
 */
function generate_pavilion_data($pavilion_id = 0)
{
	global $config;

	$pavilion_data = $pavilion_id != 0 ? load_row_from_table($pavilion_id, 'pavilion_id', PAVILIONS_TABLE) : array();

	$pavilion_data['name'] = isset($_POST['name']) ? $_POST['name'] : $pavilion_data['name'];
	$pavilion_data['number_of_floors'] = isset($_POST['number_of_floors']) ? $_POST['number_of_floors'] : $pavilion_data['number_of_floors'];
	$pavilion_data['number_of_restaurants'] = isset($_POST['number_of_restaurants']) ? $_POST['number_of_restaurants'] : $pavilion_data['number_of_restaurants'];
	$pavilion_data['number_of_buffets'] = isset($_POST['number_of_buffets']) ? $_POST['number_of_buffets'] : $pavilion_data['number_of_buffets'];
	$pavilion_data['number_of_social_settlements'] = isset($_POST['number_of_social_settlements']) ? $_POST['number_of_social_settlements'] : $pavilion_data['number_of_social_settlements'];

	return $pavilion_data;
}

/**
 * Generates pavilion data of all pavilions in the database.
 *
 * @param int $records_per_page How many records should be generated.
 * @param int $page Starting "page number" (1-*).
 * @param string $order_by Which column should be use for records ordering.
 * @param bool $descending If true, records will be ordered in descending order.
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 * @return array Generated pavilions data.
 */
function generate_pavilions_data($records_per_page, $page = 1, $order_by = 'name',
	$descending = false, $where_clause = '')
{
	global $config, $lang;

	if (!is_valid_order_by(PAVILIONS_TABLE, $order_by)) {
		$order_by = 'name';
	}

	$order_direction = $descending ? 'DESC' : 'ASC';
	$from_record = ($page - 1) * $records_per_page;
	$sql = 'SELECT pavilion_id, name, number_of_floors, number_of_restaurants, number_of_buffets, number_of_social_settlements
		FROM ' . PAVILIONS_TABLE . "
		$where_clause
		ORDER BY $order_by $order_direction
		LIMIT $from_record, $records_per_page";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load pavilions.",
			$sql, mysql_error());
	}

	$pavilions_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_pavilion_url = create_module_action_url('pavilions', 'details');
		$details_pavilion_url = add_query_to_url($details_pavilion_url, 'pavilion_id', $row['pavilion_id']);
		$edit_pavilion_url = create_module_action_url('pavilions', 'edit');
		$edit_pavilion_url = add_query_to_url($edit_pavilion_url, 'pavilion_id', $row['pavilion_id']);

		array_push($pavilions_data, array(
			'pavilion_id' => $row['pavilion_id'],
			'name' => $row['name'],
			'number_of_floors' => $row['number_of_floors'],
			'number_of_restaurants' => $row['number_of_restaurants'],
			'number_of_buffets' => $row['number_of_buffets'],
			'number_of_social_settlements' => $row['number_of_social_settlements'],
			'details_pavilion_url' => $details_pavilion_url,
			'edit_pavilion_url' => $edit_pavilion_url,
			));
	}
	mysql_free_result($result);

	return $pavilions_data;
}

/**
 * Generates place data according to the selected place ID and $_POST array.
 * If the specified ID is zero, default values will be used instead of the
 * data stored in the database (for example if the place doesn't exist yet).
 *
 * @param int $place_id Place ID.
 * @return array Generated place data.
 */
function generate_place_data($place_id = 0)
{
	global $config;

	$place_data = $place_id != 0 ? load_row_from_table($place_id, 'place_id', PLACES_TABLE) :
		array('pavilion_id' => '',
			'price_category_id' => $config->get_value('default_price_category_id'),
			);

	$place_data['number'] = isset($_POST['number']) ? $_POST['number'] : $place_data['number'];
	$place_data['area'] = isset($_POST['area']) ? $_POST['area'] : $place_data['area'];
	$place_data['pavilion_id'] = isset($_POST['pavilion_id']) ? $_POST['pavilion_id'] : $place_data['pavilion_id'];
	$place_data['price_category_id'] = isset($_POST['price_category_id']) ? $_POST['price_category_id'] : $place_data['price_category_id'];

	if ($place_id != 0) {
		$details_price_category_url = create_module_action_url('price_categories', 'details');
		$details_price_category_url = add_query_to_url($details_price_category_url, 'price_category_id', $place_data['price_category_id']);

		$place_data['details_price_category_url'] = $details_price_category_url;
	}

	return $place_data;
}

/**
 * Generates place data of all places in the database.
 *
 * @param int $records_per_page How many records should be generated.
 * @param int $page Starting "page number" (1-*).
 * @param string $order_by Which column should be use for records ordering.
 * @param bool $descending If true, records will be ordered in descending order.
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 * @return array Generated places data.
 */
function generate_places_data($records_per_page, $page = 1, $order_by = 'number',
	$descending = false, $where_clause = '')
{
	global $config, $lang;

	if (!is_valid_order_by(PLACES_TABLE, $order_by)) {
		$order_by = 'number';
	}

	$order_direction = $descending ? 'DESC' : 'ASC';
	$from_record = ($page - 1) * $records_per_page;
	$sql = 'SELECT pl.place_id, pl.number, pl.area, pa.name AS pavilion, pc.name AS price_category
		FROM ' . PLACES_TABLE . ' pl LEFT JOIN ' . PAVILIONS_TABLE . ' pa ON pl.pavilion_id = pa.pavilion_id
			 						 LEFT JOIN ' . PRICE_CATEGORIES_TABLE . " pc ON pl.price_category_id = pc.price_category_id
		$where_clause
		ORDER BY $order_by $order_direction
		LIMIT $from_record, $records_per_page";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load places.",
			$sql, mysql_error());
	}

	$places_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_place_url = create_module_action_url('places', 'details');
		$details_place_url = add_query_to_url($details_place_url, 'place_id', $row['place_id']);
		$edit_place_url = create_module_action_url('places', 'edit');
		$edit_place_url = add_query_to_url($edit_place_url, 'place_id', $row['place_id']);

		array_push($places_data, array(
			'place_id' => $row['place_id'],
			'number' => $row['number'],
			'area' => $row['area'],
			'pavilion' => $row['pavilion'],
			'price_category' => $row['price_category'],
			'details_place_url' => $details_place_url,
			'edit_place_url' => $edit_place_url,
			));
	}
	mysql_free_result($result);

	return $places_data;
}

/**
 * Generates contract data according to the selected contract ID and $_POST array.
 * If the specified ID is zero, default values will be used instead of the
 * data stored in the database (for example if the contract doesn't exist yet).
 *
 * @param int $contract_id Contract ID.
 * @return array Generated contract data.
 */
function generate_contract_data($contract_id = 0)
{
	global $config;

	$contract_data = $contract_id != 0 ? load_row_from_table($contract_id, 'contract_id', CONTRACTS_TABLE) :
		array();

	$contract_data['contract_id'] = isset($_POST['contract_id']) ? $_POST['contract_id'] : $contract_data['contract_id'];
	$contract_data['company_id'] = isset($_POST['company_id']) ? $_POST['company_id'] : $contract_data['company_id'];
	$contract_data['concluded_by'] = isset($_POST['concluded_by']) ? $_POST['concluded_by'] : $contract_data['concluded_by'];
	$contract_data['conclusion_date'] = isset($_POST['conclusion_date']) ? $_POST['conclusion_date'] : format_date($contract_data['conclusion_date']);
	$contract_data['validity_start'] = isset($_POST['validity_start']) ? $_POST['validity_start'] : format_date($contract_data['validity_start']);
	$contract_data['validity_end'] = isset($_POST['validity_end']) ? $_POST['validity_end'] : format_date($contract_data['validity_end']);
	$contract_data['maturity_date'] = isset($_POST['maturity_date']) ? $_POST['maturity_date'] : format_date($contract_data['maturity_date']);
	$contract_data['price'] = isset($_POST['price']) ? $_POST['price'] : $contract_data['price'];
	$contract_data['settled'] = isset($_POST['settled']) ? $_POST['settled'] : $contract_data['settled'];

	if ($contract_id != 0) {
		$details_company_url = create_module_action_url('companies', 'details');
		$details_company_url = add_query_to_url($details_company_url, 'company_id', $contract_data['company_id']);
		$details_user_url = create_module_action_url('users', 'details');
		$details_user_url = add_query_to_url($details_user_url, 'user_id', $contract_data['concluded_by']);

		$contract_data['details_company_url'] = $details_company_url;
		$contract_data['details_user_url'] = $details_user_url;
	}


	return $contract_data;
}

/**
 * Generates contract data of all contracts in the database.
 *
 * @param int $records_per_page How many records should be generated.
 * @param int $page Starting "page number" (1-*).
 * @param string $order_by Which column should be use for records ordering.
 * @param bool $descending If true, records will be ordered in descending order.
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 * @return array Generated contracts data.
 */
function generate_contracts_data($records_per_page, $page = 1, $order_by = 'contract_id',
	$descending = false, $where_clause = '')
{
	global $config, $lang;

	if (!is_valid_order_by(CONTRACTS_TABLE, $order_by)) {
		$order_by = 'contract_id';
	}

	$order_direction = $descending ? 'DESC' : 'ASC';
	$from_record = ($page - 1) * $records_per_page;
	$sql = 'SELECT con.contract_id, con.company_id, com.name AS company, con.concluded_by, com.name AS concluded_by_name,
				   con.conclusion_date, con.validity_start, con.validity_end, con.maturity_date, con.price, con.settled
		FROM ' . CONTRACTS_TABLE . ' con LEFT JOIN ' . COMPANIES_TABLE . ' com ON con.company_id = com.company_id
									 	 LEFT JOIN ' . USERS_TABLE . " u ON (con.concluded_by = u.user_id)
		$where_clause
		ORDER BY $order_by $order_direction
		LIMIT $from_record, $records_per_page";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load contracts.",
			$sql, mysql_error());
	}

	$contracts_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_contract_url = create_module_action_url('contracts', 'details');
		$details_contract_url = add_query_to_url($details_contract_url, 'contract_id', $row['contract_id']);
		$edit_contract_url = create_module_action_url('contracts', 'edit');
		$edit_contract_url = add_query_to_url($edit_contract_url, 'contract_id', $row['contract_id']);

		array_push($contracts_data, array(
			'contract_id' => $row['contract_id'],
			'company_id' => $row['company_id'],
			'company' => $row['company'],
			'concluded_by' => $row['concluded_by'],
			'concluded_by_name' => $row['concluded_by_name'],
			'validity_start' => format_date($row['validity_start']),
			'validity_end' => format_date($row['validity_end']),
			'conclusion_date' => format_date($row['conclusion_date']),
			'maturity_date' => format_date($row['maturity_date']),
			'price' => $row['price'],
			'settled' => $row['settled'],
			'details_contract_url' => $details_contract_url,
			'edit_contract_url' => $edit_contract_url,
			));
	}
	mysql_free_result($result);

	return $contracts_data;
}

/**
 * Generates exposition data according to the selected exposition ID and $_POST array.
 * If the specified ID is zero, default values will be used instead of the
 * data stored in the database (for example if the exposition doesn't exist yet).
 *
 * @param int $exposition_id Exposition ID.
 * @return array Generated exposition data.
 */
function generate_exposition_data($exposition_id = 0)
{
	global $config;

	$exposition_data = $exposition_id != 0 ? load_row_from_table($exposition_id, 'exposition_id', EXPOSITIONS_TABLE) : array();

	$exposition_data['exposition_id'] = isset($_POST['exposition_id']) ? $_POST['exposition_id'] : $exposition_data['exposition_id'];
	$exposition_data['exhibition_id'] = isset($_POST['exhibition_id']) ? $_POST['exhibition_id'] : $exposition_data['exhibition_id'];
	$exposition_data['place_id'] = isset($_POST['place_id']) ? $_POST['place_id'] : $exposition_data['place_id'];
	$exposition_data['contract_id'] = isset($_POST['contract_id']) ? $_POST['contract_id'] : $exposition_data['contract_id'];
	$exposition_data['branch_id'] = isset($_POST['branch_id']) ? $_POST['branch_id'] : $exposition_data['branch_id'];
	$exposition_data['description'] = isset($_POST['description']) ? $_POST['description'] : $exposition_data['description'];

	if ($exposition_id != 0) {
		$details_contract_url = create_module_action_url('contracts', 'details');
		$details_contract_url = add_query_to_url($details_contract_url, 'contract_id', $exposition_data['contract_id']);
		$details_exhibition_url = create_module_action_url('exhibitions', 'details');
		$details_exhibition_url = add_query_to_url($details_exhibition_url, 'exhibition_id', $exposition_data['exhibition_id']);
		$details_place_url = create_module_action_url('places', 'details');
		$details_place_url = add_query_to_url($details_place_url, 'place_id', $exposition_data['place_id']);
		$details_pavilion_url = create_module_action_url('pavilions', 'details');
		$details_pavilion_url = add_query_to_url($details_pavilion_url, 'pavilion_id', get_pavilion_id_for_place(
			$exposition_data['place_id']));
		$details_branch_url = create_module_action_url('branches', 'details');
		$details_branch_url = add_query_to_url($details_branch_url, 'branch_id', $exposition_data['branch_id']);

		$exposition_data['details_contract_url'] = $details_contract_url;
		$exposition_data['details_exhibition_url'] = $details_exhibition_url;
		$exposition_data['details_place_url'] = $details_place_url;
		$exposition_data['details_pavilion_url'] = $details_pavilion_url;
		$exposition_data['details_branch_url'] = $details_branch_url;
	}

	return $exposition_data;
}

/**
 * Generates exposition data of all expositions in the database.
 *
 * @param int $records_per_page How many records should be generated.
 * @param int $page Starting "page number" (1-*).
 * @param string $order_by Which column should be use for records ordering.
 * @param bool $descending If true, records will be ordered in descending order.
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 * @return array Generated expositions data.
 */
function generate_expositions_data($records_per_page, $page = 1, $order_by = 'contract_id',
	$descending = false, $where_clause = '')
{
	global $config, $lang;

	if (!is_valid_order_by(EXPOSITIONS_TABLE, $order_by)) {
		$order_by = 'contract_id';
	}

	$order_direction = $descending ? 'DESC' : 'ASC';
	$from_record = ($page - 1) * $records_per_page;
	$sql = 'SELECT ex.exposition_id, fa.name AS exhibition, pl.place_id, pl.number AS place_number,
				   ex.contract_id, co.company_id, br.name AS branch, ex.description
		FROM ' . EXPOSITIONS_TABLE . ' ex LEFT JOIN ' . EXHIBITIONS_TABLE . ' fa ON ex.exhibition_id = fa.exhibition_id
										  LEFT JOIN ' . PLACES_TABLE . ' pl ON ex.place_id = pl.place_id
										  LEFT JOIN ' . BRANCHES_TABLE . ' br ON ex.branch_id = br.branch_id
										  LEFT JOIN ' . CONTRACTS_TABLE . " co ON ex.contract_id = co.contract_id
		$where_clause
		ORDER BY $order_by $order_direction
		LIMIT $from_record, $records_per_page";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load expositions.",
			$sql, mysql_error());
	}

	$expositions_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_exposition_url = create_module_action_url('expositions', 'details');
		$details_exposition_url = add_query_to_url($details_exposition_url, 'exposition_id', $row['exposition_id']);
		$edit_exposition_url = create_module_action_url('expositions', 'edit');
		$edit_exposition_url = add_query_to_url($edit_exposition_url, 'exposition_id', $row['exposition_id']);

		array_push($expositions_data, array(
			'exposition_id' => $row['exposition_id'],
			'exhibition' => $row['exhibition'],
			'place_id' => $row['place_id'],
			'place_number' => $row['place_number'],
			'contract_id' => $row['contract_id'],
			'branch' => $row['branch'],
			'description' => $row['description'],
			'details_exposition_url' => $details_exposition_url,
			'edit_exposition_url' => $edit_exposition_url,
			));
	}
	mysql_free_result($result);

	return $expositions_data;
}

/**
 * Generates array of import/export formats.
 *
 * @return array Generated import/export formats.
 */
function generate_import_export_formats()
{
	$sql = 'SELECT format_id, name, mime_type
		FROM ' . IMPORT_EXPORT_FORMATS_TABLE . '
		ORDER BY name ASC';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load import/export formats.',
			$sql, mysql_error());
	}

	$import_export_formats = array();
	while ($format = mysql_fetch_assoc($result))	{
		array_push($import_export_formats, array(
			'format_id' => $format['format_id'],
			'name' => $format['name'],
			'mime_type' => $format['mime_type'],
			));
	}
	mysql_free_result($result);

	return $import_export_formats;
}

/**
 * Generates array of all tables in the database.
 *
 * @return array Generated array of all tables in the database.
 */
function generate_tables()
{
/*
	$tables = array(CONFIG_TABLE, LANGUAGES_TABLE, GROUPS_TABLE, USERS_TABLE,
		COMPANIES_TABLE, EXHIBITIONS_TABLE, BRANCHES_TABLE, PRICE_CATEGORIES_TABLE,
		PAVILIONS_TABLE, PLACES_TABLE, CONTRACTS_TABLE, EXPOSITIONS_TABLE,
		IMPORT_EXPORT_FORMATS_TABLE);

	return $tables;
*/
	global $db_name, $db_table_prefix;

	$sql = 'SHOW TABLES FROM ' . $db_name;
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot get table names.',
			$sql, mysql_error());
	}

	$tables = array();
	while ($table_name = mysql_fetch_row($result))	{
		if (strpos($table_name[0], $db_table_prefix) !== false) {
			array_push($tables, $table_name[0]);
		}
	}
	mysql_free_result($result);

	return $tables;
}

/**
 * Generates debtor data of all debtors in the database.
 *
 * @param int $records_per_page How many records should be generated.
 * @param int $page Starting "page number" (1-*).
 * @param string $order_by Which column should be use for records ordering.
 * @param bool $descending If true, records will be ordered in descending order.
 * @return array Generated debtors data.
 */
function generate_debtors_data($records_per_page, $page = 1, $order_by = 'company',
	$descending = false)
{
	global $config, $lang;

	if (!in_array($order_by, array('company', 'total_price', 'setled', 'owed_price'))) {
		$order_by = 'company';
	}

	$order_direction = $descending ? 'DESC' : 'ASC';
	$from_record = ($page - 1) * $records_per_page;
	$sql = 'SELECT com.company_id, com.name AS company, SUM(con.price) AS total_price,
			SUM(con.settled) AS settled, (SUM(con.price) - SUM(con.settled)) AS owed_price
		FROM ' . COMPANIES_TABLE . ' com LEFT JOIN ' . CONTRACTS_TABLE . " con ON com.company_id = con.company_id
		GROUP BY company
		HAVING settled < total_price
		ORDER BY $order_by $order_direction
		LIMIT $from_record, $records_per_page";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load debtors.",
			$sql, mysql_error());
	}

	$debtors_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_company_url = create_module_action_url('companies', 'details');
		$details_company_url = add_query_to_url($details_company_url, 'company_id', $row['company_id']);

		array_push($debtors_data, array(
			'company' => $row['company'],
			'company_id' => $row['company_id'],
			'details_company_url' => $details_company_url,
			'total_price' => $row['total_price'],
			'settled' => !empty($row['settled']) ? $row['settled'] : 0.0,
			'owed_price' => $row['owed_price'],
			));
	}
	mysql_free_result($result);

	return $debtors_data;
}
?>