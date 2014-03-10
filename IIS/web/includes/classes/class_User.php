<?php
// Encoding: utf8
/**
 * Class User.
 *
 * @author Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
 * @package IIS
 */

/**
 * User details.
 * It is built upon the singleton desing pattern.
 */
class User {
	// Sign whether there is an instance of this class or not
	private static $instance = false;

	// External objects
	private $dbc;
	private $config;
	private $session;

	// User data
	private $data = array();
	private $was_inactive;

	/**
	 * Creates an instance of this class.
	 * This is the only way to create instances of this class.
	 *
	 * @return User Instance of class User.
	 */
	public static function create_instance()
  	{
		if (self::$instance === false) {
			self::$instance = new User();
		}

		return self::$instance;
	}

	/**
	 * Constructor.
	 * Starts a new session and loads necessary userdata.
	 */
	private function __construct()
	{
		$this->dbc =& $GLOBALS['dbc'];
		$this->config =& $GLOBALS['config'];
		$this->session =& $GLOBALS['session'];
		$this->was_inactive = false;

		$this->session->start($this->config->get_value('session_name'));

		// Check whether the user is logged in
		if (isset($this->session->username) && !empty($this->session->username) &&
			isset($this->session->logged) && $this->session->logged === true) {
			// Check whether the user should be logged out because of inactivity
			if ($this->session->expiration_time <= time()) {
				$this->was_inactive = true;
				$this->logout();
			} else {
				// Ok, user is active
				$this->load_users_data();
				$this->session->expiration_time = time() +
					$this->config->get_value('session_expiration_interval');
			}
		} else {
			$this->session->logged = false;
		}

		$this->init_current_language();
		$this->init_previous_page_urls();
	}

	/**
	 * Login current user.
	 *
	 * @param string $username User's username.
	 * @param string $password User's password.
	 */
	public function login($username, $password)
	{
		// Authenticate user
		$sql = 'SELECT username
			FROM ' . USERS_TABLE . "
			WHERE username = '$username'
				AND password = '" . hash_password($password) . "'";
		$result = mysql_query($sql);
		if (!$result || mysql_num_rows($result) != 1) {
			return false;
		}
		$row = mysql_fetch_assoc($result);
		mysql_free_result($result);
		// Check whether the returned username is exactly the same as
		// the original one (possible case-insensitive collate in
		// the database problems)
		if ($row['username'] != $username) {
			return false;
		}

		// Set and load user's data
		$this->session->username = $username;
		$this->session->expiration_time = time() +
			$this->config->get_value('session_expiration_interval');
		$this->load_users_data();
		$this->session->logged = true;

		return true;
	}

	/**
	 * Logout current user.
	 */
	public function logout()
	{
		// Preserve user's language
		$last_language = $this->session->lang;
		$this->data = array();
		$this->session->restart();
		$this->session->lang = $last_language;
	}

	/**
	 * Loads user's data from the database into $this->data.
	 */
	private function load_users_data()
	{
		$user_id = get_value_corresponding_to_other_value(
			'user_id', $this->session->username, 'username', USERS_TABLE);
		$this->data = load_row_from_table($user_id, 'user_id', USERS_TABLE);
	}

	/**
	 * Initializes user's current language according to the GET arguments
	 * or session data.
	 */
	private function init_current_language()
	{
		$default_language = $this->config->get_value('default_language');

		if (isset($_GET['lang'])) {
			$this->session->lang = is_valid_language(
				$_GET['lang']) ? $_GET['lang'] : $default_language;
		} else if (isset($this->session->lang)) {
			$this->session->lang = is_valid_language(
				$this->session->lang) ? $this->session->lang : $default_language;
		} else {
			$this->session->lang = $default_language;
		}
	}

	/**
	 * Initializes user's previous page URL.
	 */
	private function init_previous_page_urls()
	{
		if (!isset($this->session->previous_page_urls) || empty($this->session->previous_page_urls)) {
			$this->session->previous_page_urls = array($_SERVER['HTTP_REFERER']);
		} else if ($_SERVER['HTTP_REFERER'] != current_full_url()) {
			$new_previous_page_urls = $this->session->previous_page_urls;

			if (in_array(current_full_url(), $new_previous_page_urls)) {
				// User got back to some previous page
				do  {
					array_pop($new_previous_page_urls);
				} while (in_array(current_full_url(), $new_previous_page_urls));
			} else {
				array_push($new_previous_page_urls, $_SERVER['HTTP_REFERER']);
			}

			$count_exceeded = count($new_previous_page_urls) >
				$this->config->get_value('max_previous_pages');
			$this->session->previous_page_urls[count($this->session->previous_page_urls) - 1];
			if ($count_exceeded) {
				// Remove the oldest page
				array_shift($new_previous_page_urls);
			}

			$this->session->previous_page_urls = $new_previous_page_urls;
		}
	}

	/**
	 * @return string User's current language.
	 */
	public function get_current_language()
	{
		return $this->session->lang;
	}

	/**
	 * Sets new user's current language.
	 *
	 * @param string $new_language New user's language to be set.
	 */
	public function set_current_language($new_language)
	{
		$default_language = $this->config->get_value('default_language');
		$this->session->lang = is_valid_language(
			$new_language) ? $new_language : $default_language;
	}

	/**
	 * @return bool True if the user is logged, false otherwise.
	 */
	public function is_logged()
	{
		return $this->session->logged;
	}

	/**
	 * @return bool True if the user was inactive (and so was logged out
	 * because of incactivity), false otherwise.
	 */
	public function was_inactive()
	{
		return $this->was_inactive;
	}

	/**
	 * @return string Previous page URL. This function is preferred over
	 * HTTP referer because this one counts with possible problems
	 * when sending POST data (HTTP referer is not change in that case).
	 */
	public function get_previous_page_url()
	{
		return $this->session->previous_page_urls[count($this->session->previous_page_urls) - 1];
	}

	/**
	 * @return interger User's ID.
	 */
	public function get_user_id()
	{
		return isset($this->data['user_id']) ? $this->data['user_id'] : 0;
	}

	/**
	 * @return string User's username.
	 */
	public function get_username()
	{
		return $this->session->username;
	}

	/**
	 * @return int User's group ID (see constants.php - Group section
	 * for apropriate symbolic constants).
	 */
	public function get_group_id()
	{
		return isset($this->data['group_id']) ? $this->data['group_id'] : VISITOR;
	}

	/**
	 * @return int User's company ID.
	 */
	public function get_company_id()
	{
		return $this->data['company_id'];
	}
}
?>