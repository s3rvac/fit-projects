<?php
// Encoding: utf8
/**
 * Web entry point - this is the only file that is viewable via browser.
 * Which page will be shown depends on URL arguments (and session data).
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

/**
 * Security precaution - all pages that are to be included are checking whether
 * this constant is defined.
 */
define('IIS_WEB', true);

// Common variables and includes
if (!isset($web_root_path)) {
	$web_root_path = './';
}
require_once $web_root_path . 'includes/common.php';

perform_server_specific_actions();

try {
	start_output_buffering();

	// Initialize all needed components
	require_once $web_root_path . 'includes/inits/init_smarty.php';
	require_once $web_root_path . 'includes/inits/init_db.php';
	require_once $web_root_path . 'includes/inits/init_config.php';
	require_once $web_root_path . 'includes/inits/init_session.php';
	require_once $web_root_path . 'includes/inits/init_user.php';
	require_once $web_root_path . 'includes/inits/init_lang.php';

	set_proper_encoding();

	// Assing general data needed in templates
	$smarty->assign(array(
		'WEB_ROOT_PATH' => $web_root_path,
		'TEMPLATES_PATH' => $templates_path,
		'BROWSER_TYPE' => get_users_browser_type(),

		// Groups
		'VISITOR' => VISITOR,
		'COMPANY_MEMBER' => COMPANY_MEMBER,
		'EXHIBITIONGROUND_MEMBER' => EXHIBITIONGROUND_MEMBER,

		// User data
		'USER_LANGUAGE' => $user->get_current_language(),
		'USER_GROUP' => $user->get_group_id(),

		'U_BACK' => $user->get_previous_page_url(),

		'L_BACK' => $lang['Back'],
		'L_MODULE_ACTIONS' => $lang['Module_Actions'],
		));

	// Show selected module
	$default_module_name = 'main';
	$module_name = (isset($_GET['module']) && is_valid_module_name($_GET['module'])) ?
		$_GET['module'] : $default_module_name;
	$smarty->assign('MODULE_NAME', $module_name);
	require_once $web_root_path . 'includes/modules/m_overall_header.php';
	require_once $web_root_path . 'includes/modules/m_overall_sidebar.php';
	require_once $web_root_path . 'includes/modules/m_' . $module_name . '.php';
	require_once $web_root_path . 'includes/modules/m_overall_footer.php';

	display_output_buffer();
} catch (IIS_Exception $e) {
	clean_output_buffer();

	if (DEBUG) {
		$e->print_error_message();
	}
	else {
		die('A system error has occured. Please contact ' .
			'<a href="mailto:xzemek02@stud.fit.vutbr.cz">xzemek02@stud.fit.vutbr.cz</a> ' .
			'if the problem persists.');
	}
}

// Clean-up
mysql_close();
?>