<?php
// Encoding: utf8
/**
 * Functions for inserting data into the database.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Inserts a new user according to the selected user data.
 *
 * @param array $user_data Data of the new user.
 * @return int ID of the inserted user.
 */
function insert_user($user_data)
{
	$sql = 'INSERT INTO ' . USERS_TABLE . "
		(username,
		 name,
		 password,
		 email,
		 telephone,
		 group_id,
		 company_id)
		VALUES ('" . $user_data['username'] . "',
				'" . $user_data['name'] . "',
				'" . hash_password($user_data['password']) . "',
				'" . $user_data['email'] . "',
				 " . (isset($user_data['telephone']) ? "'{$user_data['telephone']}'" : 'NULL') . ",
				'" . $user_data['group_id'] . "',
				 " . (isset($user_data['company_id']) ? "'{$user_data['company_id']}'" : 'NULL') . "
		)";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot insert a new user.',
			$sql, mysql_error());
	}

	return mysql_insert_id();
}

/**
 * Inserts a new company according to the selected company data.
 *
 * @param array $company_data Data of the new company.
 * @return int ID of the inserted company.
 */
function insert_company($company_data)
{
	$sql = 'INSERT INTO ' . COMPANIES_TABLE . "
		(name,
		 address,
		 email,
		 www,
		 telephone,
		 IC,
		 DIC)
		VALUES ('" . $company_data['name'] . "',
				'" . $company_data['address'] . "',
				'" . $company_data['email'] . "',
				 " . (isset($company_data['www']) ? "'{$company_data['www']}'" : 'NULL') . ",
				 " . (isset($company_data['telephone']) ? "'{$company_data['telephone']}'" : 'NULL') . ",
				'" . $company_data['IC'] . "',
				'" . $company_data['DIC'] . "'
		)";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot insert a new company.',
			$sql, mysql_error());
	}

	return mysql_insert_id();
}

/**
 * Inserts a new exhibition according to the selected exhibition data.
 *
 * @param array $exhibition_data Data of the new exhibition.
 * @return int ID of the inserted exhibition.
 */
function insert_exhibition($exhibition_data)
{
	$sql = 'INSERT INTO ' . EXHIBITIONS_TABLE . "
		(name,
		 start,
		 end)
		VALUES ('" . $exhibition_data['name'] . "',
				'" . $exhibition_data['start'] . "',
				'" . $exhibition_data['end'] . "'
		)";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot insert a new exhibition.',
			$sql, mysql_error());
	}

	return mysql_insert_id();
}

/**
 * Inserts a new branch according to the selected branch data.
 *
 * @param array $branch_data Data of the new branch.
 * @return int ID of the inserted branch.
 */
function insert_branch($branch_data)
{
	$sql = 'INSERT INTO ' . BRANCHES_TABLE . "
		(name)
		VALUES ('" . $branch_data['name'] . "'
		)";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot insert a new branch.',
			$sql, mysql_error());
	}

	return mysql_insert_id();
}

/**
 * Inserts a new price category according to the selected price category data.
 *
 * @param array $price_category_data Data of the new price category.
 * @return int ID of the inserted price category.
 */
function insert_price_category($price_category_data)
{
	$sql = 'INSERT INTO ' . PRICE_CATEGORIES_TABLE . "
		(name,
		 price_for_m2)
		VALUES ('" . $price_category_data['name'] . "',
				'" . $price_category_data['price_for_m2'] . "'
		)";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot insert a new price category.',
			$sql, mysql_error());
	}

	return mysql_insert_id();
}

/**
 * Inserts a new pavilion according to the selected pavilion data.
 *
 * @param array $pavilion_data Data of the new pavilion.
 * @return int ID of the inserted pavilion.
 */
function insert_pavilion($pavilion_data)
{
	$sql = 'INSERT INTO ' . PAVILIONS_TABLE . "
		(name,
		 number_of_floors,
		 number_of_restaurants,
		 number_of_buffets,
		 number_of_social_settlements)
		VALUES ('" . $pavilion_data['name'] . "',
				'" . $pavilion_data['number_of_floors'] . "',
				'" . $pavilion_data['number_of_restaurants'] . "',
				'" . $pavilion_data['number_of_buffets'] . "',
				'" . $pavilion_data['number_of_social_settlements'] . "'
		)";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot insert a new pavilion.',
			$sql, mysql_error());
	}

	return mysql_insert_id();
}

/**
 * Inserts a new place according to the selected place data.
 *
 * @param array $place_data Data of the new place.
 * @return int ID of the inserted place.
 */
function insert_place($place_data)
{
	$sql = 'INSERT INTO ' . PLACES_TABLE . "
		(number,
		 area,
		 pavilion_id,
		 price_category_id)
		VALUES ('" . $place_data['number'] . "',
				'" . $place_data['area'] . "',
				'" . $place_data['pavilion_id'] . "',
				'" . $place_data['price_category_id'] . "'
		)";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot insert a new place.',
			$sql, mysql_error());
	}

	return mysql_insert_id();
}

/**
 * Inserts a new contract according to the selected contract data.
 *
 * @param array $contract_data Data of the new contract.
 * @return int ID of the inserted contract.
 */
function insert_contract($contract_data)
{
	$sql = 'INSERT INTO ' . CONTRACTS_TABLE . "
		(company_id,
		 concluded_by,
		 conclusion_date,
		 validity_start,
		 validity_end,
		 maturity_date)
		VALUES ('" . $contract_data['company_id'] . "',
				'" . $contract_data['concluded_by'] . "',
				'" . $contract_data['conclusion_date'] . "',
				'" . $contract_data['validity_start'] . "',
				'" . $contract_data['validity_end'] . "',
				'" . $contract_data['maturity_date'] . "'
		)";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot insert a new contract.',
			$sql, mysql_error());
	}

	return mysql_insert_id();
}

/**
 * Inserts a new exposition according to the selected exposition data.
 *
 * @param array $exposition_data Data of the new exposition.
 * @return int ID of the inserted exposition.
 */
function insert_exposition($exposition_data)
{
	$sql = 'INSERT INTO ' . EXPOSITIONS_TABLE . "
		(exhibition_id,
		 place_id,
		 contract_id,
		 branch_id,
		 description)
		VALUES ('" . $exposition_data['exhibition_id'] . "',
				'" . $exposition_data['place_id'] . "',
				'" . $exposition_data['contract_id'] . "',
				'" . $exposition_data['branch_id'] . "',
				'" . $exposition_data['description'] . "'
		)";
	if (!mysql_query($sql)) {
		throw new IIS_DB_Exception('Cannot insert a new exposition.',
			$sql, mysql_error());
	}

	return mysql_insert_id();
}
?>