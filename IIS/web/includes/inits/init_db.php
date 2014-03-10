<?php
// Encoding: utf8
/**
 * Creates and initialize a database connection.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

// Establish database connection (data are stored in config.php)
$dbc = mysql_connect($db_host, $db_user, $db_pass);
if (!$dbc) {
	throw new IIS_DB_Exception('Cannot connect to the database.', '',
		mysql_error());
}

// Select proper database
$db_selected = mysql_select_db($db_name);
if (!$db_selected) {
	throw new IIS_DB_Exception('Cannot select database.', '',
		mysql_error());
}
?>