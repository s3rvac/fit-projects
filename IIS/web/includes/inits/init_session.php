<?php
// Encoding: utf8
/**
 * Creates and initialize an instance of class User.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

$session = Session::create_instance();
?>