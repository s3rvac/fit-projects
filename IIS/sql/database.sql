--
-- Encoding:  utf8
-- Author:    Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
--

-------------------------------- Schema ----------------------------------------

-- Drop all existing tables
DROP TABLE IF EXISTS iis_config;
DROP TABLE IF EXISTS iis_languages;
DROP TABLE IF EXISTS iis_groups;
DROP TABLE IF EXISTS iis_users;
DROP TABLE IF EXISTS iis_companies;
DROP TABLE IF EXISTS iis_exhibitions;
DROP TABLE IF EXISTS iis_branches;
DROP TABLE IF EXISTS iis_price_categories;
DROP TABLE IF EXISTS iis_pavilions;
DROP TABLE IF EXISTS iis_places;
DROP TABLE IF EXISTS iis_contracts;
DROP TABLE IF EXISTS iis_expositions;
DROP TABLE IF EXISTS iis_import_export_formats;

-- Config table
CREATE TABLE iis_config (
    name VARCHAR(255) NOT NULL,
    value VARCHAR(255) NOT NULL,
    PRIMARY KEY name (name)
) CHARACTER SET utf8;

-- Languages table
CREATE TABLE iis_languages (
	language_id TINYINT UNSIGNED NOT NULL AUTO_INCREMENT,
	name CHAR(20) NOT NULL,
	short_name CHAR(5) NOT NULL,
	PRIMARY KEY language_id (language_id)
) CHARACTER SET utf8;

-- Groups table
CREATE TABLE iis_groups (
	group_id TINYINT UNSIGNED NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL,
	PRIMARY KEY group_id (group_id)
) CHARACTER SET utf8;

-- Companies table
CREATE TABLE iis_companies (
	company_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL,
	address VARCHAR(255) NOT NULL,
	email VARCHAR(255) NOT NULL,
	www VARCHAR(255),
	telephone VARCHAR(255),
	IC CHAR(8) NOT NULL UNIQUE,
	DIC CHAR(10),
	PRIMARY KEY company_id (company_id)
) CHARACTER SET utf8;

-- Users table
CREATE TABLE iis_users (
	user_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	username VARCHAR(255) NOT NULL UNIQUE,
	name VARCHAR(255) NOT NULL,
	password CHAR(32) NOT NULL,
	email VARCHAR(255) NOT NULL,
	telephone VARCHAR(255),
	group_id SMALLINT UNSIGNED NOT NULL DEFAULT 2, -- Company member
	company_id INT UNSIGNED,
	PRIMARY KEY user_id (user_id)
) CHARACTER SET utf8;

-- Exhibitions table
CREATE TABLE iis_exhibitions (
	exhibition_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL,
	start INT UNSIGNED NOT NULL,
	end INT UNSIGNED NOT NULL,
	PRIMARY KEY exhibition_id (exhibition_id)
) CHARACTER SET utf8;

-- Branches table
CREATE TABLE iis_branches (
	branch_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL UNIQUE,
	PRIMARY KEY branch_id (branch_id)
) CHARACTER SET utf8;

-- Price categories table
CREATE TABLE iis_price_categories (
	price_category_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL UNIQUE,
	price_for_m2 DOUBLE(10,2) NOT NULL,
	PRIMARY KEY price_category_id (price_category_id)
) CHARACTER SET utf8;

-- Pavilions table
CREATE TABLE iis_pavilions (
	pavilion_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL UNIQUE,
	number_of_floors SMALLINT UNSIGNED NOT NULL,
	number_of_restaurants SMALLINT UNSIGNED NOT NULL DEFAULT 0,
	number_of_buffets SMALLINT UNSIGNED NOT NULL DEFAULT 0,
	number_of_social_settlements SMALLINT UNSIGNED NOT NULL DEFAULT 0,
	PRIMARY KEY pavilion_id (pavilion_id)
) CHARACTER SET utf8;

