--
-- Projekt do predmetu IDS
-- Petr Zemek, xzemek02@stud.fit.vutbr.cz
-- 2007
--

-- Zruseni pripadnych existujicich tabulek
DROP TABLE Expozice;
DROP TABLE Vystava;
DROP TABLE Obor;
DROP TABLE Misto;
DROP TABLE Kategorie;
DROP TABLE Pavilon;
DROP TABLE Smlouva;
DROP TABLE Firma;
DROP TABLE Uzivatel_IS;
DROP TABLE Skupina;

-- Zruseni pripadnych existujicich sekvenci
DROP SEQUENCE Seq_Skupina;
DROP SEQUENCE Seq_Firma;
DROP SEQUENCE Seq_Smlouva;
DROP SEQUENCE Seq_Vystava;
DROP SEQUENCE Seq_Obor;
DROP SEQUENCE Seq_Kategorie;
DROP SEQUENCE Seq_Misto;

-- Vytvoreni tabulky Skupina
CREATE TABLE Skupina (
	id INTEGER NOT NULL,
	nazev VARCHAR(255) NOT NULL,
	pravo_cist_detaily_firmy CHAR(1) DEFAULT 'A' NOT NULL,
	pravo_editovat_detaily_firmy CHAR(1) DEFAULT 'N' NOT NULL,
	pravo_cist_smlouvu CHAR(1) DEFAULT 'N' NOT NULL,
	pravo_editovat_smlouvu CHAR(1) DEFAULT 'N' NOT NULL,
	pravo_pridavat_uzivatele CHAR(1) DEFAULT 'N' NOT NULL,
	pravo_editovat_uzivatele char(1) DEFAULT 'N' NOT NULL,
	CONSTRAINT Skupina_pk PRIMARY KEY (id)
);

-- Vytvoreni tabulky Uzivatel_IS
CREATE TABLE Uzivatel_IS (
	login VARCHAR(255) NOT NULL,
	jmeno VARCHAR(255) NOT NULL,
	prijmeni VARCHAR(255) NOT NULL,
	heslo CHAR(32) NOT NULL,
	telefon VARCHAR(255),
	email VARCHAR(255),
	id_skupiny INTEGER NOT NULL,
	CONSTRAINT Uzivatel_IS_pk PRIMARY KEY (login),
	CONSTRAINT Uzivatel_IS_fk_1 FOREIGN KEY (id_skupiny) REFERENCES Skupina(id)
);

-- Vytvoreni tabulky Firma
CREATE TABLE Firma (
	id INTEGER NOT NULL,
	zastupuje VARCHAR(255) NOT NULL,
	nazev VARCHAR(255) NOT NULL,
	adresa VARCHAR(255) NOT NULL,
	telefon VARCHAR(255),
	email VARCHAR(255),
	ICO INTEGER,
	DIC INTEGER,
	CONSTRAINT Firma_pk PRIMARY KEY (id),
	CONSTRAINT Firma_fk_1 FOREIGN KEY (zastupuje) REFERENCES Uzivatel_IS(login)
);

-- Vytvoreni tabulky Smlouva
CREATE TABLE Smlouva (
	cislo_smlouvy INTEGER NOT NULL,
	zastupitel VARCHAR(255) NOT NULL,
	id_firmy INTEGER NOT NULL,
	platnost_od DATE NOT NULL,
	platnost_do DATE NOT NULL,
	datum_uzavreni DATE NOT NULL,
	cena NUMERIC(10,2) NOT NULL,
	uhrazeno NUMERIC(10,2) NOT NULL,
	splatnost DATE NOT NULL,
	CONSTRAINT Smlouva_pk PRIMARY KEY (cislo_smlouvy),
	CONSTRAINT Smlouva_fk_1 FOREIGN KEY (zastupitel) REFERENCES Uzivatel_IS(login),
	CONSTRAINT Smlouva_fk_2 FOREIGN KEY (id_firmy) REFERENCES Firma(id)
);

-- Vytvoreni tabulky Vystava
CREATE TABLE Vystava (
	id INTEGER NOT NULL ,
	nazev VARCHAR(255) NOT NULL,
	datum_zahajeni DATE NOT NULL,
	datum_ukonceni DATE NOT NULL,
	CONSTRAINT Vystava_pk PRIMARY KEY (id)
);

-- Vytvoreni tabulky Obor
CREATE TABLE Obor (
	id INTEGER NOT NULL ,
	nazev VARCHAR(255) NOT NULL,
	CONSTRAINT Obor_pk PRIMARY KEY (id)
);

ALTER TABLE Obor ADD CONSTRAINT Obor_unique_id UNIQUE (nazev);

