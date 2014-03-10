<?php
// Encoding: utf8
/**
 * "... can ..." functions.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Checks whether the selected user can have his group changed.
 *
 * @param int $user_id User ID.
 * @return bool True if the selected user can have his group changed, false otherwise.
 */
function user_can_have_group_changed($user_id)
{
	return !empty($user_id) ? !value_exists($user_id, 'concluded_by', CONTRACTS_TABLE) : true;
}

/**
 * Checks whether some exhibition can take place the selected period.
 * Exhibition must start before it ends.
 *
 * @param int $exhibition_id Exhibition ID.
 * @param int $exhibition_start Starting date of the exhibition.
 * @param int $exhibition_end Ending date of the exhibition.
 * @return bool True if some exhibition can take place the selected period, false otherwise.
 */
function exhibition_can_take_place($exhibition_id, $exhibition_start, $exhibition_end)
{
	$sql = "SELECT exhibition_id, start, end
		FROM " . EXHIBITIONS_TABLE;
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot load exhibition data.', $sql, mysql_error());
	}

	$can_take_place = true;
	while ($exhibition_data = mysql_fetch_assoc($result))	{
		if ($exhibition_start < $exhibition_data['end'] && $exhibition_end > $exhibition_data['start']) {
			if ($exhibition_id != $exhibition_data['exhibition_id']) {
				$can_take_place = false;
				break;
			}
		}
	}
	mysql_free_result($result);

	return $can_take_place;
}

/**
 * Checks whether the selected exposition can be deleted.
 *
 * @param int $exposition_id Exposition ID.
 * @return bool True if the selected exposition can be deleted, false otherwise.
 */
function exposition_can_be_deleted($exposition_id)
{
	// Nothing depend on expositions
	return true;
}

/**
 * Checks whether the selected user can be deleted.
 *
 * @param int $user_id User ID.
 * @return bool True if the selected user can be deleted, false otherwise.
 */
function user_can_be_deleted($user_id)
{
	// Check whether the selected user haven't concluded any contract
	return !value_exists($user_id, 'concluded_by', CONTRACTS_TABLE);
}

/**
 * Checks whether the selected company can be deleted.
 *
 * @param int $company_id Company ID.
 * @return bool True if the selected company can be deleted, false otherwise.
 */
function company_can_be_deleted($company_id)
{
	// Check whether the selected company haven't concluded any contract
	return !value_exists($company_id, 'company_id', CONTRACTS_TABLE);
}

/**
 * Checks whether the selected contract can be deleted.
 *
 * @param int $contract_id Contract ID.
 * @return bool True if the selected contract can be deleted, false otherwise.
 */
function contract_can_be_deleted($contract_id)
{
	// Check whether there are no expositions registered on the selected contract
	return !value_exists($contract_id, 'contract_id', EXPOSITIONS_TABLE);
}

/**
 * Checks whether the selected exhibition can be deleted.
 *
 * @param int $exhibition_id Exhibition ID.
 * @return bool True if the selected exhibition can be deleted, false otherwise.
 */
function exhibition_can_be_deleted($exhibition_id)
{
	// Check whether there are no expositions registered on the selected exhibition
	return !value_exists($exhibition_id, 'exhibition_id', EXPOSITIONS_TABLE);
}

/**
 * Checks whether the selected branch can be deleted.
 *
 * @param int $branch_id Branch ID.
 * @return bool True if the selected branch can be deleted, false otherwise.
 */
function branch_can_be_deleted($branch_id)
{
	// Check whether there are no expositions that belong to the selected branch
	return !value_exists($branch_id, 'branch_id', EXPOSITIONS_TABLE);
}

/**
 * Checks whether the selected price category can be deleted.
 *
 * @param int $price_category_id Price category ID.
 * @return bool True if the selected price category can be deleted, false otherwise.
 */
function price_category_can_be_deleted($price_category_id)
{
	// Check whether there are no places that belong to the selected price category
	return !value_exists($price_category_id, 'price_category_id', PLACES_TABLE);
}

/**
 * Checks whether the selected pavilion can be deleted.
 *
 * @param int $pavilion_id Pavilion ID.
 * @return bool True if the selected pavilion can be deleted, false otherwise.
 */
function pavilion_can_be_deleted($pavilion_id)
{
	// Check whether there are no places in the selected pavilion
	return !value_exists($pavilion_id, 'pavilion_id', PLACES_TABLE);
}

/**
 * Checks whether the selected place can be deleted.
 *
 * @param int $place_id Place ID.
 * @return bool True if the selected place can be deleted, false otherwise.
 */
function place_can_be_deleted($place_id)
{
	// Check whether there are no places in the selected place
	return !value_exists($place_id, 'place_id', EXPOSITIONS_TABLE);
}

/**
 * Checks the selected company member can view selected contract details.
 *
 * @param int $user_id Company member ID.
 * @param int $contract_id Contract ID.
 * @return bool True if the selected company member can view selected
 * contract details, false otherwise.
 */
function can_company_member_view_contract_details($user_id, $contract_id)
{
	$representing_company_id = get_value_corresponding_to_other_value(
			'company_id', $user_id, 'user_id', USERS_TABLE);
	$sql = 'SELECT contract_id
		FROM ' . CONTRACTS_TABLE . "
		WHERE company_id = $representing_company_id
			AND contract_id = $contract_id
		LIMIT 1";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot get contract data.',
			$sql, mysql_error());
	}

	$can_view_contract_details = mysql_num_rows($result) == 1;
	mysql_free_result($result);

	return $can_view_contract_details;
}

/**
 * Checks whether the selected company member can edit selected
 * exposition details.
 *
 * @param int $user_id Company member ID.
 * @param int $exposition_id Exposition ID.
 * @return bool True if the selected company member can edit selected
 * exposition details, false otherwise.
 */
function can_company_member_edit_exposition($user_id, $exposition_id)
{
	$representing_company_id = get_value_corresponding_to_other_value(
			'company_id', $user_id, 'user_id', USERS_TABLE);
	$sql = 'SELECT ex.exposition_id
		FROM ' . EXPOSITIONS_TABLE . ' ex LEFT JOIN ' . CONTRACTS_TABLE . " co ON ex.contract_id = co.contract_id
		WHERE co.company_id = $representing_company_id
			AND ex.exposition_id = $exposition_id
		LIMIT 1";
	$result = mysql_query($sql);
	if (!$result) {
		throw new IIS_DB_Exception('Cannot get contract data.',
			$sql, mysql_error());
	}

	$can_edit_exposition = mysql_num_rows($result) == 1;
	mysql_free_result($result);

	return $can_edit_exposition;
}
?>