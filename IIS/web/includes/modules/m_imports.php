<?php
// Encoding: utf8
/**
 * Imports module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Imports selected data in the XML format.
 *
 * @param string $data Data to be imported.
 * @return bool True if the import was successfull, false if some
 * error occures.
 */
function import_xml_data($data)
{
	// Disable error reporting (I didn't find another way how to
	// suppress warning messages produced by SimpleXML when
	// import data are invalid/corrupted)
	error_reporting(0);

	try {
		$table = new SimpleXMLElement($data);
		$table_name = $table[0]['name'];
		if (empty($table_name)) {
			return false;
		}

		$sqls = array();

		// Drop table query
		$query = 'DROP TABLE IF EXISTS ' . $table_name;
		array_push($sqls, $query);

		// Create table query
		$query = 'CREATE TABLE ' . $table_name . ' (';
		$columns = array();
		foreach ($table->schema[0]->column as $column) {
			array_push($columns, $column['field']);

			// Field
			$column_name = $column['field'];
			// Type
			$column_type = $column['type'];
			// NULL
			$column_null = $column['null'] == 'YES' ? '' : 'NOT NULL';
			// Key
			if ($column['key'] == 'PRI') {
				$column_key = 'PRIMARY KEY';
			} else if ($column['key'] == 'UNI') {
				$column_key = 'UNIQUE';
			} else {
				$column_key = '';
			}
			// Default
			$column_default = !empty($column['default']) ? "DEFAULT '" . $column['default'] . "'" : '';
			// Extra
			$column_extra = $column['extra'];

			$query .= $column_name . ' ' . $column_type . ' ' . $column_null . ' ' .
				$column_key . ' ' . $column_default . ' ' . $column_extra . ',';
		}
		if (empty($columns)) {
			return false;
		}

		$query[strlen($query) - 1] = ' '; // Remove the last comma
		$query .= ') CHARACTER SET utf8';
		array_push($sqls, $query);

		// Data queries
		foreach ($table->data[0]->row as $row) {
			$query = 'INSERT INTO ' . $table_name . '(';
			foreach ($columns as $column) {
				$query .= $column . ",";
			}
			$query[strlen($query) - 1] = ' '; // Remove the last comma
			$query .= ') VALUES (';
			foreach ($columns as $column) {
				if (isset($row[$column])) {
					$query .= "'" . handle_input_string($row[$column]) . "',";
				} else {
					$query .= 'NULL,';
				}
			}
			$query[strlen($query) - 1] = ' '; // Remove the last comma
			$query .= ')';
			array_push($sqls, $query);
		}

		foreach ($sqls as $sql) {
			if (!mysql_query($sql)) {
				throw new IIS_DB_Exception('Error during data import.',
					$sql, mysql_error());
			}
		}
	} catch (Exception $e) {
		return false;
	}

	error_reporting(E_ALL ^ E_NOTICE); // Re-enable error reporting
	return true;
}

/**
 * Imports selected data in the selected format.
 *
 * @param string $data Data to be imported.
 * @param string $format Data format.
 * @return bool True if the import was successfull, false if some
 * error occures.
 */
function import_data($data, $format)
{
	switch ($format) {
		case IMPORT_EXPORT_FORMAT_XML:
			return import_xml_data($data);
			break;

		default:
			return $lang['Error_Import_Invalid_Format'];
			break;
	}
}

if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
	not_allowed();
	return;
}

// Check whether there is an import request
if (isset($_POST['import'])) {
	try {
		// Format
		$format = handle_input_int($_POST['import_format'], 1);
		if (!is_valid_id($format, 'format_id', IMPORT_EXPORT_FORMATS_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Import_Invalid_Format']);
		}

		// File
		$file_content = handle_input_file($_FILES['file_to_import']);

		$success = import_data($file_content, $format);
		if ($success) {
			$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Import']);
		} else {
			$smarty->assign('ERROR_MESSAGE', $lang['Error_Import_Failed']);
		}
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
	}
}

// General page data
$smarty->assign(array(
	'U_IMPORT' => add_query_to_url(current_base_url(), 'module', 'imports'),

	'PAGE_HEADER' => $lang['Imports_Header'],
	'PAGE_DESCRIPTION' => $lang['Imports_Description'],
	'FORMATS' => generate_import_export_formats(),
	'SELECTED_FORMAT' => isset($_POST['import_format']) ? $_POST['import_format'] : '',

	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_IMPORT_FORMAT' => $lang['Import_Format'],
	'L_FILE_WITH_IMPORT_DATA' => $lang['File_With_Import_Data'],
	'L_DO_IMPORT' => $lang['Do_Import'],
	));

$smarty->display('imports.tpl');
?>