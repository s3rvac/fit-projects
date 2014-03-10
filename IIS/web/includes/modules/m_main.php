<?php
// Encoding: utf8
/**
 * Main (index) page module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

// General page data
$smarty->assign(array(
	'ADMINISTRATOR_EMAIL' => cloak_email($config->get_value('administrator_email')),

	'L_MAIN_HEADER' => $lang['Main_Header'],
	'L_MAIN_DESCRIPTION' => $lang['Main_Description'],
	'L_MAIN_CONTACT' => $lang['Main_Contact'],
	));

$smarty->display('main.tpl');
?>