-- Places table
CREATE TABLE iis_places (
	place_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	number SMALLINT UNSIGNED NOT NULL,
	area SMALLINT UNSIGNED NOT NULL,
	pavilion_id INT UNSIGNED NOT NULL,
	price_category_id INT UNSIGNED NOT NULL,
	PRIMARY KEY place_id (place_id)
) CHARACTER SET utf8;

-- Contracts table
CREATE TABLE iis_contracts (
	contract_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	company_id INT UNSIGNED NOT NULL,
	concluded_by INT UNSIGNED NOT NULL,
	conclusion_date INT UNSIGNED NOT NULL,
	validity_start INT UNSIGNED NOT NULL,
	validity_end INT UNSIGNED NOT NULL,
	maturity_date INT UNSIGNED NOT NULL,
	price DOUBLE(10,2) NOT NULL DEFAULT 0.0,
	settled DOUBLE(10,2) NOT NULL DEFAULT 0.0,
	PRIMARY KEY contract_id (contract_id)
) CHARACTER SET utf8;

-- Expositions table
CREATE TABLE iis_expositions (
	exposition_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	exhibition_id INT UNSIGNED NOT NULL,
	place_id INT UNSIGNED NOT NULL,
	contract_id INT UNSIGNED NOT NULL,
	branch_id INT UNSIGNED NOT NULL,
	description TEXT NOT NULL,
	PRIMARY KEY exposition_id (exposition_id)
) CHARACTER SET utf8;

-- Formats table
CREATE TABLE iis_import_export_formats (
	format_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	name VARCHAR(64) NOT NULL UNIQUE,
	mime_type VARCHAR(64) NOT NULL,
	PRIMARY KEY format_id (format_id)
) CHARACTER SET utf8;

-------------------------------- Data ----------------------------------------

-- Config
INSERT INTO iis_config (name, value) VALUES ('administrator_email', 'xzemek02@stud.fit.vutbr.cz');
INSERT INTO iis_config (name, value) VALUES ('default_language', 'en');
INSERT INTO iis_config (name, value) VALUES ('default_group_id', 1);
INSERT INTO iis_config (name, value) VALUES ('default_price_category_id', 1);
INSERT INTO iis_config (name, value) VALUES ('session_name', 'IISSID');
INSERT INTO iis_config (name, value) VALUES ('session_expiration_interval', 600);
INSERT INTO iis_config (name, value) VALUES ('records_per_page', 10);
INSERT INTO iis_config (name, value) VALUES ('max_previous_pages', 5);
INSERT INTO iis_config (name, value) VALUES ('min_username_length', 1);
INSERT INTO iis_config (name, value) VALUES ('max_username_length', 255);
INSERT INTO iis_config (name, value) VALUES ('min_name_length', 1);
INSERT INTO iis_config (name, value) VALUES ('max_name_length', 255);
INSERT INTO iis_config (name, value) VALUES ('min_password_length', 1);
INSERT INTO iis_config (name, value) VALUES ('max_password_length', 255);
INSERT INTO iis_config (name, value) VALUES ('min_address_length', 1);
INSERT INTO iis_config (name, value) VALUES ('max_address_length', 255);
INSERT INTO iis_config (name, value) VALUES ('min_IC_length', 8);
INSERT INTO iis_config (name, value) VALUES ('max_IC_length', 8);
INSERT INTO iis_config (name, value) VALUES ('min_DIC_length', 10);
INSERT INTO iis_config (name, value) VALUES ('max_DIC_length', 10);
INSERT INTO iis_config (name, value) VALUES ('min_exposition_description_length', 1);
INSERT INTO iis_config (name, value) VALUES ('max_exposition_description_length', 1024);
INSERT INTO iis_config (name, value) VALUES ('min_place_number_length', 1);
INSERT INTO iis_config (name, value) VALUES ('max_place_number_length', 65535);
INSERT INTO iis_config (name, value) VALUES ('date_format', 'YYYY-MM-DD-HH-MM');

-- Languages
INSERT INTO iis_languages (name, short_name) VALUES ('English', 'en');
INSERT INTO iis_languages (name, short_name) VALUES ('Czech', 'cz');

