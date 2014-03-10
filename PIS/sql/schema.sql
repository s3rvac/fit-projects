--
-- Encoding: utf8
-- Authors:  Libor Polčák, xpolca03@stud.fit.vutbr.cz
--           Boris Procházka, xproch63@stud.fit.vutbr.cz
--           Petr Zemek, xzemek02@stud.fit.vutbr.cz
--
-- Schema definition (MySQL).
--

--
-- Drop all existing tables
--
DROP TABLE IF EXISTS exposition;
DROP TABLE IF EXISTS contract;
DROP TABLE IF EXISTS pavilion;
DROP TABLE IF EXISTS businessField;
DROP TABLE IF EXISTS exhibition;
DROP TABLE IF EXISTS employee;
DROP TABLE IF EXISTS company;

--
-- Create all tables
--
CREATE TABLE company (
	id INT NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL,
	ic VARCHAR(15) NOT NULL UNIQUE,
	dic VARCHAR(15) NOT NULL,
	account VARCHAR(20) NOT NULL,
	login VARCHAR(32) NOT NULL UNIQUE,
	password CHAR(32) NOT NULL,
	email VARCHAR(255) NOT NULL,
	phone VARCHAR(20),
	mobile VARCHAR(20),
	street VARCHAR(255) NOT NULL,
	number INT NOT NULL,
	psc VARCHAR(10) NOT NULL,
	city VARCHAR(255) NOT NULL,
	PRIMARY KEY id (id)
) CHARACTER SET utf8 ENGINE=InnoDB;

CREATE TABLE employee (
	id INT NOT NULL AUTO_INCREMENT,
	name VARCHAR(255),
	surname VARCHAR(255),
	rc VARCHAR(11),
	active BOOL NOT NULL,
	chief BOOL NOT NULL,
	salary INT NOT NULL,
	login VARCHAR(32) NOT NULL UNIQUE,
	password CHAR(32) NOT NULL,
	email VARCHAR(255) NOT NULL,
	phone VARCHAR(20),
	mobile VARCHAR(20),
	street VARCHAR(255) NOT NULL,
	number INT NOT NULL,
	psc VARCHAR(10) NOT NULL,
	city VARCHAR(255) NOT NULL,
	PRIMARY KEY id (id)
) CHARACTER SET utf8 ENGINE=InnoDB;

CREATE TABLE exhibition (
	id INT NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL,
	startDate DATE NOT NULL,
	endDate DATE NOT NULL,
	entranceFee INT NOT NULL,
	PRIMARY KEY id (id)
) CHARACTER SET utf8 ENGINE=InnoDB;

CREATE TABLE businessField (
	id INT NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL UNIQUE,
	PRIMARY KEY id (id)
) CHARACTER SET utf8 ENGINE=InnoDB;

CREATE TABLE pavilion (
	id INT NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL UNIQUE,
	totalArea INT NOT NULL,
	openingDate DATE NOT NULL,
	m2Price INT NOT NULL,
	numberOfFloors INT NOT NULL,
	numberOfRestaurants INT NOT NULL,
	numberOfToilets INT NOT NULL,
	electricity BOOL NOT NULL,
	water BOOL NOT NULL,
	PRIMARY KEY id (id)
) CHARACTER SET utf8 ENGINE=InnoDB;

CREATE TABLE contract (
	number INT NOT NULL AUTO_INCREMENT,
	conclusionDate DATE,
	maturityDate DATE,
	price NUMERIC(65,2) NOT NULL DEFAULT 0,
	settled NUMERIC(65,2) NOT NULL DEFAULT 0,
	contractText TEXT,
	companyId INT NOT NULL,
	signedId INT,
	PRIMARY KEY number (number),
	FOREIGN KEY (companyId) REFERENCES company(id),
	FOREIGN KEY (signedId) REFERENCES employee(id)
) CHARACTER SET utf8 ENGINE=InnoDB;

CREATE TABLE exposition (
	id INT NOT NULL AUTO_INCREMENT,
	name VARCHAR(255) NOT NULL,
	description TEXT,
	area INT NOT NULL,
	contractNumber INT NOT NULL,
	fieldId INT NOT NULL,
	exhibitionId INT NOT NULL,
	pavilionId INT NOT NULL,
	PRIMARY KEY id (id),
	FOREIGN KEY (contractNumber) REFERENCES contract(number),
	FOREIGN KEY (fieldId) REFERENCES businessField(id),
	FOREIGN KEY (exhibitionId) REFERENCES exhibition(id),
	FOREIGN KEY (pavilionId) REFERENCES pavilion(id)
) CHARACTER SET utf8 ENGINE=InnoDB;

