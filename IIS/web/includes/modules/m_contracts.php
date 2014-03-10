<?php
// Encoding: utf8
/**
 * Contracts module.
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
 * @param array $contract_data Contract data.
 * @param string $action Action to be performed in the current page.
 */
function set_template_data($contract_data, $action)
{
	global $smarty, $lang;

	$action_contract_url = add_query_to_url(current_full_url(), 'module', 'contracts');

	$users = array();
	foreach (generate_users_data(PHP_INT_MAX, 1, 'company_id', false) as $user) {
		if ($user['group'] == $lang['Group_Company_Member'] && !empty($user['company'])) {
			array_push($users, $user);
		}
	}

	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Contracts_' . ucfirst($action) . '_Header'],
		'PAGE_DESCRIPTION' => $lang['Contracts_' . ucfirst($action) . '_Description'],
		'ACTION' => $action . '_contract',
		'CONTRACT_DATA' => $contract_data,
		'COMPANIES' => load_available_companies(),
		'USERS' => $users,
 		'CONTRACT_EXPOSITIONS' => !empty($contract_data['contract_id']) ?
 			generate_contract_expositions($contract_data['contract_id']) : array(),

		'U_ACTION_CONTRACT' => $action_contract_url,

		'L_ACTION_CONTRACT' => $lang[ucfirst($action) . '_Contract'],
		));
}

