<?php
// Encoding: utf8
/**
 * Users module.
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
 * @param array $user_data User data.
 * @param string $action Action to be performed in the current page.
 */
function set_template_data($user_data, $action)
{
	global $smarty, $lang;

	$action_user_url = add_query_to_url(current_full_url(), 'module', 'users');

	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Users_' . ucfirst($action) . '_Header'],
		'PAGE_DESCRIPTION' => $lang['Users_' . ucfirst($action) . '_Description'],
		'ACTION' => $action . '_user',
		'USER_DATA' => $user_data,
		'GROUPS' => generate_groups(),
		'COMPANIES' => load_available_companies(),
		'CAN_HAVE_GROUP_CHANGED' => user_can_have_group_changed($user_data['user_id']),

		'U_ACTION_USER' => $action_user_url,

		'L_ACTION_USER' => $lang[ucfirst($action) . '_User'],
		));
}

/**
 * Adds a new user (user ID is zero) or update some already existing user.
 *
 * @param int $user_id User ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_or_update_user($user_id = 0)
{
	global $smarty, $config, $lang;

	$user_data = array();
	try {
		// Username
 		$user_data['username'] = handle_input_string_information(
 			$_POST['username'], 'username',
 			$config->get_value('min_username_length'),
 			$config->get_value('max_username_length'));
 		if (value_exists($user_data['username'], 'username', USERS_TABLE) &&
 			!id_corresponds_to_value($user_id, 'user_id', $user_data['username'],
 				'username', USERS_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Username_Exists']);
 		}

 		// Name
 		$user_data['name'] = handle_input_string_information(
 			$_POST['name'], 'name',
 			$config->get_value('min_name_length'),
 			$config->get_value('max_name_length'));

 		// Password
 		if ($user_id == 0 || (isset($_POST['password']) && !empty($_POST['password']))) {
			$user_data['password'] = handle_input_string_information(
				$_POST['password'], 'password',
				$config->get_value('min_password_length'),
				$config->get_value('max_password_length'),
				false);
			$password_check = $_POST['password_check'];
			if ($user_data['password'] != $password_check) {
				throw new IIS_Input_Exception($lang['Error_Passwords_Not_Match']);
			}
 		}

 		// Email
 		$user_data['email'] = handle_input_email($_POST['email']);

		if (!user_can_have_group_changed($user_id)) {
			$user_data['group_id'] = get_value_corresponding_to_other_value(
				'group_id', $user_id, 'user_id', USERS_TABLE);
			$user_data['company_id'] = get_value_corresponding_to_other_value(
				'company_id', $user_id, 'user_id', USERS_TABLE);
 		} else {
			// Group
			$user_data['group_id'] = handle_input_int($_POST['group_id'], 1);
			if (!is_valid_id($user_data['group_id'], 'group_id', GROUPS_TABLE)) {
				throw new IIS_Input_Exception($lang['Error_Invalid_Group']);
			}

			// Company
			$user_data['company_id'] = handle_input_int($_POST['company_id'], 1);
			if ($user_data['group_id'] != COMPANY_MEMBER) {
				$user_data['company_id'] = null;
			} else {
				if (!is_valid_id($user_data['company_id'], 'company_id', COMPANIES_TABLE)) {
					throw new IIS_Input_Exception($lang['Error_Invalid_Company']);
				}
			}
 		}

		// Optional fields
		$user_data['telephone'] = handle_input_string($_POST['telephone']);

		// Insert a new user or update existing one
		if ($user_id != 0) {
			update_user($user_id, $user_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Users_Edited']);
		} else {
 			insert_user($user_data);
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Users_Added']);
		}

		return true;
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
		return false;
	}
}

/**
 * Displays an 'add a new user' page.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function add_user()
{
	global $smarty, $config, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
		array('url' => create_module_action_url('users', 'view'),
			'name' => $lang['View_Users']),
		));

	// Add user request
	$user_added = false;
	if (isset($_POST['add_user'])) {
		$user_added = add_or_update_user();
	}

	$user_data = generate_user_data();
	set_template_data($user_data, 'add');

	return $user_added;
}

/**
 * Displays details of the selected user.
 *
 * @param int $user_id User ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function details_user($user_id)
{
	global $smarty, $user, $lang;

	// Module actions
	$edit_user_url = create_module_action_url('users', 'edit');
	$edit_user_url = add_query_to_url($edit_user_url, 'user_id', $user_id);
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => $edit_user_url,
				'name' => $lang['Edit_User']),
			array('url' => create_module_action_url('users', 'add'),
				'name' => $lang['Add_User']),
			array('url' => create_module_action_url('users', 'view'),
				'name' => $lang['View_Users']),
		));

	$user_data = generate_user_data($user_id);
	set_template_data($user_data, 'details');

	return false;
}

/**
 * Displays page where the selected user can be edited.
 *
 * @param int $user_id User ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function edit_user($user_id)
{
	global $smarty, $user, $lang;

	// Module actions
	$smarty->assign('MODULE_ACTIONS', array(
			array('url' => create_module_action_url('users', 'add'),
				'name' => $lang['Add_User']),
			array('url' => create_module_action_url('users', 'view'),
				'name' => $lang['View_Users']),
		));

	// Check whether the user is not trying to edit his own account this way
	if ($user->get_user_id() == $user_id) {
		$smarty->assign('ERROR_MESSAGE', $lang['Error_Cannot_Edit_Own_Account']);
		return true;
	}

	// Edit user request
	$user_edited = false;
	if (isset($_POST['edit_user'])) {
		// Check whether the user is not trying to edit his account
		if ($user->get_user_id() == $user_id) {
			$smarty->assign('ERROR_MESSAGE', $lang['Error_Cannot_Edit_Own_Account']);
		} else {
			$user_edited = add_or_update_user($user_id);
		}
	}

	$user_data = generate_user_data($user_id);
	set_template_data($user_data, 'edit');

	return $user_edited;
}

/**
 * Displays page where the current user profile can be edited.
 *
 * @param int $user_id User ID.
 * @return bool True if the action was performed successfully, false otherwise.
 */
