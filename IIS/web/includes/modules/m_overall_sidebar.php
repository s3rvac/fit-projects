<?php
// Encoding: utf8
/**
 * Overall sidebar module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

$sidebar = generate_sidebar();
$smarty->assign('SIDEBAR', $sidebar);

$smarty->display('overall_sidebar.tpl');
?>