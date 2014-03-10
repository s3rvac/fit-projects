<?php
// Encoding: utf8
/**
 * Web entry point - this is the only file that is viewable via browser.
 * Which page will be shown depends on URL arguments (and session data).
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

$web_root_path = './web/';
require_once $web_root_path . 'index.php';
?>