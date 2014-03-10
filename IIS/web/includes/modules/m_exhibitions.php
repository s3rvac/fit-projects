<?php
// Encoding: utf8
/**
 * Exhibitions module.
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
 * @param array $exhibition_data Exhibition data.
 * @param string $action Action to be performed in the current page.
 */
function set_template_data($exhibition_data, $action)
{
	global $smarty, $lang;

	$action_exhibition_url = add_query_to_url(current_full_url(), 'module', 'exhibitions');

	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Exhibitions_' . ucfirst($action) . '_Header'],
		'PAGE_DESCRIPTION' => $lang['Exhibitions_' . ucfirst($action) . '_Description'],
		'ACTION' => $action . '_exhibition',
		'EXHIBITION_DATA' => $exhibition_data,

		'U_ACTION_EXHIBITION' => $action_exhibition_url,

		'L_ACTION_EXHIBITION' => $lang[ucfirst($action) . '_Exhibition'],
		));
}

/**
 * Adds a new exhibition (exhibition ID is zero) or update some already existing exhibition.
 *
 * @param int $exhibition_id Exhibition ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_or_update_exhibition($exhibition_id = 0)
{
	global $smarty, $config, $lang;

	$exhibition_data = array();
	try {
 		// Name
 		$exhibition_data['name'] = handle_input_string_information(
 			$_POST['name'], 'name',
 			$config->get_value('min_name_length'),
 			$config->get_value('max_name_length'));

		// Start of exhibition
 		$exhibition_data['start'] = handle_input_date($_POST['start']);

 		// End of exhibition
 		$exhibition_data['end'] = handle_input_date($_POST['end']);

		// Check whether the exhibition starts before it ends
		if ($exhibition_data['start'] == $exhibition_data['end']) {
			throw new IIS_Input_Exception($lang['Error_Exhibition_Starts_Ends_Same']);
		} else if ($exhibition_data['start'] > $exhibition_data['end']) {
			throw new IIS_Input_Exception($lang['Error_Exhibition_Ends_Before_Starts']);
		}

		// Check whether there is not another exhibition that's taking place
		// in the selected period
		if (!exhibition_can_take_place($exhibition_id, $exhibition_data['start'], $exhibition_data['end'])) {
			throw new IIS_Input_Exception($lang['Error_Exhibition_Cannot_Take_Place']);
		}

		// Insert a new exhibition or update existing one
		if ($exhibition_id != 0) {
			update_exhibition($exhibition_id, $exhibition_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Exhibitions_Edited']);
		} else {
 			insert_exhibition($exhibition_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Exhibitions_Added']);
		}

		return true;
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
		return false;
	}
}

/**
 * Displays an 'add a new exhibition' page.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_exhibition()
{
	global $smarty, $config, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
		array('url' => create_module_action_url('exhibitions', 'view'),
			'name' => $lang['View_Exhibitions']),
		));

	// Add exhibition request
	$exhibition_added = false;
	if (isset($_POST['add_exhibition'])) {
		$exhibition_added = add_or_update_exhibition();
	}

	$exhibition_data = generate_exhibition_data();
	set_template_data($exhibition_data, 'add');

	return $exhibition_added;
}

/**
 * Displays details of the selected exhibition.
 *
 * @param int $exhibition_id Exhibition ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function details_exhibition($exhibition_id)
{
	global $smarty, $user, $lang;

	// Module actions
	$edit_exhibition_url = create_module_action_url('exhibitions', 'edit');
	$edit_exhibition_url = add_query_to_url($edit_exhibition_url, 'exhibition_id', $exhibition_id);
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => $edit_exhibition_url,
				'name' => $lang['Edit_Exhibition']),
			array('url' => create_module_action_url('exhibitions', 'add'),
				'name' => $lang['Add_Exhibition']),
			array('url' => create_module_action_url('exhibitions', 'view'),
				'name' => $lang['View_Exhibitions']),
		));

	$exhibition_data = generate_exhibition_data($exhibition_id);
	set_template_data($exhibition_data, 'details');

	return false;
}

/**
 * Displays page where the selected exhibition can be edited.
 *
 * @param int $exhibition_id Exhibition ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function edit_exhibition($exhibition_id)
{
	global $smarty, $exhibition, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => create_module_action_url('exhibitions', 'add'),
				'name' => $lang['Add_Exhibition']),
			array('url' => create_module_action_url('exhibitions', 'view'),
				'name' => $lang['View_Exhibitions']),
		));

	// Edit exhibition request
	$exhibition_edited = false;
	if (isset($_POST['edit_exhibition'])) {
		$exhibition_edited = add_or_update_exhibition($exhibition_id);
	}

	$exhibition_data = generate_exhibition_data($exhibition_id);
	set_template_data($exhibition_data, 'edit');

	return $exhibition_edited;
}

/**
 * Deletes selected exhibitions (according to the $_POST array) from the database.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function delete_exhibitions()
{
	global $smarty, $lang;

	if (isset($_POST['delete_exhibitions']) && isset($_POST['delete_exhibition'])) {
		foreach ($_POST['delete_exhibition'] as $raw_exhibition_id) {
			$exhibition_id = handle_input_int($raw_exhibition_id, 1);
			if ($exhibition_id == 0) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Invalid_Exhibition_Id']);
				break;
			}

			if (!exhibition_can_be_deleted($exhibition_id)) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Exhibition_Cannot_Be_Deleted']);
				break;
			}

			$deleted = delete_exhibition($exhibition_id);
			if ($deleted) {
				$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Exhibitions_Deleted']);
			} else {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Delete_Failed']);
				break;
			}
		}
	}

	return true;
}

/**
 * Displays list of "all" exhibitions (the total number depends on the number of
 * records that will be displayed on a single page).
 *
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 */
function view_exhibitions($where_clause = '')
{
	global $smarty, $config, $lang;

	// Page header, description and module actions
	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Exhibitions_View_Header'],
		'PAGE_DESCRIPTION' => $lang['Exhibitions_View_Description'],
		'MODULE_ACTIONS' => array(
			array('url' => create_module_action_url('exhibitions', 'add'),
				'name' => $lang['Add_Exhibition']),
			),
		'ACTION' => 'view_exhibitions',
		));

	// Page and records information
	$current_page_number = handle_input_int($_GET['page']);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	$records_per_page = $config->get_value('records_per_page');

	// Generate exhibition data
	$exhibitions_data = array();
	do {
		$exhibitions_data = generate_exhibitions_data($records_per_page, $current_page_number,
			$_GET['order_by'], isset($_GET['descending']), $where_clause);
	} while (empty($exhibitions_data) && $current_page_number-- > 1);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	if (!empty($exhibitions_data)) {
		$delete_exhibitions_url = add_query_to_url(current_full_url(), 'module', 'exhibitions');
		$delete_exhibitions_url = add_query_to_url($delete_exhibitions_url, 'action', 'delete');
		$module_action_view_url = create_module_action_url('exhibitions', 'view');
		$order_by_name_url = add_query_to_url($module_action_view_url, 'order_by', 'name');
		$order_by_start_url = add_query_to_url($module_action_view_url, 'order_by', 'start');
		$order_by_end_url = add_query_to_url($module_action_view_url, 'order_by', 'end');

		$pagination = create_pagination($module_action_view_url,
			get_number_of_rows(EXHIBITIONS_TABLE), $records_per_page);

		$smarty->assign(array(
			'EXHIBITIONS_DATA' => $exhibitions_data,
			'PAGINATION' => $pagination,
			'CURRENT_PAGE_NUMBER' => $current_page_number,

			'U_EXHIBITIONS_DELETE' => $delete_exhibitions_url,
			'U_ORDER_BY_NAME_DESCENDING' => add_query_to_url($order_by_name_url, 'descending', ''),
			'U_ORDER_BY_NAME_ASCENDING' => $order_by_name_url,
			'U_ORDER_BY_START_DESCENDING' => add_query_to_url($order_by_start_url, 'descending', ''),
			'U_ORDER_BY_START_ASCENDING' => $order_by_start_url,
			'U_ORDER_BY_END_DESCENDING' => add_query_to_url($order_by_end_url, 'descending', ''),
			'U_ORDER_BY_END_ASCENDING' => $order_by_end_url,

			'L_GOTO_PAGE' => $lang['Goto_Page'],
			'L_ORDER_DESCENDING' => $lang['Order_Descending'],
			'L_ORDER_ASCENDING' => $lang['Order_Ascending'],
			'L_DETAILS' => $lang['Details'],
			'L_DETAILS_EXHIBITION' => $lang['Details_Exhibition'],
			'L_EDIT' => $lang['Edit'],
			'L_EDIT_EXHIBITION' => $lang['Edit_Exhibition'],
			'L_DELETE_EXHIBITION' => $lang['Delete_Exhibition'],
			'L_DELETE_SELECTED_EXHIBITIONS' => $lang['Delete_Selected_Exhibitions'],
			));
	} else {
		$smarty->assign(array(
			'L_NO_EXHIBITIONS_AVAILABLE' => $lang['No_Exhibitions_Available'],
			));
	}
}

// General page data
$smarty->assign(array(
	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_EXHIBITION_NAME' => $lang['Exhibition_Name'],
	'L_EXHIBITION_START' => $lang['Exhibition_Start'],
	'L_EXHIBITION_END' => $lang['Exhibition_End'],
	));

$view_exhibitions = false;
$exhibition_id = isset($_GET['exhibition_id']) ? handle_input_int($_GET['exhibition_id'], 1) : 0;
$action = isset($_GET['action']) ? $_GET['action'] : 'view';
switch ($action) {
	case 'add':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_exhibitions = add_exhibition();
		break;

	case 'details':
		$view_exhibitions = $exhibition_id > 0 ? details_exhibition($exhibition_id) : true;
		break;

	case 'edit':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_exhibitions = $exhibition_id > 0 ? edit_exhibition($exhibition_id) : true;
		break;

	case 'delete':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_exhibitions = delete_exhibitions();
		break;

	default:
	case 'view':
		$view_exhibitions = true;
		break;
}

if ($view_exhibitions) {
	if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
		not_allowed();
		return;
	} else {
		view_exhibitions();
	}
}

$smarty->display('exhibitions.tpl');
?>