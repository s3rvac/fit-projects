<?php
// Encoding: utf8
// Project:  ITW (2nd project)
// Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz

// Paths
$web_root_path = './';
$smarty_template_dir = $web_root_path . 'templates/';
$smarty_compile_dir = $web_root_path . 'templates_c/';

// Initialize Smarty
require_once $web_root_path . 'libs/Smarty/Smarty.class.php';
$smarty = new Smarty();
$smarty->template_dir = $smarty_template_dir;
$smarty->compile_dir = $smarty_compile_dir;

// Get the name of the page to be shown according to the parameter
$page = (isset($_GET['page']) ? $_GET['page'] : 'index');
$page_title = '';
switch ($page) {
 	case 'about_park':
		$page_title = 'O parku';
 		break;

 	case 'history':
		$page_title = 'Historie';
 		break;

 	case 'future':
		$page_title = 'Budoucnost';
 		break;

 	case 'park':
		$page_title = 'Kam se podívat';
 		break;

 	case 'veveri':
		$page_title = 'Trosky hradu Veveří';
 		break;

  	case 'dam':
		$page_title = 'Vyprahlá přehrada';
 		break;

 	case 'fauna_flora':
		$page_title = 'Rostlinstvo a živočichové';
 		break;

	case 'animals':
		$page_title = 'Živočichové';
 		break;

 	case 'plants':
		$page_title = 'Rostliny';
 		break;

 	case 'contact':
		$page_title = 'Kontakt';
 		break;

 	case 'web_map':
		$page_title = 'Mapa webu';
 		break;

 	case 'links':
		$page_title = 'Odkazy';
 		break;

 	case 'personalization':
		$page_title = 'Personalizace';
 		break;

	case 'factors':
		$page_title = 'Faktory';
 		break;

	// Index
	default:
		$page = 'index';
		$page_title = '';
}

$smarty->assign(array(
	'PAGE' => $page,
	'PAGE_TITLE' => $page_title,
	));

// Show selected page
$smarty->display('header.tpl');
$smarty->display($page . '.tpl');
$smarty->display('footer.tpl');
?>