--
-- Testing data
--
INSERT INTO company (id, name, ic, dic, account, login, password, email, phone, mobile, street, number, psc, city) VALUES (1, 'Dynax',    143465, 54566552546, 14144534, 'sobota',   '34eec5dd2d998a17702921702b9d6fcf', 'sobota@dynax.cz', '+420553678144', '+420788601256',  'Slovenská',    1, '743 95', "Beroun");
INSERT INTO company (id, name, ic, dic, account, login, password, email, phone, mobile, street, number, psc, city) VALUES (2, 'Tundra',   343465, 31235887945, 14445652, 'datlo',    '36654994a5311cb5356c7322bfcd2130', 'datlo@tundra.com', NULL, '+420788600257',  'Rakouská',     3, '597 91', "Deštná hora");
INSERT INTO company (id, name, ic, dic, account, login, password, email, phone, mobile, street, number, psc, city) VALUES (3, 'Koriza', 35665655, 13207456651, 32446650, 'skorepka', '045b5e07d9dc371e8c1fa99f7b892b60', 'skorepka@seznam.cz', NULL, '+420788700256',  'Psí',         34, '176 28', "Iksov");
INSERT INTO company (id, name, ic, dic, account, login, password, email, phone, mobile, street, number, psc, city) VALUES (4, 'Almera', 13312698, 46747414688, 16696400, 'modrava',  '3a4032b1bfb94a532be1e1865477caec', 'modrava@nline.cz', NULL, 2,  'Touringova',  13, '179 75', "Gnomeov");
INSERT INTO company (id, name, ic, dic, account, login, password, email, phone, mobile, street, number, psc, city) VALUES (5, 'NLine', 457430450, 13456746552, 69435456, 'oslava',   'aef0547ee643accf199aa40414a1c836', 'petr@oslava.net', NULL, 2,  'Pythagorova', 21, '907 53', "Hrošín");

INSERT INTO employee (id, rc, active, chief, salary, name, surname, login, password, email, phone, mobile, street, number, psc, city) VALUES (1, '651231/4160', true, false, 20000, "Petr", "Zemek", 'xzemek02', '06d1ba5de4541c7413207ac124df62c6', 'test@test.cz', NULL, NULL,  'Maďarská',     1, '111 11', "Aš");
INSERT INTO employee (id, rc, active, chief, salary, name, surname, login, password, email, phone, mobile, street, number, psc, city) VALUES (2, '550716/5466', true, true, 35211, "Pavel", "Tvrdík", 'tvrdik',   'af79951d24dc417d8a8add01e2f40973', 'pavel.tvrdik@vcb.cz', NULL, NULL,  'Cimrmanova',   8, '838 88', "Fryšták");
INSERT INTO employee (id, rc, active, chief, salary, name, surname, login, password, email, phone, mobile, street, number, psc, city) VALUES (3, '720917/6381', true, false, 13223, "Oto", "Truhlík", 'truhlik',  '90ce6b8ea3734d9d658db5135e7f2cee', 'truhlik@dynax.cz', '+420553678145', '+420788200256', 'Sloní',       55, '173 71', "Jakubovice");
INSERT INTO employee (id, rc, active, chief, salary, name, surname, login, password, email, phone, mobile, street, number, psc, city) VALUES (4, '670402/1478', true, false,  7456, "Ondřej", "Plašil", 'plasil',   '196f85b5fe0d75bea8f52076f52498e3', 'vedeni@almera.cz', '+420554766999', '+420788612256',  'Německá',      5, '341 97', "Erkvatice");
INSERT INTO employee (id, rc, active, chief, salary, name, surname, login, password, email, phone, mobile, street, number, psc, city) VALUES (5, '690119/0131', false, false, 45632, "Ladislav", "Polák", 'polak',    '19599d521c6e96935e10f44b56845da1', 'polak@polak.cz', '+420456223', '+420788699256',  'Polská',       2, '854 06', "Cvilín");

INSERT INTO exhibition (id, name, startDate, endDate, entranceFee) VALUES (1, 'Informační technologie', '2010-06-14', '2010-06-18', 120);
INSERT INTO exhibition (id, name, startDate, endDate, entranceFee) VALUES (2, 'Bydlení',                '2010-08-02', '2010-08-04',  80);
INSERT INTO exhibition (id, name, startDate, endDate, entranceFee) VALUES (3, 'Stavební materiály',     '2010-05-17', '2010-05-19', 140);
INSERT INTO exhibition (id, name, startDate, endDate, entranceFee) VALUES (4, 'Flóra a fauna',          '2010-04-19', '2010-04-22',  60);

INSERT INTO businessField (id, name) VALUES (1, 'Mobilní technologie');
INSERT INTO businessField (id, name) VALUES (2, 'Síťové prvky');
INSERT INTO businessField (id, name) VALUES (3, 'Elektrotechnika');
INSERT INTO businessField (id, name) VALUES (4, 'Geografické systémy');
INSERT INTO businessField (id, name) VALUES (5, 'Zateplení');
INSERT INTO businessField (id, name) VALUES (6, 'Písky a štěrky');
INSERT INTO businessField (id, name) VALUES (7, 'Dřeviny');
INSERT INTO businessField (id, name) VALUES (8, 'Podlahy a dlažby');
INSERT INTO businessField (id, name) VALUES (9, 'Živé ploty');
INSERT INTO businessField (id, name) VALUES (10, 'Kočky');
INSERT INTO businessField (id, name) VALUES (11, 'Křečci a křečíci');

