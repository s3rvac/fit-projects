<?php
// Encoding: utf8
/**
 * Pavilions module.
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
 * @param array $pavilion_data Pavilion data.
 * @param string $action Action to be performed in the current page.
 */
function set_template_data($pavilion_data, $action)
{
	global $smarty, $lang;

	$action_pavilion_url = add_query_to_url(current_full_url(), 'module', 'pavilions');

	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Pavilions_' . ucfirst($action) . '_Header'],
		'PAGE_DESCRIPTION' => $lang['Pavilions_' . ucfirst($action) . '_Description'],
		'ACTION' => $action . '_pavilion',
		'PAVILION_DATA' => $pavilion_data,

		'U_ACTION_PAVILION' => $action_pavilion_url,

		'L_ACTION_PAVILION' => $lang[ucfirst($action) . '_Pavilion'],
		));
}

/**
 * Adds a new pavilion (pavilion ID is zero) or update some already existing pavilion.
 *
 * @param int $pavilion_id Pavilion ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_or_update_pavilion($pavilion_id = 0)
{
	global $smarty, $config, $lang;

	$pavilion_data = array();
	try {
 		// Name
 		$pavilion_data['name'] = handle_input_string_information(
 			$_POST['name'], 'name',
 			$config->get_value('min_name_length'),
 			$config->get_value('max_name_length'));
 		if (value_exists($pavilion_data['name'], 'name', PAVILIONS_TABLE) &&
 			!id_corresponds_to_value($pavilion_id, 'pavilion_id', $pavilion_data['name'],
 				'name', PAVILIONS_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Name_Exists']);
 		}

		// Number of ...
 		$pavilion_data['number_of_floors'] = handle_input_int($_POST['number_of_floors'], 1);
 		if ($pavilion_data['number_of_floors'] == 0) {
			throw new IIS_Input_Exception($lang['Error_Pavilions_Invalid_Number_Of_Floors']);
 		}
 		$pavilion_data['number_of_restaurants'] = handle_input_int($_POST['number_of_restaurants'], 0);
 		$pavilion_data['number_of_buffets'] = handle_input_int($_POST['number_of_buffets'], 0);
 		$pavilion_data['number_of_social_settlements'] = handle_input_int($_POST['number_of_social_settlements'], 0);

		// Insert a new pavilion or update existing one
		if ($pavilion_id != 0) {
			update_pavilion($pavilion_id, $pavilion_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Pavilions_Edited']);
		} else {
 			insert_pavilion($pavilion_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Pavilions_Added']);
		}

		return true;
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
		return false;
	}
}

/**
 * Displays an 'add a new pavilion' page.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_pavilion()
{
	global $smarty, $config, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
		array('url' => create_module_action_url('pavilions', 'view'),
			'name' => $lang['View_Pavilions']),
		));

	// Add pavilion request
	$pavilion_added = false;
	if (isset($_POST['add_pavilion'])) {
		$pavilion_added = add_or_update_pavilion();
	}

	$pavilion_data = generate_pavilion_data();
	set_template_data($pavilion_data, 'add');

	return $pavilion_added;
}

/**
 * Displays details of the selected pavilion.
 *
 * @param int $pavilion_id Pavilion ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function details_pavilion($pavilion_id)
{
	global $smarty, $user, $lang;

	// Module actions
	$edit_pavilion_url = create_module_action_url('pavilions', 'edit');
	$edit_pavilion_url = add_query_to_url($edit_pavilion_url, 'pavilion_id', $pavilion_id);
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => $edit_pavilion_url,
				'name' => $lang['Edit_Pavilion']),
			array('url' => create_module_action_url('pavilions', 'add'),
				'name' => $lang['Add_Pavilion']),
			array('url' => create_module_action_url('pavilions', 'view'),
				'name' => $lang['View_Pavilions']),
		));

	$pavilion_data = generate_pavilion_data($pavilion_id);
	set_template_data($pavilion_data, 'details');

	return false;
}

/**
 * Displays page where the selected pavilion can be edited.
 *
 * @param int $pavilion_id Pavilion ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function edit_pavilion($pavilion_id)
{
	global $smarty, $pavilion, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => create_module_action_url('pavilions', 'add'),
				'name' => $lang['Add_Pavilion']),
			array('url' => create_module_action_url('pavilions', 'view'),
				'name' => $lang['View_Pavilions']),
		));

	// Edit pavilion request
	$pavilion_edited = false;
	if (isset($_POST['edit_pavilion'])) {
		$pavilion_edited = add_or_update_pavilion($pavilion_id);
	}

	$pavilion_data = generate_pavilion_data($pavilion_id);
	set_template_data($pavilion_data, 'edit');

	return $pavilion_edited;
}

/**
 * Deletes selected pavilions (according to the $_POST array) from the database.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function delete_pavilions()
{
	global $smarty, $lang;

	if (isset($_POST['delete_pavilions']) && isset($_POST['delete_pavilion'])) {
		foreach ($_POST['delete_pavilion'] as $raw_pavilion_id) {
			$pavilion_id = handle_input_int($raw_pavilion_id, 1);
			if ($pavilion_id == 0) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Invalid_Pavilion_Id']);
				break;
			}

			if (!pavilion_can_be_deleted($pavilion_id)) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Pavilion_Cannot_Be_Deleted']);
				break;
			}

			$deleted = delete_pavilion($pavilion_id);
			if ($deleted) {
				$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Pavilions_Deleted']);
			} else {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Delete_Failed']);
				break;
			}
		}
	}

	return true;
}

/**
 * Displays list of "all" pavilions (the total number depends on the number of
 * records that will be displayed on a single page).
 *
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 */
function view_pavilions($where_clause = '')
{
	global $smarty, $config, $lang;

	// Page header, description and module actions
	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Pavilions_View_Header'],
		'PAGE_DESCRIPTION' => $lang['Pavilions_View_Description'],
		'MODULE_ACTIONS' => array(
			array('url' => create_module_action_url('pavilions', 'add'),
				'name' => $lang['Add_Pavilion']),
			),
		'ACTION' => 'view_pavilions',
		));

	// Page and records information
	$current_page_number = handle_input_int($_GET['page']);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	$records_per_page = $config->get_value('records_per_page');

	// Generate pavilion data
	$pavilions_data = array();
	do {
		$pavilions_data = generate_pavilions_data($records_per_page, $current_page_number,
			$_GET['order_by'], isset($_GET['descending']), $where_clause);
	} while (empty($pavilions_data) && $current_page_number-- > 1);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	if (!empty($pavilions_data)) {
		$delete_pavilions_url = add_query_to_url(current_full_url(), 'module', 'pavilions');
		$delete_pavilions_url = add_query_to_url($delete_pavilions_url, 'action', 'delete');
		$module_action_view_url = create_module_action_url('pavilions', 'view');
		$order_by_name_url = add_query_to_url($module_action_view_url, 'order_by', 'name');
		$order_by_number_of_floors_url = add_query_to_url($module_action_view_url, 'order_by', 'number_of_floors');

		$pagination = create_pagination($module_action_view_url,
			get_number_of_rows(PAVILIONS_TABLE), $records_per_page);

		$smarty->assign(array(
			'PAVILIONS_DATA' => $pavilions_data,
			'PAGINATION' => $pagination,
			'CURRENT_PAGE_NUMBER' => $current_page_number,

			'U_PAVILIONS_DELETE' => $delete_pavilions_url,
			'U_ORDER_BY_NAME_DESCENDING' => add_query_to_url($order_by_name_url, 'descending', ''),
			'U_ORDER_BY_NAME_ASCENDING' => $order_by_name_url,
			'U_ORDER_BY_NUMBER_OF_FLOORS_DESCENDING' => add_query_to_url($order_by_number_of_floors_url, 'descending', ''),
			'U_ORDER_BY_NUMBER_OF_FLOORS_ASCENDING' => $order_by_number_of_floors_url,

			'L_GOTO_PAGE' => $lang['Goto_Page'],
			'L_ORDER_DESCENDING' => $lang['Order_Descending'],
			'L_ORDER_ASCENDING' => $lang['Order_Ascending'],
			'L_DETAILS' => $lang['Details'],
			'L_DETAILS_PAVILION' => $lang['Details_Pavilion'],
			'L_EDIT' => $lang['Edit'],
			'L_EDIT_PAVILION' => $lang['Edit_Pavilion'],
			'L_DELETE_PAVILION' => $lang['Delete_Pavilion'],
			'L_DELETE_SELECTED_PAVILIONS' => $lang['Delete_Selected_Pavilions'],
			));
	} else {
		$smarty->assign(array(
			'L_NO_PAVILIONS_AVAILABLE' => $lang['No_Pavilions_Available'],
			));
	}
}

