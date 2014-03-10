<?php
// Encoding: utf8
/**
 * Forthcoming exhibitions module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Generates exposition data according to the selected exposition ID.
 *
 * @param int $exposition_id Exposition ID.
 * @return array Generated exposition data.
 */
function generate_exposition_on_exhibition_data($exposition_id)
{
	$sql = 'SELECT ex.exposition_id, ex.contract_id, ex.description,
				   con.company_id, com.name AS company, br.name AS branch
		FROM ' . EXPOSITIONS_TABLE . ' ex LEFT JOIN ' . BRANCHES_TABLE . ' br ON ex.branch_id = br.branch_id
										  LEFT JOIN ' . CONTRACTS_TABLE . ' con ON ex.contract_id = con.contract_id
										  LEFT JOIN ' . COMPANIES_TABLE . " com ON con.company_id = com.company_id
		WHERE ex.exposition_id = $exposition_id";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception("Cannot load expositions.",
			$sql, mysql_error());
	}

	$exposition_data = array();
	while ($row = mysql_fetch_assoc($result)) {
		$details_exposition_url = create_module_action_url('expositions', 'details');
		$details_exposition_url = add_query_to_url($details_exposition_url, 'exposition_id', $row['exposition_id']);
		$details_company_url = create_module_action_url('companies', 'details');
		$details_company_url = add_query_to_url($details_company_url, 'company_id', $row['company_id']);

		$exposition_data = array(
			'details_exposition_url' => $details_exposition_url,
			'company' => $row['company'],
			'details_company_url' => $details_company_url,
			'branch' => $row['branch'],
			'description' => $row['description'],
			);
	}
	mysql_free_result($result);

	return $exposition_data;
}

// Generate forthcoming exhibition data
$forthcoming_exhibition_id = get_forthcoming_exhibition_id();
$forthcoming_exhibition = array();
if ($forthcoming_exhibition_id > 0) {
	$forthcoming_exhibition = generate_exhibition_data($forthcoming_exhibition_id);
}

// Generate expositions data on this particular forthcoming exhibition
$expositions = array();
if ($forthcoming_exhibition_id > 0) {
	foreach (load_expositions_on_exhibition($forthcoming_exhibition_id) as $exposition_id) {
		array_push($expositions, generate_exposition_on_exhibition_data($exposition_id));
	}
}

// General page data
$smarty->assign(array(
	'PAGE_HEADER' => $lang['Forthcoming_Exhibition_Header'],
	'PAGE_DESCRIPTION' => $lang['Forthcoming_Exhibition_Description'],
	'FORTHCOMING_EXHIBITION' => $forthcoming_exhibition,
	'EXPOSITIONS' => $expositions,

	'L_FORTHCOMING_EXHIBITION' => $lang['Forthcoming_Exhibition'],
	'L_EXPOSITIONS_ON_FORTHCOMING_EXHIBITION' => $lang['Expositions_On_Forthcoming_Exhibition'],
	'L_NO_FORTHCOMING_EXHIBITION' => $lang['No_Forthcoming_Exhibition'],
	'L_NO_EXPOSITIONS' => $lang['No_Expositions'],
	'L_EXHIBITION_NAME' => $lang['Exhibition_Name'],
	'L_EXHIBITION_START' => $lang['Exhibition_Start'],
	'L_EXHIBITION_END' => $lang['Exhibition_End'],
	'L_DETAILS' => $lang['Details'],
	'L_COMPANY' => $lang['Company'],
	'L_BRANCH' => $lang['Branch'],
	'L_DESCRIPTION' => $lang['Description'],
	));

$smarty->display('forthcoming_exhibition.tpl');
?>