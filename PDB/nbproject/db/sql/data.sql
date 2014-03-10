--
-- Project:  PDB 2008
-- Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
--           Libor Polcak,    xpolca03@stud.fit.vutbr.cz
--           Boris Prochazka, xproch63@stud.fit.vutbr.cz
--           Petr Zemek,      xzemek02@stud.fit.vutbr.cz
--
-- SQL script that creates the basic database schema and populates it
-- with basic data.
--

--
-- Delete all information about spatial data for our tables
--
DELETE FROM USER_SDO_GEOM_METADATA;

--
-- Drop all existing tables (if any)
--
DROP TABLE refreshment;
DROP TABLE entrance;
DROP TABLE path;
DROP TABLE animal_pavilion_rel;
DROP TABLE animal_exhibit_rel;
DROP TABLE animal;
DROP TABLE genus;
DROP TABLE class;
DROP TABLE exhibit;
DROP TABLE pavilion;
DROP TABLE uploaded_photo;

--
-- Drop all existing sequences (if any)
--
DROP SEQUENCE seq_pavilion_row_id;
DROP SEQUENCE seq_pavilion_id;
DROP SEQUENCE seq_exhibit_row_id;
DROP SEQUENCE seq_exhibit_id;
DROP SEQUENCE seq_class_id;
DROP SEQUENCE seq_genus_id;
DROP SEQUENCE seq_animal_id;
DROP SEQUENCE seq_animal_pavilion_rel_row_id;
DROP SEQUENCE seq_animal_exhibit_rel_row_id;
DROP SEQUENCE seq_path_row_id;
DROP SEQUENCE seq_path_id;
DROP SEQUENCE seq_entrance_row_id;
DROP SEQUENCE seq_entrance_id;
DROP SEQUENCE seq_refreshment_row_id;
DROP SEQUENCE seq_refreshment_id;
DROP SEQUENCE seq_uploaded_photo_id;

--
-- Create primary key sequences
--
CREATE SEQUENCE seq_pavilion_row_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_pavilion_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_exhibit_row_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_exhibit_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_class_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_genus_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_animal_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_animal_pavilion_rel_row_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_animal_exhibit_rel_row_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_path_row_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_path_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_entrance_row_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_entrance_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_refreshment_row_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_refreshment_id MINVALUE 1 INCREMENT BY 1 START WITH 1;
CREATE SEQUENCE seq_uploaded_photo_id MINVALUE 1 INCREMENT BY 1 START WITH 1;

--
-- Create tables
--
CREATE TABLE pavilion(
	row_id NUMBER(8) NOT NULL,
	pavilion_id NUMBER(8) NOT NULL,
	name VARCHAR(255) NOT NULL,
	location MDSYS.SDO_GEOMETRY NOT NULL,
	photo ORDSYS.ORDIMAGE,
	photo_signature ORDSYS.ORDIMAGESIGNATURE,
	date_from DATE NOT NULL,
	date_to DATE NOT NULL,
	CONSTRAINT pavilion_pk PRIMARY KEY (row_id)
);

CREATE TABLE exhibit(
	row_id NUMBER(8) NOT NULL,
	exhibit_id NUMBER(8) NOT NULL,
	location MDSYS.SDO_GEOMETRY NOT NULL,
	photo ORDSYS.ORDIMAGE,
	photo_signature ORDSYS.ORDIMAGESIGNATURE,
	date_from DATE NOT NULL,
	date_to DATE NOT NULL,
	CONSTRAINT exhibit_pk PRIMARY KEY (row_id)
);

CREATE TABLE class(
	class_id NUMBER(8) NOT NULL,
	name VARCHAR(255) NOT NULL UNIQUE,
	CONSTRAINT class_pk PRIMARY KEY (class_id)
);

CREATE TABLE genus(
	genus_id NUMBER(8) NOT NULL,
	class_id NUMBER(8) NOT NULL,
	name VARCHAR(255) NOT NULL UNIQUE,
	CONSTRAINT genus_pk PRIMARY KEY (genus_id),
	CONSTRAINT class_fk FOREIGN KEY (class_id) REFERENCES class(class_id)
);