// General page data
$smarty->assign(array(
	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_PAVILION_NAME' => $lang['Pavilion_Name'],
	'L_NUMBER_OF_FLOORS' => $lang['Number_Of_Floors'],
	'L_NUMBER_OF_RESTAURANTS' => $lang['Number_Of_Restaurants'],
	'L_NUMBER_OF_BUFFETS' => $lang['Number_Of_Buffets'],
	'L_NUMBER_OF_SOCIAL_SETTLEMENTS' => $lang['Number_Of_Settlements'],
	));

$view_pavilions = false;
$pavilion_id = isset($_GET['pavilion_id']) ? handle_input_int($_GET['pavilion_id'], 1) : 0;
$action = isset($_GET['action']) ? $_GET['action'] : 'view';
switch ($action) {
	case 'add':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_pavilions = add_pavilion();
		break;

	case 'details':
		$view_pavilions = $pavilion_id > 0 ? details_pavilion($pavilion_id) : true;
		break;

	case 'edit':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_pavilions = $pavilion_id > 0 ? edit_pavilion($pavilion_id) : true;
		break;

	case 'delete':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_pavilions = delete_pavilions();
		break;

	default:
	case 'view':
		$view_pavilions = true;
		break;
}

if ($view_pavilions) {
	if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
		not_allowed();
		return;
	} else {
		view_pavilions();
	}
}

$smarty->display('pavilions.tpl');
?>