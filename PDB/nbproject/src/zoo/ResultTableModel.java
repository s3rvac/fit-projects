/**
 *  Project:  PDB 2008
 *  Authors:  Ondrej Lengal,   xlenga00@stud.fit.vutbr.cz
 *            Libor Polcak,    xpolca03@stud.fit.vutbr.cz
 *            Boris Prochazka, xproch63@stud.fit.vutbr.cz
 *            Petr Zemek,      xzemek02@stud.fit.vutbr.cz
 *
 * @brief Table model for tables which display database results.
 *
 */

package zoo;

import java.util.ArrayList;
import javax.swing.table.AbstractTableModel;

/**
 * @brief Table model for tables which display database results.
 */
public class ResultTableModel extends AbstractTableModel {

	/// Name of table columns.
	private String[] _columnNames = null;

	/// Table rows.
	private ArrayList<Object[]> _rowData = null;

	/// Default value for table cells edition.
	private static final boolean IS_EDITABLE_DEFAULT_VALUE = true;

	/// Are the table cells editable?
	private boolean _isEditable = IS_EDITABLE_DEFAULT_VALUE;

	/**
	 * @brief Constructor.
	 *
	 * @param columnNames Name of table columns.
	 */
	public ResultTableModel(String[] columnNames) {
		this(columnNames, IS_EDITABLE_DEFAULT_VALUE);
	}

	/**
	 * @brief Constructor.
	 *
	 * @param columnNames Name of table columns.
	 * @param isEditable Are the table cells editable?
	 */
	public ResultTableModel(String[] columnNames, boolean isEditable) {
		_columnNames = columnNames;
		_rowData = new ArrayList<Object[]>();
		_isEditable = isEditable;
	}

	/**
	 * @brief Adds a new row to the table.
	 *
	 * @param row Row to be added.
	 */
	public void addRow(Object[] row) {
		_rowData.add(row);
	}

	/**
	 * @brief Returns the number of rows in the table.
	 */
	public int getRowCount() {
		return _rowData.size();
	}

	/**
	 * @brief Returns the number of columns in the table.
	 */
	public int getColumnCount() {
		return _columnNames.length;
	}

	@Override
	public String getColumnName(int col) {
		return _columnNames[col];
	}

	/**
	 * @brief Returns the object on the selected position.
	 *
	 * @param row Row number.
	 * @param col Column number.
	 */
	public Object getValueAt(int row, int col) {
		return _rowData.get(row)[col];
	}

	@Override
	public Class getColumnClass(int col) {
		return getValueAt(0, col).getClass();
	}

	@Override
	public void setValueAt(Object value, int row, int col) {
		_rowData.get(row)[col] = value;
		fireTableCellUpdated(row, col);
	}

	@Override
	public boolean isCellEditable(int row, int col) {
		return _isEditable;
	}
}