INSERT INTO pavilion (id, name, totalArea, openingDate, m2Price, numberOfFloors, numberOfRestaurants, electricity, water, numberOfToilets) VALUES (1, 'A1', 13141, '2006-12-11', 3, 1, 2, true, true, 1);
INSERT INTO pavilion (id, name, totalArea, openingDate, m2Price, numberOfFloors, numberOfRestaurants, electricity, water, numberOfToilets) VALUES (2, 'A2', 26546, '2003-01-06', 1, 1, 1, true, false, 1);
INSERT INTO pavilion (id, name, totalArea, openingDate, m2Price, numberOfFloors, numberOfRestaurants, electricity, water, numberOfToilets) VALUES (3, 'B',   1452, '2009-04-17', 6, 2, 0, false, false, 2);
INSERT INTO pavilion (id, name, totalArea, openingDate, m2Price, numberOfFloors, numberOfRestaurants, electricity, water, numberOfToilets) VALUES (4, 'C',  45646, '2002-05-23', 4, 2, 0, true, true, 3);

INSERT INTO contract (number, conclusionDate, maturityDate, price, settled, contractText, companyId, signedId) VALUES (1, '2010-03-18', '2010-12-31',  2250,      0, 'Text smlouvy 1', 1, 2);
INSERT INTO contract (number, conclusionDate, maturityDate, price, settled, contractText, companyId, signedId) VALUES (2, '2010-01-01', '2010-10-31',  2000,   2000, 'Text smlouvy 2', 2, 4);
INSERT INTO contract (number, conclusionDate, maturityDate, price, settled, contractText, companyId, signedId) VALUES (3, '2010-03-18', '2012-12-31',  1000,    500, 'Text smlouvy 3', 3, 5);
INSERT INTO contract (number, conclusionDate, maturityDate, price, settled, contractText, companyId, signedId) VALUES (4, '2010-02-01', '2010-12-31',  2000,      0, 'Text smlouvy 4', 2, 4);
INSERT INTO contract (number, conclusionDate, maturityDate, price, settled, contractText, companyId, signedId) VALUES (5, '2010-02-15', '2010-12-31', 75000,  75000, 'Text smlouvy 5', 4, 4);
INSERT INTO contract (number, conclusionDate, maturityDate, price, settled, contractText, companyId, signedId) VALUES (6, '2010-01-14', '2010-12-31',  2000,   2000, 'Text smlouvy 6', 5, 2);

INSERT INTO exposition (id, name, description, area, exhibitionId, pavilionId, contractNumber, fieldId) VALUES (1, 'Telefony s vestavěnou wifi kartou.', 'Popis 1', 123, 1, 3, 1, 1);
INSERT INTO exposition (id, name, description, area, exhibitionId, pavilionId, contractNumber, fieldId) VALUES (2, 'Směrovače Tundra.',                  'Popis 2',  49, 1, 1, 2, 2);
INSERT INTO exposition (id, name, description, area, exhibitionId, pavilionId, contractNumber, fieldId) VALUES (3, 'Software pro management sítí.',      'Popis 3',  56, 1, 1, 2, 2);
INSERT INTO exposition (id, name, description, area, exhibitionId, pavilionId, contractNumber, fieldId) VALUES (4, 'Vysokofrekvenční zesilovače.',       'Popis 4',  13, 1, 4, 3, 3);
INSERT INTO exposition (id, name, description, area, exhibitionId, pavilionId, contractNumber, fieldId) VALUES (5, 'Dřevěné parkety.',                   'Popis 5', 456, 2, 1, 4, 8);
INSERT INTO exposition (id, name, description, area, exhibitionId, pavilionId, contractNumber, fieldId) VALUES (6, 'Plovoucí podlahy.',                  'Popis 6', 135, 2, 1, 4, 8);
INSERT INTO exposition (id, name, description, area, exhibitionId, pavilionId, contractNumber, fieldId) VALUES (7, 'Živé ploty pro všechny zahrady.',    'Popis 7', 159, 4, 3, 5, 9);
INSERT INTO exposition (id, name, description, area, exhibitionId, pavilionId, contractNumber, fieldId) VALUES (8, 'Cizokrajné kočky.',                  'Popis 8', 132, 4, 1, 6, 10);
INSERT INTO exposition (id, name, description, area, exhibitionId, pavilionId, contractNumber, fieldId) VALUES (9, 'Džungarští křečci.',                 'Popis 9',  49, 4, 1, 6, 11);
