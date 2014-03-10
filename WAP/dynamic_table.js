/*
 * Project:  Dynamic table - WAP 2009 project
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Encoding: iso8859-2
 *
 * This file uses JSDoc (http://sourceforge.net/projects/jsdoc) source
 * documentation syntax.
 *
 * @fileoverview Script that allows your tables to be "dynamic", i.e. users
 *               will be able to filter and sort table rows.
 *               Browser requirements for this script:
 *                 - must support CSS and JavaScript (this script was tested
 *                   on Firefox 3.0, IE 6/7, Opera 9.63, Google Chrome 1.0,
 *                   Apple Safari 4, Konqueror 3.5)
 *               See DT.makeTableDynamic() function for more information.
 *
 * @author Petr Zemek, xzemek02@stud.fit.vutbr.cz
 */

/**
 * Namespace used in this script (DT stands for 'D'ynamic 'T'able).
 */
if (DT === null || typeof(DT) !== "object") {
	var DT = new Object();
}

DT = {
	/**
	 * Column sorting types.
	 */
	SortingTypes: Object,

	/**
	 * Returns the number of columns in the selected table.
	 *
	 * @param  HTMLTableElement  table The table.
	 *
	 * @return  Int  The number of columns in the selected table.
	 */
	getNumberOfTableColumns: function (table) {
		if (table.rows.length > 1) {
			return Math.max(table.rows[0].cells.length, table.rows[1].cells.length);
		} else if (table.rows.length > 0) {
			return table.rows[0].cells.length;
		} else {
			return 0;
		}
	},

	/**
	 * Returns filter element ID for the selected table and column index.
	 *
	 * @param  HTMLTableElement  table     The table.
	 * @param  Int               colIndex  The column index.
	 *
	 * @return  Int  Filter element ID for the selected table and column index.
	 */
	getTableFilterElementId: function (table, colIndex) {
		return table.getAttribute("id") + "_fecol_" + colIndex;
	},

	/**
	 * Returns the width of the selected column.
	 *
	 * @param  HTMLTableElement  table The table.
	 * @param  Int               colIndex  The column index.
	 *
	 * @return  Int  The width of the selected column. If the table has no
	 *               rows it returns 100.
	 */
	getColumnWidth: function (table, colIndex) {
		var columnWidth = -1;
		var tr = table.rows;

		if (tr.length > 0 && tr[0].cells.length >= (colIndex + 1)) {
			columnWidth = tr[0].cells[colIndex].clientWidth;
		} else if (tr.length > 1 && tr[1].cells.length >= (colIndex + 1)) {
			columnWidth = tr[1].cells[colIndex].clientWidth;
		}

		return columnWidth;
	},

	/**
	 * Returns the text content of the selected node.
	 *
	 * @param  HTMLElement  The node.
	 *
	 * @return  String  The text content of the selected node.
	 */
	getTextContent: function (node) {
		// Because it seems that IE does not support textContent, there
		// have to be this ugly if...
		if (node.textContent !== undefined) {
			// FF, Opera, Konqueror, ...
			return node.textContent;
		} else if (node.innerText !== undefined) {
			// IE
			return node.innerText;
		} else {
			// Fallback (altough note that this version returns only the text
			// that is directly in this node, not the text in the nodes' children)
			if (node.nodeType === 1) {
				if (node.hasChildNodes()) {
					return node.firstChild.nodeValue;
				}
			} else if (node.nodeType === 3) {
				txt = node.nodeValue;
			} else {
				return "";
			}
		}
	},

	/**
	 * Returns true if the selected row should be filtered, false otherwise.
	 *
	 * Filtering is case-insensitive.
	 *
	 * @param  HTMLTableElement         table  The table.
	 * @param  HTMLTableSectionElement  row    The row element to be checked.
	 */
	shouldBeFiltered: function (table, row) {
		var rowCells = row.getElementsByTagName("td");

		// For each column
		for (var i = 0; i < Math.min(DT.getNumberOfTableColumns(table), rowCells.length); i++) {
			// Get contents of the row cell and the corresponding filter field
			var rowContent = DT.getTextContent(rowCells[i]).toLowerCase();
			var filterFieldContent = document.getElementById(
				DT.getTableFilterElementId(table, i)).value;

			// Transform them to lowercase (case-insensitive filtering)
			rowContent = rowContent.toLowerCase();
			filterFieldContent = filterFieldContent.toLowerCase();

			// Check whether the content of the filtering field in in the row cell
			if (rowContent.indexOf(filterFieldContent) < 0) {
				// It is not, so this row should be filtered
				return true;
			}
		}

		return false;
	},

	/**
	 * Filters rows in the selected table according to text in the filtering fields.
	 *
	 * @param  String  tableId  The table ID.
	 *
	 * @return  Boolean  If there is no table with the selected ID, this function
	 *                   returns false, true otherwise.
	 */
	filterRows: function (tableId) {
		// Get DOM element that corresponds to the selected table
		var table = document.getElementById(tableId);
		if (table === null) {
			return false;
		}

		// For each row (except the first row containing filtering fields)
		var rows = table.getElementsByTagName("tBody")[0].rows;
		for (var i = 1; i < rows.length; i++) {
			rows[i].style.display = DT.shouldBeFiltered(table, rows[i]) ?
				"none" : "";
		}

		return true;
	},

	/**
	 * Sorts the selected column in the selected table.
	 *
	 * @param  HTMLTableElement  table     The table.
	 * @param  Int               colIndex  The column index.
	 * @param  String            sortType  The sorting type (see DT.SortingTypes).
	 *
	 * @return  Boolean  True if the table exists, false otherwise.
	 */
	sortColumn: function (tableId, colIndex, sortType) {
		// Get DOM element that corresponds to the selected table
		var table = document.getElementById(tableId);
		if (table === null) {
			return false;
		}

		var rows = table.getElementsByTagName("tBody")[0].rows;
		var rowsLength = rows.length - 1; // Exclude the first row (filtering field)

		//
		// Helper functions
		//

		/**
		 * Compares two values (that were returned by getValue()).
		 */
		var compareValues = function (val1, val2) {
			// Get the actual values from these two rows
			val1 = DT.getTextContent(val1.getElementsByTagName("td")[colIndex]);
			val2 = DT.getTextContent(val2.getElementsByTagName("td")[colIndex]);

			// If these values (strings) are numbers, transform them to numbers
			// because otherwise (as strings) they would be compared in a wrong way
			if (parseFloat(val1) == val1 && parseFloat(val2) == val2) {
 				val1 = parseFloat(val1);
 				val2 = parseFloat(val2);
 			} else if (parseInt(val1) == val1 && parseInt(val2) == val2) {
 				val1 = parseInt(val1);
 				val2 = parseInt(val2);
 			}

			return (sortType === DT.SortingTypes.Ascending) ?  (val1 > val2) : (val1 < val2);
		}

		/**
		 * Returns value on the selected position (index i).
		 */
		var getValue = function (i) {
			// (i + 1 is to exclude the first row (filtering field))
			if (navigator.appName === "Microsoft Internet Explorer") {
				// IE
				return rows[i + 1].cloneNode(true);
			} else {
				// Standard browsers
				return rows[i + 1];
			}
		}

		/**
		 * Sets the selected value (val) on the selected position (index i).
		 */
		var setValue = function (i, val) {
			// Copy the whole row (yes, we are sorting only by one column, but we
			// have to also set the rest of the row because most tables usually
			// have more than one column)
			// (i + 1 is to exclude the first row (filtering field))
			if (navigator.appName === "Microsoft Internet Explorer") {
				// IE
				rows[i + 1].replaceNode(val);
			} else {
				// Standard browsers
				var tBody = table.getElementsByTagName("tBody")[0];
				var tmpRow = tBody.replaceChild(rows[i + 1], val);
				if (typeof(val) === "undefined") {
					tBody.appendChild(tmpRow);
				} else {
					tBody.insertBefore(tmpRow, rows[i + 1]);
				}
			}
		}

		//
		// Sort the table rows using the "Shell sort" algorithm
		// (http://en.wikipedia.org/wiki/Shell_sort)
		//
		for (var inc = Math.floor(rowsLength / 2); inc > 0; inc = (inc === 2 ? 1 : (Math.round(inc / 2.2)))) {
			for (var i = inc; i < rowsLength; i++) {
				var tmp = getValue(i);
				for (var j = i; j >= inc && compareValues(getValue(j - inc), tmp); j -= inc) {
					setValue(j, getValue(j - inc));
					setValue(j - inc, tmp);
				}
			}
    	}

		return true;
	},

	/**
	 * Makes the selected table "dynamic", i.e. users will be able to filter and
	 * sort table rows.
	 *
	 * Table requirements:
	 *   - table structure must conform to the HTML 4.0 specification
	 *   - table must have at least one row (either a header or one row in tBody)
	 *   - there MUST NOT be any th elements in tbody
	 *   - colspan and rowspan attributes of all table elements MUST BE set to 1
	 *
	 * If the table is dynamic already, this function does nothing and returns true.
	 *
	 * @param  String  tableId  The table ID.
	 *
	 * @return  Boolean  If there is no table with the selected ID, this function
	 *                   returns false, true otherwise.
	 */
	makeTableDynamic: function (tableId) {
		// Get DOM element that corresponds to the selected table
		var table = document.getElementById(tableId);
		if (table === null) {
			return false;
		}

		// Check whether the table is dynamic already
		if (document.getElementById(DT.getTableFilterElementId(table, 0)) !== null) {
			return true;
		}

		// Make sure the table has the tBody section
		if (table.getElementsByTagName("tBody").length === 0) {
			table.appendChild(document.createElement("tBody"));
		}

		//
		// Create filtering elements for each column
		//
		var filterTr = table.getElementsByTagName("tBody")[0].insertRow(0);
		// For each column...
		for (var i = 0; i < DT.getNumberOfTableColumns(table); i++) {
			var filterTd = document.createElement("td");

			// Create sorting symbols
			var sortAscSymbol = document.createElement("span");
			sortAscSymbol.className = "DTSortSymbol DTSortSymbolAsc";
			sortAscSymbol.style.cursor = "default";
			sortAscSymbol.onclick = new Function("evt", "DT.sortColumn('" + tableId + "', " +
				i + ", '" + DT.SortingTypes.Ascending + "')");
			var sortAscSymbolText = document.createElement("span");
			sortAscSymbolText.appendChild(document.createTextNode("/\\"));
			sortAscSymbol.appendChild(sortAscSymbolText);

			var sortDescSymbol = document.createElement("span");
			sortDescSymbol.className = "DTSortSymbol DTSortSymbolDesc";
			sortDescSymbol.style.cursor = "default";
			sortDescSymbol.onclick = new Function("evt", "DT.sortColumn('" + tableId + "', " +
				i + ", '" + DT.SortingTypes.Descending + "')");
			var sortDescSymbolText = document.createElement("span");
			sortDescSymbolText.appendChild(document.createTextNode("\\/"));
			sortDescSymbol.appendChild(sortDescSymbolText);

			// Create input text field
			var inputField = document.createElement("input");
			inputField.setAttribute("id", DT.getTableFilterElementId(table, i));
			inputField.setAttribute("type", "text");
			inputField.setAttribute("value", "");
			inputField.className = "DTFilterField";
			inputField.onkeyup = new Function("evt", "DT.filterRows('" + tableId + "');");
			inputField.style.width = DT.getColumnWidth(table, i) + "px";

			// Append everything in the right order
			filterTd.appendChild(sortAscSymbol);
			filterTd.appendChild(inputField);
			filterTd.appendChild(sortDescSymbol);

			filterTr.appendChild(filterTd);
		}

		return true;
	}
}

DT.SortingTypes = {
	Ascending: "asc",
	Descending: "desc"
}

/* End of file */
