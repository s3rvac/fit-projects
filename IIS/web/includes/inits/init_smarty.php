<?php
// Encoding: utf8
/**
 * Creates and initialize an instance of class Smarty.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

$smarty = new Smarty();

// Set paths (data are stored in config.php)
$smarty->template_dir = $smarty_template_dir;
$smarty->compile_dir = $smarty_compile_dir;
$smarty->config_dir = $smarty_config_dir ;
$smarty->cache_dir = $smarty_cache_dir;

// Configuration settings
$smarty->config_booleanize = true;
$smarty->config_fix_newlines = true;

// Register custom modifiers
$smarty->default_modifiers = array('safe_output_string');

// Debugging
// $smarty->debugging = DEBUG ? true : false;
?>