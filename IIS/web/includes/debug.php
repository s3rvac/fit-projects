<?php
// Encoding: utf8
/**
 * Debugging settings.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Prints human-readable information about a variable.
 *
 * @param mixed Variable that will be printed.
 */
function debug_print_r($variable)
{
	echo '<pre>';
	print_r($variable);
	echo '</pre>';
}

if (DEBUG) {
	// Display all errors on the screen
	error_reporting(E_ALL ^ E_NOTICE);
	ini_set('display_errors', 'on');
	ini_set('html_errors', 'on');
	ini_set('log_errors', 'off');
} else {
	// Disable error displaying and set logging into file
	error_reporting(E_ALL ^ E_NOTICE);
	ini_set('display_errors', 'off');
	ini_set('html_errors', 'off');
 	ini_set('log_errors', 'on');
 	ini_set('error_log', $logs_path . 'errors.log');
	ini_set('ignore_repeated_errors', 'on');
}
?>