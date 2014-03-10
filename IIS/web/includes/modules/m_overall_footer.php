<?php
// Encoding: utf8
/**
 * Overall footer module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

// General module data
$smarty->assign(array(
	'L_PROGRAMMED_BY' => $lang['Programmed_By'],
	'L_FOR' => $lang['For'],
	'L_IIS' => $lang['IIS'],
	));

$smarty->display('overall_footer.tpl');
?>