<?php
// Encoding: utf8
/**
 * Exports module.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

if (!defined('IIS_WEB')) {
	die('Hacking attempt');
}

/**
 * Exports selected table data in the XML format into $data.
 *
 * @param string $table_name Name of the table.
 * @param string $data [out] Into this variable table data will be exported.
 * @return bool True if the import was successfull, false if some
 * error occures.
 */
function export_xml_data($table_name, &$data)
{
	try {
		$table = new SimpleXMLElement('<table></table>');
		$table->addAttribute('name', $table_name);
		$schema = $table->addChild('schema');

		// Get table fields
		$sql = 'SHOW FIELDS
			FROM ' . $table_name;
		$result = mysql_query($sql);
		if (!$result) {
			throw new IIS_DB_Exception('Cannot show fields from ' . $table_name . '.',
				$sql, mysql_error());
		}
		$columns = array();
		while ($row = mysql_fetch_row($result)) {
			$column = $schema->addChild('column');
			// Can't use !empty() because the default column can be '0'
			$row[0] != '' ? $column->addAttribute('field', $row[0]) : '';
			$row[1] != '' ? $column->addAttribute('type', $row[1]) : '';
			$row[2] != '' ? $column->addAttribute('null', $row[2]) : '';
			$row[3] != '' ? $column->addAttribute('key', $row[3]) : '';
			$row[4] != '' ? $column->addAttribute('default', $row[4]) : '';
			$row[5] != '' ? $column->addAttribute('extra', $row[5]) : '';

			array_push($columns, $row[0]);
		}
		mysql_free_result($result);

		$data = $table->addChild('data');

		// Get table data
		$sql = 'SELECT *
			FROM ' . $table_name;
		$result = mysql_query($sql);
		if (!$result) {
			throw new IIS_DB_Exception('Cannot get all data from ' . $table_name . '.',
				$sql, mysql_error());
		}
		while ($row = mysql_fetch_assoc($result)) {
			$data_row = $data->addChild('row');
			foreach ($columns as $column_name) {
				if (isset($row[$column_name])) {
					$data_row->addAttribute($column_name, $row[$column_name]);
				}
			}
		}
		mysql_free_result($result);

		$data = $table->asXML();
	} catch (IIS_DB_Exception $e) {
		throw $e;
	} catch (Exception $e) {
		return false;
	}

	return true;
}

/**
 * Exports selected table data in the selected format into $data.
 *
 * @param string $table_name Name of the table.
 * @param string $format Data format.
 * @param string $data [out] Into this variable table data will be exported.
 * @return bool True if the import was successfull, false if some
 * error occures.
 */
function export_data($table_name, $format, &$data)
{
	switch ($format) {
		case IMPORT_EXPORT_FORMAT_XML:
			return export_xml_data($table_name, $data);
			break;

		default:
			return $lang['Error_Export_Invalid_Format'];
			break;
	}
}

if ($user->get_group_id() != EXHIBITIONGROUND_MEMBER) {
	not_allowed();
	return;
}

// Check whether there is an export request
if (isset($_POST['export'])) {
	try {
		// Format
		$format = handle_input_int($_POST['export_format'], 1);
		if (!is_valid_id($format, 'format_id', IMPORT_EXPORT_FORMATS_TABLE)) {
			throw new IIS_Input_Exception($lang['Error_Export_Invalid_Format']);
		}

		// Table
		$table = handle_input_string($_POST['table']);
		if (!in_array($table, generate_tables())) {
			throw new IIS_Input_Exception($lang['Error_Export_Invalid_Table']);
		}
	} catch (IIS_Input_Exception $e) {
		$smarty->assign('ERROR_MESSAGE', $e->getMessage());
	}

	$exported_data = '';
	$success = export_data($table, $format, $exported_data);
	if ($success) {
		$smarty->assign('SUCCESS_MESSAGE', $lang['Success_Export']);
		$file_extension = get_value_corresponding_to_other_value('name', $format,
			'format_id', IMPORT_EXPORT_FORMATS_TABLE);
		$file_name = $table . '.' . strtolower($file_extension);
		$mime_type = get_value_corresponding_to_other_value('mime_type', $format,
			'format_id', IMPORT_EXPORT_FORMATS_TABLE);

 		send_file_to_user($file_name, $exported_data, $mime_type);
	} else {
		$smarty->assign('ERROR_MESSAGE', $lang['Error_Export_Failed']);
	}
}

// General page data
$smarty->assign(array(
	'U_EXPORT' => add_query_to_url(current_base_url(), 'module', 'exports'),

	'PAGE_HEADER' => $lang['Exports_Header'],
	'PAGE_DESCRIPTION' => $lang['Exports_Description'],
	'FORMATS' => generate_import_export_formats(),
	'SELECTED_FORMAT' => isset($_POST['export_format']) ? $_POST['export_format'] : '',
	'TABLES' => generate_tables(),
	'SELECTED_TABLE' => isset($_POST['table']) ? $_POST['table'] : '',

	'L_MANDATORY_FIELDS' => $lang['Mandatory_Fields'],
	'L_EXPORT_FORMAT' => $lang['Export_Format'],
	'L_TABLE' => $lang['Table'],
	'L_DO_EXPORT' => $lang['Do_Export'],
	));

$smarty->display('exports.tpl');
?>