/**
 * Adds a new contract (contract ID is zero) or update some already existing contract.
 *
 * @param int $contract_id Contract ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_or_update_contract($contract_id = 0)
{
	global $smarty, $config, $lang;

	$contract_data = array();
	try {
		// Company
 		$contract_data['company_id'] = handle_input_int($_POST['company_id'], 1);
		if (!is_valid_id($contract_data['company_id'], 'company_id', COMPANIES_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Invalid_Company']);
 		}

 		// Concluded by
 		$contract_data['concluded_by'] = handle_input_int($_POST['concluded_by'], 1);
 		if (!is_valid_id($contract_data['concluded_by'], 'user_id', USERS_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Invalid_User_Id']);
 		}

		// Check whether the selected user (concluded by) is a member
		// of the selected company
		$user_company_id = get_value_corresponding_to_other_value('company_id',
			$contract_data['concluded_by'], 'user_id', USERS_TABLE);
		if ($user_company_id != $contract_data['company_id']) {
			throw new IIS_Input_Exception($lang['Error_Contract_User_Not_From_Company']);
		}

 		// Dates
 		$contract_data['conclusion_date'] = handle_input_date($_POST['conclusion_date']);
 		$contract_data['validity_start'] = handle_input_date($_POST['validity_start']);
 		$contract_data['validity_end'] = handle_input_date($_POST['validity_end']);
 		$contract_data['maturity_date'] = handle_input_date($_POST['maturity_date']);

		// Check whether the validity starts before it ends
		if ($contract_data['validity_start'] == $contract_data['validity_end']) {
			throw new IIS_Input_Exception($lang['Error_Contract_Starts_Ends_Same']);
		} else if ($contract_data['validity_start'] > $contract_data['validity_end']) {
			throw new IIS_Input_Exception($lang['Error_Contract_Ends_Before_Starts']);
		}

		// Settled price
		$contract_data['settled'] = handle_input_double($_POST['settled']);
		if (!is_valid_price($contract_data['settled'], false, true)) {
			throw new IIS_Input_Exception($lang['Error_Invalid_Price']);
		}

		// Insert a new contract or update existing one
		if ($contract_id != 0) {
			update_contract($contract_id, $contract_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Contracts_Edited']);
		} else {
 			insert_contract($contract_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Contracts_Added']);
		}

		return true;
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
		return false;
	}
}

/**
 * Displays an 'add a new contract' page.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_contract()
{
	global $smarty, $config, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
		array('url' => create_module_action_url('contracts', 'view'),
			'name' => $lang['View_Contracts']),
		));

	// Add contract request
	$contract_added = false;
	if (isset($_POST['add_contract'])) {
		$contract_added = add_or_update_contract();
	}

	$contract_data = generate_contract_data();
	set_template_data($contract_data, 'add');

	return $contract_added;
}

/**
 * Displays details of the selected contract.
 *
 * @param int $contract_id Contract ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function details_contract($contract_id)
{
	global $smarty, $user, $lang;

	// Module actions
	$edit_contract_url = create_module_action_url('contracts', 'edit');
	$edit_contract_url = add_query_to_url($edit_contract_url, 'contract_id', $contract_id);
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => $edit_contract_url,
				'name' => $lang['Edit_Contract']),
			array('url' => create_module_action_url('contracts', 'add'),
				'name' => $lang['Add_Contract']),
			array('url' => create_module_action_url('contracts', 'view'),
				'name' => $lang['View_Contracts']),
		));

	$contract_data = generate_contract_data($contract_id);
	set_template_data($contract_data, 'details');

	return false;
}

/**
 * Displays page where the selected contract can be edited.
 *
 * @param int $contract_id Contract ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function edit_contract($contract_id)
{
	global $smarty, $contract, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => create_module_action_url('contracts', 'add'),
				'name' => $lang['Add_Contract']),
			array('url' => create_module_action_url('contracts', 'view'),
				'name' => $lang['View_Contracts']),
		));

	// Edit contract request
	$contract_edited = false;
	if (isset($_POST['edit_contract'])) {
		$contract_edited = add_or_update_contract($contract_id);
	}

	$contract_data = generate_contract_data($contract_id);
	set_template_data($contract_data, 'edit');

	return $contract_edited;
}

/**
 * Deletes selected contracts (according to the $_POST array) from the database.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function delete_contracts()
{
	global $smarty, $lang;

	if (isset($_POST['delete_contracts']) && isset($_POST['delete_contract'])) {
		foreach ($_POST['delete_contract'] as $raw_contract_id) {
			$contract_id = handle_input_int($raw_contract_id, 1);
			if ($contract_id == 0) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Invalid_Contract_Id']);
				break;
			}

			if (!contract_can_be_deleted($contract_id)) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Contract_Cannot_Be_Deleted']);
				break;
			}

			$deleted = delete_contract($contract_id);
			if ($deleted) {
				$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Contracts_Deleted']);
			} else {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Delete_Failed']);
				break;
			}
		}
	}

	return true;
}

/**
 * Displays list of "all" contracts (the total number depends on the number of
 * records that will be displayed on a single page).
 *
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 */
function view_contracts($where_clause = '')
{
	global $smarty, $config, $lang;

	// Page header, description and module actions
	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Contracts_View_Header'],
		'PAGE_DESCRIPTION' => $lang['Contracts_View_Description'],
		'MODULE_ACTIONS' => array(
			array('url' => create_module_action_url('contracts', 'add'),
				'name' => $lang['Add_Contract']),
			),
		'ACTION' => 'view_contracts',
		));

	// Page and records information
	$current_page_number = handle_input_int($_GET['page']);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	$records_per_page = $config->get_value('records_per_page');

	// Generate contract data
	$contracts_data = array();
	do {
		$contracts_data = generate_contracts_data($records_per_page, $current_page_number,
			$_GET['order_by'], isset($_GET['descending']), $where_clause);
	} while (empty($contracts_data) && $current_page_number-- > 1);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	if (!empty($contracts_data)) {
		$delete_contracts_url = add_query_to_url(current_full_url(), 'module', 'contracts');
		$delete_contracts_url = add_query_to_url($delete_contracts_url, 'action', 'delete');
		$module_action_view_url = create_module_action_url('contracts', 'view');
		$order_by_contract_number_url = add_query_to_url($module_action_view_url, 'order_by', 'contract_id');
		$order_by_company_url = add_query_to_url($module_action_view_url, 'order_by', 'company');
		$order_by_conclusion_date_url = add_query_to_url($module_action_view_url, 'order_by', 'conclusion_date');

		$pagination = create_pagination($module_action_view_url,
			get_number_of_rows(CONTRACTS_TABLE), $records_per_page);

		$smarty->assign(array(
			'CONTRACTS_DATA' => $contracts_data,
			'PAGINATION' => $pagination,
			'CURRENT_PAGE_NUMBER' => $current_page_number,

			'U_CONTRACTS_DELETE' => $delete_contracts_url,
			'U_ORDER_BY_CONTRACT_NUMBER_DESCENDING' => add_query_to_url($order_by_contract_number_url, 'descending', ''),
			'U_ORDER_BY_CONTRACT_NUMBER_ASCENDING' => $order_by_contract_number_url,
			'U_ORDER_BY_COMPANY_DESCENDING' => add_query_to_url($order_by_company_url, 'descending', ''),
			'U_ORDER_BY_COMPANY_ASCENDING' => $order_by_company_url,
			'U_ORDER_BY_CONCLUSION_DATE_DESCENDING' => add_query_to_url($order_by_conclusion_date_url, 'descending', ''),
			'U_ORDER_BY_CONCLUSION_DATE_ASCENDING' => $order_by_conclusion_date_url,

			'L_GOTO_PAGE' => $lang['Goto_Page'],
			'L_ORDER_DESCENDING' => $lang['Order_Descending'],
			'L_ORDER_ASCENDING' => $lang['Order_Ascending'],
			'L_DETAILS' => $lang['Details'],
			'L_DETAILS_CONTRACT' => $lang['Details_Contract'],
			'L_EDIT' => $lang['Edit'],
			'L_EDIT_CONTRACT' => $lang['Edit_Contract'],
			'L_DELETE_CONTRACT' => $lang['Delete_Contract'],
			'L_DELETE_SELECTED_CONTRACTS' => $lang['Delete_Selected_Contracts'],
			));
	} else {
		$smarty->assign(array(
			'L_NO_CONTRACTS_AVAILABLE' => $lang['No_Contracts_Available'],
			));
	}
}

