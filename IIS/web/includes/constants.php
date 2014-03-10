<?php
// Encoding: utf8
/**
 * Various constants.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

// PHP specific
if (!defined('PHP_INT_MIN')) {
	define('PHP_INT_MIN', -PHP_INT_MAX-1);
}

// Groups
define('VISITOR', 1);
define('COMPANY_MEMBER', 2);
define('EXHIBITIONGROUND_MEMBER', 3);

// Import/export formats
define('IMPORT_EXPORT_FORMAT_XML', 1);

// Table names
define('CONFIG_TABLE', $db_table_prefix . 'config');
define('LANGUAGES_TABLE', $db_table_prefix . 'languages');
define('GROUPS_TABLE', $db_table_prefix . 'groups');
define('USERS_TABLE', $db_table_prefix . 'users');
define('COMPANIES_TABLE', $db_table_prefix . 'companies');
define('EXHIBITIONS_TABLE', $db_table_prefix . 'exhibitions');
define('BRANCHES_TABLE', $db_table_prefix . 'branches');
define('PRICE_CATEGORIES_TABLE', $db_table_prefix . 'price_categories');
define('PAVILIONS_TABLE', $db_table_prefix . 'pavilions');
define('PLACES_TABLE', $db_table_prefix . 'places');
define('CONTRACTS_TABLE', $db_table_prefix . 'contracts');
define('EXPOSITIONS_TABLE', $db_table_prefix . 'expositions');
define('IMPORT_EXPORT_FORMATS_TABLE', $db_table_prefix . 'import_export_formats');
?>