-- Groups
INSERT INTO iis_groups (group_id, name) VALUES (1, 'Visitor');
INSERT INTO iis_groups (group_id, name) VALUES (2, 'Company_Member');
INSERT INTO iis_groups (group_id, name) VALUES (3, 'Exhibitionground_Member');

-- Formats
INSERT INTO iis_import_export_formats (format_id, name, mime_type) VALUES (1, 'XML', 'application/xml');

-- Testing data
INSERT INTO iis_users (user_id, username, name, password, email, telephone, group_id, company_id) VALUES (1, 'xzemek02', 'Petr Zemek', 'f031068ff43dec21f10c19fe3f3c38af', 'test@test.cz', NULL, 3, NULL);
INSERT INTO iis_users (user_id, username, name, password, email, telephone, group_id, company_id) VALUES (2, 'tvrdik', 'Pavel Tvrdík', '1579ef867e9de0c0556ea48379d29ece', 'pavel.tvrdik@vcb.cz', NULL, 3, NULL);
INSERT INTO iis_users (user_id, username, name, password, email, telephone, group_id, company_id) VALUES (3, 'sobota', 'Jiří Sobota', '0a4c41cb547a74303788351ffb55aba6', 'sobota@dynax.cz', '+420553678144', 2, 1);
INSERT INTO iis_users (user_id, username, name, password, email, telephone, group_id, company_id) VALUES (4, 'truhlik', 'Oto Truhlík', 'e90a2541a376f3140aee00462bd74d96', 'truhlik@dynax.cz', '+420553678145', 2, 1);
INSERT INTO iis_users (user_id, username, name, password, email, telephone, group_id, company_id) VALUES (5, 'datlo', 'Michal Datlo', '1f9f8d1612076615dd7aef3c36031ba9', 'datlo@tundra.com', NULL, 2, 2);
INSERT INTO iis_users (user_id, username, name, password, email, telephone, group_id, company_id) VALUES (6, 'skorepka', 'Pavel Skořepka', 'e562431b35857cdf928d6084bda180ae', 'skorepka@seznam.cz', NULL, 2, 3);
INSERT INTO iis_users (user_id, username, name, password, email, telephone, group_id, company_id) VALUES (7, 'plasil', 'Ondřej Plašil', '37a05c2bc9e34c4228911a726d0da702', 'vedeni@almera.cz', '+420554766999', 2, 4);
INSERT INTO iis_users (user_id, username, name, password, email, telephone, group_id, company_id) VALUES (8, 'modrava', 'Stanislav Modrava', '892059352a7b3468ea6ef9083b653b36', 'modrava@nline.cz', NULL, 2, 5);
INSERT INTO iis_users (user_id, username, name, password, email, telephone, group_id, company_id) VALUES (9, 'polak', 'Ladislav Polák', '6bd4c54f97d1322ee92259df31d04f2a', 'polak@polak.cz', '+420456223', 2, 6);
INSERT INTO iis_users (user_id, username, name, password, email, telephone, group_id, company_id) VALUES (10, 'oslava', 'Petr Oslava', 'f52d474fe6189ffcd02a40fad620c256', 'petr@oslava.net', NULL, 2, 7);
INSERT INTO iis_companies (company_id, name, address, email, www, telephone, IC, DIC) VALUES (1, 'Dynax', 'Molasova 56, Olomouc', 'info@dynax.cz', 'http://dynax.cz', NULL, '47382639', 'CZ38469367');
INSERT INTO iis_companies (company_id, name, address, email, www, telephone, IC, DIC) VALUES (2, 'Tundra', 'Hermanovo náměstí 457, Ostrava', 'info@tundra.com', 'http://www.tundra.com', NULL, '23465234', 'CZ95025384');
INSERT INTO iis_companies (company_id, name, address, email, www, telephone, IC, DIC) VALUES (3, 'Koriza', 'Lučné Háje 239, Přerov', 'koriza@seznam.cz', '', '+420643555', '56924578', NULL);
INSERT INTO iis_companies (company_id, name, address, email, www, telephone, IC, DIC) VALUES (4, 'Almera', 'Moštovní 567, Praha 5', 'vedeni@almera.cz', 'http://www.almera.cz', '+420554766999', '45732890', 'CZ00548372');
INSERT INTO iis_companies (company_id, name, address, email, www, telephone, IC, DIC) VALUES (5, 'NLine', 'Ratibořská 212, Opava', 'info@nline.cz', 'http://www.nline.cz', NULL, '62538456', 'CZ37142334');
INSERT INTO iis_companies (company_id, name, address, email, www, telephone, IC, DIC) VALUES (6, 'Polák', 'Rytířská 21, Prostějov', 'polak@polak.cz', '', '+420456223', '83529469', 'CZ-60947352');
INSERT INTO iis_companies (company_id, name, address, email, www, telephone, IC, DIC) VALUES (7, 'Frýmol', 'Ebauergova 45, Milostovice', 'frymol@wz.cz', 'http://frymol.wz.cz', NULL, '32218967', 'CZ30607932');
INSERT INTO iis_exhibitions (exhibition_id, name, start, end) VALUES (1, 'Informační technologie', 1214125200, 1214816400);
INSERT INTO iis_exhibitions (exhibition_id, name, start, end) VALUES (2, 'Bydlení', 1218618000, 1219136400);
INSERT INTO iis_exhibitions (exhibition_id, name, start, end) VALUES (3, 'Stavební materiály', 1235127600, 1235818800);
INSERT INTO iis_exhibitions (exhibition_id, name, start, end) VALUES (4, 'Flóra a fauna', 1195552800, 1196330400);
INSERT INTO iis_branches (branch_id, name) VALUES (1, 'Mobilní technologie');
INSERT INTO iis_branches (branch_id, name) VALUES (2, 'Síťové prvky');
INSERT INTO iis_branches (branch_id, name) VALUES (3, 'Elektrotechnika');
INSERT INTO iis_branches (branch_id, name) VALUES (4, 'Geografické systémy');
INSERT INTO iis_branches (branch_id, name) VALUES (5, 'Zateplení');
INSERT INTO iis_branches (branch_id, name) VALUES (6, 'Písky a štěrky');
INSERT INTO iis_branches (branch_id, name) VALUES (7, 'Dřeviny');
INSERT INTO iis_branches (branch_id, name) VALUES (8, 'Podlahy a dlažby');
INSERT INTO iis_branches (branch_id, name) VALUES (9, 'Živé ploty');
INSERT INTO iis_branches (branch_id, name) VALUES (10, 'Kočky');
INSERT INTO iis_branches (branch_id, name) VALUES (11, 'Křečci a křečíci');
INSERT INTO iis_price_categories (price_category_id, name, price_for_m2) VALUES (1, 'Základní', 100.00);
INSERT INTO iis_price_categories (price_category_id, name, price_for_m2) VALUES (2, 'Nadstandardní', 150.00);
INSERT INTO iis_price_categories (price_category_id, name, price_for_m2) VALUES (3, 'Deluxe', 250.00);
INSERT INTO iis_pavilions (pavilion_id, name, number_of_floors, number_of_restaurants, number_of_buffets, number_of_social_settlements) VALUES (1, 'A1', 2, 1, 1, 2);
INSERT INTO iis_pavilions (pavilion_id, name, number_of_floors, number_of_restaurants, number_of_buffets, number_of_social_settlements) VALUES (2, 'A2', 2, 1, 1, 2);
INSERT INTO iis_pavilions (pavilion_id, name, number_of_floors, number_of_restaurants, number_of_buffets, number_of_social_settlements) VALUES (3, 'B', 3, 1, 2, 3);
INSERT INTO iis_pavilions (pavilion_id, name, number_of_floors, number_of_restaurants, number_of_buffets, number_of_social_settlements) VALUES (4, 'C', 2, 2, 2, 2);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (1, 100, 10, 1, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (2, 101, 10, 1, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (3, 102, 10, 1, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (4, 200, 10, 1, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (5, 201, 10, 1, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (6, 202, 10, 1, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (7, 100, 10, 2, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (8, 101, 10, 2, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (9, 102, 10, 2, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (10, 200, 10, 2, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (11, 201, 10, 2, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (12, 202, 10, 2, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (13, 100, 15, 3, 2);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (14, 200, 25, 3, 3);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (15, 300, 25, 3, 3);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (16, 100, 10, 4, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (17, 101, 10, 4, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (18, 102, 10, 4, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (19, 103, 10, 4, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (20, 104, 10, 4, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (21, 200, 10, 4, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (22, 201, 10, 4, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (23, 202, 10, 4, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (24, 203, 10, 4, 1);
INSERT INTO iis_places (place_id, number, area, pavilion_id, price_category_id) VALUES (25, 204, 10, 4, 1);
INSERT INTO iis_contracts (contract_id, company_id, concluded_by, conclusion_date, validity_start, validity_end, maturity_date, price, settled) VALUES (1, 1, 3, 1197190800, 1214125200, 1214816400, 1214816400, 2250.0, 0.0);
INSERT INTO iis_contracts (contract_id, company_id, concluded_by, conclusion_date, validity_start, validity_end, maturity_date, price, settled) VALUES (2, 2, 4, 1193547600, 1214125200, 1214816400, 1214816400, 2000.0, 2000.0);
INSERT INTO iis_contracts (contract_id, company_id, concluded_by, conclusion_date, validity_start, validity_end, maturity_date, price, settled) VALUES (3, 4, 7, 1197277200, 1214211600, 1214730000, 1214730000, 1000.0, 500.0);
INSERT INTO iis_contracts (contract_id, company_id, concluded_by, conclusion_date, validity_start, validity_end, maturity_date, price, settled) VALUES (4, 5, 8, 1193047200, 1218618000, 1219136400, 1219050000, 2000.0, 0.0);
INSERT INTO iis_contracts (contract_id, company_id, concluded_by, conclusion_date, validity_start, validity_end, maturity_date, price, settled) VALUES (5, 6, 9, 1194710400, 1195552800, 1196330400, 1196330400, 75000.0, 75000.0);
INSERT INTO iis_contracts (contract_id, company_id, concluded_by, conclusion_date, validity_start, validity_end, maturity_date, price, settled) VALUES (6, 7, 10, 1195475400, 1195552800, 1196330400, 1196330400, 2000.0, 2000.0);
INSERT INTO iis_expositions (exposition_id, exhibition_id, place_id, contract_id, branch_id, description) VALUES (1, 1, 13, 1, 1, 'Telefony s vestavěnou wifi kartou.');
INSERT INTO iis_expositions (exposition_id, exhibition_id, place_id, contract_id, branch_id, description) VALUES (2, 1, 2, 2, 2, 'Směrovače Tundra.');
INSERT INTO iis_expositions (exposition_id, exhibition_id, place_id, contract_id, branch_id, description) VALUES (3, 1, 3, 2, 2, 'Software pro management sítí.');
INSERT INTO iis_expositions (exposition_id, exhibition_id, place_id, contract_id, branch_id, description) VALUES (4, 1, 16, 3, 3, 'Vysokofrekvenční zesilovače.');
INSERT INTO iis_expositions (exposition_id, exhibition_id, place_id, contract_id, branch_id, description) VALUES (5, 2, 1, 4, 8, 'Dřevěné parkety.');
INSERT INTO iis_expositions (exposition_id, exhibition_id, place_id, contract_id, branch_id, description) VALUES (6, 2, 2, 4, 8, 'Plovoucí podlahy.');
INSERT INTO iis_expositions (exposition_id, exhibition_id, place_id, contract_id, branch_id, description) VALUES (7, 4, 15, 5, 9, 'Živé ploty pro všechny zahrady.');
INSERT INTO iis_expositions (exposition_id, exhibition_id, place_id, contract_id, branch_id, description) VALUES (8, 4, 1, 6, 10, 'Cizokrajné kočky.');
INSERT INTO iis_expositions (exposition_id, exhibition_id, place_id, contract_id, branch_id, description) VALUES (9, 4, 2, 6, 11, 'Džungarští křečci.');