-- Vytvoreni tabulky Kategorie
CREATE TABLE Kategorie (
	id INTEGER NOT NULL ,
	nazev VARCHAR(255) NOT NULL,
	orientacni_cena_za_m2 NUMERIC(10,2) NOT NULL,
	CONSTRAINT Kategorie_pk PRIMARY KEY (id)
);

-- Vytvoreni tabulky Pavilon
CREATE TABLE Pavilon (
	nazev VARCHAR(20) NOT NULL,
	pocet_pater INTEGER NOT NULL,
	pocet_restauraci INTEGER NOT NULL,
	pocet_obcersveni INTEGER NOT NULL,
	pocet_soc_zarizeni INTEGER NOT NULL,
	CONSTRAINT Pavilon_pk PRIMARY KEY (nazev)
);

-- Vytvoreni tabulky Misto
CREATE TABLE Misto (
	cislo INTEGER NOT NULL ,
	nazev_pavilonu VARCHAR(255) NOT NULL,
	id_kategorie INT NOT NULL,
	plocha INTEGER NOT NULL,
	CONSTRAINT Misto_pk PRIMARY KEY (cislo, nazev_pavilonu),
	CONSTRAINT Misto_fk_1 FOREIGN KEY (id_kategorie) REFERENCES Kategorie(id)
);

-- Vytvoreni tabulky Expozice
CREATE TABLE Expozice (
	id_vystavy INTEGER NOT NULL,
	id_mista INTEGER NOT NULL,
	cislo_smlouvy INTEGER NOT NULL,
	id_oboru INTEGER NOT NULL,
	popis VARCHAR(255) NOT NULL,
	CONSTRAINT Expozice_pk PRIMARY KEY (id_vystavy, id_mista),
	CONSTRAINT Expozice_fk_1 FOREIGN KEY (cislo_smlouvy) REFERENCES Smlouva(cislo_smlouvy),
	CONSTRAINT Expozice_fk_2 FOREIGN KEY (id_oboru) REFERENCES Obor(id)
);

-- Vytvoreni sekvenci
CREATE SEQUENCE Seq_Skupina;
CREATE SEQUENCE Seq_Firma;
CREATE SEQUENCE Seq_Smlouva;
CREATE SEQUENCE Seq_Vystava;
CREATE SEQUENCE Seq_Obor;
CREATE SEQUENCE Seq_Kategorie;
CREATE SEQUENCE Seq_Misto;

-- Vlozeni dat do tabulky Skupina
INSERT INTO Skupina
VALUES (Seq_Skupina.NEXTVAL, 'Provozovatel vystaviste', 'A', 'A', 'A', 'A', 'A', 'A');
INSERT INTO Skupina
VALUES (Seq_Skupina.NEXTVAL, 'Zastupitel firmy', 'A', 'N', 'A', 'N', 'N', 'N');
INSERT INTO Skupina
VALUES (Seq_Skupina.NEXTVAL, 'Navstevnik', 'A', 'N', 'N', 'N', 'N', 'N');

-- Vlozeni dat do tabulky Uzivatel_IS
INSERT INTO Uzivatel_IS
VALUES ('tvrdik', 'Jan', 'Tvrdik', 'ef784g3ev0', '553789117', 'tvrdik@vystaviste.cz', 1);
INSERT INTO Uzivatel_IS
VALUES ('skorepka', 'Pavel', 'Skorepka', '7f0x53nf75', '556435770', 'skorepka@dynax.cz', 2);
INSERT INTO Uzivatel_IS
VALUES ('datlo', 'Michal', 'Datlo', '8v63hx2c33', '552609899', 'datlo@tundra.cz', 2);
INSERT INTO Uzivatel_IS
VALUES ('filas', 'Jirka', 'Filas', '123n5v897d', '554896221', 'filas@koriza.cz', 2);
INSERT INTO Uzivatel_IS
VALUES ('mohila', 'Rudolf', 'Mohila', 'h46xf3d95m', NULL, 'mohila@seznam.cz', 3);

-- Vlozeni dat do tabulky Firma
INSERT INTO Firma
VALUES (Seq_Firma.NEXTVAL, 'skorepka', 'Dynax', 'Molasova 56, Olomouc', '556345987', 'info@dynax.cz', '47114312', '44992785');
INSERT INTO Firma
VALUES (Seq_Firma.NEXTVAL, 'datlo', 'Tundra', 'Hermanovo namesti 457, Ostrava', '552609899', 'firma@tundra.cz', '59372530', NULL);
INSERT INTO Firma
VALUES (Seq_Firma.NEXTVAL, 'filas', 'Koriza', 'Lucne Haje 239, Prerov', '554236788', 'kontakt@koriza.cz', '58163857', NULL);