// General page data
$smarty->assign(array(
	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_CONTRACT_N' => $lang['Contract_N'],
	'L_CONTRACT_NUMBER' => $lang['Contract_Number'],
	'L_COMPANY' => $lang['Company'],
	'L_CONCLUSION_DATE' => $lang['Conclusion_Date'],
	'L_CONCLUDED_BY' => $lang['Concluded_By'],
	'L_VALIDITY_START' => $lang['Validity_Start'],
	'L_VALIDITY_END' => $lang['Validity_End'],
	'L_MATURITY_DATE' => $lang['Maturity_Date'],
	'L_TOTAL_PRICE' => $lang['Total_Price'],
	'L_SETTLED'  => $lang['Settled'],
	'L_EXPOSITIONS' => $lang['Expositions'],
	));

$view_contracts = false;
$contract_id = isset($_GET['contract_id']) ? handle_input_int($_GET['contract_id'], 1) : 0;
$action = isset($_GET['action']) ? $_GET['action'] : 'view';
switch ($action) {
	case 'add':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_contracts = add_contract();
		break;

	case 'details':
		if ($user->get_group_id() == VISITOR) {
			not_allowed();
			return;
		} else if ($user->get_group_id() == COMPANY_MEMBER) {
			// Company members can view only contracts with the company they
			// represents
			if (!can_company_member_view_contract_details($user->get_user_id(), $contract_id)) {
				not_allowed();
				return;
			}
		}
		$view_contracts = $contract_id > 0 ? details_contract($contract_id) : true;
		break;

	case 'edit':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_contracts = $contract_id > 0 ? edit_contract($contract_id) : true;
		break;

	case 'delete':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_contracts = delete_contracts();
		break;

	default:
	case 'view':
		$view_contracts = true;
		break;
}

if ($view_contracts) {
	if ($user->get_group_id() == COMPANY_MEMBER) {
		$company_id = get_value_corresponding_to_other_value(
			'company_id', $user->get_user_id(), 'user_id', USERS_TABLE);
		view_contracts("WHERE con.company_id = $company_id");
	} else if ($user->get_group_id() == VISITOR) {
		not_allowed();
		return;
	} else {
		view_contracts();
	}
}

$smarty->display('contracts.tpl');
?>