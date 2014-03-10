/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Maintains database connection and querying.
 *
 */

package zoo;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;
import java.sql.SQLException;
import java.sql.ResultSet;
import java.util.logging.Level;
import java.util.logging.Logger;
import oracle.jdbc.OracleResultSet;
import oracle.jdbc.OraclePreparedStatement;
import oracle.jdbc.OracleTypes;
import oracle.jdbc.driver.OracleDriver;

/**
 * @brief Maintains database connection and querying.
 *
 * Implements the "singleton" design pattern.
 */
public class DBConnection {

	/// Database connection.
	private Connection _conn;

	/// Object used for executing a static SQL statement and returning
	/// the results it produces.
	private Statement _stmt;

	/// Singleton instance.
	private static DBConnection _instance;

	/// JDBC connection string.
	private static final String CONNECTION_STRING =
			"jdbc:oracle:thin:xzemek02/drmz39up@pcuifs1.fit.vutbr.cz:1521:stud";

	/// Default commit policy (i.e. if true, after each query execution, if there
	/// is not specified if there should be a commit, it will commit).
	private static final boolean COMMIT_BY_DEFAULT = true;

	/**
	 * @brief Default constructor.
	 */
	private DBConnection() {
		try {
			createDbConnection();
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
		}
	}

	/**
	 * @brief Finalizer.
	 *
	 * Closes the database connection.
	 */
	@Override
	protected void finalize() throws SQLException {
		_stmt.close();
		_conn.close();
	}

	/**
	 * @brief Returns the only instance of this class.
	 */
	public static DBConnection getInstance() {
		if (_instance == null) {
			_instance = new DBConnection();
		}

		return _instance;
	}

	/**
	 * @brief Creates a new database connection.
	 */
	private void createDbConnection() throws SQLException {
		// Register the oracle JDBC driver (it can be registered more than once,
		// so there is no problem if this function is called more than once)
		DriverManager.registerDriver(new OracleDriver());

		// Create a new db connection
		_conn = DriverManager.getConnection(CONNECTION_STRING);

		// Enable two-phase select-commit of BLOBs
		_conn.setAutoCommit(false);

		// Create a JDBC Statement object to execute SQL in the database
		_stmt = _conn.createStatement();

		Logger.getLogger("zoo").log(Level.INFO, "Connection created! JDBC version: " +
				_conn.getMetaData().getDriverVersion());
	}

	/**
	 * @brief Checks the db connection and if it's closed, reconnects.
	 */
	private void checkDbConnection() throws SQLException {
		if (_conn == null) {
			System.err.println("Cannot connect to the database " +
					"(using connection string: " + CONNECTION_STRING + ").");
			System.exit(1);
		}

		if (_conn.isClosed()) {
			// Reconnect
			createDbConnection();
		}
	}

	/**
	 * @brief Creates a PreparedStatement object for sending parameterized SQL
	 *        statements to the database.
	 *
	 * @param[in] query SQL statement that may contain one or more '?' IN
	 *                  parameter placeholders.
	 * @return New default PreparedStatement object containing the pre-compiled
	 *         SQL statement.
	 */
	public OraclePreparedStatement oraclePrepareStatement(String query)
			throws SQLException {
		try {
			checkDbConnection();

			return (OraclePreparedStatement) _conn.prepareStatement(query);
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			throw ex;
		}
	}

	/**
	 * @brief Executes the SQL statement in this PreparedStatement object,
	 *        which may be any kind of SQL statement.
	 *
	 * @param[in] pstmt PreparedStatement object containing the pre-compiled
	 *         SQL statement with all parameters set.
	 */
	public void oracleExecutePreparedStatement(OraclePreparedStatement pstmt)
			throws SQLException {
		oracleExecutePreparedStatement(pstmt, COMMIT_BY_DEFAULT);
	}

	/**
	 * @brief Executes the SQL statement in this PreparedStatement object,
	 *        which may be any kind of SQL statement.
	 *
	 * @param[in] pstmt PreparedStatement object containing the pre-compiled
	 *         SQL statement with all parameters set.
	 * @param[in] commit Should there be a commit after the query execution?
	 */
	public void oracleExecutePreparedStatement(OraclePreparedStatement pstmt,
			boolean commit) throws SQLException {
		try {
			checkDbConnection();

			pstmt.execute();
			pstmt.close();
			if (commit) {
				_conn.commit();
			}
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			throw ex;
		}
	}

	/**
	 * @brief Executes the selected query on the database with response.
	 *
	 * @param[in] query The SQL query string.
	 * @return The query result.
	 *
	 * It executes an Oracle SQL query on the database and returns the result.
	 */
	public OracleResultSet oracleQueryResponse(String query) throws SQLException {
		return oracleQueryResponse(query, COMMIT_BY_DEFAULT);
	}

