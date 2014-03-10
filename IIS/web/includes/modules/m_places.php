<?php
// Encoding: utf8
/**
 * Places module.
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
 * @param array $place_data Place data.
 * @param string $action Action to be performed in the current page.
 */
function set_template_data($place_data, $action)
{
	global $smarty, $lang;

	$action_place_url = add_query_to_url(current_full_url(), 'module', 'places');

	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Places_' . ucfirst($action) . '_Header'],
		'PAGE_DESCRIPTION' => $lang['Places_' . ucfirst($action) . '_Description'],
		'ACTION' => $action . '_place',
		'PLACE_DATA' => $place_data,
		'PAVILIONS' => load_available_pavilions(),
		'PRICE_CATEGORIES' => load_available_price_categories(),

		'U_ACTION_PLACE' => $action_place_url,

		'L_ACTION_PLACE' => $lang[ucfirst($action) . '_Place'],
		));
}

/**
 * Adds a new place (place ID is zero) or update some already existing place.
 *
 * @param int $place_id Place ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_or_update_place($place_id = 0)
{
	global $smarty, $config, $lang;

	$place_data = array();
	try {
		// Pavilion
 		$place_data['pavilion_id'] = handle_input_int($_POST['pavilion_id'], 1);
 		if (!is_valid_id($place_data['pavilion_id'], 'pavilion_id', PAVILIONS_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Invalid_Price_Category_Id']);
 		}

		// Place number
		$place_data['number'] = handle_input_int($_POST['number'],
			$config->get_value('min_place_number_length'),
			$config->get_value('max_place_number_length'));
		if ($place_data['number'] < 1) {
			throw new IIS_Input_Exception($lang['Error_Invalid_Place_Number']);
		} else if (value_exists($place_data['number'], 'number', PLACES_TABLE) &&
 			id_corresponds_to_value($place_data['pavilion_id'], 'pavilion_id', $place_data['number'],
 				'number', PLACES_TABLE) &&
 			!id_corresponds_to_value($place_id, 'place_id', $place_data['pavilion_id'],
 				'pavilion_id', PLACES_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Place_Number_Exists_In_Pavilion']);
 		}

		// Area
		$place_data['area'] = handle_input_int($_POST['area'], 1);
		if ($place_data['area'] < 1) {
			throw new IIS_Input_Exception($lang['Error_Invalid_Area']);
		}

		// Price category
 		$place_data['price_category_id'] = handle_input_int($_POST['price_category_id'], 1);
 		if (!is_valid_id($place_data['price_category_id'], 'price_category_id', PRICE_CATEGORIES_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Invalid_Price_Category_Id']);
 		}

		// Insert a new place or update existing one
		if ($place_id != 0) {
			update_place($place_id, $place_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Places_Edited']);
		} else {
 			insert_place($place_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Places_Added']);
		}

		return true;
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
		return false;
	}
}

/**
 * Displays an 'add a new place' page.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_place()
{
	global $smarty, $config, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
		array('url' => create_module_action_url('places', 'view'),
			'name' => $lang['View_Places']),
		));

	// Add place request
	$place_added = false;
	if (isset($_POST['add_place'])) {
		$place_added = add_or_update_place();
	}

	$place_data = generate_place_data();
	set_template_data($place_data, 'add');

	return $place_added;
}

/**
 * Displays details of the selected place.
 *
 * @param int $place_id Place ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function details_place($place_id)
{
	global $smarty, $user, $lang;

	// Module actions
	$edit_place_url = create_module_action_url('places', 'edit');
	$edit_place_url = add_query_to_url($edit_place_url, 'place_id', $place_id);
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => $edit_place_url,
				'name' => $lang['Edit_Place']),
			array('url' => create_module_action_url('places', 'add'),
				'name' => $lang['Add_Place']),
			array('url' => create_module_action_url('places', 'view'),
				'name' => $lang['View_Places']),
		));

	$place_data = generate_place_data($place_id);
	set_template_data($place_data, 'details');

	return false;
}

/**
 * Displays page where the selected place can be edited.
 *
 * @param int $place_id Place ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function edit_place($place_id)
{
	global $smarty, $place, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => create_module_action_url('places', 'add'),
				'name' => $lang['Add_Place']),
			array('url' => create_module_action_url('places', 'view'),
				'name' => $lang['View_Places']),
		));

	// Edit place request
	$place_edited = false;
	if (isset($_POST['edit_place'])) {
		$place_edited = add_or_update_place($place_id);
		if ($place_edited) {
			actualize_all_contracts();
		}
	}

	$place_data = generate_place_data($place_id);
	set_template_data($place_data, 'edit');

	return $place_edited;
}

/**
 * Deletes selected places (according to the $_POST array) from the database.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function delete_places()
{
	global $smarty, $lang;

	if (isset($_POST['delete_places']) && isset($_POST['delete_place'])) {
		foreach ($_POST['delete_place'] as $raw_place_id) {
			$place_id = handle_input_int($raw_place_id, 1);
			if ($place_id == 0) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Invalid_Place_Id']);
				break;
			}

			if (!place_can_be_deleted($place_id)) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Place_Cannot_Be_Deleted']);
				break;
			}

			$deleted = delete_place($place_id);
			if ($deleted) {
				$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Places_Deleted']);
			} else {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Delete_Failed']);
				break;
			}
		}
	}

	return true;
}

/**
 * Displays list of "all" places (the total number depends on the number of
 * records that will be displayed on a single page).
 *
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 */
function view_places($where_clause = '')
{
	global $smarty, $config, $lang;

	// Page header, description and module actions
	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Places_View_Header'],
		'PAGE_DESCRIPTION' => $lang['Places_View_Description'],
		'MODULE_ACTIONS' => array(
			array('url' => create_module_action_url('places', 'add'),
				'name' => $lang['Add_Place']),
			),
		'ACTION' => 'view_places',
		));

	// Page and records information
	$current_page_number = handle_input_int($_GET['page']);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	$records_per_page = $config->get_value('records_per_page');

	// Generate place data
	$places_data = array();
	do {
		$places_data = generate_places_data($records_per_page, $current_page_number,
			$_GET['order_by'], isset($_GET['descending']), $where_clause);
	} while (empty($places_data) && $current_page_number-- > 1);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	if (!empty($places_data)) {
		$delete_places_url = add_query_to_url(current_full_url(), 'module', 'places');
		$delete_places_url = add_query_to_url($delete_places_url, 'action', 'delete');
		$module_action_view_url = create_module_action_url('places', 'view');
		$order_by_number_url = add_query_to_url($module_action_view_url, 'order_by', 'number');
		$order_by_pavilion_url = add_query_to_url($module_action_view_url, 'order_by', 'pavilion');
		$order_by_price_category_url = add_query_to_url($module_action_view_url, 'order_by', 'price_category');

		$pagination = create_pagination($module_action_view_url,
			get_number_of_rows(PLACES_TABLE), $records_per_page);

		$smarty->assign(array(
			'PLACES_DATA' => $places_data,
			'PAGINATION' => $pagination,
			'CURRENT_PAGE_NUMBER' => $current_page_number,

			'U_PLACES_DELETE' => $delete_places_url,
			'U_ORDER_BY_NUMBER_DESCENDING' => add_query_to_url($order_by_number_url, 'descending', ''),
			'U_ORDER_BY_NUMBER_ASCENDING' => $order_by_number_url,
			'U_ORDER_BY_PAVILION_DESCENDING' => add_query_to_url($order_by_pavilion_url, 'descending', ''),
			'U_ORDER_BY_PAVILION_ASCENDING' => $order_by_pavilion_url,
			'U_ORDER_BY_PRICE_CATEGORY_DESCENDING' => add_query_to_url($order_by_price_category_url, 'descending', ''),
			'U_ORDER_BY_PRICE_CATEGORY_ASCENDING' => $order_by_price_category_url,

			'L_GOTO_PAGE' => $lang['Goto_Page'],
			'L_ORDER_DESCENDING' => $lang['Order_Descending'],
			'L_ORDER_ASCENDING' => $lang['Order_Ascending'],
			'L_DETAILS' => $lang['Details'],
			'L_DETAILS_PLACE' => $lang['Details_Place'],
			'L_EDIT' => $lang['Edit'],
			'L_EDIT_PLACE' => $lang['Edit_Place'],
			'L_DELETE_PLACE' => $lang['Delete_Place'],
			'L_DELETE_SELECTED_PLACES' => $lang['Delete_Selected_Places'],
			));
	} else {
		$smarty->assign(array(
			'L_NO_PLACES_AVAILABLE' => $lang['No_Places_Available'],
			));
	}
}

// General page data
$smarty->assign(array(
	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_PLACE_NUMBER' => $lang['Place_Number'],
	'L_AREA' => $lang['Area'],
	'L_PAVILION' => $lang['Pavilion'],
	'L_PRICE_CATEGORY' => $lang['Price_Category'],
	));

$view_places = false;
$place_id = isset($_GET['place_id']) ? handle_input_int($_GET['place_id'], 1) : 0;
$action = isset($_GET['action']) ? $_GET['action'] : 'view';
switch ($action) {
	case 'add':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_places = add_place();
		break;

	case 'details':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_places = $place_id > 0 ? details_place($place_id) : true;
		break;

	case 'edit':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_places = $place_id > 0 ? edit_place($place_id) : true;
		break;

	case 'delete':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_places = delete_places();
		break;

	default:
	case 'view':
		$view_places = true;
		break;
}

if ($view_places) {
	if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
		not_allowed();
		return;
	} else {
		view_places();
	}
}

$smarty->display('places.tpl');
?>