function profile_user($user_id)
{
	global $smarty, $user, $lang;

	// Edit user profile request
	$profile_edited = false;
	if (isset($_POST['profile_user'])) {
		// Set some data that cannot be changed in the profile
		$_POST['group_id'] = $user->get_group_id();
		$_POST['company_id'] = $user->get_company_id();
		$_POST['username'] = $user->get_username();

		$profile_edited = add_or_update_user($user_id);
		if ($profile_edited) {
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Profile_Edited']);
		}
	}

	$user_data = generate_user_data($user_id);
	set_template_data($user_data, 'profile');

	return $profile_edited;
}

/**
 * Deletes selected users (according to the $_POST array) from the database.
 *
 * @return bool True if the action was performed successfully, false otherwise.
 */
function delete_users()
{
	global $smarty, $user, $lang;

	if (isset($_POST['delete_users']) && isset($_POST['delete_user'])) {
		foreach ($_POST['delete_user'] as $raw_user_id) {
			$user_id = handle_input_int($raw_user_id, 1);
			if ($user_id == 0) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Invalid_User_Id']);
				break;
			}

			// Check whether the user is not trying to delete his own account
			if ($user->get_user_id() == $user_id) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Cannot_Delete_Own_Account']);
				break;
			}

			if (!user_can_be_deleted($user_id)) {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_User_Cannot_Be_Deleted']);
				break;
			}

			$deleted = delete_user($user_id);
			if ($deleted) {
				$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Users_Deleted']);
			} else {
				$smarty->assign('ERROR_MESSAGE', $lang['Error_Delete_Failed']);
				break;
			}
		}
	}

	return true;
}

/**
 * Displays list of "all" users (the total number depends on the number of
 * records that will be displayed on a single page).
 *
 * @param string $where_clause Complete MySQL "WHERE ..." clause.
 */
