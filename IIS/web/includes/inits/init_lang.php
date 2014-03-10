<?php
// Encoding: utf8
/**
 * Initializes language data.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

// Load language data according to the user's language
$lang = array();
$full_languages_path = $languages_path . $user->get_current_language() . '/';
foreach (scandir($full_languages_path) as $entry) {
	if ($entry != '.' && $entry != '..') {
		if (!is_dir($entry) && strpos($entry, '.php')) {
			require_once $full_languages_path . $entry;
		}
	}
}
?>