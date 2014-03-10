<?php
// Encoding: utf8
/**
 * General configuration.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

/**
 * Debugging enabled/disabled.
 */
define('DEBUG', false);

// Various paths
$templates_path = $web_root_path . 'templates/';
$languages_path = $web_root_path . 'languages/';
$modules_path = $web_root_path . 'includes/modules/';
$exports_path = $web_root_path . 'exports/';
$logs_path = '../' . $web_root_path . 'logs/';

// Smarty paths
$smarty_template_dir = $templates_path . 'templates/';
$smarty_compile_dir = $templates_path . 'templates_c/';
$smarty_config_dir = $templates_path . 'configs/';
$smarty_cache_dir = $templates_path . 'cache/';

// Database configuration
$db_host = 'localhost';
$db_name = '';
$db_user = '';
$db_pass = '';
$db_table_prefix = 'iis_';
?>