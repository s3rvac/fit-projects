<?php
// Encoding: utf8
/**
 * Class Config.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

/**
 * Configuration class.
 * Loads/stores configuation data from/into the database.
 * It is built upon the singleton desing pattern.
 */
class Config {
	// Sign whether there is an instance of this class or not
	private static $instance = false;

	// Database connection
	private $dbc;

	// Config data (associative array, name -> value)
	private $data = array();

	/**
	 * Constructor.
	 * Saves db connection and loads all config data from the db.
	 */
	private function __construct()
	{
		$this->dbc =& $GLOBALS['dbc'];

		$this->load_config_data();
	}

	/**
	 * Creates an instance of this class.
	 * This is the only way to create instances of this class.
	 *
	 * @return Config Instance of class Config.
	 */
	public static function create_instance()
  	{
		if (self::$instance === false) {
			self::$instance = new Config();
		}

		return self::$instance;
	}

	/**
	 * Loads all config data into $this->data (so it won't need to load them
	 * in each access to them - performance boost).
	 */
	private function load_config_data()
	{
		$sql = 'SELECT name, value
			FROM ' . CONFIG_TABLE;
		$result = mysql_query($sql);
		if (!$result) {
			throw new IIS_DB_Exception('Cannot load config data.',
				$sql, mysql_error());
		}

		while ($row = mysql_fetch_assoc($result)) {
			$this->data[$row['name']] = $row['value'];
		}
		mysql_free_result($result);
	}

	/**
	 * Returns value of the selected configuration $name. If there is no value
	 * associated with the selected name, IIS_DB_Exception will be thrown.
	 *
	 * @param string $name Name
	 * @return string Value of the selected configuration $name.
	 */
	public function get_value($name)
	{
		if (!isset($this->data[$name])) {
			throw new IIS_DB_Exception("There is no value associated with: '" .
				$name . "'.");
		} else {
			return $this->data[$name];
		}
	}

	/**
	 * Sets new $value for the selected configuration $name.
	 *
	 * @param string $value Value
	 * @param string $name
	 */
	public function set_value($name, $value)
	{
		if (isset($this->data[$name])) {
			// Change the value
			$sql = 'UPDATE ' . CONFIG_TABLE . "
				SET value = '$value'
				WHERE name = '$name'";
			if (!mysql_query($sql))	{
				throw new IIS_DB_Exception('Config table update failed.',
					$sql, mysql_error());
			}
		} else {
			// Add a new name => value pair to the database
			$sql = 'UPDATE ' . CONFIG_TABLE . "
				SET value = '$value'
				WHERE name = '$name'";
			if (!mysql_query($sql))	{
				throw new IIS_DB_Exception('Config table update failed.',
					$sql, mysql_error());
			}
		}

		$this->data[$name] = $value;
	}
}
?>