CREATE TABLE animal(
	animal_id NUMBER(8) NOT NULL,
	genus_id NUMBER(8) NOT NULL,
	species VARCHAR(255) NOT NULL UNIQUE,
	description VARCHAR(255) NOT NULL,
	photo ORDSYS.ORDIMAGE,
	photo_signature ORDSYS.ORDIMAGESIGNATURE,
	CONSTRAINT animal_pk PRIMARY KEY (animal_id),
	CONSTRAINT genus_fk FOREIGN KEY (genus_id) REFERENCES genus(genus_id)
);

CREATE TABLE animal_pavilion_rel(
	row_id NUMBER(8) NOT NULL,
	pavilion_id NUMBER(8) NOT NULL,
	animal_id NUMBER(8) NOT NULL,
	date_from DATE NOT NULL,
	date_to DATE NOT NULL,
	CONSTRAINT animal_pavilion_rel_pk PRIMARY KEY (row_id)
);

CREATE TABLE animal_exhibit_rel(
	row_id NUMBER(8) NOT NULL,
	exhibit_id NUMBER(8) NOT NULL,
	animal_id NUMBER(8) NOT NULL,
	date_from DATE NOT NULL,
	date_to DATE NOT NULL,
	CONSTRAINT animal_exhibit_rel_pk PRIMARY KEY (row_id)
);

CREATE TABLE path(
	row_id NUMBER(8) NOT NULL,
	path_id NUMBER(8) NOT NULL,
	location MDSYS.SDO_GEOMETRY NOT NULL,
	date_from DATE NOT NULL,
	date_to DATE NOT NULL,
	CONSTRAINT path_pk PRIMARY KEY (row_id)
);

CREATE TABLE entrance(
	row_id NUMBER(8) NOT NULL,
	entrance_id NUMBER(8) NOT NULL,
	location MDSYS.SDO_GEOMETRY NOT NULL,
	photo ORDSYS.ORDIMAGE,
	photo_signature ORDSYS.ORDIMAGESIGNATURE,
	date_from DATE NOT NULL,
	date_to DATE NOT NULL,
	CONSTRAINT entrance_pk PRIMARY KEY (row_id)
);

CREATE TABLE refreshment(
	row_id NUMBER(8) NOT NULL,
	refreshment_id NUMBER(8) NOT NULL,
	description VARCHAR(255) NOT NULL,
	location MDSYS.SDO_GEOMETRY NOT NULL,
	photo ORDSYS.ORDIMAGE,
	photo_signature ORDSYS.ORDIMAGESIGNATURE,
	date_from DATE NOT NULL,
	date_to DATE NOT NULL,
	CONSTRAINT refreshment_pk PRIMARY KEY (row_id)
);

CREATE TABLE uploaded_photo(
	uploaded_photo_id NUMBER(8) NOT NULL,
	photo ORDSYS.ORDIMAGE,
	photo_signature ORDSYS.ORDIMAGESIGNATURE,
	CONSTRAINT uploaded_photo_pk PRIMARY KEY (uploaded_photo_id)
);

--
-- Register information about spatial data for all tables that has
-- the 'location' column
--
INSERT INTO USER_SDO_GEOM_METADATA VALUES (
	'pavilion',
	'location',
	MDSYS.SDO_DIM_ARRAY(
		MDSYS.SDO_DIM_ELEMENT('X', 0, 4000, 0.005),
		MDSYS.SDO_DIM_ELEMENT('Y', 0, 3000, 0.005)
	),
	NULL
);

INSERT INTO USER_SDO_GEOM_METADATA VALUES (
	'exhibit',
	'location',
	MDSYS.SDO_DIM_ARRAY(
		MDSYS.SDO_DIM_ELEMENT('X', 0, 4000, 0.005),
		MDSYS.SDO_DIM_ELEMENT('Y', 0, 3000, 0.005)
	),
	NULL
);

INSERT INTO USER_SDO_GEOM_METADATA VALUES (
	'path',
	'location',
	MDSYS.SDO_DIM_ARRAY(
		MDSYS.SDO_DIM_ELEMENT('X', 0, 4000, 0.005),
		MDSYS.SDO_DIM_ELEMENT('Y', 0, 3000, 0.005)
	),
	NULL
);

INSERT INTO USER_SDO_GEOM_METADATA VALUES (
	'entrance',
	'location',
	MDSYS.SDO_DIM_ARRAY(
		MDSYS.SDO_DIM_ELEMENT('X', 0, 4000, 0.005),
		MDSYS.SDO_DIM_ELEMENT('Y', 0, 3000, 0.005)
	),
	NULL
);