-- Vlozeni dat do tabulky Smlouva
INSERT INTO Smlouva
VALUES (Seq_Smlouva.NEXTVAL, 'skorepka', 1, TO_DATE('2007-04-05', 'YY-MM-DD'), TO_DATE('2007-07-15', 'YY-MM-DD'), TO_DATE('2007-04-05', 'YY-MM-DD'), 4500.00, 0.00, TO_DATE('2007-07-05', 'YY-MM-DD'));
INSERT INTO Smlouva
VALUES (Seq_Smlouva.NEXTVAL, 'datlo', 2, TO_DATE('2007-04-20', 'YY-MM-DD'), TO_DATE('2007-07-15', 'YY-MM-DD'), TO_DATE('2007-04-20', 'YY-MM-DD'), 5000.00, 0.00, TO_DATE('2007-07-05', 'YY-MM-DD'));
INSERT INTO Smlouva
VALUES (Seq_Smlouva.NEXTVAL, 'filas', 3, TO_DATE('2007-04-21', 'YY-MM-DD'), TO_DATE('2007-07-15', 'YY-MM-DD'), TO_DATE('2007-04-21', 'YY-MM-DD'), 3700.00, 0.00, TO_DATE('2007-07-05', 'YY-MM-DD'));

-- Vlozeni dat do tabulky Vystava
INSERT INTO Vystava
VALUES (Seq_Vystava.NEXTVAL, 'Informacni technologie', TO_DATE('2007-07-05', 'YY-MM-DD'), TO_DATE('2007-07-15', 'YY-MM-DD'));
INSERT INTO Vystava
VALUES (Seq_Vystava.NEXTVAL, 'Stavebni materialy', TO_DATE('2008-01-23', 'YY-MM-DD'), TO_DATE('2008-01-30', 'YY-MM-DD'));

-- Vlozeni dat do tabulky Obor
INSERT INTO Obor
VALUES (Seq_Obor.NEXTVAL, 'Mobilni technologie');
INSERT INTO Obor
VALUES (Seq_Obor.NEXTVAL, 'Sitove prvky');
INSERT INTO Obor
VALUES (Seq_Obor.NEXTVAL, 'Logicke obvody');
INSERT INTO Obor
VALUES (Seq_Obor.NEXTVAL, 'Databazove systemy');
INSERT INTO Obor
VALUES (Seq_Obor.NEXTVAL, 'Geograficke systemy');
INSERT INTO Obor
VALUES (Seq_Obor.NEXTVAL, 'Testovaci zarizeni');

-- Vlozeni dat do tabulky Kategorie
INSERT INTO Kategorie
VALUES (Seq_Kategorie.NEXTVAL, 'Basic', 500);
INSERT INTO Kategorie
VALUES (Seq_Kategorie.NEXTVAL, 'Standard', 1000);
INSERT INTO Kategorie
VALUES (Seq_Kategorie.NEXTVAL, 'Deluxe', 2000);

-- Vlozeni dat do tabulky Pavilon
INSERT INTO Pavilon
VALUES ('A1', 2, 0, 1, 2);
INSERT INTO Pavilon
VALUES ('A2', 3, 1, 2, 4);
INSERT INTO Pavilon
VALUES ('B1', 2, 0, 0, 4);
INSERT INTO Pavilon
VALUES ('C1', 4, 1, 1, 6);

-- Vlozeni dat do tabulky Misto
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'A1', 1, 20);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'A1', 1, 20);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'A1', 2, 25);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'A1', 3, 25);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'A2', 1, 15);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'A2', 2, 20);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'A2', 2, 20);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'A2', 2, 20);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'A2', 3, 20);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'B1', 2, 25);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'B1', 3, 30);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'B1', 3, 50);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'C1', 1, 10);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'C1', 1, 15);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'C1', 1, 15);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'C1', 2, 15);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'C1', 2, 20);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'C1', 2, 15);
INSERT INTO Misto
VALUES (Seq_Misto.NEXTVAL, 'C1', 3, 30);

-- Vlozeni dat do tabulky Expozice
INSERT INTO Expozice
VALUES (1, 1, 1, 1, 'Minikamery v mobilnich zarizenich');
INSERT INTO Expozice
VALUES (1, 4, 2, 2, 'Vysokorychlostni routery');
INSERT INTO Expozice
VALUES (1, 6, 2, 2, 'Opticke kabely');
INSERT INTO Expozice
VALUES (1, 10, 3, 4, 'Databazovy system Koriza');
INSERT INTO Expozice
VALUES (1, 12, 3, 4, 'Nasazeni databazovych systemu ve firmach');

-- Konec souboru xzemek02.sql

