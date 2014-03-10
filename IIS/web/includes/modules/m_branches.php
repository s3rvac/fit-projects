<?php
// Encoding: utf8
/**
 * Branches module.
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
 * @param array $branch_data Branch data.
 * @param string $action Action to be performed in the current page.
 */
function set_template_data($branch_data, $action)
{
	global $smarty, $lang;

	$action_branch_url = add_query_to_url(current_full_url(), 'module', 'branches');

	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Branches_' . ucfirst($action) . '_Header'],
		'PAGE_DESCRIPTION' => $lang['Branches_' . ucfirst($action) . '_Description'],
		'ACTION' => $action . '_branch',
		'BRANCH_DATA' => $branch_data,

		'U_ACTION_BRANCH' => $action_branch_url,

		'L_ACTION_BRANCH' => $lang[ucfirst($action) . '_Branch'],
		));
}

/**
 * Adds a new branch (branch ID is zero) or update some already existing branch.
 *
 * @param int $branch_id Branch ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_or_update_branch($branch_id = 0)
{
	global $smarty, $config, $lang;

	$branch_data = array();
	try {
 		// Name
 		$branch_data['name'] = handle_input_string_information(
 			$_POST['name'], 'name',
 			$config->get_value('min_name_length'),
 			$config->get_value('max_name_length'));
 		if (value_exists($branch_data['name'], 'name', BRANCHES_TABLE) &&
 			!id_corresponds_to_value($branch_id, 'branch_id', $branch_data['name'],
 				'name', BRANCHES_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Name_Exists']);
 		}

		// Insert a new branch or update existing one
		if ($branch_id != 0) {
			update_branch($branch_id, $branch_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Branches_Edited']);
		} else {
 			insert_branch($branch_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Branches_Added']);
		}

		return true;
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
		return false;
	}
}

/**
 * Displays an 'add a new branch' page.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_branch()
{
	global $smarty, $config, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
		array('url' => create_module_action_url('branches', 'view'),
			'name' => $lang['View_Branches']),
		));

	// Add branch request
	$branch_added = false;
	if (isset($_POST['add_branch'])) {
		$branch_added = add_or_update_branch();
	}

	$branch_data = generate_branch_data();
	set_template_data($branch_data, 'add');

	return $branch_added;
}

/**
 * Displays details of the selected branch.
 *
 * @param int $branch_id Branch ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function details_branch($branch_id)
{
	global $smarty, $lang;

	// Module actions
	$edit_branch_url = create_module_action_url('branches', 'edit');
	$edit_branch_url = add_query_to_url($edit_branch_url, 'branch_id', $branch_id);
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => $edit_branch_url,
				'name' => $lang['Edit_Branch']),
			array('url' => create_module_action_url('branches', 'add'),
				'name' => $lang['Add_Branch']),
			array('url' => create_module_action_url('branches', 'view'),
				'name' => $lang['View_Branches']),
		));

	$branch_data = generate_branch_data($branch_id);
	set_template_data($branch_data, 'details');

	return false;
}

/**
 * Displays page where the selected branch can be edited.
 *
 * @param int $branch_id Branch ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function edit_branch($branch_id)
{
	global $smarty, $branch, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => create_module_action_url('branches', 'add'),
				'name' => $lang['Add_Branch']),
			array('url' => create_module_action_url('branches', 'view'),
				'name' => $lang['View_Branches']),
		));

	// Edit branch request
	$branch_edited = false;
	if (isset($_POST['edit_branch'])) {
		$branch_edited = add_or_update_branch($branch_id);
	}

	$branch_data = generate_branch_data($branch_id);
	set_template_data($branch_data, 'edit');

	return $branch_edited;
}

/**
 * Deletes selected branches (according to the $_POST array) from the database.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function delete_branches()
{
	global $smarty, $branch, $lang;

	if (isset($_POST['delete_branches']) && isset($_POST['delete_branch'])) {
		foreach ($_POST['delete_branch'] as $raw_branch_id) {
			$branch_id = handle_input_int($raw_branch_id, 1);
			if ($branch_id == 0) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Invalid_Branch_Id']);
				break;
			}

			if (!branch_can_be_deleted($branch_id)) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Branch_Cannot_Be_Deleted']);
				break;
			}

			$deleted = delete_branch($branch_id);
			if ($deleted) {
				$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Branches_Deleted']);
			} else {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Delete_Failed']);
				break;
			}
		}
	}

	return true;
}

/**
 * Displays list of "all" branches (the total number depends on the number of
 * records that will be displayed on a single page).
 *
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 */
function view_branches($where_clause = '')
{
	global $smarty, $config, $lang;

	// Page header, description and module actions
	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Branches_View_Header'],
		'PAGE_DESCRIPTION' => $lang['Branches_View_Description'],
		'MODULE_ACTIONS' => array(
			array('url' => create_module_action_url('branches', 'add'),
				'name' => $lang['Add_Branch']),
			),
		'ACTION' => 'view_branches',
		));

	// Page and records information
	$current_page_number = handle_input_int($_GET['page']);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	$records_per_page = $config->get_value('records_per_page');

	// Generate branch data
	$branches_data = array();
	do {
		$branches_data = generate_branches_data($records_per_page, $current_page_number,
			$_GET['order_by'], isset($_GET['descending']), $where_clause);
	} while (empty($branches_data) && $current_page_number-- > 1);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	if (!empty($branches_data)) {
		$delete_branches_url = add_query_to_url(current_full_url(), 'module', 'branches');
		$delete_branches_url = add_query_to_url($delete_branches_url, 'action', 'delete');
		$module_action_view_url = create_module_action_url('branches', 'view');
		$order_by_name_url = add_query_to_url($module_action_view_url, 'order_by', 'name');

		$pagination = create_pagination($module_action_view_url,
			get_number_of_rows(BRANCHES_TABLE), $records_per_page);

		$smarty->assign(array(
			'BRANCHES_DATA' => $branches_data,
			'PAGINATION' => $pagination,
			'CURRENT_PAGE_NUMBER' => $current_page_number,

			'U_BRANCHES_DELETE' => $delete_branches_url,
			'U_ORDER_BY_NAME_DESCENDING' => add_query_to_url($order_by_name_url, 'descending', ''),
			'U_ORDER_BY_NAME_ASCENDING' => $order_by_name_url,

			'L_GOTO_PAGE' => $lang['Goto_Page'],
			'L_ORDER_DESCENDING' => $lang['Order_Descending'],
			'L_ORDER_ASCENDING' => $lang['Order_Ascending'],
			'L_DETAILS' => $lang['Details'],
			'L_DETAILS_BRANCH' => $lang['Details_Branch'],
			'L_EDIT' => $lang['Edit'],
			'L_EDIT_BRANCH' => $lang['Edit_Branch'],
			'L_DELETE_BRANCH' => $lang['Delete_Branch'],
			'L_DELETE_SELECTED_BRANCHES' => $lang['Delete_Selected_Branches'],
			));
	} else {
		$smarty->assign(array(
			'L_NO_BRANCHES_AVAILABLE' => $lang['No_Branches_Available'],
			));
	}
}

// General page data
$smarty->assign(array(
	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_BRANCH_NAME' => $lang['Branch_Name'],
	));

$view_branches = false;
$branch_id = isset($_GET['branch_id']) ? handle_input_int($_GET['branch_id'], 1) : 0;
$action = isset($_GET['action']) ? $_GET['action'] : 'view';
switch ($action) {
	case 'add':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_branches = add_branch();
		break;

	case 'details':
		$view_branches = $branch_id > 0 ? details_branch($branch_id) : true;
		break;

	case 'edit':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_branches = $branch_id > 0 ? edit_branch($branch_id) : true;
		break;

	case 'delete':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_branches = delete_branches();
		break;

	default:
	case 'view':
		$view_branches = true;
		break;
}

if ($view_branches) {
	if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
		not_allowed();
		return;
	} else {
		view_branches();
	}
}

$smarty->display('branches.tpl');
?>