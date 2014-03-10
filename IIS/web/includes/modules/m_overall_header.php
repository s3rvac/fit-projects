<?php
// Encoding: utf8
/**
 * Overall header module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

// Check for an auth request
if (isset($_POST['login']) && !$user->is_logged()) {
	// Login request
	try {
 		$username = handle_input_string($_POST['login_username']);
 		$password = $_POST['login_password'];

		$logged_successfully = $user->login($username, $password);
		if (!$logged_successfully) {
			throw new IIS_Input_Exception($lang['Error_Login_Failed']);
		}
		$module_name = 'main';
		$smarty->assign('MODULE_NAME', $module_name);
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('AUTH_ERROR', true);
		$smarty->assign('AUTH_ERROR_MESSAGE', $e->getMessage());
	}
} else if (isset($_POST['logout']) && $user->is_logged()) {
	// Logout request
	$user->logout();

	$smarty->assign('AUTH_SUCCESS', true);
	$smarty->assign('AUTH_SUCCESS_MESSAGE', $lang['Success_Logout']);
	$module_name = 'main';
	$smarty->assign('MODULE_NAME', $module_name);
}

if ($user->was_inactive()) {
	// User was logged out because of inactivity
	$smarty->assign('AUTH_ERROR', true);
	$smarty->assign('AUTH_ERROR_MESSAGE', $lang['Error_Logout_Expired_Session']);
	$module_name = 'main';
}

// Languages part
$available_languages = load_available_languages();
$modified_available_languages = array();
foreach (array_values($available_languages) as $short_name) {
	$change_language_url = add_query_to_url(current_full_url(),
		'lang', $short_name);
	array_push($modified_available_languages,
		array($short_name => $change_language_url));
}
$smarty->assign('AVAILABLE_LANGUAGES', $modified_available_languages);

// Login/logout forms
if ($user->is_logged()) {
	$smarty->assign(array(
		'IS_LOGGED' => true,
		'LOGGED_USERNAME' => $user->get_username(),
		'L_LOGGED' => $lang['Logged'],
		'L_LOGOUT' => $lang['Logout'],
		'L_PROFILE' => $lang['Profile'],
		'U_LOGOUT' => current_full_url(),
		'U_PROFILE' => create_module_action_url('users', 'profile'),
	));
} else {
	$smarty->assign(array(
		'IS_LOGGED' => false,
		'LOGIN_USERNAME' => $_POST['login_username'],
		'L_LOGIN' => $lang['Login'],
		'L_USERNAME' => $lang['Username'],
		'L_PASSWORD' => $lang['Password'],
		'U_LOGIN' => current_full_url(),
	));
}

// General module data
$smarty->assign(array(
	'WEB_DESCRIPTION' => $lang['Web_Description'],
	'WEB_KEYWORDS' => $lang['Web_Keywords'],
	'WEB_HEAD_TITLE' => $lang['Web_Head_Title'],
	'WEB_TITLE' => $lang['Web_Title'],
	));

$smarty->display('overall_header.tpl');
?>