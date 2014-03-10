<?php
// Encoding: utf8
/**
 * Expositions module.
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
 * @param array $exposition_data Exposition data.
 * @param string $action Action to be performed in the current page.
 */
function set_template_data($exposition_data, $action)
{
	global $smarty, $lang;

	$action_exposition_url = add_query_to_url(current_full_url(), 'module', 'expositions');

	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Expositions_' . ucfirst($action) . '_Header'],
		'PAGE_DESCRIPTION' => $lang['Expositions_' . ucfirst($action) . '_Description'],
		'ACTION' => $action . '_exposition',
		'EXPOSITION_DATA' => $exposition_data,
		'EXPOSITION_PRICE' => !empty($exposition_data['exposition_id']) ?
			get_price_for_exposition($exposition_data['exposition_id']) : 0.0,
		'EXHIBITIONS' => load_available_exhibitions(),
		'BRANCHES' => load_available_branches(),
		'CONTRACTS' => load_available_contracts(),

		'U_ACTION_EXPOSITION' => $action_exposition_url,

		'L_ACTION_EXPOSITION' => $lang[ucfirst($action) . '_Exposition'],
		));
}

/**
 * Adds a new exposition (exposition ID is zero) or update some already existing exposition.
 *
 * @param int $exposition_id Exposition ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_or_update_exposition($exposition_id = 0)
{
	global $smarty, $config, $user, $lang;

	$exposition_data = array();
	try {
		if ($user->get_group_id() == EXHIBITIONGROUND_MEMBER) {
			// Contract
			$exposition_data['contract_id'] = handle_input_int($_POST['contract_id'], 1);
			if (!is_valid_id($exposition_data['contract_id'], 'contract_id', CONTRACTS_TABLE)) {
				throw new IIS_Input_Exception($lang['Error_Invalid_Contract']);
			}

			// Exhibition
			$exposition_data['exhibition_id'] = handle_input_int($_POST['exhibition_id'], 1);
			if (!is_valid_id($exposition_data['exhibition_id'], 'exhibition_id', EXHIBITIONS_TABLE)) {
				throw new IIS_Input_Exception($lang['Error_Invalid_Exhibition']);
			}

			// Place
			$exposition_data['place_id'] = handle_input_int($_POST['place_id'], 1);
			if (!is_valid_id($exposition_data['place_id'], 'place_id', PLACES_TABLE)) {
				throw new IIS_Input_Exception($lang['Error_Invalid_Place']);
			}

			// Check whether the selected place is not engaged
			if (is_engaged_place($exposition_data['exhibition_id'], $exposition_data['place_id']) &&
				!id_corresponds_to_value($exposition_id, 'exposition_id', $exposition_data['place_id'],
					'place_id', EXPOSITIONS_TABLE)) {
				throw new IIS_Input_Exception($lang['Error_Engaged_Place']);
			}
		} else {
			// Company members can't modify the fields listed above, so
			// load their values from the database
			$exposition_data = load_row_from_table($exposition_id,
				'exposition_id', EXPOSITIONS_TABLE);
		}

 		// Branch
 		$exposition_data['branch_id'] = handle_input_int($_POST['branch_id'], 1);
		if (!is_valid_id($exposition_data['branch_id'], 'branch_id', BRANCHES_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Invalid_Branch']);
 		}

		// Description
		$exposition_data['description'] = handle_input_string_information(
 			$_POST['description'], 'description',
 			$config->get_value('min_exposition_description_length'),
 			$config->get_value('max_exposition_description_length'));

		// Insert a new exposition or update existing one
		if ($exposition_id != 0) {
			$old_contract_id = get_value_corresponding_to_other_value('contract_id',
				$exposition_id, 'exposition_id', EXPOSITIONS_TABLE);

			update_exposition($exposition_id, $exposition_data);
			actualize_contract($exposition_data['contract_id']);
			actualize_contract($old_contract_id);

			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Expositions_Edited']);
		} else {
 			insert_exposition($exposition_data);
 			actualize_contract($exposition_data['contract_id']);

			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Expositions_Added']);
		}

		return true;
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
		return false;
	}
}

/**
 * Displays an 'add a new exposition' page.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_exposition()
{
	global $smarty, $config, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
		array('url' => create_module_action_url('expositions', 'view'),
			'name' => $lang['View_Expositions']),
		));

	// Add exposition request
	$exposition_added = false;
	if (isset($_POST['add_exposition'])) {
		$exposition_added = add_or_update_exposition();
	}

	$exposition_data = generate_exposition_data();
	set_template_data($exposition_data, 'add');

	return $exposition_added;
}

/**
 * Displays details of the selected exposition.
 *
 * @param int $exposition_id Exposition ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function details_exposition($exposition_id)
{
	global $smarty, $user, $lang;

	// Module actions
	$edit_exposition_url = create_module_action_url('expositions', 'edit');
	$edit_exposition_url = add_query_to_url($edit_exposition_url, 'exposition_id', $exposition_id);
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => $edit_exposition_url,
				'name' => $lang['Edit_Exposition']),
			array('url' => create_module_action_url('expositions', 'add'),
				'name' => $lang['Add_Exposition']),
			array('url' => create_module_action_url('expositions', 'view'),
				'name' => $lang['View_Expositions']),
		));

	$exposition_data = generate_exposition_data($exposition_id);
	set_template_data($exposition_data, 'details');

	return false;
}

/**
 * Displays page where the selected exposition can be edited.
 *
 * @param int $exposition_id Exposition ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function edit_exposition($exposition_id)
{
	global $smarty, $exposition, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => create_module_action_url('expositions', 'add'),
				'name' => $lang['Add_Exposition']),
			array('url' => create_module_action_url('expositions', 'view'),
				'name' => $lang['View_Expositions']),
		));

	// Edit exposition request
	$exposition_edited = false;
	if (isset($_POST['edit_exposition'])) {
		$exposition_edited = add_or_update_exposition($exposition_id);
	}

	$exposition_data = generate_exposition_data($exposition_id);
	set_template_data($exposition_data, 'edit');

	return $exposition_edited;
}

/**
 * Deletes selected expositions (according to the $_POST array) from the database.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function delete_expositions()
{
	global $smarty, $lang;

	if (isset($_POST['delete_expositions']) && isset($_POST['delete_exposition'])) {
		foreach ($_POST['delete_exposition'] as $raw_exposition_id) {
			$exposition_id = handle_input_int($raw_exposition_id, 1);
			if ($exposition_id == 0) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Invalid_Exposition_Id']);
				break;
			}

			if (!exposition_can_be_deleted($exposition_id)) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Exposition_Cannot_Be_Deleted']);
				break;
			}

			$contract_id = get_value_corresponding_to_other_value(
					'contract_id', $exposition_id, 'exposition_id', EXPOSITIONS_TABLE);
			$deleted = delete_exposition($exposition_id);
			if ($deleted) {
				actualize_contract($contract_id);
				$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Expositions_Deleted']);
			} else {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Delete_Failed']);
				break;
			}
		}
	}

	return true;
}

/**
 * Displays list of "all" expositions (the total number depends on the number of
 * records that will be displayed on a single page).
 *
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 */
function view_expositions($where_clause = '')
{
	global $smarty, $config, $lang;

	// Page header, description and module actions
	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Expositions_View_Header'],
		'PAGE_DESCRIPTION' => $lang['Expositions_View_Description'],
		'MODULE_ACTIONS' => array(
			array('url' => create_module_action_url('expositions', 'add'),
				'name' => $lang['Add_Exposition']),
			),
		'ACTION' => 'view_expositions',
		));

	// Page and records information
	$current_page_number = handle_input_int($_GET['page']);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	$records_per_page = $config->get_value('records_per_page');

	// Generate exposition data
	$expositions_data = array();
	do {
		$expositions_data = generate_expositions_data($records_per_page, $current_page_number,
			$_GET['order_by'], isset($_GET['descending']), $where_clause);
	} while (empty($expositions_data) && $current_page_number-- > 1);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	if (!empty($expositions_data)) {
		$delete_expositions_url = add_query_to_url(current_full_url(), 'module', 'expositions');
		$delete_expositions_url = add_query_to_url($delete_expositions_url, 'action', 'delete');
		$module_action_view_url = create_module_action_url('expositions', 'view');
		$order_by_contract_number_url = add_query_to_url($module_action_view_url, 'order_by', 'contract_id');
		$order_by_exhibition_url = add_query_to_url($module_action_view_url, 'order_by', 'exhibition');
		$order_by_place_url = add_query_to_url($module_action_view_url, 'order_by', 'place_number');

		$pagination = create_pagination($module_action_view_url,
			get_number_of_rows(EXPOSITIONS_TABLE), $records_per_page);

		$smarty->assign(array(
			'EXPOSITIONS_DATA' => $expositions_data,
			'PAGINATION' => $pagination,
			'CURRENT_PAGE_NUMBER' => $current_page_number,

			'U_EXPOSITIONS_DELETE' => $delete_expositions_url,
			'U_ORDER_BY_CONTRACT_NUMBER_DESCENDING' => add_query_to_url($order_by_contract_number_url, 'descending', ''),
			'U_ORDER_BY_CONTRACT_NUMBER_ASCENDING' => $order_by_contract_number_url,
			'U_ORDER_BY_EXHIBITION_DESCENDING' => add_query_to_url($order_by_exhibition_url, 'descending', ''),
			'U_ORDER_BY_EXHIBITION_ASCENDING' => $order_by_exhibition_url,
			'U_ORDER_BY_PLACE_DESCENDING' => add_query_to_url($order_by_place_url, 'descending', ''),
			'U_ORDER_BY_PLACE_ASCENDING' => $order_by_place_url,

			'L_GOTO_PAGE' => $lang['Goto_Page'],
			'L_ORDER_DESCENDING' => $lang['Order_Descending'],
			'L_ORDER_ASCENDING' => $lang['Order_Ascending'],
			'L_DETAILS' => $lang['Details'],
			'L_DETAILS_EXPOSITION' => $lang['Details_Exposition'],
			'L_EDIT' => $lang['Edit'],
			'L_EDIT_EXPOSITION' => $lang['Edit_Exposition'],
			'L_DELETE_EXPOSITION' => $lang['Delete_Exposition'],
			'L_DELETE_SELECTED_EXPOSITIONS' => $lang['Delete_Selected_Expositions'],
			));
	} else {
		$smarty->assign(array(
			'L_NO_EXPOSITIONS_AVAILABLE' => $lang['No_Expositions_Available'],
			));
	}
}

