<?php
// Encoding: utf8
/**
 * Language data - error messages - czech version (CZ).
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

// General
$lang['Error_Not_Allowed_To_Do_Action'] = 'Nemáte povolení vykonat tuto akci.';
$lang['Error_Delete_Failed'] = 'Rušení se nepodařilo.';

// Input data
$lang['Error_Empty_Username'] = 'Jméno uživatele nemůže být prázdné.';
$lang['Error_Too_Short_Username'] = 'Jméno uživatele je příliš krátké.';
$lang['Error_Too_Long_Username'] = 'Jméno uživatele je příliš dlouhé.';
$lang['Error_Empty_Name'] = 'Jméno/název nemůže být prázdné.';
$lang['Error_Too_Short_Name'] = 'Jméno/název je příliš krátké.';
$lang['Error_Too_Long_Name'] = 'Jméno/název je příliš dlouhé.';
$lang['Error_Empty_Password'] = 'Heslo nemůže být prázdné.';
$lang['Error_Too_Short_Password'] = 'Heslo je příliš krátké.';
$lang['Error_Too_Long_Password'] = 'Heslo je příliš dlouhé.';
$lang['Error_Passwords_Not_Match'] = 'Zadaná hesla nesouhlasí.';
$lang['Error_Invalid_Email_Format'] = 'Email má neplatný formát.';
$lang['Error_Empty_Address'] = 'Adresa nemůže být prázdná.';
$lang['Error_Too_Short_Address'] = 'Adresa je příliš krátká.';
$lang['Error_Too_Long_Address'] = 'Adresa je příliš dlouhá.';
$lang['Error_Empty_IC'] = 'IČ nemůže být prázdné.';
$lang['Error_Too_Short_IC'] = 'IČ je příliš krátké.';
$lang['Error_Too_Long_IC'] = 'IČ je příliš dlouhé.';
$lang['Error_Empty_DIC'] = 'DIČ nemůže být prázdné.';
$lang['Error_Too_Short_DIC'] = 'DIČ je příliš krátké.';
$lang['Error_Too_Long_DIC'] = 'DIČ nemůže být prázdné.';
$lang['Error_Invalid_Date_Format'] = 'Datum má neplatný formát.';
$lang['Error_Empty_Date'] = 'Datum nemůže být prázdné.';
$lang['Error_Invalid_Date'] = 'Zvolené datum je neplatné.';
$lang['Error_Name_Exists'] = 'Zvolené jméno/název už existuje.';
$lang['Error_Empty_Description'] = 'Popis nemůže být prázdný.';
$lang['Error_Too_Short_Description'] = 'Popis je příliš krátký.';
$lang['Error_Too_Long_Description'] = 'Popis je příliš dlouhý.';
$lang['Error_No_File_Selected'] = 'Nebyl vybrán žádný soubor.';
$lang['Error_Empty_File'] = 'Zvolený soubor je prázdný.';
$lang['Error_File_Transfer_Error'] = 'Během přenosu souboru došlo k chybě.';

// Auth
$lang['Error_Login_Failed'] = 'Neplatné jméno uživatele nebo heslo.';
$lang['Error_Logout_Expired_Session'] = 'Byl(a) jste odhlášen(a) kvůli neaktivitě.';

// Users
$lang['Error_Invalid_User_Id'] = 'Neplatný uživatel.';
$lang['Error_Invalid_Group'] = 'Byla zvolena neplatná skupina.';
$lang['Error_Invalid_Company'] = 'Byla zvolena neplatná firma.';
$lang['Error_Username_Exists'] = 'Zvolené jméno uživatele již existuje.';
$lang['Error_Cannot_Edit_Own_Account'] = 'Nemůžete editovat svůj vlastní účet tímto způsobem - použijte profil.';
$lang['Error_Cannot_Delete_Own_Account'] = 'Nemůžete zrušit svůj vlastní účet.';
$lang['Error_User_Cannot_Be_Deleted'] = 'Zvolený uživatel nemůže být zrušen (podepsal smlouvu).';

// Companies
$lang['Error_Invalid_Company_Id'] = 'Neplatná firma.';
$lang['Error_Invalid_IC'] = 'Zvolené IČ je neplatné.';
$lang['Error_IC_Exists'] = 'Zvolené IČ již existuje.';
$lang['Error_Invalid_DIC'] = 'Zvolené DIČ je neplatné.';
$lang['Error_DIC_Exists'] = 'Zvolené DIČ již existuje.';
$lang['Error_Company_Cannot_Be_Deleted'] = 'Zvolená firma nemůže být zrušena (má podepsanou smlouvu).';

// Exhibitions
$lang['Error_Invalid_Exhibition_Id'] = 'Neplatná výstava.';
$lang['Error_Exhibition_Starts_Ends_Same'] = 'Začátek výstavy je ve stejné době jako konec výstavy.';
$lang['Error_Exhibition_Ends_Before_Starts'] = 'Výstava nemůže skončit předtím, než vůbec začne.';
$lang['Error_Exhibition_Cannot_Take_Place'] = 'V zadanou dobu se výstava nemůže konat.';
$lang['Error_Exhibition_Cannot_Be_Deleted'] = 'Zvolená výstava nemůže být zrušena (jsou v ní registrovány expozice).';

// Branches
$lang['Error_Invalid_Branch_Id'] = 'Neplatný obor.';
$lang['Error_Branch_Cannot_Be_Deleted'] = 'Zvolený obor nemůže být zrušen (existují expozice, které patří do tohoto oboru).';

// Price categories
$lang['Error_Invalid_Price_Category_Id'] = 'Neplatná cenová kategorie.';
$lang['Error_Invalid_Price'] = 'Zvolená cena je neplatná.';
$lang['Error_Price_Category_Cannot_Be_Deleted'] = 'Zvolená cenová kategorie nemůže být zrušena (existují místa v pavilonech,
	které patří do této kategorie).';

// Pavilions
$lang['Error_Invalid_Pavilion_Id'] = 'Neplatný pavilón.';
$lang['Error_Pavilions_Invalid_Number_Of_Floors'] = 'Zvolený počet podlaží je neplatný.';
$lang['Error_Pavilion_Cannot_Be_Deleted'] = 'Zvolený pavilón nemůže být zrušen (obsahuje místa určená pro expozice).';

// Places
$lang['Error_Invalid_Place_Id'] = 'Neplatné místo.';
$lang['Error_Invalid_Place_Number'] = 'Zvolené číslo místa je neplatné.';
$lang['Error_Place_Number_Exists_In_Pavilion'] = 'Místo s tímto číslem již ve zvoleném pavilónu existuje.';
$lang['Error_Invalid_Area'] = 'Zvolená plocha je neplatná.';
$lang['Error_Place_Cannot_Be_Deleted'] = 'Zvolené místo nemůže být zrušeno (existují expozice, které se konají na tomto místě).';

// Contracts
$lang['Error_Invalid_Contract_Id'] = 'Neplatná smlouva.';
$lang['Error_Contract_Starts_Ends_Same'] = 'Zvolený interval platnosti smlouvy je neplatný.';
$lang['Error_Contract_Ends_Before_Starts'] = 'Smlouva nemůže být neplatná dříve, než začne být platná.';
$lang['Error_Contract_User_Not_From_Company'] = 'Zvolený uživatel nereprezentuje zvolenou firmu.';
$lang['Error_Contract_Cannot_Be_Deleted'] = 'Zvolená smlouva nemůže být zrušena (jsou na ni registrovány expozice).';

// Expositions
$lang['Error_Invalid_Exposition_Id'] = 'Neplatná expozice.';
$lang['Error_Invalid_Exhibition'] = 'Neplatná výstava.';
$lang['Error_Invalid_Place'] = 'Neplatné místo.';
$lang['Error_Invalid_Branch'] = 'Neplatný obor.';
$lang['Error_Invalid_Contract'] = 'Neplatná smlouva.';
$lang['Error_Engaged_Place'] = 'Zvolené místo je již pro tuto výstavu obsazené.';
$lang['Error_Exposition_Cannot_Be_Deleted'] = 'Zvolená expozice nemůže být zrušena (něco na ni závisí).';

// Import
$lang['Error_Import_Failed'] = 'Import se nepodařil - importovná data byla zřejmě neplatná/poškozená.';
$lang['Error_Import_Invalid_Format'] = 'Byl zvolen neplatný formát.';

// Export
$lang['Error_Export_Failed'] = 'Export se nepodařil.';
$lang['Error_Export_Invalid_Format'] = 'Byl zvolen neplatný formát.';
$lang['Error_Export_Invalid_Table'] = 'Byla zvolena neplatná tabulka.';

// Debtors
$lang['Error_Warn_Debtors'] = 'Během varování dlužníků došlo k chybě.';
?>