function view_users($where_clause = '')
{
	global $smarty, $config, $lang;

	// Page header, description and module actions
	$smarty->assign(array(
		'PAGE_HEADER' => $lang['Users_View_Header'],
		'PAGE_DESCRIPTION' => $lang['Users_View_Description'],
		'MODULE_ACTIONS' => array(
			array('url' => create_module_action_url('users', 'add'),
				'name' => $lang['Add_User']),
			),
		'ACTION' => 'view_users',
		));

	// Page and records information
	$current_page_number = handle_input_int($_GET['page']);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	$records_per_page = $config->get_value('records_per_page');

	// Generate user data
	$users_data = array();
	do {
		$users_data = generate_users_data($records_per_page, $current_page_number,
			$_GET['order_by'], isset($_GET['descending']), $where_clause);
	} while (empty($users_data) && $current_page_number-- > 1);
	$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
	if (!empty($users_data)) {
		$delete_users_url = add_query_to_url(current_full_url(), 'module', 'users');
		$delete_users_url = add_query_to_url($delete_users_url, 'action', 'delete');
		$module_action_view_url = create_module_action_url('users', 'view');
		$order_by_username_url = add_query_to_url($module_action_view_url, 'order_by', 'username');
		$order_by_name_url = add_query_to_url($module_action_view_url, 'order_by', 'name');
		$order_by_group_url = add_query_to_url($module_action_view_url, 'order_by', 'group_id');
		$order_by_company_url = add_query_to_url($module_action_view_url, 'order_by', 'company');

		$pagination = create_pagination($module_action_view_url,
			get_number_of_rows(USERS_TABLE), $records_per_page);

		$smarty->assign(array(
			'USERS_DATA' => $users_data,
			'PAGINATION' => $pagination,
			'CURRENT_PAGE_NUMBER' => $current_page_number,

			'U_USERS_DELETE' => $delete_users_url,
			'U_ORDER_BY_USERNAME_DESCENDING' => add_query_to_url($order_by_username_url, 'descending', ''),
			'U_ORDER_BY_USERNAME_ASCENDING' => $order_by_username_url,
			'U_ORDER_BY_NAME_DESCENDING' => add_query_to_url($order_by_name_url, 'descending', ''),
			'U_ORDER_BY_NAME_ASCENDING' => $order_by_name_url,
			'U_ORDER_BY_GROUP_DESCENDING' => add_query_to_url($order_by_group_url, 'descending', ''),
			'U_ORDER_BY_GROUP_ASCENDING' => $order_by_group_url,
			'U_ORDER_BY_COMPANY_DESCENDING' => add_query_to_url($order_by_company_url, 'descending', ''),
			'U_ORDER_BY_COMPANY_ASCENDING' => $order_by_company_url,

			'L_GOTO_PAGE' => $lang['Goto_Page'],
			'L_ORDER_DESCENDING' => $lang['Order_Descending'],
			'L_ORDER_ASCENDING' => $lang['Order_Ascending'],
			'L_DETAILS' => $lang['Details'],
			'L_DETAILS_USER' => $lang['Details_User'],
			'L_EDIT' => $lang['Edit'],
			'L_EDIT_USER' => $lang['Edit_User'],
			'L_DELETE_USER' => $lang['Delete_User'],
			'L_DELETE_SELECTED_USERS' => $lang['Delete_Selected_Users'],
			));
	} else {
		$smarty->assign(array(
			'L_NO_USERS_AVAILABLE' => $lang['No_Users_Available'],
			));
	}
}

// General page data
$smarty->assign(array(
	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_USERNAME' => $lang['Username'],
	'L_NAME' => $lang['Name'],
	'L_PASSWORD' => $lang['Password'],
	'L_PASSWORD_CHECK' => $lang['Password_Check'],
	'L_EMAIL' => $lang['Email'],
	'L_TELEPHONE' => $lang['Telephone'],
	'L_GROUP' => $lang['Group'],
	'L_COMPANY' => $lang['Company'],
	));

$view_users = false;
$user_id = isset($_GET['user_id']) ? handle_input_int($_GET['user_id'], 1) : 0;
$action = isset($_GET['action']) ? $_GET['action'] : 'view';
switch ($action) {
	case 'add':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_users = add_user();
		break;

	case 'details':
		$view_users = $user_id > 0 ? details_user($user_id) : true;
		break;

	case 'profile':
		if (!$user->is_logged()) {
			not_allowed();
			return;
		}
		profile_user($user->get_user_id());
		break;

	case 'edit':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_users = $user_id > 0 ? edit_user($user_id) : true;
		break;

	case 'delete':
		if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
			not_allowed();
			return;
		}
		$view_users = delete_users();
		break;

	default:
	case 'view':
		$view_users = true;
		break;
}

if ($view_users) {
	if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
		not_allowed();
		return;
	} else {
		view_users();
	}
}

$smarty->display('users.tpl');
?>