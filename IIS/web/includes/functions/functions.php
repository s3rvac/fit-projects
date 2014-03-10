<?php
// Encoding: utf8
/**
 * Functions for general usage.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Starts output buffering.
 */
function start_output_buffering()
{
	ob_start();
}

/**
 * Displays output buffer.
 */
function display_output_buffer()
{
	ob_end_flush();
}

/**
 * Cleans output buffer.
 */
function clean_output_buffer()
{
	// Clean buffer and end buffering
	while (ob_get_level() > 0) {
		ob_end_clean();
	}
}

/**
 * @return string User's browser type:
 *                'KHTML' - Konqueror,
 *                'Gecko' - Netscape, Mozilla, Firefox, Seamonkey,
 *                'Opera' - Opera,
 *                'IE' - MSIE,
 *                 or '???' (unknown browser).
 */
function get_users_browser_type()
{
	$user_agent = $_SERVER['HTTP_USER_AGENT'];
	if (strpos($user_agent, 'KHTML')) {
		return 'KHTML';
	} else if (strpos($user_agent, 'Gecko')) {
		return 'Gecko';
	} elseif (strpos($user_agent, 'MSIE')) {
		if (strpos($user_agent, 'Opera')) {
			return 'Opera';
		} else {
			return 'IE';
		}
	} elseif(strpos($user_agent, 'Opera')){
		return 'Opera';
	} else {
		return '???';
	}
}

/**
 * Sets proper content and database encoding.
 */
function set_proper_encoding()
{
	// Database encoding (some of these queries might not work on
	// all MySQL versions, but it doesn't matter)
	mysql_query("SET NAMES 'utf8'");
	mysql_query("SET CHARACTER SET 'utf8'");
	mysql_query("SET CHARACTER_SET_CLIENT = 'utf8'");
	mysql_query("SET CHARACTER_SET_RESULTS = 'utf8'");
	mysql_query("SET CHARACTER_SET_CONNECTION = 'utf8'");
	mysql_query("SET COLLATION_CONNECTION = 'utf8_general_ci'");
}

/**
 * Performs server specific actions.
 */
function perform_server_specific_actions()
{
	// Well, I know this is bad and it wouldn't be here if our
	// students server was properly configured, but without this
	// the source encoding is (hardly) wrong...
	if (in_array($_SERVER['SERVER_NAME'], array('eva.fit.vutbr.cz', 'www.eva.fit.vutbr.cz',
		'stud.fit.vutbr.cz', 'www.stud.fit.vutbr.cz'))) {
		// Force server to show content in utf8 (mod_html)
		if (strpos(current_full_url(), 'index.php.utf-8') === false) {
			if (strpos(current_full_url(), 'index.php') !== false) {
				header('Location: ' . str_replace('index.php',
					'index.php.utf-8', current_full_url()));
			} else {
				header('Location: index.php.utf-8');
			}
			exit();
		}

		// Source encoding
		//  - IE cannot handle "Content-Type: application/xhtml+xml"
		//  - Gecko based browsers display the web incorrectly
		//    with "Content-Type: application/xhtml+xml" on this server
		if (in_array(get_users_browser_type(), array('Gecko', 'IE'))) {
			header("Content-Type: text/html; charset=utf-8");
		} else {
			header("Content-Type: application/xhtml+xml; charset=utf-8");
		}
	}
}

/**
 * @return string Hashed $password.
 * @param string $password to be hashed
 */
function hash_password($password)
{
	return md5($password);
}

/**
 * Adds selected query ($name=$value) to the selected $url and returns it.
 * If the $name already exists, it's $value will be modified.
 *
 * @param string $url Original URL.
 * @param string $name Name part of the query.
 * @param string $value Value part of the query.
 */
function add_query_to_url($url, $name, $value = '')
{
	$parsed_url = parse_url($url);

	// scheme://username:password@host/path
	$modified_url = !empty($parsed_url['scheme']) ? $parsed_url['scheme'] .  '://' : '';
	$modified_url .= !empty($parsed_url['username']) ? $parsed_url['username'] : '';
	$modified_url .= !empty($parsed_url['password']) ? ':' . $parsed_url['password'] : '';
	$modified_url .= !empty($parsed_url['username']) ? '@' : '';
	$modified_url .= $parsed_url['host'] . $parsed_url['path'];

	// queries
	$query_part = str_replace('&amp;', '&', urldecode($parsed_url['query']));
	$queries = array();
	foreach (explode('&', $query_part) as $query) {
		if (!empty($query)) {
			list($query_name, $query_value) = explode('=', $query);
			$queries[$query_name] = $query_value;
		}
	}
	// This will cause original value modification if the $name already exists
	$queries[$name] = $value;

	foreach ($queries as $query_name => $query_value) {
		$modified_url .= ((strpos($modified_url, '?') !== false) ? '&' : '?') .
			urlencode($query_name) . (trim($query_value) != '' ? '=' . urlencode($query_value) : '');
	}

	// #fragment
	$modified_url .= !empty($parsed_url['fragment']) ? '#' . $parsed_url['fragment'] : '';

	return $modified_url;
}

/**
 * @return string Current base URL without any queries (URL arguments) - except
 * session ID (if necessary).
 */