INSERT INTO USER_SDO_GEOM_METADATA VALUES (
	'refreshment',
	'location',
	MDSYS.SDO_DIM_ARRAY(
		MDSYS.SDO_DIM_ELEMENT('X', 0, 4000, 0.005),
		MDSYS.SDO_DIM_ELEMENT('Y', 0, 3000, 0.005)
	),
	NULL
);

--
-- Populate our tables with basic data
--

--
-- Entrance
--
INSERT INTO entrance (
	row_id, entrance_id, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_entrance_row_id.NEXTVAL,
	seq_entrance_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2001, -- 2D point
		NULL, -- system of coordinates
		MDSYS.SDO_POINT_TYPE(50, 150, NULL), -- coordinates (X, Y)
		NULL, NULL -- Not valid for points
	),
	NULL,
	NULL,
	TO_DATE('2008-01-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);


--
-- Pavilions
--
INSERT INTO pavilion (
	row_id, pavilion_id, name, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_pavilion_row_id.NEXTVAL,
	seq_pavilion_id.NEXTVAL,
	'Pavilion of Elephants',
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 3), -- rectangle defined by points
		MDSYS.SDO_ORDINATE_ARRAY(240,20, 340,60) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-10-31', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO pavilion (
	row_id, pavilion_id, name, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_pavilion_row_id.NEXTVAL,
	seq_pavilion_id.NEXTVAL,
	'Pavilion of Monkeys',
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 3), -- rectangle defined by points
		MDSYS.SDO_ORDINATE_ARRAY(325,110, 385,150) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-10-10', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO pavilion (
	row_id, pavilion_id, name, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_pavilion_row_id.NEXTVAL,
	seq_pavilion_id.NEXTVAL,
	'Sea Paradise',
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 1), -- polygon defined by points
		MDSYS.SDO_ORDINATE_ARRAY(60,150, 110,85, 110,220, 60,220) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);


--
-- Exhibits
--
INSERT INTO exhibit (
	row_id, exhibit_id, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_exhibit_row_id.NEXTVAL,
	seq_exhibit_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 1), -- polygon defined by points
		MDSYS.SDO_ORDINATE_ARRAY(180,75, 230,75, 300,140, 180,140) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO exhibit (
	row_id, exhibit_id, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_exhibit_row_id.NEXTVAL,
	seq_exhibit_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 1), -- polygon defined by points
		MDSYS.SDO_ORDINATE_ARRAY(180,140, 300,140, 320,170, 320,220,
			220,220, 220,180, 180, 180) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO exhibit (
	row_id, exhibit_id, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_exhibit_row_id.NEXTVAL,
	seq_exhibit_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 3), -- rectangle defined by points
		MDSYS.SDO_ORDINATE_ARRAY(120,75, 170,140) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO exhibit (
	row_id, exhibit_id, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_exhibit_row_id.NEXTVAL,
	seq_exhibit_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 3), -- rectangle defined by points
		MDSYS.SDO_ORDINATE_ARRAY(120,140, 170,210) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO exhibit (
	row_id, exhibit_id, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_exhibit_row_id.NEXTVAL,
	seq_exhibit_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 3), -- rectangle defined by points
		MDSYS.SDO_ORDINATE_ARRAY(140,20, 240,50) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

