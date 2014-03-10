<?php
// Encoding: utf8
/**
 * Creates and initialize an instance of class Config.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */
if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

$config = Config::create_instance();
?>