function current_base_url()
{
	return sprintf('http%s://%s%s',
		(isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == true ? 's': ''),
		$_SERVER['HTTP_HOST'],
		$_SERVER['PHP_SELF']);
}

/**
 * @return string Current URL with all queries (URL arguments) + session ID (if necessary).
 */
function current_full_url()
{
	return sprintf('http%s://%s%s',
		(isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == true ? 's': ''),
		$_SERVER['HTTP_HOST'],
		$_SERVER['REQUEST_URI']);
}

/**
 * Creates URL with module and action parameters.
 *
 * @param string $module Name of the module.
 * @param string $action Name of the action.
 * @return string URL with module and action parameters.
 */
function create_module_action_url($module, $action)
{
	$url = add_query_to_url(current_base_url(), 'module', $module);
	$url = add_query_to_url($url, 'action', $action);
	return $url;
}

/**
 * Creates and returns an array of the form 'page number' => 'page URL'
 * according to the parameters.
 *
 * @param string $url Base URL.
 * @param int $number_of_records Total number of records.
 * @param int $records_per_page Number of records on a single page
 * @return array Array of the form 'page number' => 'page URL'.
 */
function create_pagination($url, $number_of_records, $records_per_page)
{
	$number_of_processed_records = 0;
	$current_page_number = 1;
	$pagination = array();
	while ($number_of_processed_records < $number_of_records) {
		$page_url = add_query_to_url($url, 'page', $current_page_number);
		$pagination[$current_page_number] = $page_url;
		$current_page_number++;
		$number_of_processed_records += $records_per_page;
	}

	return $pagination;
}

/**
 * Checks whether the selected ID corresponds to the selected value.
 *
 * @param int $id ID.
 * @param string $id_column Column in which the ID should be.
 * @param string $value Value that should correspond to the selceted ID.
 * @param string $value_column Column in which the value should exists.
 * @param string $table Name of the table.
 * @return bool true, if the seleced ID ID corresponds to the selected value,
 * false otherwise.
 */
function id_corresponds_to_value($id, $id_column, $value, $value_column, $table)
{
	$sql = "SELECT *
		FROM " . $table . "
		WHERE $id_column = $id
			AND $value_column = '$value'
		LIMIT 1";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot check ID correspondence to the selected value.',
			$sql, mysql_error());
	}

	$id_corresponds_to_value = mysql_num_rows($result) == 1;
	mysql_free_result($result);

	return $id_corresponds_to_value;
}

/**
 * Checks whether the selected value exists in the selected table in the
 * selected column.
 *
 * @param string $value Value that should exists in the selected table.
 * @param string $value_column Column in which the value should exists.
 * @param string $table Name of the table.
 * @return bool true, if the selected value exists in the selected table in the
 * selected column, false otherwise.
 */
function value_exists($value, $value_column, $table)
{
	$sql = "SELECT $value_column
		FROM " . $table . "
		WHERE $value_column = '$value'
		LIMIT 1";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot check whether the selected value exists.',
			$sql, mysql_error());
	}

	// Perform case-sensitive comparison
	if (mysql_num_rows($result) == 1) {
		$row = mysql_fetch_assoc($result);
		mysql_free_result($result);
		return $row[$value_column] == $value;
	} else {
		return false;
	}
}

/**
 * Formats selected date.
 *
 * @param int $date_ts Date in the UNIX timestamp format.
 * @return string Formatted date.
 */
function format_date($date_ts)
{
	if ($date_ts !== 0 && empty($date_ts)) {
		return '';
	} else {
		return date('Y-m-d-H-i', $date_ts);
	}
}

/**
 * This function should be called when a user is not allowed to perform
 * some action or to view some page.
 */
function not_allowed()
{
	global $smarty, $lang;

	$smarty->assign(array(
		'ERROR_MESSAGE' => $lang['Error_Not_Allowed_To_Do_Action'],
		));
	$smarty->display('not_allowed.tpl');
}

/**
 * @return bool True if the selected place is engaged for the selected exhibition,
 * false oteherwise.
 * @param int $exhibition_id Exhibition ID.
 * @param int $place_id Place ID.
 */
function is_engaged_place($exhibition_id, $place_id)
{
	$sql = 'SELECT *
		FROM ' . EXPOSITIONS_TABLE . "
		WHERE exhibition_id = $exhibition_id
			AND place_id = $place_id
		LIMIT 1";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot get exhibition data.',
			$sql, mysql_error());
	}

	$is_engaged_place = mysql_num_rows($result) == 1;
	mysql_free_result($result);

	return $is_engaged_place;
}

/**
 * Sends a file to the user with the selected file name and content.
 *
 * @param string $file Name of the file.
 * @param string $file_content File content.
 * @param string $content_typ File content type (MIME).
 */
function send_file_to_user($file_name, $file_content, $content_type)
{
	clean_output_buffer();

	header('Content-Encoding: utf-8');
	header('Content-Type: ' . $content_type);
	header('Expires: ' . gmdate('D, d M Y H:i:s') . ' GMT');
	header('Content-Disposition: attachment; filename="' . $file_name . '"');
	if (get_users_browser_type() == 'IE') {
		header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
		header('Pragma: public');
	} else {
		header('Pragma: no-cache');
	}

	echo $file_content;
	exit();
}
?>