--
-- Paths
--
INSERT INTO path (
	row_id, path_id, location, date_from, date_to
) VALUES (
	seq_path_row_id.NEXTVAL,
	seq_path_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2002, -- line/spline
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 2, 1), -- line
		MDSYS.SDO_ORDINATE_ARRAY(50,150, 120,60) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO path (
	row_id, path_id, location, date_from, date_to
) VALUES (
	seq_path_row_id.NEXTVAL,
	seq_path_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2002, -- line/spline
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 2, 1), -- line
		MDSYS.SDO_ORDINATE_ARRAY(50,150, 50,230) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO path (
	row_id, path_id, location, date_from, date_to
) VALUES (
	seq_path_row_id.NEXTVAL,
	seq_path_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2002, -- line/spline
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 2, 1), -- line
		MDSYS.SDO_ORDINATE_ARRAY(120,60, 230,60) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO path (
	row_id, path_id, location, date_from, date_to
) VALUES (
	seq_path_row_id.NEXTVAL,
	seq_path_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2002, -- line/spline
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 2, 1), -- line
		MDSYS.SDO_ORDINATE_ARRAY(230,60, 340,170) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO path (
	row_id, path_id, location, date_from, date_to
) VALUES (
	seq_path_row_id.NEXTVAL,
	seq_path_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2002, -- line/spline
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 2, 1), -- line
		MDSYS.SDO_ORDINATE_ARRAY(340,170, 340,230) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO path (
	row_id, path_id, location, date_from, date_to
) VALUES (
	seq_path_row_id.NEXTVAL,
	seq_path_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2002, -- line/spline
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 2, 1), -- line
		MDSYS.SDO_ORDINATE_ARRAY(50,230, 340,230) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO path (
	row_id, path_id, location, date_from, date_to
) VALUES (
	seq_path_row_id.NEXTVAL,
	seq_path_id.NEXTVAL,
	MDSYS.SDO_GEOMETRY(
		2002, -- line/spline
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 2, 1), -- line
		MDSYS.SDO_ORDINATE_ARRAY(175,60, 175,230) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

--
-- Refreshments
--
INSERT INTO refreshment (
	row_id, refreshment_id, description, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_refreshment_row_id.NEXTVAL,
	seq_refreshment_id.NEXTVAL,
	'Pizza',
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 4), -- circle defined by points
		MDSYS.SDO_ORDINATE_ARRAY(200,190, 215,205, 200, 220) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO refreshment (
	row_id, refreshment_id, description, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_refreshment_row_id.NEXTVAL,
	seq_refreshment_id.NEXTVAL,
	'Ice cream',
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 4), -- circle defined by points
		MDSYS.SDO_ORDINATE_ARRAY(295,70, 310,85, 295,100) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO refreshment (
	row_id, refreshment_id, description, location, photo, photo_signature, date_from, date_to
) VALUES (
	seq_refreshment_row_id.NEXTVAL,
	seq_refreshment_id.NEXTVAL,
	'Candies',
	MDSYS.SDO_GEOMETRY(
		2003, -- 2D polygon
		NULL, -- system of coordinates
		NULL, -- it is not a point
		MDSYS.SDO_ELEM_INFO_ARRAY(1, 1003, 4), -- circle defined by points
		MDSYS.SDO_ORDINATE_ARRAY(65,80, 90,95, 75,110) -- coordinates (X1,Y1, X2,Y2, ...)
	),
	NULL,
	NULL,
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

--
-- Classes
--
INSERT INTO class (
	class_id, name
) VALUES (
	seq_class_id.NEXTVAL,
	'Mammalia'
);

INSERT INTO class (
	class_id, name
) VALUES (
	seq_class_id.NEXTVAL,
	'Aves'
);

INSERT INTO class (
	class_id, name
) VALUES (
	seq_class_id.NEXTVAL,
	'Reptilia'
);

INSERT INTO class (
	class_id, name
) VALUES (
	seq_class_id.NEXTVAL,
	'Amphibia'
);

INSERT INTO class (
	class_id, name
) VALUES (
	seq_class_id.NEXTVAL,
	'Actinopterygii'
);

--
-- Genuses
--
INSERT INTO genus(
	genus_id, class_id, name
) VALUES (
	seq_genus_id.NEXTVAL,
	(SELECT class_id FROM class WHERE name = 'Mammalia'),
	'Elephants'
);

INSERT INTO genus(
	genus_id, class_id, name
) VALUES (
	seq_genus_id.NEXTVAL,
	(SELECT class_id FROM class WHERE name = 'Mammalia'),
	'Macaque'
);

INSERT INTO genus(
	genus_id, class_id, name
) VALUES (
	seq_genus_id.NEXTVAL,
	(SELECT class_id FROM class WHERE name = 'Mammalia'),
	'Pam'
);

INSERT INTO genus(
	genus_id, class_id, name
) VALUES (
	seq_genus_id.NEXTVAL,
	(SELECT class_id FROM class WHERE name = 'Mammalia'),
	'Equus'
);

INSERT INTO genus(
	genus_id, class_id, name
) VALUES (
	seq_genus_id.NEXTVAL,
	(SELECT class_id FROM class WHERE name = 'Actinopterygii'),
	'Piranhas'
);

INSERT INTO genus(
	genus_id, class_id, name
) VALUES (
	seq_genus_id.NEXTVAL,
	(SELECT class_id FROM class WHERE name = 'Aves'),
	'Flamingos'
);

INSERT INTO genus(
	genus_id, class_id, name
) VALUES (
	seq_genus_id.NEXTVAL,
	(SELECT class_id FROM class WHERE name = 'Aves'),
	'Peafowl'
);

INSERT INTO genus(
	genus_id, class_id, name
) VALUES (
	seq_genus_id.NEXTVAL,
	(SELECT class_id FROM class WHERE name = 'Reptilia'),
	'Naja'
);

INSERT INTO genus(
	genus_id, class_id, name
) VALUES (
	seq_genus_id.NEXTVAL,
	(SELECT class_id FROM class WHERE name = 'Amphibia'),
	'Litoria'
);

--
-- Animals
--
INSERT INTO animal(
	animal_id, genus_id, species, description, photo, photo_signature
) VALUES (
	seq_animal_id.NEXTVAL,
	(SELECT genus_id FROM genus WHERE name = 'Elephants'),
	'Asian Elephant',
	'Elephas maximus ...',
	NULL,
	NULL
);

INSERT INTO animal(
	animal_id, genus_id, species, description, photo, photo_signature
) VALUES (
	seq_animal_id.NEXTVAL,
	(SELECT genus_id FROM genus WHERE name = 'Equus'),
	'Plain Zebra',
	'Equus quagga ...',
	NULL,
	NULL
);

INSERT INTO animal(
	animal_id, genus_id, species, description, photo, photo_signature
) VALUES (
	seq_animal_id.NEXTVAL,
	(SELECT genus_id FROM genus WHERE name = 'Macaque'),
	'Crab-eating Macaque',
	'Macaca fascicularis ...',
	NULL,
	NULL
);

INSERT INTO animal(
	animal_id, genus_id, species, description, photo, photo_signature
) VALUES (
	seq_animal_id.NEXTVAL,
	(SELECT genus_id FROM genus WHERE name = 'Pam'),
	'Common Chimpanzee',
	'Pan troglodytes ...',
	NULL,
	NULL
);

INSERT INTO animal(
	animal_id, genus_id, species, description, photo, photo_signature
) VALUES (
	seq_animal_id.NEXTVAL,
	(SELECT genus_id FROM genus WHERE name = 'Piranhas'),
	'Wimple piranha',
	'Catoprion mento ...',
	NULL,
	NULL
);

INSERT INTO animal(
	animal_id, genus_id, species, description, photo, photo_signature
) VALUES (
	seq_animal_id.NEXTVAL,
	(SELECT genus_id FROM genus WHERE name = 'Flamingos'),
	'Greater Flamingo',
	'Phoenicopterus roseus ...',
	NULL,
	NULL
);

INSERT INTO animal(
	animal_id, genus_id, species, description, photo, photo_signature
) VALUES (
	seq_animal_id.NEXTVAL,
	(SELECT genus_id FROM genus WHERE name = 'Peafowl'),
	'Indian Peafowl',
	'Pavo cristatus ...',
	NULL,
	NULL
);

INSERT INTO animal(
	animal_id, genus_id, species, description, photo, photo_signature
) VALUES (
	seq_animal_id.NEXTVAL,
	(SELECT genus_id FROM genus WHERE name = 'Naja'),
	'Egyptian cobra',
	'Naja haje ...',
	NULL,
	NULL
);

INSERT INTO animal(
	animal_id, genus_id, species, description, photo, photo_signature
) VALUES (
	seq_animal_id.NEXTVAL,
	(SELECT genus_id FROM genus WHERE name = 'Litoria'),
	'Australian Green Tree Frog',
	'Litoria caerulea ...',
	NULL,
	NULL
);

--
-- Animals in pavilions
--
INSERT INTO animal_pavilion_rel (
	row_id, pavilion_id, animal_id, date_from, date_to
) VALUES (
	seq_animal_pavilion_rel_row_id.NEXTVAL,
	(SELECT min(pavilion_id) FROM pavilion WHERE name = 'Pavilion of Elephants'),
	(SELECT animal_id FROM animal WHERE species = 'Asian Elephant'),
	TO_DATE('2008-10-31', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO animal_pavilion_rel (
	row_id, pavilion_id, animal_id, date_from, date_to
) VALUES (
	seq_animal_pavilion_rel_row_id.NEXTVAL,
	(SELECT min(pavilion_id) FROM pavilion WHERE name = 'Pavilion of Monkeys'),
	(SELECT animal_id FROM animal WHERE species = 'Common Chimpanzee'),
	TO_DATE('2008-10-31', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO animal_pavilion_rel (
	row_id, pavilion_id, animal_id, date_from, date_to
) VALUES (
	seq_animal_pavilion_rel_row_id.NEXTVAL,
	(SELECT min(pavilion_id) FROM pavilion WHERE name = 'Pavilion of Monkeys'),
	(SELECT animal_id FROM animal WHERE species = 'Crab-eating Macaque'),
	TO_DATE('2008-10-31', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO animal_pavilion_rel (
	row_id, pavilion_id, animal_id, date_from, date_to
) VALUES (
	seq_animal_pavilion_rel_row_id.NEXTVAL,
	(SELECT min(pavilion_id) FROM pavilion WHERE name = 'Sea Paradise'),
	(SELECT animal_id FROM animal WHERE species = 'Wimple piranha'),
	TO_DATE('2008-10-31', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

--
-- Animals in exhibits
--
INSERT INTO animal_exhibit_rel (
	row_id, exhibit_id, animal_id, date_from, date_to
) VALUES (
	seq_animal_exhibit_rel_row_id.NEXTVAL,
	5, -- Exhibit next to Pavilion of Elephant, it is not my idea to use magical constant
	(SELECT animal_id FROM animal WHERE species = 'Asian Elephant'),
	TO_DATE('2008-09-01', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO animal_exhibit_rel (
	row_id, exhibit_id, animal_id, date_from, date_to
) VALUES (
	seq_animal_exhibit_rel_row_id.NEXTVAL,
	4,
	(SELECT animal_id FROM animal WHERE species = 'Greater Flamingo'),
	TO_DATE('2008-11-11', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO animal_exhibit_rel (
	row_id, exhibit_id, animal_id, date_from, date_to
) VALUES (
	seq_animal_exhibit_rel_row_id.NEXTVAL,
	4,
	(SELECT animal_id FROM animal WHERE species = 'Indian Peafowl'),
	TO_DATE('2008-10-31', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO animal_exhibit_rel (
	row_id, exhibit_id, animal_id, date_from, date_to
) VALUES (
	seq_animal_exhibit_rel_row_id.NEXTVAL,
	3,
	(SELECT animal_id FROM animal WHERE species = 'Egyptian cobra'),
	TO_DATE('2008-10-31', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO animal_exhibit_rel (
	row_id, exhibit_id, animal_id, date_from, date_to
) VALUES (
	seq_animal_exhibit_rel_row_id.NEXTVAL,
	2,
	(SELECT animal_id FROM animal WHERE species = 'Plain Zebra'),
	TO_DATE('2008-10-31', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

INSERT INTO animal_exhibit_rel (
	row_id, exhibit_id, animal_id, date_from, date_to
) VALUES (
	seq_animal_exhibit_rel_row_id.NEXTVAL,
	1,
	(SELECT animal_id FROM animal WHERE species = 'Australian Green Tree Frog'),
	TO_DATE('2008-10-31', 'YYYY-MM-DD'),
	TO_DATE('9999-12-31', 'YYYY-MM-DD') -- forever
);

--
-- Define index for all tables that has the 'location' column
--
CREATE INDEX pavilion_index ON pavilion(location)
INDEXTYPE IS MDSYS.SPATIAL_INDEX;

CREATE INDEX exhibit_index ON exhibit(location)
INDEXTYPE IS MDSYS.SPATIAL_INDEX;

CREATE INDEX path_index ON path(location)
INDEXTYPE IS MDSYS.SPATIAL_INDEX;

CREATE INDEX entrance_index ON entrance(location)
INDEXTYPE IS MDSYS.SPATIAL_INDEX;

CREATE INDEX refreshment_index ON refreshment(location)
INDEXTYPE IS MDSYS.SPATIAL_INDEX;

--
-- Commit changes
--
COMMIT;