// General page data
$smarty->assign(array(
	'PLACES' => load_available_places(),

	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_CONTRACT_N' => $lang['Contract_N'],
	'L_CONTRACT_NUMBER' => $lang['Contract_Number'],
	'L_EXHIBITION' => $lang['Exhibition'],
	'L_PLACE' => $lang['Place'],
	'L_BRANCH' => $lang['Branch'],
	'L_DESCRIPTION' => $lang['Description'],
	'L_EXPOSITION_PRICE' => $lang['Exposition_Price'],
	));

$view_expositions = false;
$exposition_id = isset($_GET['exposition_id']) ? handle_input_int($_GET['exposition_id'], 1) : 0;
$action = isset($_GET['action']) ? $_GET['action'] : 'view';
switch ($action) {
	case 'add':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_expositions = add_exposition();
		break;

	case 'details':
		$view_contracts = $exposition_id > 0 ? details_exposition($exposition_id) : true;
		break;

	case 'edit':
		if ($user->get_group_id() == VISITOR) {
			not_allowed();
			return;
		} else if ($user->get_group_id() == COMPANY_MEMBER) {
			// Company members can edit only expositions of the company they
			// represent
			if (!can_company_member_edit_exposition($user->get_user_id(), $exposition_id)) {
				not_allowed();
				return;
			}
		}
		$view_expositions = $exposition_id > 0 ? edit_exposition($exposition_id) : true;
		break;

	case 'delete':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_expositions = delete_expositions();
		break;

	default:
	case 'view':
		$view_expositions = true;
		break;
}

if ($view_expositions) {
	if ($user->get_group_id() == COMPANY_MEMBER) {
		$company_id = get_value_corresponding_to_other_value(
			'company_id', $user->get_user_id(), 'user_id', USERS_TABLE);
		view_expositions("WHERE co.company_id = $company_id");
	} else if ($user->get_group_id() == VISITOR) {
		not_allowed();
		return;
	}else {
		view_expositions();
	}
}

$smarty->display('expositions.tpl');
?>