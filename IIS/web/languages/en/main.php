<?php
// Encoding: utf8
/**
 * Main language data - english version (EN).
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

// Web
$lang['Web_Description'] = 'IIS Exhibition ground Information System';
$lang['Web_Keywords'] = 'IIS, exhibition ground, project, information system';
$lang['Web_Head_Title'] = 'IIS Exhibition Ground - IS';
$lang['Web_Title'] = 'IIS Exhibition Ground Information System';
$lang['IIS'] = 'Information Systems';
$lang['Programmed_By'] = 'Programmed by';
$lang['For'] = 'for';

// Main
$lang['Main_Header'] = 'Main page';
$lang['Main_Description'] = 'Welcome to the IIS Exhibition Ground Information System! <br />
	If you are a company or IIS exhibition ground member, please authorize yourself by logging in.';
$lang['Main_Contact'] = 'If you encouter a problem, please contact the system administrator';

// General
$lang['Username'] = 'Username';
$lang['Login'] = 'Login';
$lang['Password'] = 'Password';
$lang['Password_Check'] = 'Check password';
$lang['Name'] = 'Name';
$lang['Telephone'] = 'Telephone';
$lang['Email'] = 'Email';
$lang['Group'] = 'Group';
$lang['Company'] = 'Company';
$lang['Logged'] = 'Logged';
$lang['Logout'] = 'Logout';
$lang['Profile'] = 'Profile';
$lang['Module_Actions'] = 'Other module actions';
$lang['Mandatory_Fields'] = 'Mandatory fields';
$lang['Edit'] = 'Edit';
$lang['Details'] = 'Details';
$lang['Order_Descending'] = 'Descending order';
$lang['Order_Ascending'] = 'Ascending order';
$lang['Back'] = 'Back';
$lang['Goto_Page'] = 'Go to page';
$lang['IC'] = 'IC';
$lang['DIC'] = 'DIC';
$lang['Address'] = 'Address';
$lang['WWW'] = 'Website';
$lang['Represented_By'] = 'Represented by';
$lang['Price_For_m2'] = 'Price for m<sup>2</sup>';
$lang['Number_Of_Floors'] = 'Number of floors';
$lang['Number_Of_Restaurants'] = 'Number of restaurants';
$lang['Number_Of_Buffets'] = 'Number of buffets';
$lang['Number_Of_Settlements'] = 'Number of settlements';
$lang['Place_Number'] = 'Place number';
$lang['Area'] = 'Area (m<sup>2</sup>)';
$lang['Pavilion'] = 'Pavilion';
$lang['Price_Category'] = 'Price category';
$lang['Contract_N'] = 'Contract n.';
$lang['Contract_Number'] = 'Contract number';
$lang['Concluded_By'] = 'Concluded by';
$lang['Conclusion_Date'] = 'Conclusion date';
$lang['Validity_Start'] = 'Start of validity';
$lang['Validity_End'] = 'End of validity';
$lang['Maturity_Date'] = 'Maturity date';
$lang['Total_Price'] = 'Total price';
$lang['Settled'] = 'Settled';
$lang['Exhibition'] = 'Exhibition';
$lang['Place'] = 'Place';
$lang['Branch'] = 'Branch';
$lang['Description'] = 'Description';
$lang['Expositions'] = 'Expositions';
$lang['Price'] = 'Price';
$lang['Exposition_Price'] = 'Exposition price';
$lang['Contracts'] = 'Contracts';
$lang['Table'] = 'Table';
$lang['Owed_Price'] = 'Owed price';
$lang['Search'] = 'Search';
$lang['Users'] = 'Users';
$lang['Company_Members'] = 'Company members';
$lang['Companies'] = 'Companies';
$lang['Exhibitions'] = 'Exhibitions';
$lang['Pavilions'] = 'Pavilions';
$lang['Exhibition_Name'] = 'Name';
$lang['Company_Name'] = 'Name';
$lang['Exhibition_Name'] = 'Name';
$lang['Pavilion_Name'] = 'Name';
$lang['Branch_Name'] = 'Name';
$lang['Price_Category_Name'] = 'Name';

// Languages
$lang['en'] = 'English';
$lang['cz'] = 'Czech';

// Sidebar
$lang['Sidebar_Main_Menu'] = 'Main menu';
$lang['Sidebar_Administration'] = 'Administration';
$lang['Sidebar_Main_Page'] = 'Main page';
$lang['Sidebar_Stats'] = 'Statistics';
$lang['Sidebar_Search'] = 'Search';
$lang['Sidebar_Users'] = 'Users';
$lang['Sidebar_Company_Details'] = 'Company details';
$lang['Sidebar_Companies'] = 'Companies';
$lang['Sidebar_Contracts'] = 'Contracts';
$lang['Sidebar_Exhibitions'] = 'Exhibitions';
$lang['Sidebar_Expositions'] = 'Expositions';
$lang['Sidebar_Branches'] = 'Branches';
$lang['Sidebar_Price_Categories'] = 'Price categories';
$lang['Sidebar_Pavilions'] = 'Pavilions';
$lang['Sidebar_Places'] = 'Places';
$lang['Sidebar_Imports'] = 'Data imports';
$lang['Sidebar_Exports'] = 'Data exports';
$lang['Sidebar_Debtors'] = 'Debtors';
$lang['Sidebar_Forthcoming_Exhibition'] = 'Forthcoming exhibition';

// Groups
$lang['Group_Visitor'] = 'Visitor';
$lang['Group_Company_Member'] = 'Company member';
$lang['Group_Exhibitionground_Member'] = 'Administrator';

// Users
$lang['Users_View_Header'] = 'View users';
$lang['Users_View_Description'] = 'Here you can view all registered users and perform users specific actions -
	add a new user, edit a user or delete a user. User can be deleted only if he have not concluded a contract.
	You cannot delete your own account.';
$lang['Users_Add_Header'] = 'Add user';
$lang['Users_Add_Description'] = 'Here you can add a new user to the information system. Username must be unique
	and if you choose that the new user will be a company member, you have to also select his company.
	Telephone can be in any format.';
$lang['Users_Details_Header'] = 'User details';
$lang['Users_Details_Description'] = 'Here you can see user details. If the user is a company member you can
	click on the company name to get that company details.';
$lang['Users_Edit_Header'] = 'Edit user';
$lang['Users_Edit_Description'] = 'Here you can edit user data. Password must be set only if you wish to change users password.
	You cannot change users group/company if he have concluded a contract. Telephone can be in any format.';
$lang['Users_Profile_Header'] = 'Profile';
$lang['Users_Profile_Description'] = 'Here you can change your account information. Password must be set only if
	you wish to change your password. Telephone can be in any format.';
$lang['Add_User'] = 'Add user';
$lang['Edit_User'] = 'Edit user';
$lang['Profile_User'] = 'Edit profile';
$lang['Details_User'] = 'User details';
$lang['Delete_User'] = 'Delete user';
$lang['Delete_Selected_Users'] = 'Delete selected users';
$lang['View_Users'] = 'View users';
$lang['No_Users_Available'] = 'No users available.';

// Companies
$lang['Companies_View_Header'] = 'View companies';
$lang['Companies_View_Description'] = 'Here you can view all registered companies and perfom companies specific actions -
	add a new company, edit a company or delete a company. Company can be deleted only if it does not have a contract.';
$lang['Companies_Add_Header'] = 'Add company';
$lang['Companies_Add_Description'] = 'Here you can add a new company to the information system.
	Website must start with full protocol identification, like "http://". IC must be set and valid (exactly 8 digits, 0-9).
	DIC (if set) must be in format XXYYYYYYYY (XX is a country identifier, like CZ and Y is a digit, 0-9).';
$lang['Companies_Details_Header'] = 'Company details';
$lang['Companies_Details_Description'] = 'Here you can see company details, including list of people who represent this company.';
$lang['Companies_Edit_Header'] = 'Edit company';
$lang['Companies_Edit_Description'] = 'Here you can edit company data. Website must start with full protocol identification.
	IC must be set and valid (exactly 8 digits, 0-9).
	DIC (if set) must be in format XXYYYYYYYY (XX is a country identifier, like CZ and Y is a digit, 0-9).';
$lang['Add_Company'] = 'Add company';
$lang['Edit_Company'] = 'Edit company';
$lang['Details_Company'] = 'Company details';
$lang['Delete_Company'] = 'Delete company';
$lang['Delete_Selected_Companies'] = 'Delete selected companies';
$lang['View_Companies'] = 'View companies';
$lang['No_Companies_Available'] = 'No companies available.';

// Exhibitions
$lang['Exhibitions_View_Header'] = 'View exhibitions';
$lang['Exhibitions_View_Description'] = 'Here you can view all exhibitions and perfom exhibitions specific actions -
	add a new exhibition, edit an exhibition or delete an exhibition. Exhibition can be deleted only
	if there are no expositions that take place on the exposition.';
$lang['Exhibitions_Add_Header'] = 'Add exhibition';
$lang['Exhibitions_Add_Description'] = 'Here you can add a new exhibition. All dates must in format YYYY-MM-DD-HH-MM.';
$lang['Exhibitions_Details_Header'] = 'Exhibition details';
$lang['Exhibitions_Details_Description'] = 'Here you can see exhibition details.';
$lang['Exhibitions_Edit_Header'] = 'Edit exhibition';
$lang['Exhibitions_Edit_Description'] = 'Here you can edit the selected exhibition. All dates must in format YYYY-MM-DD-HH-MM.';
$lang['Add_Exhibition'] = 'Add exhibition';
$lang['Edit_Exhibition'] = 'Edit exhibition';
$lang['Details_Exhibition'] = 'Exhibition details';
$lang['Delete_Exhibition'] = 'Delete exhibition';
$lang['Delete_Selected_Exhibitions'] = 'Delete selected exhibitions';
$lang['View_Exhibitions'] = 'View exhibitions';
$lang['No_Exhibitions_Available'] = 'No exhibitions available.';
$lang['Exhibition_Start'] = 'Start of exhibition';
$lang['Exhibition_End'] = 'End of exhibition';

// Branches
$lang['Branches_View_Header'] = 'View branches';
$lang['Branches_View_Description'] = 'Here you can view all exposition branches and perfom branches specific actions -
	add a new branch, edit a branch or delete a branch. Branch can be deleted only
	if there are no expositions that belongs to that branch.';
$lang['Branches_Add_Header'] = 'Add branch';
$lang['Branches_Add_Description'] = 'Here you can add a new branch. Branch name must be unique.';
$lang['Branches_Details_Header'] = 'Branch details';
$lang['Branches_Details_Description'] = 'Here you can see branch data.';
$lang['Branches_Edit_Header'] = 'Edit branch';
$lang['Branches_Edit_Description'] = 'Here you can edit the selected branch. Branch name must be unique.';
$lang['Add_Branch'] = 'Add branch';
$lang['Edit_Branch'] = 'Edit branch';
$lang['Details_Branch'] = 'Branch details';
$lang['Delete_Branch'] = 'Delete branch';
$lang['Delete_Selected_Branches'] = 'Delete selected branches';
$lang['View_Branches'] = 'View branches';
$lang['No_Branches_Available'] = 'No branches available.';

// Price categories
$lang['Price_Categories_View_Header'] = 'View price categories';
$lang['Price_Categories_View_Description'] = 'Here you can view all price categories and perfom price categories specific actions -
	add a new price category, edit a price category or delete a price category. Price category can be deleted only
	if there is no place in that category.';
$lang['Price_Categories_Add_Header'] = 'Add price category';
$lang['Price_Categories_Add_Description'] = 'Here you can add a new price category.
	Price categories are used in conjunction with places - each place must belong to some price category.
	Name of the category must be unique.';
$lang['Price_Categories_Details_Header'] = 'Price category details';
$lang['Price_Categories_Details_Description'] = 'Here you can see price category details.';
$lang['Price_Categories_Edit_Header'] = 'Edit price categories';
$lang['Price_Categories_Edit_Description'] = 'Here you can edit the price category. Name of the category must be unique.';
$lang['Add_Price_Category'] = 'Add price category';
$lang['Edit_Price_Category'] = 'Edit price category';
$lang['Details_Price_Category'] = 'Price category details';
$lang['Delete_Price_Category'] = 'Delete price category';
$lang['Delete_Selected_Price_Categories'] = 'Delete selected price categories';
$lang['View_Price_Categories'] = 'View price categories';
$lang['No_Price_Categories_Available'] = 'No price categories available.';

// Pavilions
$lang['Pavilions_View_Header'] = 'View pavilions';
$lang['Pavilions_View_Description'] = 'Here you can view all pavilions and perfom pavilions specific actions -
	add a new pavilion, edit a pavilion or delete a pavilion. Pavilion can be deleted only
	if there is no place in this pavilion.';
$lang['Pavilions_Add_Header'] = 'Add pavilion';
$lang['Pavilions_Add_Description'] = 'Here you can add a new pavilion. Only the name (which must be unique) and
	the number of floors must be set, the rest is optional and will be set to zero by default.';
$lang['Pavilions_Details_Header'] = 'Pavilion details';
$lang['Pavilions_Details_Description'] = 'Here you can see pavilion details.';
$lang['Pavilions_Edit_Header'] = 'Edit pavilion';
$lang['Pavilions_Edit_Description'] = 'Here you can edit the selected pavilion. Pavilion name must be unique.';
$lang['Add_Pavilion'] = 'Add pavilion';
$lang['Edit_Pavilion'] = 'Edit pavilion';
$lang['Details_Pavilion'] = 'Pavilion details';
$lang['Delete_Pavilion'] = 'Delete pavilion';
$lang['Delete_Selected_Pavilions'] = 'Delete selected pavilions';
$lang['View_Pavilions'] = 'View pavilions';
$lang['No_Pavilions_Available'] = 'No pavilions available.';

// Places
$lang['Places_View_Header'] = 'View places';
$lang['Places_View_Description'] = 'Here you can view all places and perfom places specific actions -
	add a new place, edit a place or delete a place. Place can be deleted only if there
	is no exposition on that place.';
$lang['Places_Add_Header'] = 'Add place';
$lang['Places_Add_Description'] = 'Here you can add a new place. Place number must be unique in the selected
	pavilion. You also have to choose a price category of the new place.';
$lang['Places_Details_Header'] = 'Place details';
$lang['Places_Details_Description'] = 'Here you can see place details.';
$lang['Places_Edit_Header'] = 'Edit place';
$lang['Places_Edit_Description'] = 'Here you can edit the selected place. Place number must be unique in the selected
	pavilion.';
$lang['Add_Place'] = 'Add place';
$lang['Edit_Place'] = 'Edit place';
$lang['Details_Place'] = 'Place details';
$lang['Delete_Place'] = 'Delete place';
$lang['Delete_Selected_Places'] = 'Delete selected places';
$lang['View_Places'] = 'View places';
$lang['No_Places_Available'] = 'No places available.';

// Contracts
$lang['Contracts_View_Header'] = 'View contracts';
$lang['Contracts_View_Description'] = 'Here you can view concluded contracts and (if you are an exhibition ground member)
	perfom contracts specific actions - add a new contract, edit a contract or delete a contract. Contract cannot be
	deleted if there are expositions registered on it.';
$lang['Contracts_Add_Header'] = 'Add contract';
$lang['Contracts_Add_Description'] = 'Here you can add a new contract. Contracts must be concluded by a member of
	that company with which the contract is concluded. All dates must in format YYYY-MM-DD-HH-MM.';
$lang['Contracts_Details_Header'] = 'Contract details';
$lang['Contracts_Details_Description'] = 'Here you can view contract details. There is also a list of expositions
	that are registered on this contract.';
$lang['Contracts_Edit_Header'] = 'Edit contract';
$lang['Contracts_Edit_Description'] = 'Here you can edit an existing contract. Contracts must be concluded by a member of
	that company with which the contract is concluded. All dates must in format YYYY-MM-DD-HH-MM.';
$lang['Add_Contract'] = 'Add contract';
$lang['Edit_Contract'] = 'Edit contract';
$lang['Details_Contract'] = 'Contract details';
$lang['Delete_Contract'] = 'Delete contract';
$lang['Delete_Selected_Contracts'] = 'Delete selected contracts';
$lang['View_Contracts'] = 'View contracts';
$lang['No_Contracts_Available'] = 'No contracts available.';

// Expositions
$lang['Expositions_View_Header'] = 'View expositions';
$lang['Expositions_View_Description'] = 'Here you can view registered expositions and (if you are an exhibition ground member)
	perfom expositions specific actions - add a new exposition, edit an exposition or delete an exposition.';
$lang['Expositions_Add_Header'] = 'Add exposition';
$lang['Expositions_Add_Description'] = 'Here you can add a new exposition. You must select an existing contract number
	on which the new exposition will be registered, then some exhibition on which the new exposition will take place
	and a free place. Each exhibition must also have branch and description set.';
$lang['Expositions_Details_Header'] = 'Exposition details';
$lang['Expositions_Details_Description'] = 'Here you can view exposition details. You can also click on the most of
	the exposition data to see more detailed description of the exposition parts.';
$lang['Expositions_Edit_Header'] = 'Edit exposition';
$lang['Expositions_Edit_Description'] = 'Here you can edit an existing exposition.';
$lang['Add_Exposition'] = 'Add exposition';
$lang['Edit_Exposition'] = 'Edit exposition';
$lang['Details_Exposition'] = 'Exposition details';
$lang['Delete_Exposition'] = 'Delete exposition';
$lang['Delete_Selected_Expositions'] = 'Delete selected expositions';
$lang['View_Expositions'] = 'View expositions';
$lang['No_Expositions_Available'] = 'No expositions available.';

// Imports
$lang['Imports_Header'] = 'Data imports';
$lang['Imports_Description'] = 'Here you can import database tables in various formats (however, only XML
	format is supported for the moment). Be sure that the import file is not corrupted and contains valid data,
	or the import will fail.';
$lang['File_With_Import_Data'] = 'File with import data';
$lang['Do_Import'] = 'Do import';
$lang['Import_Format'] = 'Import format';

// Exports
$lang['Exports_Header'] = 'Data exports';
$lang['Exports_Description'] = 'Here you can export database tables in various formats (however, only XML
	format is supported for the moment). Just select a format and then a table you wish to export and
	click on the "Do export" button - file with exported data will be offered to you to download.';
$lang['Do_Export'] = 'Do export';
$lang['Export_Format'] = 'Export format';

// Forthcoming exhibitions
$lang['Forthcoming_Exhibition_Header'] = 'Forthcoming exhibition';
$lang['Forthcoming_Exhibition_Description'] = 'Details of the forthcoming exhibition.
	You can browse through the expositions that will take place on this exhibition.
	Note that even this exhibition is marked as "forthcoming", it can actually be
	the exhibition that currently takes place.';
$lang['Forthcoming_Exhibition'] = 'Forthcoming exhibition';
$lang['No_Forthcoming_Exhibition'] = 'No forthcoming exhibition.';
$lang['No_Expositions'] = 'This exhibition currently have no expositions.';
$lang['Expositions_On_Forthcoming_Exhibition'] = 'Expositions on this exhibition';

// Search
$lang['Search_Header'] = 'Search';
$lang['Search_Description'] = 'You can search company members, companies, exhibitions, expositions and pavilions
	by using proper keys. Character \'*\' means "any string" and \'?\' means "any character".
	If it returns some results, you can browse them to find more information.
	Comparison is case-insensitive, so "Alfred Aho" is the same as "alfred aho".';
$lang['Do_Search'] = 'Search';
$lang['Search_Query'] = 'Search query';
$lang['Search_Results'] = 'Search results';
$lang['No_Search_Results'] = 'No results.';

// Stats
$lang['Stats_Header'] = 'Statistics';
$lang['Stats_Description'] = 'Here you can see various statistics from the exhibition ground.';
$lang['Standard_Stats'] = 'Standard statistics';
$lang['Extended_Stats'] = 'Extended statistics';
$lang['Stat_Name'] = 'Name';
$lang['Stat_Value'] = 'Value';
$lang['No_Stats'] = 'No statistics available.';
$lang['Stat_Number_Of_All_Exhibitions_So_Far'] = 'Number of all exhibitions so far';
$lang['Stat_Number_Of_All_Forthcoming_Exhibitions'] = 'Number of all forthcoming exhibitions';
$lang['Stat_Number_Of_All_Expositions_On_Forthcoming_Exhibition'] = 'Number of all expositions on the forthcoming exhibition';
$lang['Stat_Average_Number_Of_Expositions_On_Exhibition'] = 'Average number of expositions on one exhibition';
$lang['Stat_Average_Number_Of_Companies_On_Exhibition'] = 'Average number of companies that have expositions on one exhibition';
$lang['Stat_Total_Owed_Price_By_Companies'] = 'Total owed price by companies';
$lang['Stat_Total_Settled_Price_By_Companies'] = 'Total settled price by companies';
$lang['Stat_Number_Of_All_Users'] = 'Number of all users';
$lang['Stat_Number_Of_All_Companies'] = 'Number of all companies';
$lang['Stat_Number_Of_All_Contracts'] = 'Number of all contracts';
$lang['Stat_Number_Of_All_Exhibitions'] = 'Number of all exhibitions';
$lang['Stat_Number_Of_All_Expositions'] = 'Number of all expositions';
$lang['Stat_Number_Of_All_Branches'] = 'Number of all branches';
$lang['Stat_Number_Of_All_Price_Categories'] = 'Number of all price categories';
$lang['Stat_Number_Of_All_Pavilions'] = 'Number of all pavilions';
$lang['Stat_Number_Of_All_Places'] = 'Number of all places (for expositions)';

// Debtors
$lang['Debtors_Header'] = 'Debtors';
$lang['Debtors_Description'] = 'Here you have a list of all companies that are in debt with precise ammount
	of settled and owed money for each company. You can warn the debtors by sending warning email to them via this page.';
$lang['Warn_Debtor'] = 'Warn debtor';
$lang['Warn_All_Debtors'] = 'Warn all debtors';
$lang['Warn_Selected_Debtors'] = 'Warn selected deptors';
$lang['No_Debtors_Available'] = 'There are currently no debtors.';
?>