	/**
	 * @brief Executes the selected query on the database with response.
	 *
	 * @param[in] query The SQL query string.
	 * @param[in] commit Should there be a commit after the query execution?
	 * @return The query result.
	 *
	 * It executes an Oracle SQL query on the database and returns the result.
	 */
	public OracleResultSet oracleQueryResponse(String query, boolean commit)
			throws SQLException {
		try {
			checkDbConnection();
			Logger.getLogger("zoo").log(Level.INFO, query);

			// create the result set
			OracleResultSet rset;
			rset = (OracleResultSet) _stmt.executeQuery(query);

			if (commit) {
				_conn.commit();
			}
			return rset;
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE, "Exception caught (" +
					ex.getClass() + "): " + ex.getMessage());
			throw ex;
		}
	}

	/**
	 * @brief Executes the selected query on the database without response.
	 *
	 * @param[in] query The SQL query string.
	 *
	 * It executes an Oracle SQL query on the database and does not
	 * return anything.
	 */
	public void oracleQueryVoid(String query) throws SQLException {
		oracleQueryVoid(query, COMMIT_BY_DEFAULT);
	}

	/**
	 * @brief Executes the selected query on the database without response.
	 *
	 * @param[in] query The SQL query string.
	 * @param[in] commit Should there be a commit after the query execution?
	 *
	 * It executes an Oracle SQL query on the database and does not
	 * return anything.
	 */
	public void oracleQueryVoid(String query, boolean commit) throws SQLException {
		try	{
			checkDbConnection();
			Logger.getLogger("zoo").log(Level.INFO, query);

			_stmt.executeQuery(query);
			if (commit) {
				_conn.commit();
			}
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE,
					"Exception caught (" + ex.getClass() + "): " + ex.getMessage());
			throw ex;
		}
	}

	/**
	 * @brief Inserts a new row into the database and returns the ID of the row.
	 *
	 * @param[in] query An SQL insert query, must be in format
	 *                     "INSERT INTO <table> VALUES <values>".
	 * @param[in] idName The name of the column with row IDs.
	 * @return The row ID of the inserted row.
	 */
	public int oracleQueryInsert(String query, String idName) throws SQLException {
		return oracleQueryInsert(query, idName, COMMIT_BY_DEFAULT);
	}

	/**
	 * @brief Inserts a new row into the database and returns the ID of the row.
	 *
	 * @param[in] query An SQL insert query, must be in format
	 *                     "INSERT INTO <table> VALUES <values>".
	 * @param[in] idName The name of the column with row IDs.
	 * @param[in] commit Should there be a commit after the query execution?
	 * @return The row ID of the inserted row.
	 */
	public int oracleQueryInsert(String query, String idName, boolean commit)
			throws SQLException {
		try	{
			checkDbConnection();
			Logger.getLogger("zoo").log(Level.INFO, query);

			query +=  " RETURNING " + idName + " INTO ?";

			// prepare the statement
			OraclePreparedStatement pstmt =
				(OraclePreparedStatement)_conn.prepareStatement(query);

			pstmt.registerReturnParameter(1, OracleTypes.NUMBER);
			pstmt.executeUpdate();
			if (commit) {
				_conn.commit();
			}
			ResultSet rset = pstmt.getReturnResultSet();
			if (rset.next()) {
				return rset.getInt(1);
			}	else {
				return -1;
			}
		} catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE,
					"Exception caught (" + ex.getClass() + "): " + ex.getMessage());
			throw ex;
		}
	}

	/**
	 * @brief Executes the selected sql script in file.
	 *
	 * @param file The SQL script file.
	 *
	 * TODO: Rewrite?
	 */
	public void runSqlScript(String file) {
		try {
			checkDbConnection();

			String line;
			String lineWithoutComment;
			String query = "";
			BufferedReader reader = new BufferedReader(new FileReader(file));

			while ((line = reader.readLine()) != null) {
				// find a comment
				final int commentIndex = line.indexOf("--");
				if (commentIndex == -1) {
					// no comment
					lineWithoutComment = line;
				}
				else {
					// strip comment
					lineWithoutComment = line.substring(0, commentIndex);
				}

				// merge lines.
				query += (" " +lineWithoutComment);

				// simple query found
				if (query.contains(";")) {
					// no space
					query = query.trim();
					// no ';'
					query = query.replace(";", "");
					// run
					oracleQueryVoid(query);
					// new query
					query = "";
				}
			}
		}
		catch (IOException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE,
					"Exception caught (" + ex.getClass() + "): " + ex.getMessage());
	  	}
		catch (SQLException ex) {
			Logger.getLogger("zoo").log(Level.SEVERE,
					"Exception caught (" + ex.getClass() + "): " + ex.getMessage());
		}
	}

	/**
	 * @brief Main.
	 */
	public static void main(String[] args) {
		try {
			OracleResultSet res =
				DBConnection.getInstance().oracleQueryResponse("SELECT * FROM pavilion");
			res.next();
			System.out.println(res.getNUMBER(1).longValue());
		} catch (SQLException ex) {
			System.out.println("Error: " + ex.getMessage());
		}
	}
}
