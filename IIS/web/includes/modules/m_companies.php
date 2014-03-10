<?php
// Encoding: utf8
/**
 * Companies module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Sets common template data according to the selected action.
 *
 * @param array $company_data Company data.
 * @param string $action Action to be performed in the current page.
 */
function set_template_data($company_data, $action)
{
	global $smarty, $lang;

	$companies = array_merge(array('company_id' => ''), load_available_companies());
	$action_company_url = add_query_to_url(current_full_url(), 'module', 'companies');

	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Companies_' . ucfirst($action) . '_Header'],
		'PAGE_DESCRIPTION' => $lang['Companies_' . ucfirst($action) . '_Description'],
		'ACTION' => $action . '_company',
		'COMPANY_DATA' => $company_data,
		'COMPANIES' => $companies,
		'COMPANY_CONTRACTS' => !empty($company_data['company_id']) ?
			generate_company_contracts($company_data['company_id']) : array(),

		'U_ACTION_COMPANY' => $action_company_url,

		'L_ACTION_COMPANY' => $lang[ucfirst($action) . '_Company'],
		));
}

/**
 * Adds a new company (company ID is zero) or update some already existing company.
 *
 * @param int $company_id Company ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_or_update_company($company_id = 0)
{
	global $smarty, $config, $lang;

	$company_data = array();
	try {
 		// Name
 		$company_data['name'] = handle_input_string_information(
 			$_POST['name'], 'name',
 			$config->get_value('min_name_length'),
 			$config->get_value('max_name_length'));

 		// Address
 		$company_data['address'] = handle_input_string_information(
 			$_POST['address'], 'address',
 			$config->get_value('min_address_length'),
 			$config->get_value('max_address_length'));

 		// Email
 		$company_data['email'] = handle_input_email($_POST['email']);

		// IC
 		$company_data['IC'] = handle_input_string_information(
 			$_POST['IC'], 'IC',
 			$config->get_value('min_IC_length'),
 			$config->get_value('max_IC_length'));
 		if (!is_valid_IC($company_data['IC'])) {
			throw new IIS_Input_Exception($lang['Error_Invalid_IC']);
 		}
		if (value_exists($company_data['IC'], 'IC', COMPANIES_TABLE) &&
 			!id_corresponds_to_value($company_id, 'company_id',
 				$company_data['IC'], 'IC', COMPANIES_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_IC_Exists']);
 		}

		// Optional fields
		$company_data['www'] = handle_input_string($_POST['www']);
		$company_data['telephone'] = handle_input_string($_POST['telephone']);
		// DIC
		if (!empty($_POST['DIC'])) {
			$company_data['DIC'] = handle_input_string_information(
				$_POST['DIC'], 'DIC',
				$config->get_value('min_DIC_length'),
				$config->get_value('max_DIC_length'));
			if (!is_valid_DIC($company_data['DIC'])) {
				throw new IIS_Input_Exception($lang['Error_Invalid_DIC']);
			}
			if (value_exists($company_data['DIC'], 'DIC', COMPANIES_TABLE) &&
				!id_corresponds_to_value($company_id, 'company_id',
					$company_data['DIC'], 'DIC', COMPANIES_TABLE)) {
				throw new IIS_Input_Exception($lang['Error_DIC_Exists']);
 			}
 		}

		// Insert a new company or update existing one
		if ($company_id != 0) {
			update_company($company_id, $company_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Companies_Edited']);
		} else {
 			insert_company($company_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Companies_Added']);
		}

		return true;
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
		return false;
	}
}

/**
 * Displays an 'add a new company' page.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_company()
{
	global $smarty, $config, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
		array('url' => create_module_action_url('companies', 'view'),
			'name' => $lang['View_Companies']),
		));

	// Add company request
	$company_added = false;
	if (isset($_POST['add_company'])) {
		$company_added = add_or_update_company();
	}

	$company_data = generate_company_data();
	set_template_data($company_data, 'add');

	return $company_added;
}

/**
 * Displays details of the selected company.
 *
 * @param int $company_id Company ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function details_company($company_id)
{
	global $smarty, $user, $lang;

	// Module actions
	$edit_company_url = create_module_action_url('companies', 'edit');
	$edit_company_url = add_query_to_url($edit_company_url, 'company_id', $company_id);
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => $edit_company_url,
				'name' => $lang['Edit_Company']),
			array('url' => create_module_action_url('companies', 'add'),
				'name' => $lang['Add_Company']),
			array('url' => create_module_action_url('companies', 'view'),
				'name' => $lang['View_Companies']),
		));

	$smarty->assign(array(
		'COMPANY_REPRESENTATIVES' => generate_company_representatives($company_id),

		'L_REPRESENTED_BY' => $lang['Represented_By'],
		));

	$company_data = generate_company_data($company_id);
	set_template_data($company_data, 'details');

	return false;
}

/**
 * Displays page where the selected company can be edited.
 *
 * @param int $company_id Company ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function edit_company($company_id)
{
	global $smarty, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => create_module_action_url('companies', 'add'),
				'name' => $lang['Add_Company']),
			array('url' => create_module_action_url('companies', 'view'),
				'name' => $lang['View_Companies']),
		));

	// Edit company request
	$company_edited = false;
	if (isset($_POST['edit_company'])) {
		$company_edited = add_or_update_company($company_id);
	}

	$company_data = generate_company_data($company_id);
	set_template_data($company_data, 'edit');

	return $company_edited;
}

/**
 * Deletes selected companies (according to the $_POST array) from the database.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function delete_companies()
{
	global $smarty, $lang;

	if (isset($_POST['delete_companies']) && isset($_POST['delete_company'])) {
		foreach ($_POST['delete_company'] as $raw_company_id) {
			$company_id = handle_input_int($raw_company_id, 1);
			if ($company_id == 0) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Invalid_Company_Id']);
				break;
			}

			if (!company_can_be_deleted($company_id)) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Company_Cannot_Be_Deleted']);
				break;
			}

			$deleted = delete_company($company_id);
			if ($deleted) {
				actualize_company_representatives($company_id);
				$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Companies_Deleted']);
			} else {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Delete_Failed']);
				break;
			}
		}
	}

	return true;
}

/**
 * Displays list of "all" companies (the total number depends on the number of
 * records that will be displayed on a single page).
 *
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 */
function view_companies($where_clause = '')
{
	global $smarty, $config, $lang;

	// Page header, description and module actions
	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Companies_View_Header'],
		'PAGE_DESCRIPTION' => $lang['Companies_View_Description'],
		'MODULE_ACTIONS' => array(
			array('url' => create_module_action_url('companies', 'add'),
				'name' => $lang['Add_Company']),
			),
		'ACTION' => 'view_companies',
		));

	// Page and records information
	$current_page_number = handle_input_int($_GET['page']);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	$records_per_page = $config->get_value('records_per_page');

	// Generate company data
	$companies_data = array();
	do {
		$companies_data = generate_companies_data($records_per_page, $current_page_number,
			$_GET['order_by'], isset($_GET['descending']), $where_clause);
	} while (empty($companies_data) && $current_page_number-- > 1);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	if (!empty($companies_data)) {
		$delete_companies_url = add_query_to_url(current_full_url(), 'module', 'companies');
		$delete_companies_url = add_query_to_url($delete_companies_url, 'action', 'delete');
		$module_action_view_url = create_module_action_url('companies', 'view');
		$order_by_name_url = add_query_to_url($module_action_view_url, 'order_by', 'name');
		$order_by_address_url = add_query_to_url($module_action_view_url, 'order_by', 'address');
		$order_by_IC_url = add_query_to_url($module_action_view_url, 'order_by', 'IC');

		$pagination = create_pagination($module_action_view_url,
			get_number_of_rows(COMPANIES_TABLE), $records_per_page);

		$smarty->assign(array(
			'COMPANIES_DATA' => $companies_data,
			'PAGINATION' => $pagination,
			'CURRENT_PAGE_NUMBER' => $current_page_number,

			'U_COMPANIES_DELETE' => $delete_companies_url,
			'U_ORDER_BY_NAME_DESCENDING' => add_query_to_url($order_by_name_url, 'descending', ''),
			'U_ORDER_BY_NAME_ASCENDING' => $order_by_name_url,
			'U_ORDER_BY_ADDRESS_DESCENDING' => add_query_to_url($order_by_address_url, 'descending', ''),
			'U_ORDER_BY_ADDRESS_ASCENDING' => $order_by_address_url,
			'U_ORDER_BY_IC_DESCENDING' => add_query_to_url($order_by_IC_url, 'descending', ''),
			'U_ORDER_BY_IC_ASCENDING' => $order_by_IC_url,

			'L_GOTO_PAGE' => $lang['Goto_Page'],
			'L_ORDER_DESCENDING' => $lang['Order_Descending'],
			'L_ORDER_ASCENDING' => $lang['Order_Ascending'],
			'L_DETAILS' => $lang['Details'],
			'L_DETAILS_COMPANY' => $lang['Details_Company'],
			'L_EDIT' => $lang['Edit'],
			'L_EDIT_COMPANY' => $lang['Edit_Company'],
			'L_DELETE_COMPANY' => $lang['Delete_Company'],
			'L_DELETE_SELECTED_COMPANIES' => $lang['Delete_Selected_Companies'],
			));
	} else {
		$smarty->assign(array(
			'L_NO_COMPANIES_AVAILABLE' => $lang['No_Companies_Available'],
			));
	}
}

