<?php
// Encoding: utf8
/**
 * Debtors module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
	not_allowed();
	return;
}

/**
 * Sends a warning message (via email address) to the selected debtors.
 *
 * @param array $debtors Array of debtor IDs (company_id).
 * @return bool True if there is no problem, false otherwise.
 */
function warn_debtors_via_email($debtors)
{
	// TODO
	// Since the company emails in the database are sham,
	// no emails are currently sent...

	return true;
}

/**
 * @return int Number of all debtors.
 */
function get_number_of_all_debtors()
{
	$sql = 'SELECT com.name AS company, SUM(con.price) AS owed_price, SUM(con.settled) AS settled
		FROM ' . COMPANIES_TABLE . ' com LEFT JOIN ' . CONTRACTS_TABLE . ' con ON com.company_id = con.company_id
		GROUP BY company
		HAVING settled < owed_price';
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load number of debtors.",
			$sql, mysql_error());
	}

	$number_of_all_debtors = mysql_num_rows($result);
	mysql_free_result($result);

	return $number_of_all_debtors;
}

// Check whether there in a warn all debtors request
if (isset($_POST['warn_all_debtors'])) {
	$success = warn_debtors_via_email(get_all_debtor_ids());
	if ($success) {
		$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Warn_All_Debtors']);
	} else {
		$smarty->assign('ERROR_MESSAGE', $lang['Error_Warn_Debtors']);
	}
}

// Check whether there in a warn selected debtors request
if (isset($_POST['warn_debtors']) && isset($_POST['debtors'])) {
	$debtors = array();
	$debtor_ids = get_all_debtor_ids();
	foreach ($_POST['debtors'] as $debtor_id) {
		if (in_array($debtor_id, $debtor_ids)) {
			array_push($debtors, $debtor_id);
		}
	}

	$success = warn_debtors_via_email($debtors);
	if ($success) {
		$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Warn_Debtors']);
	} else {
		$smarty->assign('ERROR_MESSAGE', $lang['Error_Warn_Debtors']);
	}
}

// Page and records information
$current_page_number = handle_input_int($_GET['page']);
$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
$records_per_page = $config->get_value('records_per_page');

// Generate debtors data
$debtors_data = array();
do {
	$debtors_data = generate_debtors_data($records_per_page, $current_page_number,
		$_GET['order_by'], isset($_GET['descending']));
} while (empty($debtors_data) && $current_page_number-- > 1);
$current_page_number = $current_page_number < 1 ? 1 : $current_page_number;
if (!empty($debtors_data)) {
	$module_url = add_query_to_url(current_base_url(), 'module', 'debtors');
	$pagination = create_pagination($module_url, get_number_of_all_debtors(),
		$records_per_page);

	$module_url = add_query_to_url(current_base_url(), 'module', 'debtors');
	$order_by_company_url = add_query_to_url($module_url, 'order_by', 'company');
	$order_by_total_price_url = add_query_to_url($module_url, 'order_by', 'total_price');
	$order_by_settled_url = add_query_to_url($module_url, 'order_by', 'settled');
	$order_by_owed_price_url = add_query_to_url($module_url, 'order_by', 'owed_price');

	$smarty->assign(array(
		'DEBTORS_DATA' => $debtors_data,
		'PAGINATION' => $pagination,
		'CURRENT_PAGE_NUMBER' => $current_page_number,

		'U_ORDER_BY_COMPANY_DESCENDING' => add_query_to_url($order_by_company_url, 'descending', ''),
		'U_ORDER_BY_COMPANY_ASCENDING' => $order_by_company_url,

		'U_ORDER_BY_OWED_PRICE_DESCENDING' => add_query_to_url($order_by_owed_price_url, 'descending', ''),
		'U_ORDER_BY_OWED_PRICE_ASCENDING' => $order_by_owed_price_url,

		'U_ORDER_BY_SETTLED_DESCENDING' => add_query_to_url($order_by_settled_url, 'descending', ''),
		'U_ORDER_BY_SETTLED_ASCENDING' => $order_by_settled_url,

		'U_ORDER_BY_TOTAL_PRICE_DESCENDING' => add_query_to_url($order_by_total_price_url, 'descending', ''),
		'U_ORDER_BY_TOTAL_PRICE_ASCENDING' => $order_by_total_price_url,

		'L_GOTO_PAGE' => $lang['Goto_Page'],
		'L_ORDER_DESCENDING' => $lang['Order_Descending'],
		'L_ORDER_ASCENDING' => $lang['Order_Ascending'],
		));
} else {
	$smarty->assign(array(
		'L_NO_DEBTORS_AVAILABLE' => $lang['No_Debtors_Available'],
	));
}

// General page data
$smarty->assign(array(
	'PAGE_HEADER' => $lang['Debtors_Header'],
	'PAGE_DESCRIPTION' => $lang['Debtors_Description'],

	'U_WARN_DEBTORS' => '',

	'L_COMPANY' => $lang['Company'],
	'L_TOTAL_PRICE' => $lang['Total_Price'],
	'L_SETTLED' => $lang['Settled'],
	'L_OWED_PRICE' => $lang['Owed_Price'],
	'L_WARN_DEBTOR' => $lang['Warn_Debtor'],
	'L_WARN_ALL_DEBTORS' => $lang['Warn_All_Debtors'],
	'L_WARN_SELECTED_DEBTORS' => $lang['Warn_Selected_Debtors'],
	));

$smarty->display('debtors.tpl');
?>