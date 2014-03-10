<?php
// Encoding: utf8
/**
 * Class Session.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

/**
 * Session details.
 * It is built upon the singleton desing pattern.
 */
class Session {
	// Sign whether there is an instance of this class or not
	private static $instance = false;

	// Config
	private $config;

	/**
	 * Constructor.
	 */
	private function __construct()
	{
		$this->config =& $GLOBALS['config'];

		$old_session_data = array();

		ini_set('session.use_cookies', 1);
		ini_set('session.use_only_cookies', 1);
		ini_set('session.use_trans_sid', 0);
	}

	/**
	 * Creates an instance of this class.
	 * This is the only way to create instances of this class.
	 *
	 * @return Session Instance of class Session.
	 */
	public static function create_instance()
  	{
		if (self::$instance === false) {
			self::$instance = new Session();
		}

		return self::$instance;
	}

	/**
	 * @return int Session ID.
	 */
	public function get_session_id()
	{
		return session_id();
	}

	/**
	 * @return int Session name.
	 */
	public function get_session_name()
	{
		return session_name();
	}

	/**
	 * Regenerates session ID.
	 */
	public function regenerate_id()
	{
		session_regenerate_id();
	}

	/**
	 * Starts a session.
	 *
	 * @param string $session_name Name of the session
	 */
	public function start($session_name)
	{
		session_name($session_name);
  		session_start();

 		$this->validate_session();
	}

	/**
	 * Validates a session and if it's not valid, destroys it.
	 */
	private function validate_session()
	{
		// Check whether the user is using the same browser as he did in the
		// previous request
		if (isset($_SESSION['__PREVIOUS_USER_AGENT_HASH']) &&
			md5($_SERVER['HTTP_USER_AGENT']) !== $_SESSION['__PREVIOUS_USER_AGENT_HASH']) {
			$this->restart();
		}

		$_SESSION['__PREVIOUS_USER_AGENT_HASH'] = md5($_SERVER['HTTP_USER_AGENT']);
	}

	/**
	 * Destroys a session.
	 */
	public function destroy()
	{
		$_SESSION = array();
 		if (isset($_COOKIE[session_name()])) {
 			setcookie(session_name(), '', time() - 65536, '/');
 		}
		session_destroy();
	}

	/**
	 * "Restarts" the session by destroying it and creating another one.
	 */
	public function restart()
	{
		$session_name = $this->get_session_name();
		$this->destroy();
		$this->start($session_name);
		$this->regenerate_id();
	}

	/**
	 * Sets selected session variable.
	 *
	 * @param string $name Name of the variable.
	 * @param string $value Value of the variable.
	 */
	public function __set($name, $value)
	{
		$_SESSION[$name] = $value;
	}

	/**
	 * @return mixed Selected session variable.
	 * @param string $name Name of the variable.
	 */
	public function __get($name)
	{
		return $_SESSION[$name];
	}

	/**
	 * @return bool True if the selected variable is set, false otherwise.
	 * @param string $name Name of the variable.
	 */
	public function __isset($name)
	{
		return isset($_SESSION[$name]);
	}

	/**
	 * Unsets selected variable.
	 *
	 * @param string $name Name of the variable.
	 */
	public function __unset($name)
	{
		unset($_SESSION[$name]);
	}
}
?>