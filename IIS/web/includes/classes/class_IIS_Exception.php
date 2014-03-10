<?php
// Encoding: utf8
/**
 * Exception classes.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

/**
 * Base IIS exception class. All exceptions are derived from this class.
 */
abstract class IIS_Exception extends Exception {
	/**
	 * Constructor.
	 *
	 * @param string $message Error message.
	 * @param int $code Error code.
	 */
	public function __construct($message = '', $code = 0)
	{
		parent::__construct($message, $code);
	}

	/**
	 * Prints stored error message (optionaly formatted with additional
	 * information) on the screen.
	 */
	abstract public function print_error_message();
}


/**
 * Database exceptions.
 */
class IIS_DB_Exception extends IIS_Exception
{
	// Last SQL statement
	protected $sql_query;
	// SQL error message from the last operation
	protected $sql_error_message;

	/**
	 * Constructor.
	 *
	 * @param string $message Error message.
	 * @param string $sql_query Last SQL statement.
	 * @param string $sql_error_message SQL error message from the last operation.
	 * @param string $sql_error_code SQL error code.
	 */
	public function __construct($message = '', $sql_query = '',
		$sql_error_message = '', $sql_error_code = 0)
	{
		parent::__construct($message, $sql_error_code);

		$this->sql_query = $sql_query;
		$this->sql_error_message = $sql_error_message;
	}

	/**
	 * @return string Saved sql statement.
	 */
	final public function sql_query()
	{
		return $this->sql_query;
	}

	/**
	 * @return string Saved SQL error message from the last operation.
	 */
	final public function sql_error_message()
	{
		return $this->sql_error_message;
	}

	/**
	 * Prints stored error message (formatted with additional information)
	 * on the screen.
	 */
	public function print_error_message()
	{
		echo '<?xml version="1.0" encoding="utf-8" ?>
			<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
				"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
			<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="cs" lang="cs">
			<head>
				<meta http-equiv="content-type" content="text/html; charset=utf-8" />
				<title>Database error</title>
			</head>
			<body>
			<h1>Database error</h1>
			<table>
				<tr>
					<td><strong>In file</strong></td>
					<td> : </td>
					<td>' . $this->getFile() . '</td>
				</tr>
				<tr>
					<td><strong>On line</strong></td>
					<td> : </td>
					<td>' .  $this->getLine() . '</td>
				</tr>
				<tr>
					<td><strong>Message</strong></td>
					<td> : </td>
					<td>' . $this->getMessage() . '</td>
				</tr>
				<tr>
					<td><strong>Last SQL statement</strong></td>
					<td> : </td>
					<td>' . $this->sql_query() . '</td>
				</tr>
				<tr>
					<td><strong>Last SQL error</strong></td>
					<td> : </td>
					<td>' . $this->sql_error_message() . '</td>
				</tr>
			</table>
			</body>
			</html>';

		exit();
	}
}

/**
 * I/O exceptions (files etc.)
 */
class IIS_IO_Exception extends IIS_Exception
{
	/**
	 * Constructor.
	 *
	 * @param string $message Error message.
	 * @param int $code Error code.
	 */
	public function __construct($message = '', $code = 0)
	{
		parent::__construct($message, $code);
	}

	/**
	 * Prints stored error message (formatted with additional information)
	 * on the screen.
	 */
	public function print_error_message()
	{
		echo '<?xml version="1.0" encoding="utf-8" ?>
			<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
				"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
			<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="cs" lang="cs">
			<head>
				<meta http-equiv="content-type" content="text/html; charset=utf-8" />
				<title>I/O error</title>
			</head>
			<body>
			<h1>I/O error</h1>
			<table>
				<tr>
					<td><strong>In file</strong></td>
					<td> : </td>
					<td>' . $this->getFile() . '</td>
				</tr>
				<tr>
					<td><strong>On line</strong></td>
					<td> : </td>
					<td>' .  $this->getLine() . '</td>
				</tr>
				<tr>
					<td><strong>Message</strong></td>
					<td> : </td>
					<td>' . $this->getMessage() . '</td>
				</tr>
			</table>
			</body>
			</html>';

		exit();
	}
}

/**
 * Input data exceptions (wrong, invalid etc. input).
 */
class IIS_Input_Exception extends IIS_Exception
{
	/**
	 * Constructor.
	 *
	 * @param string $message Error message.
	 */
	public function __construct($message = '')
	{
		parent::__construct($message, 0);
	}

	/**
	 * Prints stored error message (formatted with additional information)
	 * on the screen.
	 */
	public function print_error_message()
	{
		echo '<?xml version="1.0" encoding="utf-8" ?>
			<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
				"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
			<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="cs" lang="cs">
			<head>
				<meta http-equiv="content-type" content="text/html; charset=utf-8" />
				<title>I/O error</title>
			</head>
			<body>
			<h1>Input error</h1>
			<table>
				<tr>
					<td><strong>In file</strong></td>
					<td> : </td>
					<td>' . $this->getFile() . '</td>
				</tr>
				<tr>
					<td><strong>On line</strong></td>
					<td> : </td>
					<td>' .  $this->getLine() . '</td>
				</tr>
				<tr>
					<td><strong>Message</strong></td>
					<td> : </td>
					<td>' . $this->getMessage() . '</td>
				</tr>
			</table>
			</body>
			</html>';

		exit();
	}
}
?>