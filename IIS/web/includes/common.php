<?php
// Encoding: utf8
/**
 * Includes all common files that are needed in the index.php file.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

// General files
require_once $web_root_path . 'config.php';
require_once $web_root_path . 'includes/debug.php';
require_once $web_root_path . 'includes/constants.php';

// Functions
require_once $web_root_path . 'includes/functions/functions.php';
require_once $web_root_path . 'includes/functions/functions_actualize.php';
require_once $web_root_path . 'includes/functions/functions_can.php';
require_once $web_root_path . 'includes/functions/functions_get.php';
require_once $web_root_path . 'includes/functions/functions_input.php';
require_once $web_root_path . 'includes/functions/functions_output.php';
require_once $web_root_path . 'includes/functions/functions_insert.php';
require_once $web_root_path . 'includes/functions/functions_load.php';
require_once $web_root_path . 'includes/functions/functions_delete.php';
require_once $web_root_path . 'includes/functions/functions_update.php';
require_once $web_root_path . 'includes/functions/functions_generate.php';
require_once $web_root_path . 'includes/functions/functions_validate.php';

// 3rd party libraries
require_once $web_root_path . 'libs/Smarty/Smarty.class.php';

// Classes
require_once($web_root_path . 'includes/classes/class_Config.php');
require_once($web_root_path . 'includes/classes/class_IIS_Exception.php');
require_once($web_root_path . 'includes/classes/class_Session.php');
require_once($web_root_path . 'includes/classes/class_User.php');
?>