<?php
// Encoding: utf8
/**
 * Language data - error messages - english version (EN).
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

// General
$lang['Error_Not_Allowed_To_Do_Action'] = 'You are not allowed to perform this this action.';
$lang['Error_Delete_Failed'] = 'Delete failed.';

// Input data
$lang['Error_Empty_Username'] = 'Username cannot be empty.';
$lang['Error_Too_Short_Username'] = 'Username is too short.';
$lang['Error_Too_Long_Username'] = 'Username is too long.';
$lang['Error_Empty_Name'] = 'Name cannot be empty.';
$lang['Error_Too_Short_Name'] = 'Name is too short.';
$lang['Error_Too_Long_Name'] = 'Name is too long.';
$lang['Error_Empty_Password'] = 'Password cannot be empty.';
$lang['Error_Too_Short_Password'] = 'Password is too short.';
$lang['Error_Too_Long_Password'] = 'Password is too long.';
$lang['Error_Passwords_Not_Match'] = 'Passwords does not match.';
$lang['Error_Invalid_Email_Format'] = 'Email has invalid format.';
$lang['Error_Empty_Address'] = 'Address cannot be empty.';
$lang['Error_Too_Short_Address'] = 'Address is too short.';
$lang['Error_Too_Long_Address'] = 'Address is too long.';
$lang['Error_Empty_IC'] = 'IC cannot be empty.';
$lang['Error_Too_Short_IC'] = 'IC is too short.';
$lang['Error_Too_Long_IC'] = 'IC is too long.';
$lang['Error_Empty_DIC'] = 'DIC cannot be empty.';
$lang['Error_Too_Short_DIC'] = 'DIC is too short.';
$lang['Error_Too_Long_DIC'] = 'DIC is too long.';
$lang['Error_Invalid_Date_Format'] = 'Date has invalid format.';
$lang['Error_Empty_Date'] = 'Date cannot be empty.';
$lang['Error_Invalid_Date'] = 'Selected date is invalid.';
$lang['Error_Name_Exists'] = 'Selected name already exists.';
$lang['Error_Empty_Description'] = 'Description cannot be empty.';
$lang['Error_Too_Short_Description'] = 'Description is too short.';
$lang['Error_Too_Long_Description'] = 'Description is too long.';
$lang['Error_No_File_Selected'] = 'No file selected.';
$lang['Error_Empty_File'] = 'Selected file is empty';
$lang['Error_File_Transfer_Error'] = 'There was an error during the file transfer.';

// Auth
$lang['Error_Login_Failed'] = 'Invalid username or password.';
$lang['Error_Logout_Expired_Session'] = 'You were logged out because of your inactivity.';

// Users
$lang['Error_Invalid_User_Id'] = 'Invalid user.';
$lang['Error_Invalid_Group'] = 'Invalid group was selected.';
$lang['Error_Invalid_Company'] = 'Invalid company was selected.';
$lang['Error_Username_Exists'] = 'Selected username already exists.';
$lang['Error_Cannot_Edit_Own_Account'] = 'You cannot edit your account this way - use profile instead.';
$lang['Error_Cannot_Delete_Own_Account'] = 'You cannot delete your own account.';
$lang['Error_User_Cannot_Be_Deleted'] = 'Selected user cannot be deleted (he has concluded a contract).';


// Companies
$lang['Error_Invalid_Company_Id'] = 'Invalid company.';
$lang['Error_Invalid_IC'] = 'Selected IC is invalid.';
$lang['Error_IC_Exists'] = 'Selected IC already exists.';
$lang['Error_Invalid_DIC'] = 'Selected DIC is invalid.';
$lang['Error_DIC_Exists'] = 'Selected DIC already exists.';
$lang['Error_Company_Cannot_Be_Deleted'] = 'Selected company cannot be deleted (it has a contract).';

// Exhibitions
$lang['Error_Invalid_Exhibition_Id'] = 'Invalid exhibition.';
$lang['Error_Exhibition_Starts_Ends_Same'] = 'Starting date cannot be equal to the ending date.';
$lang['Error_Exhibition_Ends_Before_Starts'] = 'Exhibition cannot end before it starts.';
$lang['Error_Exhibition_Cannot_Take_Place'] = 'Exhibition cannot take place in the selected period.';
$lang['Error_Exhibition_Cannot_Be_Deleted'] = 'Selected exhibition cannot be deleted (there are registered expositions in it).';

// Branches
$lang['Error_Invalid_Branch_Id'] = 'Invalid branch.';
$lang['Error_Branch_Cannot_Be_Deleted'] = 'Selected branch cannot be deleted (there are expositions that belong to this branch).';

// Price categories
$lang['Error_Invalid_Price_Category_Id'] = 'Invalid price category.';
$lang['Error_Invalid_Price'] = 'Selected price is invalid.';
$lang['Error_Price_Category_Cannot_Be_Deleted'] = 'Selected price category cannot be deleted (there are places that belong to this category).';

// Pavilions
$lang['Error_Invalid_Pavilion_Id'] = 'Invalid pavilion.';
$lang['Error_Pavilions_Invalid_Number_Of_Floors'] = 'Selected number of floors is invalid.';
$lang['Error_Pavilion_Cannot_Be_Deleted'] = 'Selected pavilion cannot be deleted (there are places in this pavilion).';

// Places
$lang['Error_Invalid_Place_Id'] = 'Invalid place.';
$lang['Error_Invalid_Place_Number'] = 'Selected place number is invalid.';
$lang['Error_Place_Number_Exists_In_Pavilion'] = 'Place of the selected number already exists in the selected pavilion.';
$lang['Error_Invalid_Area'] = 'Selected area is invalid.';
$lang['Error_Place_Cannot_Be_Deleted'] = 'Selected place cannot be deleted (there are registered expositions on it).';

// Contracts
$lang['Error_Invalid_Contract_Id'] = 'Invalid contract.';
$lang['Error_Contract_Starts_Ends_Same'] = 'Contract validation dates cannot be equal.';
$lang['Error_Contract_Ends_Before_Starts'] = 'Contract cannot become invalid before it gets valid.';
$lang['Error_Contract_User_Not_From_Company'] = 'Selected user does not represent the selected company.';
$lang['Error_Contract_Cannot_Be_Deleted'] = 'Selected contract cannot be deleted (there are expositions registred on it).';

// Expositions
$lang['Error_Invalid_Exposition_Id'] = 'Invalid exposition.';
$lang['Error_Invalid_Exhibition'] = 'Invalid exhibition.';
$lang['Error_Invalid_Place'] = 'Invalid place.';
$lang['Error_Invalid_Branch'] = 'Invalid branch.';
$lang['Error_Invalid_Contract'] = 'Invalid contract.';
$lang['Error_Engaged_Place'] = 'Selected place is engaged.';
$lang['Error_Exposition_Cannot_Be_Deleted'] = 'Selected exposition cannot be deleted (something depends on it).';

// Import
$lang['Error_Import_Failed'] = 'Import failed - most probably the export data were corrupted.';
$lang['Error_Import_Invalid_Format'] = 'Invalid import format selected.';

// Export
$lang['Error_Export_Failed'] = 'Export failed.';
$lang['Error_Export_Invalid_Format'] = 'Invalid export format selected.';
$lang['Error_Export_Invalid_Table'] = 'Invalid table selected.';

// Debtors
$lang['Error_Warn_Debtors'] = 'There was an error during debtors warning.';
?>
