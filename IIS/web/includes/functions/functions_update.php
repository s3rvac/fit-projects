<?php
// Encoding: utf8
/**
 * Functions for updating data in the database.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Updates data of the selected user.
 *
 * @param int $user_id User ID.
 * @param array $user_data Updated user data.
 */
function update_user($user_id, $user_data)
{
	$password_line = !empty($user_data['password']) ?
		"password = '" . hash_password($user_data['password']) . "'," : '';
	$sql = 'UPDATE ' . USERS_TABLE . "
		SET username = '" . $user_data['username'] . "',
			name = '" . $user_data['name'] . "',
			$password_line
			email = '" . $user_data['email'] . "',
			telephone = " . (isset($user_data['telephone']) ? "'{$user_data['telephone']}'" : 'NULL') . ",
			group_id = '" . $user_data['group_id'] . "',
			company_id = " . (isset($user_data['company_id']) ? "'{$user_data['company_id']}'" : 'NULL') . "
		WHERE user_id = $user_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update the selected user.',
			$sql, mysql_error());
	}
}

/**
 * Updates data of the selected company.
 *
 * @param int $company_id Company ID.
 * @param array $company_data Updated company data.
 */
function update_company($company_id, $company_data)
{
	$sql = 'UPDATE ' . COMPANIES_TABLE . "
		SET name = '" . $company_data['name'] . "',
			address = '" . $company_data['address'] . "',
			email = '" . $company_data['email'] . "',
			www = " . (isset($company_data['www']) ? "'{$company_data['www']}'" : 'NULL') . ",
			telephone = " . (isset($company_data['telephone']) ? "'{$company_data['telephone']}'" : 'NULL') . ",
			IC = '" . $company_data['IC'] . "',
			DIC = '" . $company_data['DIC'] . "'
		WHERE company_id = $company_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update the selected company.',
			$sql, mysql_error());
	}
}

/**
 * Updates data of the selected exhibition.
 *
 * @param int $exhibition_id Exhibition ID.
 * @param array $exhibition_data Updated exhibition data.
 */
function update_exhibition($exhibition_id, $exhibition_data)
{
	$sql = 'UPDATE ' . EXHIBITIONS_TABLE . "
		SET name = '" . $exhibition_data['name'] . "',
			start = '" . $exhibition_data['start'] . "',
			end = '" . $exhibition_data['end'] . "'
		WHERE exhibition_id = $exhibition_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update the selected exhibition.',
			$sql, mysql_error());
	}
}

/**
 * Updates data of the selected branch.
 *
 * @param int $branch_id Branch ID.
 * @param array $branch_data Updated branch data.
 */
function update_branch($branch_id, $branch_data)
{
	$sql = 'UPDATE ' . BRANCHES_TABLE . "
		SET name = '" . $branch_data['name'] . "'
		WHERE branch_id = $branch_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update the selected branch.',
			$sql, mysql_error());
	}
}

/**
 * Updates data of the selected price category.
 *
 * @param int $price_category_id Price category ID.
 * @param array $price_category_data Updated price category data.
 */
function update_price_category($price_category_id, $price_category_data)
{
	$sql = 'UPDATE ' . PRICE_CATEGORIES_TABLE . "
		SET name = '" . $price_category_data['name'] . "',
			price_for_m2 = '" . $price_category_data['price_for_m2'] . "'
		WHERE price_category_id = $price_category_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update the selected price category.',
			$sql, mysql_error());
	}
}

/**
 * Updates data of the selected pavilion.
 *
 * @param int $pavilion_id Pavilion ID.
 * @param array $pavilion_data Updated pavilion data.
 */
function update_pavilion($pavilion_id, $pavilion_data)
{
	$sql = 'UPDATE ' . PAVILIONS_TABLE . "
		SET name = '" . $pavilion_data['name'] . "',
			number_of_floors = '" . $pavilion_data['number_of_floors'] . "',
		 	number_of_restaurants = '" . $pavilion_data['number_of_restaurants'] . "',
		 	number_of_buffets = '" . $pavilion_data['number_of_buffets'] . "',
		 	number_of_social_settlements = '" . $pavilion_data['number_of_social_settlements'] . "'
		WHERE pavilion_id = $pavilion_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update the selected pavilion.',
			$sql, mysql_error());
	}
}

/**
 * Updates data of the selected place.
 *
 * @param int $place_id Place ID.
 * @param array $place_data Updated place data.
 */
function update_place($place_id, $place_data)
{
	$sql = 'UPDATE ' . PLACES_TABLE . "
		SET number = '" . $place_data['number'] . "',
			area = '" . $place_data['area'] . "',
			pavilion_id = '" . $place_data['pavilion_id'] . "',
			price_category_id = '" . $place_data['price_category_id'] . "'
		WHERE place_id = $place_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update the selected place.',
			$sql, mysql_error());
	}
}

/**
 * Updates data of the selected contract.
 *
 * @param int $contract_id Contract ID.
 * @param array $contract_data Updated contract data.
 */
function update_contract($contract_id, $contract_data)
{
	$sql = 'UPDATE ' . CONTRACTS_TABLE . "
		SET company_id = '" . $contract_data['company_id'] . "',
			concluded_by = '" . $contract_data['concluded_by'] . "',
			conclusion_date = '" . $contract_data['conclusion_date'] . "',
			validity_start = '" . $contract_data['validity_start'] . "',
			validity_end = '" . $contract_data['validity_end'] . "',
			maturity_date = '" . $contract_data['maturity_date'] . "',
			settled = '" . $contract_data['settled'] . "'
		WHERE contract_id = $contract_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update the selected contract.',
			$sql, mysql_error());
	}
}

/**
 * Updates data of the selected exposition.
 *
 * @param int $exposition_id Exposition ID.
 * @param array $exposition_data Updated exposition data.
 */
function update_exposition($exposition_id, $exposition_data)
{
	$sql = 'UPDATE ' . EXPOSITIONS_TABLE . "
		SET exhibition_id = '" . $exposition_data['exhibition_id'] . "',
			place_id = '" . $exposition_data['place_id'] . "',
			contract_id = '" . $exposition_data['contract_id'] . "',
			branch_id = '" . $exposition_data['branch_id'] . "',
			description = '" . $exposition_data['description'] . "'
		WHERE exposition_id = $exposition_id";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot update the selected exposition.',
			$sql, mysql_error());
	}
}
?>