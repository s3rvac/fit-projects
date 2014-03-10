<?php
// Encoding: utf8
/**
 * Functions for database data actualization.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Actualizes contracts table. This function must be called:
 *  - after a new exposition is registered to some contract
 *  - after some exposition is deleted
 *
 * @param int $contract_id Contract ID.
 */
function actualize_contract($contract_id)
{
	// Get all registered expositions
	$sql = 'SELECT exposition_id
		FROM ' . EXPOSITIONS_TABLE . "
		WHERE contract_id = $contract_id";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load expositions.',
			$sql, mysql_error());
	}

	// Compute total price for all registered expositions
	$total_price = 0;
	while ($exposition_data = mysql_fetch_assoc($result)) {
		$total_price += get_price_for_exposition($exposition_data['exposition_id']);
	}
	mysql_free_result($result);

	// Update contracts table with the computed total price for registered expositions
	$sql = 'UPDATE ' . CONTRACTS_TABLE . "
		SET price = $total_price
		WHERE contract_id = $contract_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update the selected contract.',
			$sql, mysql_error());
	}
}

/**
 * Actualizes all contracts in the contracts table. This function must be called:
 *  - after some price category is edited
 */
function actualize_all_contracts()
{
	foreach (load_available_contracts() as $contract_data) {
		actualize_contract($contract_data['contract_id']);
	}
}

/**
 * Actualizes users table. This function must be called:
 *  - after some company is deleted
 *
 * @param int $company_id Company ID.
 */
function actualize_company_representatives($company_id)
{
	$sql = 'UPDATE ' . USERS_TABLE . "
		SET company_id = NULL
		WHERE company_id = $company_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update company representatives.',
			$sql, mysql_error());
	}
}
?>