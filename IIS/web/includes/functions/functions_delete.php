<?php
// Encoding: utf8
/**
 * Functions for deleting data from database.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Deletes selected user from the database.
 *
 * @param int $user_id User ID.
 * @return bool True if the action was performed correctly, false otherwise.
 */
function delete_user($user_id)
{
	global $lang;

	$sql = 'DELETE
		FROM ' . USERS_TABLE . "
		WHERE user_id = $user_id
		LIMIT 1";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot delete the selected user.',
			$sql, mysql_error());
	}

	// Was the user deleted?
	return mysql_affected_rows() == 1;
}

/**
 * Deletes selected company from the database.
 *
 * @param int $company_id Company ID.
 * @return bool True if the action was performed correctly, false otherwise.
 */
function delete_company($company_id)
{
	global $lang;

	$sql = 'DELETE
		FROM ' . COMPANIES_TABLE . "
		WHERE company_id = $company_id
		LIMIT 1";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot delete the selected company.',
			$sql, mysql_error());
	}

	// Was the company deleted?
	return mysql_affected_rows() == 1;
}

/**
 * Deletes selected exhibition from the database.
 *
 * @param int $exhibition_id Exhibition ID.
 * @return bool True if the action was performed correctly, false otherwise.
 */
function delete_exhibition($exhibition_id)
{
	global $lang;

	$sql = 'DELETE
		FROM ' . EXHIBITIONS_TABLE . "
		WHERE exhibition_id = $exhibition_id
		LIMIT 1";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot delete the selected exhibition.',
			$sql, mysql_error());
	}

	// Was the exhibition deleted?
	return mysql_affected_rows() == 1;
}

/**
 * Deletes selected branch from the database.
 *
 * @param int $branch_id Branch ID.
 * @return bool True if the action was performed correctly, false otherwise.
 */
function delete_branch($branch_id)
{
	global $lang;

	$sql = 'DELETE
		FROM ' . BRANCHES_TABLE . "
		WHERE branch_id = $branch_id
		LIMIT 1";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot delete the selected branch.',
			$sql, mysql_error());
	}

	// Was the branch deleted?
	return mysql_affected_rows() == 1;
}

/**
 * Deletes selected price category from the database.
 *
 * @param int $price_category_id Price category ID.
 * @return bool True if the action was performed correctly, false otherwise.
 */
function delete_price_category($price_category_id)
{
	global $lang;

	$sql = 'DELETE
		FROM ' . PRICE_CATEGORIES_TABLE . "
		WHERE price_category_id = $price_category_id
		LIMIT 1";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot delete the selected price category.',
			$sql, mysql_error());
	}

	// Was the price_category deleted?
	return mysql_affected_rows() == 1;
}

/**
 * Deletes selected pavilion from the database.
 *
 * @param int $pavilion_id Pavilion ID.
 * @return bool True if the action was performed correctly, false otherwise.
 */
function delete_pavilion($pavilion_id)
{
	global $lang;

	$sql = 'DELETE
		FROM ' . PAVILIONS_TABLE . "
		WHERE pavilion_id = $pavilion_id
		LIMIT 1";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot delete the selected pavilion.',
			$sql, mysql_error());
	}

	// Was the pavilion deleted?
	return mysql_affected_rows() == 1;
}

/**
 * Deletes selected place from the database.
 *
 * @param int $place_id Place ID.
 * @return bool True if the action was performed correctly, false otherwise.
 */
function delete_place($place_id)
{
	global $lang;

	$sql = 'DELETE
		FROM ' . PLACES_TABLE . "
		WHERE place_id = $place_id
		LIMIT 1";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot delete the selected place.',
			$sql, mysql_error());
	}

	// Was the place deleted?
	return mysql_affected_rows() == 1;
}

/**
 * Deletes selected contract from the database.
 *
 * @param int $contract_id Contract ID.
 * @return bool True if the action was performed correctly, false otherwise.
 */
function delete_contract($contract_id)
{
	global $lang;

	$sql = 'DELETE
		FROM ' . CONTRACTS_TABLE . "
		WHERE contract_id = $contract_id
		LIMIT 1";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot delete the selected contract.',
			$sql, mysql_error());
	}

	// Was the contract deleted?
	return mysql_affected_rows() == 1;
}

/**
 * Deletes selected exposition from the database.
 *
 * @param int $exposition_id Exposition ID.
 * @return bool True if the action was performed correctly, false otherwise.
 */
function delete_exposition($exposition_id)
{
	global $lang;

	$sql = 'DELETE
		FROM ' . EXPOSITIONS_TABLE . "
		WHERE exposition_id = $exposition_id
		LIMIT 1";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot delete the selected exposition.',
			$sql, mysql_error());
	}

	// Was the exposition deleted?
	return mysql_affected_rows() == 1;
}
?>