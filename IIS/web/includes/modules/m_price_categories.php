<?php
// Encoding: utf8
/**
 * Price categories module.
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
 * @param array $price_category_data Price category data.
 * @param string $action Action to be performed in the current page.
 */
function set_template_data($price_category_data, $action)
{
	global $smarty, $lang;

	$action_price_category_url = add_query_to_url(current_full_url(), 'module', 'price_categories');

	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Price_Categories_' . ucfirst($action) . '_Header'],
		'PAGE_DESCRIPTION' => $lang['Price_Categories_' . ucfirst($action) . '_Description'],
		'ACTION' => $action . '_price_category',
		'PRICE_CATEGORY_DATA' => $price_category_data,

		'U_ACTION_PRICE_CATEGORY' => $action_price_category_url,

		'L_ACTION_PRICE_CATEGORY' => $lang[ucfirst($action) . '_Price_Category'],
		));
}

/**
 * Adds a new price_category (price category ID is zero) or update some already existing price category.
 *
 * @param int $price_category_id Price category ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_or_update_price_category($price_category_id = 0)
{
	global $smarty, $config, $lang;

	$price_category_data = array();
	try {
 		// Name
 		$price_category_data['name'] = handle_input_string_information(
 			$_POST['name'], 'name',
 			$config->get_value('min_name_length'),
 			$config->get_value('max_name_length'));
 		if (value_exists($price_category_data['name'], 'name', PRICE_CATEGORIES_TABLE) &&
 			!id_corresponds_to_value($price_category_id, 'price_category_id', $price_category_data['name'],
 				'name', PRICE_CATEGORIES_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Name_Exists']);
 		}

		$price_category_data['price_for_m2'] = handle_input_double($_POST['price_for_m2']);
		if (!is_valid_price($price_category_data['price_for_m2'], false, false)) {
			throw new IIS_Input_Exception($lang['Error_Invalid_Price']);
		}

		// Insert a new price category or update existing one
		if ($price_category_id != 0) {
			update_price_category($price_category_id, $price_category_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Price_Categories_Edited']);
		} else {
 			insert_price_category($price_category_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Price_Categories_Added']);
		}

		return true;
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
		return false;
	}
}

/**
 * Displays an 'add a new price category' page.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_price_category()
{
	global $smarty, $config, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
		array('url' => create_module_action_url('price_categories', 'view'),
			'name' => $lang['View_Price_Categories']),
		));

	// Add price category request
	$price_category_added = false;
	if (isset($_POST['add_price_category'])) {
		$price_category_added = add_or_update_price_category();
	}

	$price_category_data = generate_price_category_data();
	set_template_data($price_category_data, 'add');

	return $price_category_added;
}

/**
 * Displays details of the selected price category.
 *
 * @param int $price_category_id Price category ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function details_price_category($price_category_id)
{
	global $smarty, $user, $lang;

	// Module actions
	$edit_price_category_url = create_module_action_url('price_categories', 'edit');
	$edit_price_category_url = add_query_to_url($edit_price_category_url, 'price_category_id', $price_category_id);
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => $edit_price_category_url,
				'name' => $lang['Edit_Price_Category']),
			array('url' => create_module_action_url('price_categories', 'add'),
				'name' => $lang['Add_Price_Category']),
			array('url' => create_module_action_url('price_categories', 'view'),
				'name' => $lang['View_Price_Categories']),
		));

	$price_category_data = generate_price_category_data($price_category_id);
	set_template_data($price_category_data, 'details');

	return false;
}

/**
 * Displays page where the selected price category can be edited.
 *
 * @param int $price_category_id Price category ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function edit_price_category($price_category_id)
{
	global $smarty, $price_category, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => create_module_action_url('price_categories', 'add'),
				'name' => $lang['Add_Price_Category']),
			array('url' => create_module_action_url('price_categories', 'view'),
				'name' => $lang['View_Price_Categories']),
		));

	// Edit price category request
	$price_category_edited = false;
	if (isset($_POST['edit_price_category'])) {
		$price_category_edited = add_or_update_price_category($price_category_id);
		if ($price_category_edited) {
			actualize_all_contracts();
		}
	}

	$price_category_data = generate_price_category_data($price_category_id);
	set_template_data($price_category_data, 'edit');

	return $price_category_edited;
}

/**
 * Deletes selected price categories (according to the $_POST array) from the database.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function delete_price_categories()
{
	global $smarty, $lang;

	if (isset($_POST['delete_price_categories']) && isset($_POST['delete_price_category'])) {
		foreach ($_POST['delete_price_category'] as $raw_price_category_id) {
			$price_category_id = handle_input_int($raw_price_category_id, 1);
			if ($price_category_id == 0) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Invalid_Price_Category_Id']);
				break;
			}

			if (!price_category_can_be_deleted($price_category_id)) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Price_Category_Cannot_Be_Deleted']);
				break;
			}

			$deleted = delete_price_category($price_category_id);
			if ($deleted) {
				$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Price_Categories_Deleted']);
			} else {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Delete_Failed']);
				break;
			}
		}
	}

	return true;
}

/**
 * Displays list of "all" price_categories (the total number depends on the number of
 * records that will be displayed on a single page).
 *
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 */
function view_price_categories($where_clause = '')
{
	global $smarty, $config, $lang;

	// Page header, description and module actions
	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Price_Categories_View_Header'],
		'PAGE_DESCRIPTION' => $lang['Price_Categories_View_Description'],
		'MODULE_ACTIONS' => array(
			array('url' => create_module_action_url('price_categories', 'add'),
				'name' => $lang['Add_Price_Category']),
			),
		'ACTION' => 'view_price_categories',
		));

	// Page and records information
	$current_page_number = handle_input_int($_GET['page']);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	$records_per_page = $config->get_value('records_per_page');

	// Generate price category data
	$price_categories_data = array();
	do {
		$price_categories_data = generate_price_categories_data($records_per_page, $current_page_number,
			$_GET['order_by'], isset($_GET['descending']), $where_clause);
	} while (empty($price_categories_data) && $current_page_number-- > 1);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	if (!empty($price_categories_data)) {
		$delete_price_categories_url = add_query_to_url(current_full_url(), 'module', 'price_categories');
		$delete_price_categories_url = add_query_to_url($delete_price_categories_url, 'action', 'delete');
		$module_action_view_url = create_module_action_url('price_categories', 'view');
		$order_by_name_url = add_query_to_url($module_action_view_url, 'order_by', 'name');
		$order_by_price_for_m2_url = add_query_to_url($module_action_view_url, 'order_by', 'price_for_m2');

		$pagination = create_pagination($module_action_view_url,
			get_number_of_rows(PRICE_CATEGORIES_TABLE), $records_per_page);

		$smarty->assign(array(
			'PRICE_CATEGORIES_DATA' => $price_categories_data,
			'PAGINATION' => $pagination,
			'CURRENT_PAGE_NUMBER' => $current_page_number,

			'U_PRICE_CATEGORIES_DELETE' => $delete_price_categories_url,
			'U_ORDER_BY_NAME_DESCENDING' => add_query_to_url($order_by_name_url, 'descending', ''),
			'U_ORDER_BY_NAME_ASCENDING' => $order_by_name_url,
			'U_ORDER_BY_PRICE_FOR_M2_DESCENDING' => add_query_to_url($order_by_price_for_m2_url, 'descending', ''),
			'U_ORDER_BY_PRICE_FOR_M2_ASCENDING' => $order_by_price_for_m2_url,

			'L_GOTO_PAGE' => $lang['Goto_Page'],
			'L_ORDER_DESCENDING' => $lang['Order_Descending'],
			'L_ORDER_ASCENDING' => $lang['Order_Ascending'],
			'L_DETAILS' => $lang['Details'],
			'L_DETAILS_PRICE_CATEGORY' => $lang['Details_Price_Category'],
			'L_EDIT' => $lang['Edit'],
			'L_EDIT_PRICE_CATEGORY' => $lang['Edit_Price_Category'],
			'L_DELETE_PRICE_CATEGORY' => $lang['Delete_Price_Category'],
			'L_DELETE_SELECTED_PRICE_CATEGORIES' => $lang['Delete_Selected_Price_Categories'],
			));
	} else {
		$smarty->assign(array(
			'L_NO_PRICE_CATEGORIES_AVAILABLE' => $lang['No_Price_Categories_Available'],
			));
	}
}

// General page data
$smarty->assign(array(
	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_PRICE_CATEGORY_NAME' => $lang['Price_Category_Name'],
	'L_PRICE_FOR_M2' => $lang['Price_For_m2'],
	));

$view_price_categories = false;
$price_category_id = isset($_GET['price_category_id']) ? handle_input_int($_GET['price_category_id'], 1) : 0;
$action = isset($_GET['action']) ? $_GET['action'] : 'view';
switch ($action) {
	case 'add':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_price_categories = add_price_category();
		break;

	case 'details':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_price_categories = $price_category_id > 0 ? details_price_category($price_category_id) : true;
		break;

	case 'edit':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_price_categories = $price_category_id > 0 ? edit_price_category($price_category_id) : true;
		break;

	case 'delete':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_price_categories = delete_price_categories();
		break;

	default:
	case 'view':
		$view_price_categories = true;
		break;
}

if ($view_price_categories) {
	if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
		not_allowed();
		return;
	} else {
		view_price_categories();
	}
}

$smarty->display('price_categories.tpl');
?>