// General page data
$smarty->assign(array(
	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_COMPANY_NAME' => $lang['Company_Name'],
	'L_ADDRESS' => $lang['Address'],
	'L_EMAIL' => $lang['Email'],
	'L_WWW' => $lang['WWW'],
	'L_TELEPHONE' => $lang['Telephone'],
	'L_IC' => $lang['IC'],
	'L_DIC' => $lang['DIC'],
	'L_CONTRACTS' => $lang['Contracts'],
	));

$view_companies = false;
$company_id = isset($_GET['company_id']) ? handle_input_int($_GET['company_id'], 1) : 0;
$action = isset($_GET['action']) ? $_GET['action'] : 'view';
switch ($action) {
	case 'add':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_companies = add_company();
		break;

	case 'details':
		if ($user->get_group_id() == COMPANY_MEMBER && empty($company_id)) {
			// Show details of the company which the user represents
			$company_id = get_value_corresponding_to_other_value(
				'company_id', $user->get_user_id(), 'user_id', USERS_TABLE);
			details_company($company_id);
		}
		$view_companies = $company_id > 0 ? details_company($company_id) : true;
		break;

	case 'edit':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_companies = $company_id > 0 ? edit_company($company_id) : true;
		break;

	case 'delete':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_companies = delete_companies();
		break;

	default:
	case 'view':
		$view_companies = true;
		break;
}

if ($view_companies) {
	if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
		not_allowed();
		return;
	} else {
		view_companies();
	}
}

$smarty->display('companies.tpl');
?>